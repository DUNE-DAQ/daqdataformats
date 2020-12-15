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

#ifndef CDF_INCLUDE_CDF_FRAGMENT_HPP_
#define CDF_INCLUDE_CDF_FRAGMENT_HPP_

#include "cdf/FragmentHeader.hpp"
#include "cdf/GeoID.hpp"
#include "cdf/Types.hpp"

#include <bitset>
#include <cstdlib>
#include <cstring>
#include <numeric>
#include <utility>
#include <vector>

namespace dunedaq {
namespace cdf {

class Fragment
{
public:
  explicit Fragment(std::vector<std::pair<void*, size_t>> pieces)
  {
    size_ = std::accumulate(pieces.begin(), pieces.end(), 0ULL, [](auto& a, auto& b) { return a + b.second; });
    data_ = malloc(size_);

    size_t offset = 0;
    for (auto& piece : pieces) {
      memcpy(static_cast<uint8_t*>(data_) + offset, piece.first, piece.second); // NOLINT(build/unsigned)
      offset += piece.second;
    }
  }
  Fragment(void* buffer, size_t size)
    : Fragment({ std::make_pair(buffer, size) })
  {}

  FragmentHeader const& header() { return header_; }
  void set_header(FragmentHeader header) { header_ = header; }

  // Header setters and getters
  trigger_number_t trigger_number() { return header_.trigger_number; }
  void set_trigger_number(trigger_number_t trigger_number) { header_.trigger_number = trigger_number; }
  run_number_t run_number() { return header_.run_number; }
  void set_run_number(run_number_t run_number) { header_.run_number = run_number; }

  timestamp_t trigger_timestamp() { return header_.trigger_timestamp; }
  void set_trigger_timestamp(timestamp_t trigger_timestamp) { header_.trigger_timestamp = trigger_timestamp; }
  timestamp_t data_start() { return header_.data_start; }
  void set_data_start(timestamp_t data_start) { header_.data_start = data_start; }
  timestamp_t data_end() { return header_.data_end; }
  void set_data_end(timestamp_t data_end) { header_.data_end = data_end; }

  GeoID link_ID() { return header_.link_ID; }
  void set_link_ID(GeoID link_ID) { header_.link_ID = link_ID; }
  std::bitset<32> error_bits() { return header_.error_bits; }
  void set_error_bits(std::bitset<32> error_bits) { header_.error_bits = error_bits; }
  bool error_bit(size_t bit) { return header_.error_bits[bit]; }
  void set_error_bit(size_t bit, bool value) { header_.error_bits[bit] = value; }
  fragment_type_t fragment_type() { return header_.fragment_type; }
  void set_type(fragment_type_t fragment_type) { header_.fragment_type = fragment_type; }

  size_t size() { return size_; }
  void* data() { return data_; }

private:
  FragmentHeader header_;

  void* data_;
  size_t size_;
};
} // namespace cdf
} // namespace dunedaq

#endif // CDF_INCLUDE_CDF_FRAGMENT_HPP_