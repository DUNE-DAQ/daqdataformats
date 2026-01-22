/**
 * @file FragmentHeader.hpp  FragmentHeader struct definition
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
  /**
   * @brief Marker bytes to identify a FragmentHeader entry in a raw data stream
   */
  static constexpr uint32_t s_fragment_header_marker = 0x11112222; // NOLINT(build/unsigned)

  /**
   * @brief The current version of the Fragment
   */
  static constexpr uint32_t s_fragment_header_version = 5; // NOLINT(build/unsigned)

  /**
   * @brief By default, all status bits are unset
   */
  static constexpr uint32_t s_default_status_bits = 0; // NOLINT(build/unsigned)

  /**
   * @brief Marker Bytes used to identify FragmentHeaders in a raw data stream
   */
  uint32_t fragment_header_marker = s_fragment_header_marker; // NOLINT(build/unsigned)

  /**
   * @brief Version of the FragmentHeader
   */
  uint32_t version = s_fragment_header_version; // NOLINT(build/unsigned)

  /**
   * @brief Size of the Fragment (including header and payload)
   */
  fragment_size_t size{ TypeDefaults::s_invalid_fragment_size }; // NOLINT(build/unsigned)

  /**
   * @brief Trigger Number this Fragment is associated with
   */
  trigger_number_t trigger_number{ TypeDefaults::s_invalid_trigger_number };

  /**
   * @brief Timestamp of the TriggerDecision
   */
  timestamp_t trigger_timestamp{ TypeDefaults::s_invalid_timestamp };

  /**
   * @brief Window begin of data in the Fragment
   */
  timestamp_t window_begin{ TypeDefaults::s_invalid_timestamp };

  /**
   * @brief Window end of data in the Fragment
   */
  timestamp_t window_end{ TypeDefaults::s_invalid_timestamp };

  /**
   * @brief Run number this Fragment is associated with
   */
  run_number_t run_number{ TypeDefaults::s_invalid_run_number };

  /**
   * @brief Status bits set by the Upstream DAQ
   */
  uint32_t status_bits{ s_default_status_bits }; // NOLINT(build/unsigned)

  /**
   * @brief Type of the Fragment, indicating the format of the contained payload
   */
  fragment_type_t fragment_type{ TypeDefaults::s_invalid_fragment_type };

  /**
   * @brief Sequence number of this Fragment within a trigger record
   */
  sequence_number_t sequence_number{ TypeDefaults::s_invalid_sequence_number };

  /**
   * @brief Identifier for the subdetector that produced the raw data in the Fragment payload
   */

  uint16_t detector_id;

  /**
   * @brief Component that generated the data in this Fragment
   */
  SourceID element_id;

  //  uint32_t unused2{ // NOLINT(build/unsigned)
  //                   0xFFFFFFFF
  //  }; ///< Padding to ensure 64-bit alignment of FragmentHeader basic fields
};

static_assert(FragmentHeader::s_fragment_header_version == 5,
              "This is intentionally designed to tell the developer to update the static_assert checks (including this "
              "one) when the version is bumped");

static_assert(sizeof(FragmentHeader) == 72, "FragmentHeader struct size different than expected!");
static_assert(offsetof(FragmentHeader, fragment_header_marker) == 0,
              "FragmentHeader fragment_header_marker field not at expected offset!");
static_assert(offsetof(FragmentHeader, version) == 4, "FragmentHeader version field not at expected offset!");
static_assert(offsetof(FragmentHeader, size) == 8, "FragmentHeader size field not at expected offset!");
static_assert(offsetof(FragmentHeader, trigger_number) == 16,
              "FragmentHeader trigger_number field not at expected offset!");
static_assert(offsetof(FragmentHeader, trigger_timestamp) == 24,
              "FragmentHeader trigger_timestamp field not at expected offset!");
static_assert(offsetof(FragmentHeader, window_begin) == 32,
              "FragmentHeader window_begin field not at expected offset!");
static_assert(offsetof(FragmentHeader, window_end) == 40, "FragmentHeader window_end field not at expected offset!");
static_assert(offsetof(FragmentHeader, run_number) == 48, "FragmentHeader run_number field not at expected offset!");
static_assert(offsetof(FragmentHeader, status_bits) == 52, "FragmentHeader status_bits field not at expected offset!");
static_assert(offsetof(FragmentHeader, fragment_type) == 56,
              "FragmentHeader fragment_type field not at expected offset!");
static_assert(offsetof(FragmentHeader, sequence_number) == 60,
              "FragmentHeader sequence_number field not at expected offset!");
static_assert(offsetof(FragmentHeader, detector_id) == 62, "FragmentHeader detector_id field not at expected offset!");
static_assert(offsetof(FragmentHeader, element_id) == 64, "FragmentHeader element_id field not at expected offset!");

/**
 * @brief This enumeration should list all defined status bits, as well as a short documentation of their meaning
 */
