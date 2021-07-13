/**
 * @file FragmentHeader.hpp  FragmentHeader struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENTHEADER_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENTHEADER_HPP_

#include "dataformats/GeoID.hpp"
#include "dataformats/Types.hpp"

#include "logging/Logging.hpp"

#include <bitset>
#include <cstdlib>
#include <map>
#include <numeric>
#include <string>
#include <vector>

namespace dunedaq {
/**
 * @brief An ERS Issue indicating that an attempted FragmentType conversion failed
 * @param fragment_type_input Input that failed conversion
 * @cond Doxygen doesn't like ERS macros LCOV_EXCL_START
 */
ERS_DECLARE_ISSUE(dataformats,
                  FragmentTypeConversionError,
                  "Supplied input " << fragment_type_input << " did not match any in s_fragment_type_names",
                  ((std::string)fragment_type_input)) // NOLINT
                                                      /// @endcond LCOV_EXCL_STOP
namespace dataformats {

/**
 * @brief The header for a DUNE Fragment
 */
struct FragmentHeader
{
  /**
   * @brief Magic bytes to identify a FragmentHeader entry in a raw data stream
   */
  static constexpr uint32_t s_fragment_header_magic = 0x11112222; // NOLINT(build/unsigned)

  /**
   * @brief The current version of the Fragment
   */
  static constexpr uint32_t s_fragment_header_version = 3; // NOLINT(build/unsigned)

  /**
   * @brief By default, all error bits are unset
   */
  static constexpr uint32_t s_default_error_bits = 0; // NOLINT(build/unsigned)

  /**
   * @brief Magic Bytes used to identify FragmentHeaders in a raw data stream
   */
  uint32_t fragment_header_marker = s_fragment_header_magic; // NOLINT(build/unsigned)

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
   * @brief Error bits set by the Upstream DAQ
   *
   * Defined Error bits should be documented here, along with the Fragment Type(s) that they apply to
   */
  uint32_t error_bits{ s_default_error_bits }; // NOLINT(build/unsigned)

  /**
   * @brief Type of the Fragment, indicating the format of the contained payload
   */
  fragment_type_t fragment_type{ TypeDefaults::s_invalid_fragment_type };
  /**
   * @brief Sequence number of this Fragment within a trigger record
   */
  sequence_number_t sequence_number{ TypeDefaults::s_invalid_sequence_number };

  uint16_t unused{ // NOLINT(build/unsigned)
                   0xFFFF
  }; ///< Padding to ensure 64-bit alignment of FragmentHeader basic fields

