/**
 * @file FragmentHeader.hpp  FragmentHeader struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "GeoID.hpp"
#include "Types.hpp"

#include <bitset>
#include <cstdlib>
#include <numeric>
#include <pair>
#include <vector>

namespace dunedaq {
namespace cdf {

struct FragmentHeader
{
  static constexpr uint32_t FRAGMENT_HEADER_MAGIC = 0x11112222;
  static constexpr uint32_t FRAGMENT_HEADER_VERSION = 1;

  uint32_t HeaderMarker = FRAGMENT_HEADER_MAGIC;
  uint32_t Version = FRAGMENT_HEADER_VERSION;
  trigger_number_t TriggerNumber;
  run_number_t RunNumber;

  timestamp_t TriggerTimestamp;
  timestamp_t DataStart;
  timestamp_t DataEnd;

  GeoID LinkID;
  std::bitset<32> ErrorBits;
  // fragment_id_t FragmentID;
  type_t Type;
};
} // namespace cdf
} // namespace dunedaq