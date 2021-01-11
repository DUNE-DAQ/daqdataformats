/**
 * @file Fragment.hpp Data Fragment Interface
 *
 * This class respresents the data response of one link to a Dataflow DataRequest message.
 * This version is not serializable, and classes wanting to stream it/write it to disk must
 * also fetch the data associated with the Fragment.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENT_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENT_HPP_

#include "dataformats/FragmentHeader.hpp"
#include "dataformats/GeoID.hpp"
#include "dataformats/Types.hpp"

#include <bitset>
#include <cstdlib>
#include <cstring>
#include <numeric>
#include <utility>
#include <vector>

namespace dunedaq {
namespace dataformats {

/**
 * @brief C++ Representation of a DUNE Fragment, wrapping the flat byte array that is the Fragment's "actual" form
*/
class Fragment
{
public:
  /**
   * @brief Fragment constructor using a vector of buffer pointers
   * @param pieces Vector of pairs of pointer/size pairs used to initialize Fragment payload
  */
  explicit Fragment(std::vector<std::pair<void*, size_t>> pieces)
  {
    size_t size = sizeof(FragmentHeader) +
                  std::accumulate(pieces.begin(), pieces.end(), 0ULL, [](auto& a, auto& b) { return a + b.second; });

    data_arr_ = malloc(size); // NOLINT(build/unsigned)
    alloc_ = true;

    FragmentHeader header;
    header.size = size;
    memcpy(data_arr_, &header, sizeof(header));

    size_t offset = sizeof(FragmentHeader);
    for (auto& piece : pieces) {
      memcpy(static_cast<uint8_t*>(data_arr_) + offset, piece.first, piece.second); // NOLINT(build/unsigned)
      offset += piece.second;
    }
  }
  /**
   * @brief Fragment constructor using a buffer and size
   * @param buffer Pointer to Fragment payload
   * @param size Size of payload
  */
  Fragment(void* buffer, size_t size)
    : Fragment({ std::make_pair(buffer, size) })
  {}
  /**
   * @brief Framgnet constructor using existing Fragment array
   * @param existing_fragment_buffer Pointer to existing Fragment array
   * @param copy_from_buffer Whether the Fragment should be a copy of the buffer (true), or just use the pointer (but not take ownership) (false)
  */
  explicit Fragment(void* existing_fragment_buffer, bool copy_from_buffer = false)
  {
    if (!copy_from_buffer) {
      data_arr_ = existing_fragment_buffer;
    } else {
      auto header = reinterpret_cast<FragmentHeader*>(existing_fragment_buffer); // NOLINT
      data_arr_ = malloc(header->size);
      alloc_ = true;
      memcpy(data_arr_, existing_fragment_buffer, header->size);
    }
  }

  Fragment(Fragment const&) = delete; ///< Fragment copy constructor is deleted
  Fragment(Fragment&&) = default; ///< Default Fragment move constructor
  Fragment& operator=(Fragment const&) = delete; ///< Fragment copy assignment operator is deleted
  Fragment& operator=(Fragment&&) = default; ///< Default move assignment operator

  /**
  * @brief Fragment destructor
   */
  ~Fragment()
  {
    if (alloc_)
      free(data_arr_);
  }

  /**
   * @brief Get a copy of the FragmentHeader struct
   * @return A copy of the FragmentHeader struct stored in this Fragment
   */
  FragmentHeader const& get_header() const { return *header_(); }
  /**
   * @brief Copy fields from the provided header in this Fragment's header
   * @param header Header to copy into the Fragment data array
   *
   * The fragment_header_marker, version and size FragmentHeader fields are *not* copied from the given FragmentHeader
   */
  void set_header_fields(FragmentHeader header)
  {
    header_()->trigger_number = header.trigger_number;
    header_()->trigger_timestamp = header.trigger_timestamp;
    header_()->window_offset = header.window_offset;
    header_()->window_width = header.window_width;
    header_()->run_number = header.run_number;
    header_()->link_ID = header.link_ID;
    header_()->error_bits = header.error_bits;
    header_()->fragment_type = header.fragment_type;
  }
  /**
   * @brief Get a pointer to the Fragment's data array for I/O
   * @return Pointer to the Fragment's data array
   */
  void* get_storage_location() { return data_arr_; }