enum class FragmentStatusBits : size_t
{
  kLatencyBufferEmpty = 0,        ///< The latency buffer had zero occupancy when the data request was made
  kIncomplete = 1,                ///< Only part of the requested data is present in the fragment
  kInvalidRequestWindow = 2,      ///< The requested data window was too large
  kRequestTimeout = 3,            ///< A timeout occurred while processing the data request
  kRequestWindowBeforeBuffer = 4, ///< The request window extends before the latency buffer start
  kRequestWindowAfterBuffer = 5,  ///< The request window extends after the latency buffer end
  kEmptyFragment = 6,             ///< This Fragment contains no data
  kUnassigned7 = 7,               ///< Status bit 7 is not assigned
  kUnassigned8 = 8,               ///< Status bit 8 is not assigned
  kUnassigned9 = 9,               ///< Status bit 9 is not assigned
  kUnassigned10 = 10,             ///< Status bit 10 is not assigned
  kUnassigned11 = 11,             ///< Status bit 11 is not assigned
  kUnassigned12 = 12,             ///< Status bit 12 is not assigned
  kUnassigned13 = 13,             ///< Status bit 13 is not assigned
  kUnassigned14 = 14,             ///< Status bit 14 is not assigned
  kUnassigned15 = 15,             ///< Status bit 15 is not assigned
  kUnassigned16 = 16,             ///< Status bit 16 is not assigned
  kUnassigned17 = 17,             ///< Status bit 17 is not assigned
  kUnassigned18 = 18,             ///< Status bit 18 is not assigned
  kUnassigned19 = 19,             ///< Status bit 19 is not assigned
  kUnassigned20 = 20,             ///< Status bit 20 is not assigned
  kUnassigned21 = 21,             ///< Status bit 21 is not assigned
  kUnassigned22 = 22,             ///< Status bit 22 is not assigned
  kUnassigned23 = 23,             ///< Status bit 23 is not assigned
  kUnassigned24 = 24,             ///< Status bit 24 is not assigned
  kUnassigned25 = 25,             ///< Status bit 25 is not assigned
  kUnassigned26 = 26,             ///< Status bit 26 is not assigned
  kUnassigned27 = 27,             ///< Status bit 27 is not assigned
  kUnassigned28 = 28,             ///< Status bit 28 is not assigned
  kUnassigned29 = 29,             ///< Status bit 29 is not assigned
  kUnassigned30 = 30,             ///< Status bit 30 is not assigned
  kUnassigned31 = 31,             ///< Status bit 31 is not assigned
  kInvalid = 32                   ///< Status bit 32 and higher are not valid (status_bits is only 32 bits)
};

/**
 * @brief This enumeration should list all defined Fragment types
 */
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

/**
 * @brief Convert a FragmentType enum value to string
 * @param type Type to convert
 * @return String representation of the given type
 */
inline std::string
fragment_type_to_string(const FragmentType& type)
{
  try {
    return get_fragment_type_names().at(type);
  } catch (std::exception& e) {
  }
  return "Unknown";
}

/**
 * @brief Convert a string to a FragmentType value
 * @param name Name of the type
 * @return FragmentType corresponding to given string
 */
inline FragmentType
string_to_fragment_type(const std::string& name)
{
  for (auto& it : get_fragment_type_names()) {
    if (it.second == name)
      return it.first;
  }
  return FragmentType::kUnknown;
}

/**
 * @brief Stream a Fragment Header in human-readable form
 * @param o Stream to write to
 * @param hdr FragmentHeader to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, FragmentHeader const& hdr)
{
  return o << "check_word: " << std::hex << hdr.fragment_header_marker << std::dec << ", " << "version: " << hdr.version
           << ", " << "size: " << hdr.size << ", " << "trigger_number: " << hdr.trigger_number << ", "
           << "run_number: " << hdr.run_number << ", " << "trigger_timestamp: " << hdr.trigger_timestamp << ", "
           << "window_begin: " << hdr.window_begin << ", " << "window_end: " << hdr.window_end << ", "
           << "status_bits: " << hdr.status_bits << ", " << "fragment_type: " << hdr.fragment_type << ", "
           << "sequence_number: " << hdr.sequence_number << ", " << "detector_id: " << hdr.detector_id << ", "
           << "element_id: " << hdr.element_id;
}

/**
 * @brief Read a FragmentHeader instance from a string stream
 * @param is Stream to read from
 * @param hdr FragmentHeader to read
 * @return Stream instance for continued streaming
 */
inline std::istream&
operator>>(std::istream& o, FragmentHeader& hdr)
{
  std::string tmp;
  return o >> tmp >> std::hex >> hdr.fragment_header_marker >> std::dec >> tmp >> tmp >> hdr.version >> tmp >> tmp >>
         hdr.size >> tmp >> tmp >> hdr.trigger_number >> tmp >> tmp >> hdr.run_number >> tmp >> tmp >>
         hdr.trigger_timestamp >> tmp >> tmp >> hdr.window_begin >> tmp >> tmp >> hdr.window_end >> tmp >> tmp >>
         hdr.status_bits >> tmp >> tmp >> hdr.fragment_type >> tmp >> tmp >> hdr.sequence_number >> tmp >> tmp >>
         hdr.detector_id >> tmp >> tmp >> hdr.element_id;
}
} // namespace dunedaq::daqdataformats

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_FRAGMENTHEADER_HPP_
