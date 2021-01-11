/**
 * @file TriggerRecordHeaderData.hpp  TriggerRecordHeaderData struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORDHEADERDATA_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORDHEADERDATA_HPP_

#include "dataformats/ComponentRequest.hpp"
#include "dataformats/Types.hpp"

#include <ostream>
#include <vector>

namespace dunedaq {

namespace dataformats {

/**
 * @brief Additional data fields associated with a TriggerRecordHeader
 */
struct TriggerRecordHeaderData
{
#define TRIGGER_RECORD_HEADER_MAGIC 0x33334444
#define TRIGGER_RECORD_HEADER_VERSION 1

  /**
   * @brief Magic bytes used to identify a TriggerRecordHeaderData struct in a raw data stream
   */
  uint32_t trigger_record_header_marker = TRIGGER_RECORD_HEADER_MAGIC; // NOLINT(build/unsigned)
  /**
   * @brief Version of the TriggerRecordHeaderData structure
   */
  uint32_t version = TRIGGER_RECORD_HEADER_VERSION; // NOLINT(build/unsigned)

  /**
   * @brief Trigger Number
   */
  trigger_number_t trigger_number;
  /**
   * @brief Timestamp of the TriggerDecision
   */
  timestamp_t trigger_timestamp;

  /**
   * @brief Number of ComponentRequest objects stored in the TriggerRecordHeader
   */
  uint64_t num_requested_components; // NOLINT(build/unsigned)

  /**
   * @brief Run Number for the TriggerRecord
   */
  run_number_t run_number;
  /**
   * @brief Error bits for the TriggerRecord
   *
   * Defined error bits should be documented here
   */
  uint32_t error_bits{ 0 }; // NOLINT(build/unsigned)

  /**
   * @brief Type of the TriggerDecision
   */
  trigger_type_t trigger_type;
  uint64_t unused : 48; // NOLINT(build/unsigned)
};

/**
 * @brief Stream a TriggerRecordHeaderData instance in human-readable form
 * @param o Stream to write to
 * @param hdr TriggerRecordHeaderData to write
 * @return Stream instance for continued streaming
 */
inline std::ostream&
operator<<(std::ostream& o, TriggerRecordHeaderData const& hdr)
{
  return o << "check_word: " << std::hex << hdr.trigger_record_header_marker << std::dec << ", "
           << "version: " << hdr.version << ", "

           << "trigger_number: " << hdr.trigger_number << ", "
           << "run_number: " << hdr.run_number << ", "
           << "trigger_timestamp: " << hdr.trigger_timestamp << ", "
           << "trigger_type: " << hdr.trigger_type << ", "

           << "error_bits: " << hdr.error_bits << ", "

           << "num_requested_components: " << hdr.num_requested_components;
}

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORDHEADERDATA_HPP_
