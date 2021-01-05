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

class Fragment
{
public:
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
  Fragment(void* buffer, size_t size)
    : Fragment({ std::make_pair(buffer, size) })
  {}
  Fragment(void* existing_fragment_buffer, bool copy_from_buffer = false)
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

  ~Fragment()
  {
    if (alloc_)
      free(data_arr_);
  }

  FragmentHeader const& get_header() { return *header_(); }
  void set_header(FragmentHeader header) { memcpy(data_arr_, &header, sizeof(header)); }
  void* get_storage_location() { return data_arr_; }

  // Header setters and getters
  trigger_number_t get_trigger_number() { return header_()->trigger_number; }
  void set_trigger_number(trigger_number_t trigger_number) { header_()->trigger_number = trigger_number; }
  run_number_t get_run_number() { return header_()->run_number; }
  void set_run_number(run_number_t run_number) { header_()->run_number = run_number; }

  timestamp_t get_trigger_timestamp() { return header_()->trigger_timestamp; }
  void set_trigger_timestamp(timestamp_t trigger_timestamp) { header_()->trigger_timestamp = trigger_timestamp; }
  timestamp_diff_t get_window_offset() { return header_()->window_offset; }
  void set_window_offset(timestamp_diff_t window_offset) { header_()->window_offset = window_offset; }
  timestamp_diff_t get_window_width() { return header_()->window_width; }
  void set_window_width(timestamp_diff_t window_width) { header_()->window_width = window_width; }

  GeoID get_link_ID() { return header_()->link_ID; }
  void set_link_ID(GeoID link_ID) { header_()->link_ID = link_ID; }
  std::bitset<32> get_error_bits() { return header_()->error_bits; }
  void set_error_bits(std::bitset<32> error_bits) { header_()->error_bits = error_bits.to_ulong(); }
  bool get_error_bit(size_t bit) { return get_error_bits()[bit]; }
  void set_error_bit(size_t bit, bool value)
  {
    auto bits = get_error_bits();
    bits[bit] = value;
    set_error_bits(bits);
  }
  fragment_type_t get_fragment_type() { return header_()->fragment_type; }
  void set_type(fragment_type_t fragment_type) { header_()->fragment_type = fragment_type; }

  /**
   * @brief Get the total size of the Fragment
   * @return The size of the Fragment, including header and all payload pieces
   */
  fragment_size_t get_size() { return header_()->size; }
  void* data()
  {
    // Increment header pointer by one to skip header
    return static_cast<void*>(header_() + 1); // NOLINT
  }

private:
  FragmentHeader* header_() { return static_cast<FragmentHeader*>(data_arr_); } // NOLINT
  void* data_arr_{ nullptr };
  bool alloc_{ false };
};
} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENT_HPP_
