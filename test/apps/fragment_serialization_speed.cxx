#include "dataformats/Fragment.hpp"
#include "ipm/Receiver.hpp"
#include "serialization/NetworkObjectReceiver.hpp"
#include "serialization/NetworkObjectSender.hpp"
#include "ipm/Sender.hpp"

#include <chrono>
#include <cstddef>
#include <thread>
#include <mutex>

std::mutex print_mutex;

using namespace std::chrono_literals;

dunedaq::dataformats::Fragment make_fragment(size_t fragment_size)
{
  using dunedaq::dataformats::Fragment;
  using dunedaq::dataformats::FragmentHeader;
  
  FragmentHeader header;
  header.m_size = sizeof(FragmentHeader) + fragment_size;
  header.m_trigger_number = 1;
  header.m_trigger_timestamp = 2;
  header.m_run_number = 3;

  auto frag_buff = malloc(sizeof(FragmentHeader) + fragment_size);
  memcpy(frag_buff, &header, sizeof(FragmentHeader));

  for(size_t i=0; i<fragment_size; ++i){
    *(static_cast<uint8_t*>(frag_buff) + sizeof(FragmentHeader) + i)=i%255;
  }

  Fragment frag(frag_buff); // frag memory now owned by Fragment

  return frag;
}

void
sender_thread_serialization_fn(dunedaq::serialization::networkobjectsender::Conf sender_conf, size_t n_messages, size_t fragment_size)
{
  dunedaq::NetworkObjectSender<dunedaq::dataformats::Fragment> sender(sender_conf);
  
  dunedaq::dataformats::Fragment frag=make_fragment(fragment_size);

  for (size_t i = 0; i < n_messages; ++i) {
    frag.set_run_number(i+1);
    sender.send(frag, std::chrono::milliseconds(1000000));
  }
}

void
receiver_thread_serialization_fn(dunedaq::serialization::networkobjectreceiver::Conf receiver_conf, size_t n_messages)
{
  using dunedaq::dataformats::Fragment;
  using dunedaq::dataformats::FragmentHeader;

  size_t total = 0;
  dunedaq::NetworkObjectReceiver<Fragment> receiver(receiver_conf);
  for (size_t i = 0; i < n_messages; ++i) {
    Fragment frag = receiver.recv(10000000ms);
    total += frag.get_run_number();
  }
  if(total!=n_messages*(n_messages+1)/2){
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "Incorrect total:" << total << std::endl;
  }
}

void
sender_thread_no_serialization_fn(std::shared_ptr<dunedaq::ipm::Sender> sender, size_t n_messages, size_t fragment_size)
{
  dunedaq::dataformats::Fragment frag=make_fragment(fragment_size);

  for (size_t i = 0; i < n_messages; ++i) {
    frag.set_run_number(i+1);
    sender->send(frag.get_storage_location(), frag.get_size(), std::chrono::milliseconds(1000000));
  }
}

void
receiver_thread_no_serialization_fn(std::shared_ptr<dunedaq::ipm::Receiver> receiver, size_t n_messages, int offset, bool copy_data)
{
  using dunedaq::dataformats::Fragment;
  using dunedaq::dataformats::FragmentHeader;

  size_t total = 0;
  for (size_t i = 0; i < n_messages; ++i) {
    auto recvd = receiver->receive(10000000ms);
    Fragment frag(recvd.m_data.data()+offset, copy_data);
    total += frag.get_run_number();
  }
  if(total!=n_messages*(n_messages+1)/2){
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "Incorrect total:" << total << std::endl;
  }
}

