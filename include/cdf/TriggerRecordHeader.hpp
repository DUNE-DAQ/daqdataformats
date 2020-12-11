/**
 * @file TriggerRecordHeader.hpp  TriggerRecordHeader struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef CDF_INCLUDE_CDF_TRIGGERRECORDHEADER_HPP_
#define CDF_INCLUDE_CDF_TRIGGERRECORDHEADER_HPP_

#include "cdf/Types.hpp"

namespace dunedaq {
namespace cdf {

struct TriggerRecordHeader
{
  static constexpr uint32_t TRIGGER_RECORD_HEADER_MAGIC = 0x33334444; // NOLINT(build/unsigned)
  static constexpr uint32_t TRIGGER_RECORD_HEADER_VERSION = 1;        // NOLINT(build/unsigned)

  uint32_t HeaderMarker = TRIGGER_RECORD_HEADER_MAGIC; // NOLINT(build/unsigned)
  uint32_t Version = TRIGGER_RECORD_HEADER_VERSION;    // NOLINT(build/unsigned)

  trigger_number_t TriggerNumber;
  run_number_t RunNumber;
  timestamp_t TriggerTimestamp;
  trigger_type_t TriggerType;
};
} // namespace cdf
} // namespace dunedaq

#endif // CDF_INCLUDE_CDF_TRIGGERRECORDHEADER_HPP_