  /**
   * @brief Component that generated the data in this Fragment
   */
  GeoID element_id;
};

/**
 * @brief This enumeration should list all defined error bits, as well as a short documentation of their meaning
 */
enum class FragmentErrorBits : size_t
{
  kDataNotFound = 0,  ///< The requested data was not found at all, so the fragment is empty
  kIncomplete = 1,    ///< Only part of the requested data is present in the fragment
  kInvalidWindow = 2, ///< The requested data window was too large
  kUnassigned3 = 3,   ///< Error bit 3 is not assigned
  kUnassigned4 = 4,   ///< Error bit 4 is not assigned
  kUnassigned5 = 5,   ///< Error bit 5 is not assigned
  kUnassigned6 = 6,   ///< Error bit 6 is not assigned
  kUnassigned7 = 7,   ///< Error bit 7 is not assigned
  kUnassigned8 = 8,   ///< Error bit 8 is not assigned
  kUnassigned9 = 9,   ///< Error bit 9 is not assigned
  kUnassigned10 = 10, ///< Error bit 10 is not assigned
  kUnassigned11 = 11, ///< Error bit 11 is not assigned
  kUnassigned12 = 12, ///< Error bit 12 is not assigned
  kUnassigned13 = 13, ///< Error bit 13 is not assigned
  kUnassigned14 = 14, ///< Error bit 14 is not assigned
  kUnassigned15 = 15, ///< Error bit 15 is not assigned
  kUnassigned16 = 16, ///< Error bit 16 is not assigned
  kUnassigned17 = 17, ///< Error bit 17 is not assigned
  kUnassigned18 = 18, ///< Error bit 18 is not assigned
  kUnassigned19 = 19, ///< Error bit 19 is not assigned
  kUnassigned20 = 20, ///< Error bit 20 is not assigned
  kUnassigned21 = 21, ///< Error bit 21 is not assigned
  kUnassigned22 = 22, ///< Error bit 22 is not assigned
  kUnassigned23 = 23, ///< Error bit 23 is not assigned
  kUnassigned24 = 24, ///< Error bit 24 is not assigned
  kUnassigned25 = 25, ///< Error bit 25 is not assigned
  kUnassigned26 = 26, ///< Error bit 26 is not assigned
  kUnassigned27 = 27, ///< Error bit 27 is not assigned
  kUnassigned28 = 28, ///< Error bit 28 is not assigned
  kUnassigned29 = 29, ///< Error bit 29 is not assigned
  kUnassigned30 = 30, ///< Error bit 30 is not assigned
  kUnassigned31 = 31, ///< Error bit 31 is not assigned
  kInvalid = 32       ///< Error bit 32 and higher are not valid (error_bits is only 32 bits)
};

/**
 * @brief This enumeration should list all defined Fragment types
 */
enum class FragmentType : fragment_type_t
{
  kFakeData = 0,                                   ///< Data created in dfmodules' FakeDataProducer
  kTPCData = 1,                                    ///< Data from the TPC
  kPDSData = 2,                                    ///< Data from the PDS
  kNDLArTPC = 3,                                   ///< Data from the NDLArTPC
  kUnknown = TypeDefaults::s_invalid_fragment_type ///< Used when given a string that does not match any in
                                                   ///< get_fragment_type_names
};

/**
 * @brief This map relates FragmentType values to string names
 *
 * These names can be used, for example, as HDF5 Group names
 */
inline std::map<FragmentType, std::string>
get_fragment_type_names()
{
  return { { FragmentType::kFakeData, "FakeData" },
           { FragmentType::kTPCData, "TPC" },
           { FragmentType::kPDSData, "PDS" },
           { FragmentType::kNDLArTPC, "NDLArTPC" } };
}

/**
 * @brief Convert a FragmentType enum value to string
 * @param type Type to convert
 * @return String representation of the given type
 */
inline std::string
fragment_type_to_string(FragmentType type)
{
  if (!get_fragment_type_names().count(type)) {
    ers::error(FragmentTypeConversionError(ERS_HERE, std::to_string(static_cast<int>(type))));
    return "UNKNOWN";
  }
  return get_fragment_type_names().at(type);
}

/**
 * @brief Convert a string to a FragmentType value
 * @param name Name of the type
 * @return FragmentType corresponding to given string
 */
inline FragmentType
string_to_fragment_type(std::string name)
{
  for (auto& it : get_fragment_type_names()) {
    if (it.second == name)
      return it.first;
  }
  ers::error(FragmentTypeConversionError(ERS_HERE, name));
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
  return o << "check_word: " << std::hex << hdr.fragment_header_marker << std::dec << ", "
           << "version: " << hdr.version << ", "
           << "size: " << hdr.size << ", "
           << "trigger_number: " << hdr.trigger_number << ", "
           << "run_number: " << hdr.run_number << ", "
           << "trigger_timestamp: " << hdr.trigger_timestamp << ", "
           << "window_begin: " << hdr.window_begin << ", "
           << "window_end: " << hdr.window_end << ", "
           << "element_id: " << hdr.element_id << ", "
           << "error_bits: " << hdr.error_bits << ", "
           << "fragment_type: " << hdr.fragment_type << ", "
           << "sequence_number: " << hdr.sequence_number;
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
         hdr.element_id >> tmp >> tmp >> hdr.error_bits >> tmp >> tmp >> hdr.fragment_type >> tmp >> tmp >>
         hdr.sequence_number;
}
} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENTHEADER_HPP_
