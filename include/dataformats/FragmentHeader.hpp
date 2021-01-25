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
  uint32_t m_fragment_header_marker = FRAGMENT_HEADER_MAGIC; // NOLINT(build/unsigned)
  /**
   * @brief Version of the FragmentHeader
   */
  uint32_t m_version = FRAGMENT_HEADER_VERSION; // NOLINT(build/unsigned)

  /**
   * @brief Size of the Fragment (including header and payload)
   */
  fragment_size_t m_size{ 0 }; // NOLINT(build/unsigned)

  /**
   * @brief Trigger Number this Fragment is associated with
   */
  trigger_number_t m_trigger_number{ 0 };

  /**
   * @brief Timestamp of the TriggerDecision
   */
  timestamp_t m_trigger_timestamp{ 0 };
  /**
   * @brief Window offset of data in the Fragment
   */
  timestamp_diff_t m_window_offset{ 0 };
  /**
   * @brief Window width of data in the Fragment
   */
  timestamp_diff_t m_window_width{ 0 };

  /**
   * @brief Run number this Fragment is associated with
   */
  run_number_t m_run_number{ 0 };
  /**
   * @brief Component that generated the data in this Fragment
   */
  GeoID m_link_id;
  /**
   * @brief Error bits set by the Upstream DAQ
   *
   * Defined Error bits should be documented here, along with the Fragment Type(s) that they apply to
   */
  uint32_t m_error_bits{ 0 }; // NOLINT(build/unsigned)
  /**
   * @brief Type of the Fragment, indicating the format of the contained payload
   */
  fragment_type_t m_fragment_type{ 0 };
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