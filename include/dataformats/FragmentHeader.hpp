/**
 * @file FragmentHeader.hpp  FragmentHeader struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENTHEADER_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENTHEADER_HPP_

#include "dataformats/GeoID.hpp"
#include "dataformats/Types.hpp"

#include <bitset>
#include <cstdlib>
#include <numeric>
#include <vector>

namespace dunedaq {
namespace dataformats {

struct FragmentHeader
{
  static constexpr uint32_t FRAGMENT_HEADER_MAGIC = 0x11112222; // NOLINT(build/unsigned)
  static constexpr uint32_t FRAGMENT_HEADER_VERSION = 1;        // NOLINT(build/unsigned)

  uint32_t fragment_header_marker = FRAGMENT_HEADER_MAGIC; // NOLINT(build/unsigned)
  uint32_t version = FRAGMENT_HEADER_VERSION;              // NOLINT(build/unsigned)

  fragment_size_t size; // NOLINT(build/unsigned)

  trigger_number_t trigger_number;
  run_number_t run_number;

  timestamp_t trigger_timestamp;
  timestamp_diff_t window_offset;
  timestamp_diff_t window_width;

  GeoID link_ID;
  uint32_t error_bits; // NOLINT(build/unsigned)
  fragment_type_t fragment_type;
};

inline std::ostream&
operator<<(std::ostream& o, FragmentHeader const& hdr)
{
  return o << "version: " << hdr.version << ", "
           << "size: " << hdr.size << ", "
           << "trigger_number: " << hdr.trigger_number << ", "
           << "run_number: " << hdr.run_number << ", "
           << "trigger_timestamp: " << hdr.trigger_timestamp << ", "
           << "window_offset: " << hdr.window_offset << ", "
           << "window_width: " << hdr.window_width << ", "
           << "link_ID: " << hdr.link_ID << ", "
           << "error_bits: " << hdr.error_bits << ", "
           << "fragment_type : " << hdr.fragment_type;
}

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENTHEADER_HPP_