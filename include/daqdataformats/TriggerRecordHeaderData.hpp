/**
 * @file TriggerRecordHeaderData.hpp  TriggerRecordHeaderData struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TRIGGERRECORDHEADERDATA_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TRIGGERRECORDHEADERDATA_HPP_

#include "daqdataformats/ComponentRequest.hpp"
#include "daqdataformats/SourceID.hpp"
#include "daqdataformats/Types.hpp"

#include <cstddef>
#include <limits>
#include <ostream>
#include <string>
#include <vector>

namespace dunedaq::daqdataformats {

/**
 * @brief Data fields associated with a TriggerRecordHeader
 */
struct TriggerRecordHeaderData
{
  /// @brief Magic bytes used to identify a TriggerRecordHeaderData struct in a raw data stream
  static constexpr uint32_t s_trigger_record_header_magic = 0x33334444; // NOLINT(build/unsigned)

  static constexpr uint32_t s_trigger_record_header_version = 5; // NOLINT(build/unsigned)
  static constexpr uint64_t s_invalid_number_components = // NOLINT(build/unsigned)
    std::numeric_limits<uint64_t>::max();                 // NOLINT(build/unsigned)

  static constexpr uint32_t s_default_status_bits = 0; // NOLINT(build/unsigned)

  uint32_t trigger_record_header_marker = s_trigger_record_header_magic; // NOLINT(build/unsigned)
  uint32_t version = s_trigger_record_header_version; // NOLINT(build/unsigned)

  trigger_number_t trigger_number{ TypeDefaults::s_invalid_trigger_number };

  /// @brief Timestamp of the TriggerDecision
  timestamp_t trigger_timestamp{ TypeDefaults::s_invalid_timestamp };

  /// @brief Number of ComponentRequest objects stored in the TriggerRecordHeader
  uint64_t num_requested_components{ s_invalid_number_components }; // NOLINT(build/unsigned)

  run_number_t run_number{ TypeDefaults::s_invalid_run_number };

  uint32_t status_bits{ s_default_status_bits }; // NOLINT(build/unsigned)

  /// @brief Type of the TriggerDecision
  trigger_type_t trigger_type{ TypeDefaults::s_invalid_trigger_type };

  /// @brief Sequence number of this TriggerRecord within the trigger response
  sequence_number_t sequence_number{ TypeDefaults::s_invalid_sequence_number };

  /// @brief Maximum sequence number of TriggerRecords corresponding to this trigger
  sequence_number_t max_sequence_number{ TypeDefaults::s_invalid_sequence_number };

  uint32_t unused{ 0xFFFFFFFF }; // NOLINT(build/unsigned) Padding to ensure 64-bit alignment

  SourceID element_id;
};

/// @brief This enumeration lists all defined status bits, as well as a short documentation of their meaning when not obvious
enum class TriggerRecordStatusBits : size_t
{
  kIncomplete = 0,    ///< Indicates a trigger record that is missing requested components
  kMismatch = 1,      ///< We have as many fragments as requested but they do not match the requested components
  kUnassigned2 = 2,
  kUnassigned3 = 3,
  kUnassigned4 = 4,
  kUnassigned5 = 5,
  kUnassigned6 = 6,
  kUnassigned7 = 7,
  kUnassigned8 = 8,
  kUnassigned9 = 9,
  kUnassigned10 = 10,
  kUnassigned11 = 11,
  kUnassigned12 = 12,
  kUnassigned13 = 13,
  kUnassigned14 = 14,
  kUnassigned15 = 15,
  kUnassigned16 = 16,
  kUnassigned17 = 17,
  kUnassigned18 = 18,
  kUnassigned19 = 19,
  kUnassigned20 = 20,
  kUnassigned21 = 21,
  kUnassigned22 = 22,
  kUnassigned23 = 23,
  kUnassigned24 = 24,
  kUnassigned25 = 25,
  kUnassigned26 = 26,
  kUnassigned27 = 27,
  kUnassigned28 = 28,
  kUnassigned29 = 29,
  kUnassigned30 = 30,
  kUnassigned31 = 31,
  kInvalid = 32       ///< Status bit 32 and higher are not valid (status_bits is only 32 bits)
};

std::ostream&
operator<<(std::ostream& o, TriggerRecordHeaderData const& hdr);

} // namespace dunedaq::daqdataformats

#include "detail/TriggerRecordHeaderData.hxx"

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TRIGGERRECORDHEADERDATA_HPP_
