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
#include "cdf/ComponentRequest.hpp"

#include <vector>

namespace dunedaq {
namespace cdf {

struct TriggerRecordHeader
{
  static constexpr uint32_t TRIGGER_RECORD_HEADER_MAGIC = 0x33334444; // NOLINT(build/unsigned)
  static constexpr uint32_t TRIGGER_RECORD_HEADER_VERSION = 1;        // NOLINT(build/unsigned)

  uint32_t trigger_record_header_marker = TRIGGER_RECORD_HEADER_MAGIC; // NOLINT(build/unsigned)
  uint32_t version = TRIGGER_RECORD_HEADER_VERSION;    // NOLINT(build/unsigned)

  trigger_number_t trigger_number;
  run_number_t run_number;
  timestamp_t trigger_timestamp;
  trigger_type_t trigger_type;

  timestamp_t trigger_record_start_time;
  timestamp_t trigger_record_end_time;

  std::vector<ComponentRequest> requested_components;
  std::vector<ComponentRequest> included_components;
};
} // namespace cdf
} // namespace dunedaq

#endif // CDF_INCLUDE_CDF_TRIGGERRECORDHEADER_HPP_