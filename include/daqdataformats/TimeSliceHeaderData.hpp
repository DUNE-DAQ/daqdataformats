/**
 * @file TimeSliceHeaderData.hpp  TimeSliceHeaderData struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TIMESLICEHEADERDATA_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TIMESLICEHEADERDATA_HPP_

#include "daqdataformats/ComponentRequest.hpp"
#include "daqdataformats/Types.hpp"

#include <limits>
#include <ostream>
#include <string>
#include <vector>

namespace dunedaq {

namespace daqdataformats {

/**
 * @brief Additional data fields associated with a TimeSliceHeader
 */
struct TimeSliceHeaderData
{
  /**
   * @brief Magic bytes to identify a TimeSliceHeader entry in a raw data stream
   */
  static constexpr uint32_t s_timeslice_header_magic = 0x55556666; // NOLINT(build/unsigned)

  /**
   * @brief The current version of the TimeSliceHeader
   */
  static constexpr uint32_t s_timeslice_header_version = 1; // NOLINT(build/unsigned)

  /**
   * @brief Magic bytes used to identify a TimeSliceHeaderData struct in a raw data stream
   */
  uint32_t timeslice_header_marker = s_timeslice_header_magic; // NOLINT(build/unsigned)

  /**
   * @brief Version of the TimeSliceHeaderData structure
   */
  uint32_t version = s_timeslice_header_version; // NOLINT(build/unsigned)

  /**
   * @brief Slice number of this TimeSlice within the stream
   */
  timeslice_number_t timeslice_number{ TypeDefaults::s_invalid_timeslice_number };

  /**
   * @brief Run Number for the TimeSlice
   */
  run_number_t run_number{ TypeDefaults::s_invalid_run_number };

  /**
   * @brief Padding to ensure 64-bit alignment
   */
  uint32_t unused{ 0xFFFFFFFF }; // NOLINT(build/unsigned)
};

/**
 * @brief Stream a TimeSliceHeaderData instance in human-readable form
 * @param o Stream to write to
 * @param hdr TimeSliceHeaderData to write
 * @return Stream instance for continued streaming
 */
inline std::ostream&
operator<<(std::ostream& o, TimeSliceHeaderData const& hdr)
{
  return o << "check_word: " << std::hex << hdr.timeslice_header_marker << std::dec << ", "
           << "version: " << hdr.version << ", "

           << "trigger_number: " << hdr.trigger_number << ", "
           << "run_number: " << hdr.run_number << ", "
           << "trigger_timestamp: " << hdr.trigger_timestamp << ", "
           << "trigger_type: " << hdr.trigger_type << ", "

           << "num_requested_components: " << hdr.num_requested_components << ", "
           << "sequence_number: " << hdr.sequence_number << ", "
           << "max_sequence_number: " << hdr.max_sequence_number;
}

/**
 * @brief Read a TimeSliceHeaderData instance from a string stream
 * @param is Stream to read from
 * @param hdr TimeSliceHeaderData toread
 * @return Stream instance for continued streaming
 */
inline std::istream&
operator>>(std::istream& o, TimeSliceHeaderData& hdr)
{
  std::string tmp;
  return o >> tmp >> std::hex >> hdr.timeslice_header_marker >> std::dec >> tmp >> tmp >> hdr.version >> tmp >>
         tmp >> hdr.trigger_number >> tmp >> tmp >> hdr.run_number >> tmp >> tmp >> hdr.trigger_timestamp >> tmp >>
         tmp >> hdr.trigger_type >> tmp >> tmp >> hdr.error_bits >> tmp >> tmp >> hdr.num_requested_components >> tmp >>
         tmp >> hdr.sequence_number >> tmp >> tmp >> hdr.max_sequence_number;
}

} // namespace daqdataformats
} // namespace dunedaq

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TIMESLICEHEADERDATA_HPP_
