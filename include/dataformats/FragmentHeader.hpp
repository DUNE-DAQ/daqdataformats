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

#include <bitset>
#include <cstdlib>
#include <numeric>
#include <vector>

namespace dunedaq {
namespace dataformats {

/**
 * @brief The header for a DUNE Fragment
 */
struct FragmentHeader
{
/**
 * @brief Magic bytes to identify a FragmentHeader entry in a raw data stream
 */
#define FRAGMENT_HEADER_MAGIC 0x11112222
/**
 * @brief The current version of the Fragment
 */
#define FRAGMENT_HEADER_VERSION 1

  /**
   * @brief Magic Bytes used to identify FragmentHeaders in a raw data stream
   */
  uint32_t fragment_header_marker = FRAGMENT_HEADER_MAGIC; // NOLINT(build/unsigned)
  /**
   * @brief Version of the FragmentHeader
   */
  uint32_t version = FRAGMENT_HEADER_VERSION; // NOLINT(build/unsigned)

  /**
   * @brief Size of the Fragment (including header and payload)
   */
  fragment_size_t size{ 0 }; // NOLINT(build/unsigned)

  /**
   * @brief Trigger Number this Fragment is associated with
   */
  trigger_number_t trigger_number{ 0 };

  /**
   * @brief Timestamp of the TriggerDecision
   */
  timestamp_t trigger_timestamp{ 0 };
  /**
   * @brief Window offset of data in the Fragment
   */
  timestamp_diff_t window_offset{ 0 };
  /**
   * @brief Window width of data in the Fragment
   */
  timestamp_diff_t window_width{ 0 };

  /**
   * @brief Run number this Fragment is associated with
   */
  run_number_t run_number{ 0 };
  /**
   * @brief Component that generated the data in this Fragment
   */
  GeoID link_id;
  /**
   * @brief Error bits set by the Upstream DAQ
   */
  uint32_t error_bits{ 0 }; // NOLINT(build/unsigned)
  /**
   * @brief Type of the Fragment, indicating the format of the contained payload
   */
  fragment_type_t fragment_type{ 0 };
};

/**
 * @brief This enumeration should list all defined error bits, as well as a short documentation of their meaning
 */
enum class FragmentErrorBits : size_t
{
  kUnassigned0 = 0,   ///< Error bit 0 is not assigned
  kUnassigned1 = 1,   ///< Error bit 1 is not assigned
  kUnassigned2 = 2,   ///< Error bit 2 is not assigned
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
  kInvalid = 32       ///< Error bit 32 and higher are not valid (m_error_bits is only 32 bits)
};

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
           << "window_offset: " << hdr.window_offset << ", "
           << "window_width: " << hdr.window_width << ", "
           << "link_id: " << hdr.link_id << ", "
           << "error_bits: " << hdr.error_bits << ", "
           << "fragment_type : " << hdr.fragment_type;
}

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENTHEADER_HPP_