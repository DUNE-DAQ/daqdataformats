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
  uint32_t version = FRAGMENT_HEADER_VERSION;              // NOLINT(build/unsigned)

  /**
   * @brief Size of the Fragment (including header and payload)
  */
  fragment_size_t size {0}; // NOLINT(build/unsigned)

  /**
   * @brief Trigger Number this Fragment is associated with
  */
  trigger_number_t trigger_number {0};

  /**
   * @brief Timestamp of the TriggerDecision
  */
  timestamp_t trigger_timestamp {0};
  /**
   * @brief Window offset of data in the Fragment
  */
  timestamp_diff_t window_offset {0};
  /**
   * @brief Window width of data in the Fragment
  */
  timestamp_diff_t window_width {0};

  /**
   * @brief Run number this Fragment is associated with
  */
  run_number_t run_number {0};
  /**
   * @brief Component that generated the data in this Fragment
  */
  GeoID link_id;
  /**
   * @brief Error bits set by the Upstream DAQ
   * 
   * Defined Error bits should be documented here, along with the Fragment Type(s) that they apply to
  */
  uint32_t error_bits{ 0 }; // NOLINT(build/unsigned)
  /**
   * @brief Type of the Fragment, indicating the format of the contained payload
  */
  fragment_type_t fragment_type {0};
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