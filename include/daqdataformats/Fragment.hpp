/**
 * @file Fragment.hpp Data Fragment Interface
 *
 * This class respresents the data response of one element to a Dataflow DataRequest message.
 * This version is not serializable, and classes wanting to stream it/write it to disk must
 * also fetch the data associated with the Fragment.
 *
 * A Fragment can be expected to begin with a Fragment header, usually followed by a payload
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_FRAGMENT_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_FRAGMENT_HPP_

#include "daqdataformats/FragmentHeader.hpp"
#include "daqdataformats/SourceID.hpp"
#include "daqdataformats/Types.hpp"

#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dunedaq::daqdataformats {

/**
 * @brief C++ Representation of a DUNE Fragment, wrapping the flat byte array that is the Fragment's "actual" form
 */
class Fragment
{
public:
  /// @brief Describes how the "existing Fragment buffer" constructor should treat the given buffer
  enum class BufferAdoptionMode
  {
    kTakeOverBuffer, ///< Take over control of the buffer
    kCopyFromBuffer  ///< Copy the contents of the buffer into a new Fragment array
  };

  /**
   * @brief Fragment constructor using a vector of buffer pointers
   * @param pieces Vector of pairs of pointer/size pairs used to initialize Fragment payload
   */
  explicit Fragment(const std::vector<std::pair<void*, size_t>>& pieces);
  /**
   * @brief Fragment constructor using a buffer and size
   * @param buffer Pointer to Fragment payload
   * @param size Size of payload
   */
  Fragment(void* buffer, size_t size);
  /**
   * @brief Fragment constructor using existing Fragment array
   * @param existing_fragment_buffer Pointer to existing Fragment array
   * @param adoption_mode How the constructor should treat the existing_fragment_buffer
   */
  explicit Fragment(void* existing_fragment_buffer, BufferAdoptionMode adoption_mode);

  FragmentHeader get_header() const { return *header_(); }

  /// @brief Fields from the provided header overwrite this Fragment's header, *except* for the size field
  void set_header_fields(const FragmentHeader& header);

  /// @brief Get read-only access to the Fragment's underlying data array via a pointer
  const void* get_storage_location() const { return m_data_arr; }

  trigger_number_t get_trigger_number() const { return header_()->trigger_number; }
  void set_trigger_number(trigger_number_t trigger_number) { header_()->trigger_number = trigger_number; }

  run_number_t get_run_number() const { return header_()->run_number; }
  void set_run_number(run_number_t run_number) { header_()->run_number = run_number; }

  timestamp_t get_trigger_timestamp() const { return header_()->trigger_timestamp; }
  void set_trigger_timestamp(timestamp_t trigger_timestamp) { header_()->trigger_timestamp = trigger_timestamp; }

  timestamp_t get_window_begin() const { return header_()->window_begin; }
  void set_window_begin(timestamp_t window_begin) { header_()->window_begin = window_begin; }

  timestamp_t get_window_end() const { return header_()->window_end; }
  void set_window_end(timestamp_t window_end) { header_()->window_end = window_end; }

  /**
   * @brief Get the SourceID for the Fragment
   * @return The element_id header field
   */
  SourceID get_element_id() const { return header_()->element_id; }

  /**
   * @brief Set the SourceID for the Fragment
   * @param element_id SourceID to use as element_id
   */
  void set_element_id(SourceID element_id) { header_()->element_id = element_id; }

  uint16_t get_detector_id() const noexcept { return header_()->detector_id; }                         // NOLINT
  void set_detector_id(const uint16_t& detector_id) noexcept { header_()->detector_id = detector_id; } // NOLINT

  /**
   * @brief Get the status_bits header field
   * @return Bitset generated from header's status_bits field
   */
  std::bitset<32> get_status_bits() const { return header_()->status_bits; }
  /**
   * @brief Overwrite the status_bits header field
   * @param status_bits Bitset of status bits to set
   */
  void set_status_bits(std::bitset<32> status_bits) { header_()->status_bits = status_bits.to_ulong(); }
  /**
   * @brief Get the value of a designated status bit
   * @param bit Bit to query
   * @return Value of bit (true/false)
   */
  bool get_status_bit(FragmentStatusBits bit) const { return get_status_bits()[static_cast<size_t>(bit)]; }

  /**
   * @brief Set the designated status bit
   * @param bit Bit to set
   * @param value Value (true/false) for the status bit
   */
  void set_status_bit(FragmentStatusBits bit, bool value);

