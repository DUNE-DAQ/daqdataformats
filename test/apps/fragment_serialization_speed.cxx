#include "dataformats/Fragment.hpp"
#include "ipm/Receiver.hpp"
#include "serialization/NetworkObjectReceiver.hpp"
#include "serialization/NetworkObjectSender.hpp"
#include "ipm/Sender.hpp"

#include <chrono>
#include <thread>

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
sender_thread_serialization_fn(dunedaq::serialization::networkobjectsender::Conf sender_conf, int n_messages, size_t fragment_size)
{
  dunedaq::NetworkObjectSender<dunedaq::dataformats::Fragment> sender(sender_conf);
  
  dunedaq::dataformats::Fragment frag=make_fragment(fragment_size);
  
  for (int i = 0; i < n_messages; ++i) {
    frag.set_run_number(i);
    sender.send(frag, std::chrono::milliseconds(1000000));
  }
}

void
receiver_thread_serialization_fn(dunedaq::serialization::networkobjectreceiver::Conf receiver_conf, int n_messages)
{
  using dunedaq::dataformats::Fragment;
  using dunedaq::dataformats::FragmentHeader;

  int total = 0;
  dunedaq::NetworkObjectReceiver<Fragment> receiver(receiver_conf);
  for (int i = 0; i < n_messages; ++i) {
    Fragment frag = receiver.recv(10000000ms);
    total += frag.get_run_number();
  }
  std::cout << "Total:" << total << std::endl;
}

void
sender_thread_no_serialization_fn(std::shared_ptr<dunedaq::ipm::Sender> sender, int n_messages, size_t fragment_size)
{
  dunedaq::dataformats::Fragment frag=make_fragment(fragment_size);
  
  for (int i = 0; i < n_messages; ++i) {
    frag.set_run_number(i);
    sender->send(frag.get_storage_location(), frag.get_size(), std::chrono::milliseconds(1000000));
  }
}

void
receiver_thread_no_serialization_fn(std::shared_ptr<dunedaq::ipm::Receiver> receiver, int n_messages)
{
  using dunedaq::dataformats::Fragment;
  using dunedaq::dataformats::FragmentHeader;

  int total = 0;
  for (int i = 0; i < n_messages; ++i) {
    auto recvd = receiver->receive(10000000ms);
    Fragment frag(recvd.m_data.data());
    total += frag.get_run_number();
  }
  std::cout << "Total:" << total << std::endl;
}

// Return the current steady clock in microseconds
inline uint64_t
now_us()
{
  using namespace std::chrono;
  // std::chrono is the worst
  return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}

int
main()
{

  {
    dunedaq::serialization::networkobjectsender::Conf sender_conf;
    sender_conf.ipm_plugin_type = "ZmqSender";
    sender_conf.stype = "msgpack";
    sender_conf.address = "inproc://foo";
    
    dunedaq::serialization::networkobjectreceiver::Conf receiver_conf;
    receiver_conf.ipm_plugin_type = "ZmqReceiver";
    receiver_conf.address = "inproc://foo";

    const int N = 1000000;
    uint64_t start_time = now_us();
    std::thread sender_thread(sender_thread_serialization_fn, sender_conf, N, 46400);
    std::thread receiver_thread(receiver_thread_serialization_fn, receiver_conf, N);
    sender_thread.join();
    receiver_thread.join();
    uint64_t end_time = now_us();
    double time_taken_s = 1e-6 * (end_time - start_time);
    double kHz = 1e-3 * N / time_taken_s;
    printf("Explicit serialization: Sent %d messages in %.3fs (%.1f kHz)\n", N, time_taken_s, kHz);
  }

  {
    std::shared_ptr<dunedaq::ipm::Sender> sender=dunedaq::ipm::make_ipm_sender("ZmqSender");
    sender->connect_for_sends({{"connection_string", "inproc://foo"}});

    std::shared_ptr<dunedaq::ipm::Receiver> receiver=dunedaq::ipm::make_ipm_receiver("ZmqReceiver");
    receiver->connect_for_receives({{"connection_string", "inproc://foo"}});

    const int N = 1000000;
    uint64_t start_time = now_us();
    std::thread sender_thread(sender_thread_no_serialization_fn, sender, N, 46400);
    std::thread receiver_thread(receiver_thread_no_serialization_fn, receiver, N);
    sender_thread.join();
    receiver_thread.join();
    uint64_t end_time = now_us();
    double time_taken_s = 1e-6 * (end_time - start_time);
    double kHz = 1e-3 * N / time_taken_s;
    printf("Implicit serialization: Sent %d messages in %.3fs (%.1f kHz)\n", N, time_taken_s, kHz);
  }

}
