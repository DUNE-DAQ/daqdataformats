/**
 * @file FragmentHeader.hpp  FragmentHeader struct definition
 *
 * The FragmentHeader represents the metadata describing the contents of a DUNE DAQ Fragment
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_FRAGMENTHEADER_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_FRAGMENTHEADER_HPP_

#include "daqdataformats/SourceID.hpp"
#include "daqdataformats/Types.hpp"

#include <bitset>
#include <cstddef>
#include <cstdlib>
#include <map>
#include <numeric>
#include <string>
#include <vector>

namespace dunedaq::daqdataformats {

/**
 * @brief The header for a DUNE Fragment
 */
struct FragmentHeader
{
  /// @brief Marker bytes to identify a FragmentHeader entry in a raw data stream
  static constexpr uint32_t s_fragment_header_marker = 0x11112222; // NOLINT(build/unsigned)

  static constexpr uint32_t s_fragment_header_version = 6; // NOLINT(build/unsigned)
  static constexpr uint32_t s_default_status_bits = 0;     // NOLINT(build/unsigned)

  uint32_t fragment_header_marker = s_fragment_header_marker; // NOLINT(build/unsigned)
  uint32_t version = s_fragment_header_version;               // NOLINT(build/unsigned)

  /// @brief Size of the Fragment (including header and payload)
  fragment_size_t size{ TypeDefaults::s_invalid_fragment_size }; // NOLINT(build/unsigned)

  /// @brief Trigger Number this Fragment is associated with
  trigger_number_t trigger_number{ TypeDefaults::s_invalid_trigger_number };

  /// @brief Timestamp of the TriggerDecision
  timestamp_t trigger_timestamp{ TypeDefaults::s_invalid_timestamp };

  /// @brief Window begin of data in the Fragment
  timestamp_t window_begin{ TypeDefaults::s_invalid_timestamp };

  /// @brief Window end of data in the Fragment
  timestamp_t window_end{ TypeDefaults::s_invalid_timestamp };

  run_number_t run_number{ TypeDefaults::s_invalid_run_number };

  /// @brief Status bits set by the Upstream DAQ
  uint32_t status_bits{ s_default_status_bits }; // NOLINT(build/unsigned)

  /// @brief Type of the Fragment, indicating the format of the contained payload
  fragment_type_t fragment_type{ TypeDefaults::s_invalid_fragment_type };

  /// @brief Sequence number of this Fragment within a trigger record
  sequence_number_t sequence_number{ TypeDefaults::s_invalid_sequence_number };

  /// @brief Identifier for the subdetector that produced the raw data in the Fragment payload
  uint16_t detector_id; // NOLINT(build/unsigned)

  /// @brief Component that generated the data in this Fragment
  SourceID element_id;
};

/// @brief All defined status bits, with a short documentation of their meaning if non-obvious
enum class FragmentStatusBits : size_t
{
  kLatencyBufferEmpty = 0,        ///< The latency buffer had zero occupancy when the data request was made
  kIncomplete = 1,                ///< Only part of the requested data is present in the fragment
  kInvalidRequestWindow = 2,      ///< The requested data window was too large
  kRequestTimeout = 3,            ///< A timeout occurred while processing the data request
  kRequestWindowBeforeBuffer = 4, ///< The request window extends before the latency buffer start
  kRequestWindowAfterBuffer = 5,  ///< The request window extends after the latency buffer end
  kEmptyFragment = 6,             ///< This Fragment contains no data
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
  kInvalid = 32 ///< Status bit 32 and higher are not valid (status_bits is only 32 bits)
};

/// @brief All defined Fragment types
enum class FragmentType : fragment_type_t
{
  kUnknown = 0,
  kProtoWIB = 1,
  kWIB = 2,
  kDAPHNE = 3,
  kTDE_AMC = 4,
  // This fragment type is for the "raw" data from the firmware
  // trigger primitive generation. We store this in fragments for the
  // purposes of inspecting/debugging the firmware TPG
  kFW_TriggerPrimitive = 5, ///< FW TP frame format
  // This fragment type is for TPs saved via the trigger subsystem's
  // TP buffer, which are in the format defined by
  // dunedaq::trgdataformats::TriggerPrimitive. It is also
  // used for the stream of TPs that are sent directly from the readout
  // subsystem to the dataflow subsystem for storage on disk.
  // TPs stored in this format may have been _originally_ produced
  // by either software _or_ firmware.
  kTriggerPrimitive = 6, ///< Trigger format TPs produced by trigger code
  kTriggerActivity = 7,
  kTriggerCandidate = 8,
  kHardwareSignal = 9,
  kPACMAN = 10,
  kMPD = 11,
  kWIBEth = 12,
  kDAPHNEStream = 13,
  kCRT = 14,
  kTDEEth = 15,
  kCRTBern = 16,
  kCRTGrenoble = 17,
  kDAPHNEEth = 18,
  kDAPHNEEthStream = 19
};

/**
 * @brief This map relates FragmentType values to string names
 *
 * These names can be used, for example, as HDF5 Group names
 */
inline std::map<FragmentType, std::string>
get_fragment_type_names()
{
  return {
    { FragmentType::kUnknown, "Unknown" },
    { FragmentType::kProtoWIB, "ProtoWIB" },
    { FragmentType::kWIB, "WIB" },
    { FragmentType::kDAPHNE, "DAPHNE" },
    { FragmentType::kDAPHNEStream, "DAPHNEStream" },
    { FragmentType::kTDE_AMC, "TDE_AMC" },
    { FragmentType::kFW_TriggerPrimitive, "FW_Trigger_Primitive" },
    { FragmentType::kTriggerPrimitive, "Trigger_Primitive" },
    { FragmentType::kTriggerActivity, "Trigger_Activity" },
    { FragmentType::kTriggerCandidate, "Trigger_Candidate" },
    { FragmentType::kHardwareSignal, "Hardware_Signal" },
    { FragmentType::kPACMAN, "PACMAN" },
    { FragmentType::kMPD, "MPD" },
    { FragmentType::kWIBEth, "WIBEth" },
    { FragmentType::kCRT, "CRT" },
    { FragmentType::kTDEEth, "TDEEth" },
    { FragmentType::kCRTBern, "CRTBern" },
    { FragmentType::kCRTGrenoble, "CRTGrenoble" },
    { FragmentType::kDAPHNEEth, "DAPHNEEth" },
    { FragmentType::kDAPHNEEthStream, "DAPHNEEthStream" },
  };
}

std::string
fragment_type_to_string(const FragmentType& type);

FragmentType
string_to_fragment_type(const std::string& name);

std::ostream&
operator<<(std::ostream& o, FragmentHeader const& hdr);

} // namespace dunedaq::daqdataformats

#include "detail/FragmentHeader.hxx"

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_FRAGMENTHEADER_HPP_