// Return the current steady clock in microseconds
inline uint64_t
now_us()
{
  using namespace std::chrono;
  // std::chrono is the worst
  return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

class Timer
{
public:
  Timer(std::string serialize_type, std::string deserialize_type, size_t N, size_t fragment_size, bool copy_data)
    : m_serialize_type(serialize_type)
    , m_deserialize_type(deserialize_type)
    , m_N(N)
    , m_fragment_size(fragment_size)
    , m_copy_data(copy_data)
    , m_start(now_us())
  {}

  ~Timer()
  {
    uint64_t end_time = now_us();
    double time_taken_s = 1e-6 * (end_time - m_start);
    double kHz = 1e-3 * m_N / time_taken_s;
    double GBs = 1e-9 * m_N * m_fragment_size / time_taken_s;
    std::lock_guard<std::mutex> lock(print_mutex);
    printf(" %10s | %10s | %10s |   % 7.2f |     % 6.3g |     % 6.2g\n", m_serialize_type.c_str(), m_deserialize_type.c_str(), m_copy_data ? "yes" : " no", time_taken_s, kHz, GBs);
  }
  
private:
  std::string m_serialize_type;
  std::string m_deserialize_type;
  size_t m_N;
  size_t m_fragment_size;
  bool m_copy_data;
  uint64_t m_start;
};

int
main(int argc, char** argv)
{
  // Usage: fragment_serialization_speed [number_of_fragments] [fragment_size_in_bytes]
  int N = 500000;
  size_t fragment_size = 46400;
  if(argc>1) N=atoi(argv[1]);
  if(argc>2) fragment_size=atol(argv[2]);

  printf("Sending %d fragments of size %ld bytes\n\n", N, fragment_size);
  printf("        ser |      deser |      copy? |  time (s) | rate (kHz) | rate (GB/s) \n");
  printf("----------------------------------------------------------------------------\n");
  
  for(bool copy_data : {true, false})
  {

    // Sometimes the IPM connections fail with "address already in
    // use", presumably because the OS hasn't finished tearing down
    // the old connection before we get to the next loop
    // iteration. Work around this issue with a sleep
    std::this_thread::sleep_for(1s);
    
    dunedaq::serialization::networkobjectsender::Conf sender_conf;
    sender_conf.ipm_plugin_type = "ZmqSender";
    sender_conf.stype = "msgpack";
    sender_conf.address = "inproc://foo";

    std::shared_ptr<dunedaq::ipm::Receiver> receiver=dunedaq::ipm::make_ipm_receiver("ZmqReceiver");
    receiver->connect_for_receives({{"connection_string", "inproc://foo"}});

    Timer t("msgpack", "raw", N, fragment_size, copy_data);
    std::thread sender_thread(sender_thread_serialization_fn, sender_conf, N, fragment_size);
    // There is one extra byte at the start of the message for the
    // serialization type, and then 2  bytes of messagepack "header" if
    // the binary array size is less than 2^8-1; 3 bytes if less than 2^16-1, or 5 bytes if the
    // binary array size is greater than 2^16-1
    size_t header_bytes=1;
    size_t binary_size=fragment_size+sizeof(dunedaq::dataformats::FragmentHeader);
    if(binary_size < (1<<8)) header_bytes+=2;
    else if(binary_size < (1<<16)) header_bytes+=3;
    else header_bytes+=5;
    std::thread receiver_thread(receiver_thread_no_serialization_fn, receiver, N, header_bytes, copy_data);
    sender_thread.join();
    receiver_thread.join();
  }



  {
    std::this_thread::sleep_for(1s);
    dunedaq::serialization::networkobjectsender::Conf sender_conf;
    sender_conf.ipm_plugin_type = "ZmqSender";
    sender_conf.stype = "msgpack";
    sender_conf.address = "inproc://foo";
    
    dunedaq::serialization::networkobjectreceiver::Conf receiver_conf;
    receiver_conf.ipm_plugin_type = "ZmqReceiver";
    receiver_conf.address = "inproc://foo";

    Timer t("msgpack", "msgpack", N, fragment_size, true);
    std::thread sender_thread(sender_thread_serialization_fn, sender_conf, N, fragment_size);
    std::thread receiver_thread(receiver_thread_serialization_fn, receiver_conf, N);
    sender_thread.join();
    receiver_thread.join();
  }
  
  for(bool copy_data : {true, false})
  {
    std::this_thread::sleep_for(1s);
    std::shared_ptr<dunedaq::ipm::Sender> sender=dunedaq::ipm::make_ipm_sender("ZmqSender");
    sender->connect_for_sends({{"connection_string", "inproc://foo"}});

    std::shared_ptr<dunedaq::ipm::Receiver> receiver=dunedaq::ipm::make_ipm_receiver("ZmqReceiver");
    receiver->connect_for_receives({{"connection_string", "inproc://foo"}});

    Timer t("raw", "raw", N, fragment_size, copy_data);
    std::thread sender_thread(sender_thread_no_serialization_fn, sender, N, fragment_size);
    std::thread receiver_thread(receiver_thread_no_serialization_fn, receiver, N, 0, copy_data);
    sender_thread.join();
    receiver_thread.join();


  }

}
