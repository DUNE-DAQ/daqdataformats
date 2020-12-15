/**
 * @file FragmentHeader.hpp  FragmentHeader struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef CDF_INCLUDE_CDF_FRAGMENTHEADER_HPP_
#define CDF_INCLUDE_CDF_FRAGMENTHEADER_HPP_

#include "cdf/GeoID.hpp"
#include "cdf/Types.hpp"

#include <bitset>
#include <cstdlib>
#include <numeric>
#include <vector>

namespace dunedaq {
namespace cdf {

struct FragmentHeader
{
  static constexpr uint32_t FRAGMENT_HEADER_MAGIC = 0x11112222; // NOLINT(build/unsigned)
  static constexpr uint32_t FRAGMENT_HEADER_VERSION = 1;        // NOLINT(build/unsigned)

  uint32_t fragment_header_marker = FRAGMENT_HEADER_MAGIC; // NOLINT(build/unsigned)
  uint32_t version = FRAGMENT_HEADER_VERSION;    // NOLINT(build/unsigned)
  trigger_number_t trigger_number;
  run_number_t run_number;

  timestamp_t trigger_timestamp;
  timestamp_t data_start;
  timestamp_t data_end;

  GeoID link_ID;
  std::bitset<32> error_bits;
  fragment_type_t fragment_type;
};
} // namespace cdf
} // namespace dunedaq

#endif // CDF_INCLUDE_CDF_FRAGMENTHEADER_HPP_