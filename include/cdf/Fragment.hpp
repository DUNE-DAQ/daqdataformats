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

#ifndef dataformats_INCLUDE_dataformats_FRAGMENT_HPP_
#define dataformats_INCLUDE_dataformats_FRAGMENT_HPP_

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

    data_arr_ = std::vector<uint8_t>(size); // NOLINT(build/unsigned)

    FragmentHeader header;
    header.size = size;
    memcpy(&data_arr_[0], &header, sizeof(header));

    size_t offset = sizeof(FragmentHeader);
    for (auto& piece : pieces) {
      memcpy(&data_arr_[0] + offset, piece.first, piece.second); // NOLINT(build/unsigned)
      offset += piece.second;
    }
  }
  Fragment(void* buffer, size_t size)
    : Fragment({ std::make_pair(buffer, size) })
  {}

  FragmentHeader const& get_header() { return *header_(); }
  void set_header(FragmentHeader header) { memcpy(&data_arr_[0], &header, sizeof(header)); }

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

  fragment_size_t get_size() { return header_()->size; }
  void* data() { 
    // Increment header pointer by one to skip header
    return static_cast<void*>(header_() + 1);  // NOLINT
  }
private:
  FragmentHeader* header_() { return reinterpret_cast<FragmentHeader*>(&data_arr_[0]); } // NOLINT
  std::vector<uint8_t> data_arr_; // NOLINT(build/unsigned)
};
} // namespace dataformats
} // namespace dunedaq

#endif // dataformats_INCLUDE_dataformats_FRAGMENT_HPP_
