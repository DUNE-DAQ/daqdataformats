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
  static constexpr uint32_t s_fragment_header_magic = 0x11112222; // NOLINT(build/unsigned)

  /**
   * @brief The current version of the Fragment
   */
  static constexpr uint32_t s_fragment_header_version = 1; // NOLINT(build/unsigned)

  /**
   * @brief Magic Bytes used to identify FragmentHeaders in a raw data stream
   */
  uint32_t m_fragment_header_marker = s_fragment_header_magic; // NOLINT(build/unsigned)

  /**
   * @brief Version of the FragmentHeader
   */
  uint32_t m_version = s_fragment_header_version; // NOLINT(build/unsigned)

  static constexpr fragment_size_t s_invalid_fragment_size =
    0; ///< Invalid size for a Fragment (as FragmentHeader is counted as well)
  /**
   * @brief Size of the Fragment (including header and payload)
   */
  fragment_size_t m_size{ s_invalid_fragment_size }; // NOLINT(build/unsigned)

  static constexpr trigger_number_t s_invalid_trigger_number =
    std::numeric_limits<trigger_number_t>::max(); ///< An invalid trigger number
  /**
   * @brief Trigger Number this Fragment is associated with
   */
  trigger_number_t m_trigger_number{ s_invalid_trigger_number };

  static constexpr timestamp_t s_invalid_timestamp = std::numeric_limits<timestamp_t>::max(); ///< An invalid timestamp
  /**
   * @brief Timestamp of the TriggerDecision
   */
  timestamp_t m_trigger_timestamp{ s_invalid_timestamp };

  static constexpr timestamp_diff_t s_invalid_offset = std::numeric_limits<timestamp_diff_t>::max(); ///< An invalid timestamp offset
  /**
   * @brief Window offset of data in the Fragment
   */
  timestamp_diff_t m_window_offset{ s_invalid_offset };

  static constexpr timestamp_diff_t s_invalid_width = std::numeric_limits<timestamp_diff_t>::max(); ///< An invalid window width
  /**
   * @brief Window width of data in the Fragment
   */
  timestamp_diff_t m_window_width{ s_invalid_width };

  static constexpr run_number_t s_invalid_run_number = std::numeric_limits<run_number_t>::max(); ///< An invalid run number
  /**
   * @brief Run number this Fragment is associated with
   */
  run_number_t m_run_number{ s_invalid_run_number };

  /**
   * @brief Component that generated the data in this Fragment
   */
  GeoID m_link_id;

  /**
   * @brief By default, all error bits are unset
   */
  static constexpr uint32_t s_default_error_bits = 0; // NOLINT(build/unsigned)
  /**
   * @brief Error bits set by the Upstream DAQ
   *
   * Defined Error bits should be documented here, along with the Fragment Type(s) that they apply to
   */
  uint32_t m_error_bits{ s_default_error_bits }; // NOLINT(build/unsigned)

  static constexpr fragment_type_t s_invalid_fragment_type = std::numeric_limits<fragment_type_t>::max(); ///< An invalid fragment type
  /**
   * @brief Type of the Fragment, indicating the format of the contained payload
   */
  fragment_type_t m_fragment_type{ s_invalid_fragment_type };
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
  return o << "check_word: " << std::hex << hdr.m_fragment_header_marker << std::dec << ", "
           << "version: " << hdr.m_version << ", "
           << "size: " << hdr.m_size << ", "
           << "trigger_number: " << hdr.m_trigger_number << ", "
           << "run_number: " << hdr.m_run_number << ", "
           << "trigger_timestamp: " << hdr.m_trigger_timestamp << ", "
           << "window_offset: " << hdr.m_window_offset << ", "
           << "window_width: " << hdr.m_window_width << ", "
           << "link_id: " << hdr.m_link_id << ", "
           << "error_bits: " << hdr.m_error_bits << ", "
           << "fragment_type : " << hdr.m_fragment_type;
}

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_FRAGMENTHEADER_HPP_
