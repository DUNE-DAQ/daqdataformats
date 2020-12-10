/**
 * @file TriggerRecordHeader.hpp  TriggerRecordHeader struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "GeoID.hpp"
#include "Types.hpp"

namespace dunedaq {
namespace cdf {

struct TriggerRecordHeader
{
  static constexpr uint32_t TRIGGER_RECORD_HEADER_MAGIC = 0x33334444;
  static constexpr uint32_t TRIGGER_RECORD_HEADER_VERSION = 1;

  uint32_t HeaderMarker = TRIGGER_RECORD_HEADER_MAGIC;
  uint32_t Version = TRIGGER_RECORD_HEADER_VERSION;

  trigger_number_t TriggerNumber;
  run_number_t RunNumber;
  timestamp_t TriggerTimestamp;
  trigger_type_t TriggerType;
};
} // namespace cdf
} // namespace dunedaq
