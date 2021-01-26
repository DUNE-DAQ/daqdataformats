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
/**
 * @brief Magic bytes to identify a TriggerRecordHeader entry in a raw data stream
 */
  static constexpr uint32_t s_trigger_record_header_magic = 0x33334444; // NOLINT(build/unsigned)
/**
 * @brief The current version of the TriggerRecordHeader
 */
  static constexpr uint32_t s_trigger_record_header_version = 1; // NOLINT(build/unsigned)

  /**
   * @brief Magic bytes used to identify a TriggerRecordHeaderData struct in a raw data stream
   */
  uint32_t m_trigger_record_header_marker = s_trigger_record_header_magic; // NOLINT(build/unsigned)
  /**
   * @brief Version of the TriggerRecordHeaderData structure
   */
  uint32_t m_version = s_trigger_record_header_version; // NOLINT(build/unsigned)

  /**
   * @brief Trigger Number
   */
  trigger_number_t m_trigger_number{ 0 };
  /**
   * @brief Timestamp of the TriggerDecision
   */
  timestamp_t m_trigger_timestamp{ 0 };

  /**
   * @brief Number of ComponentRequest objects stored in the TriggerRecordHeader
   */
  uint64_t m_num_requested_components{ 0 }; // NOLINT(build/unsigned)

  /**
   * @brief Run Number for the TriggerRecord
   */
  run_number_t m_run_number{ 0 };
  /**
   * @brief Error bits for the TriggerRecord
   *
   * Defined error bits should be documented here
   */
  uint32_t m_error_bits{ 0 }; // NOLINT(build/unsigned)

  /**
   * @brief Type of the TriggerDecision
   */
  trigger_type_t m_trigger_type{ 0 };
  /**
   * @brief Padding to ensure 64-bit alignment
   */
  uint64_t m_unused : 48; // NOLINT(build/unsigned)
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
  return o << "check_word: " << std::hex << hdr.m_trigger_record_header_marker << std::dec << ", "
           << "version: " << hdr.m_version << ", "

           << "trigger_number: " << hdr.m_trigger_number << ", "
           << "run_number: " << hdr.m_run_number << ", "
           << "trigger_timestamp: " << hdr.m_trigger_timestamp << ", "
           << "trigger_type: " << hdr.m_trigger_type << ", "

           << "error_bits: " << hdr.m_error_bits << ", "

           << "num_requested_components: " << hdr.m_num_requested_components;
}

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORDHEADERDATA_HPP_