  // Header setters and getters
  /**
  * @brief Get the trigger_number field from the header
  * @return The trigger_number header field
  */
  trigger_number_t get_trigger_number() const { return header_()->trigger_number; }
  /**
   * @brief Set the trigger_number for the Fragment
   * @param trigger_number Value of trigger_number to set
  */
  void set_trigger_number(trigger_number_t trigger_number) { header_()->trigger_number = trigger_number; }
  /**
   * @brief Get the run_number field from the header
   * @return The run_number header field
  */
  run_number_t get_run_number() const { return header_()->run_number; }
  /**
   * @brief Set the run_number for the Fragment
   * @param run_number Value of run_number to set
  */
  void set_run_number(run_number_t run_number) { header_()->run_number = run_number; }

  /**
   * @brief Get the trigger_timestamp field from the header
   * @return The trigger_timestamp header field
  */
  timestamp_t get_trigger_timestamp() const { return header_()->trigger_timestamp; }
  /**
   * @brief Set the trigger_timestamp for the Fragment
   * @param trigger_timestamp Value of trigger_timestamp to set
  */
  void set_trigger_timestamp(timestamp_t trigger_timestamp) { header_()->trigger_timestamp = trigger_timestamp; }
  /**
   * @brief Get the window_offset field from the header
   * @return The window_offset header field
  */
  timestamp_diff_t get_window_offset() const { return header_()->window_offset; }
  /**
   * @brief Set the window_offset for the Fragment
   * @param window_offset Value of the window_offset to set
  */
  void set_window_offset(timestamp_diff_t window_offset) { header_()->window_offset = window_offset; }
  /**
   * @brief Get the window_width field from the header
   * @return The window_width header field
  */
  timestamp_diff_t get_window_width() const { return header_()->window_width; }
  /**
   * @brief Set the window_width for the Fragment
   * @param window_width Value of the window_width to set
  */
  void set_window_width(timestamp_diff_t window_width) { header_()->window_width = window_width; }

  /**
   * @brief Get the GeoID for the Fragment
   * @return The link_id header field
  */
  GeoID get_link_id() const { return header_()->link_id; }
  /**
   * @brief Set the GeoID for the Fragment
   * @param link_id GeoID to use as link_id
  */
  void set_link_id(GeoID link_id) { header_()->link_id = link_id; }
  /**
   * @brief Get the error_bits header field
   * @return Bitset generated from header's error_bits field
  */
  std::bitset<32> get_error_bits() const { return header_()->error_bits; }
  /**
   * @brief Overwrite the error_bits header field
   * @param error_bits Bitset of error bits to set
  */
  void set_error_bits(std::bitset<32> error_bits) { header_()->error_bits = error_bits.to_ulong(); }
  /**
   * @brief Get the value of a designated error bit
   * @param bit Bit to query
   * @return Value of bit (true/false)
  */
  bool get_error_bit(size_t bit) const { return get_error_bits()[bit]; }
  /**
   * @brief Set the designated error bit
   * @param bit Bit to set
   * @param value Value (true/false) for the error bit
  */
  void set_error_bit(size_t bit, bool value)
  {
    auto bits = get_error_bits();
    bits[bit] = value;
    set_error_bits(bits);
  }
  /**
   * @brief Get the fragment_type header field
   * @return Current value of the fragment_type header field
  */
  fragment_type_t get_fragment_type() const { return header_()->fragment_type; }
  /**
   * @brief Set the fragment_type header field
   * @param fragment_type Value to set
  */
  void set_type(fragment_type_t fragment_type) { header_()->fragment_type = fragment_type; }

  /**
   * @brief Get the total size of the Fragment
   * @return The size of the Fragment, including header and all payload pieces
   */
  fragment_size_t get_size() const { return header_()->size; }
  /**
   * @brief Get a pointer to the data payload in the Fragmnet
   * @return Pointer to the data payload in the Fragment
  */
  void* data()
  {
    // Increment header pointer by one to skip header
    return static_cast<void*>(header_() + 1); // NOLINT
  }

private:
  /**
   * @brief Get the FragmentHeader from the data_arr_ array
   * @return Pointer to the FragmentHeader
   */
  FragmentHeader* header_() const { return static_cast<FragmentHeader*>(data_arr_); } // NOLINT
  void* data_arr_{ nullptr }; ///< Flat memory containing a FragmentHeader and the data payload
  bool alloc_{ false }; ///< Whether the Fragment owns the memory pointed by data_arr_
};
} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENT_HPP_