  /**
   * @brief Get the fragment_type_t value stored in the header
   * @return Current value of the fragment_type header field
   */
  fragment_type_t get_fragment_type_code() const { return header_()->fragment_type; }
  /**
   * @brief Get the fragment_type header field
   * @return Current value of the fragment_type header field
   */
  FragmentType get_fragment_type() const { return static_cast<FragmentType>(get_fragment_type_code()); }
  /**
   * @brief Set the fragment_type header field
   * @param fragment_type Value to set
   */
  void set_type(FragmentType fragment_type) { header_()->fragment_type = static_cast<fragment_type_t>(fragment_type); }

  sequence_number_t get_sequence_number() const { return header_()->sequence_number; }
  void set_sequence_number(sequence_number_t number) { header_()->sequence_number = number; }

  /// @brief Get the total size of the Fragment in bytes, including header and all payload pieces
  fragment_size_t get_size() const { return header_()->size; }

  /// @brief Get the size of the Fragment payload in bytes (total size minus FragmentHeader)
  fragment_size_t get_data_size() const { return header_()->size - sizeof(FragmentHeader); }

  /// @brief Get a pointer to the data payload in the Fragmnet
  void* get_data() const
  {
    // Increment header pointer by one to skip header
    return static_cast<void*>(header_() + 1); // NOLINT
  }

  Fragment(Fragment const&) = delete;
  Fragment& operator=(Fragment const&) = delete;
  Fragment(Fragment&& other)
  {
    m_alloc = other.m_alloc;
    other.m_alloc = false;
    m_data_arr = other.m_data_arr;
  }
  Fragment& operator=(Fragment&& other)
  {
    m_alloc = other.m_alloc;
    other.m_alloc = false;
    m_data_arr = other.m_data_arr;
    return *this;
  }

  ~Fragment();

private:
  FragmentHeader* header_() const { return static_cast<FragmentHeader*>(m_data_arr); }
  void* m_data_arr{ nullptr }; ///< Points to flat memory containing a FragmentHeader and the data payload
  bool m_alloc{ false };       ///< Whether the Fragment owns the memory pointed by m_data_arr
};

inline Fragment::Fragment(const std::vector<std::pair<void*, size_t>>& pieces)
{

  size_t size =
    sizeof(FragmentHeader) +
    std::accumulate(pieces.begin(), pieces.end(), 0ULL, [](const size_t& a, const std::pair<void*, size_t>& b) {
      return a + b.second;
    });

  m_data_arr = malloc(size); // NOLINT
  if (m_data_arr == nullptr) {
    throw std::bad_alloc();
  }
  m_alloc = true;

  FragmentHeader header;
  header.size = size;
  memcpy(m_data_arr, &header, sizeof(header));

  size_t offset = sizeof(header);
  for (auto& piece : pieces) {
    if (piece.first == nullptr) {
      free(m_data_arr); // NOLINT
      throw std::invalid_argument("The Fragment buffer points to NULL.");
    }
    memcpy(static_cast<uint8_t*>(m_data_arr) + offset, piece.first, piece.second); // NOLINT
    offset += piece.second;
  }
}

inline Fragment::Fragment(void* buffer, size_t size)
  : Fragment({ std::make_pair(buffer, size) })
{
}

inline Fragment::Fragment(void* existing_fragment_buffer, BufferAdoptionMode adoption_mode)
{
  if (adoption_mode == BufferAdoptionMode::kTakeOverBuffer) {
    m_data_arr = existing_fragment_buffer;
    m_alloc = true;
  } else if (adoption_mode == BufferAdoptionMode::kCopyFromBuffer) {
    auto header = reinterpret_cast<FragmentHeader*>(existing_fragment_buffer); // NOLINT
    m_data_arr = malloc(header->size);                                         // NOLINT
    if (m_data_arr == nullptr) {
      throw std::bad_alloc();
    }
    m_alloc = true;
    memcpy(m_data_arr, existing_fragment_buffer, header->size);
  }
}

inline Fragment::~Fragment()
{
  if (m_alloc)
    free(m_data_arr); // NOLINT
}

inline void
Fragment::set_header_fields(const FragmentHeader& header)
{
  FragmentHeader* header_ptr{ header_() };
  fragment_size_t orig_size{ header_ptr->size };

  *header_ptr = header;
  header_ptr->size = orig_size;
}

inline void
Fragment::set_status_bit(FragmentStatusBits bit, bool value)

{
  auto bits = get_status_bits();
  bits[static_cast<size_t>(bit)] = value;
  set_status_bits(bits);
}

} // namespace dunedaq::daqdataformats

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_FRAGMENT_HPP_
