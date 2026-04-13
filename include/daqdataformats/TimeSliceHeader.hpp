/**
 * @file TimeSliceHeaderData.hpp  TimeSliceHeaderData struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TIMESLICEHEADER_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TIMESLICEHEADER_HPP_

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
 * @brief Additional data fields associated with a TimeSliceHeader
 */
struct TimeSliceHeader
{
  /**
   * @brief Marker bytes to identify a TimeSliceHeader entry in a raw data stream
   */
  static constexpr uint32_t s_timeslice_header_marker = 0x55556666; // NOLINT(build/unsigned)

  /**
   * @brief The current version of the TimeSliceHeader
   */
  static constexpr uint32_t s_timeslice_header_version = 2; // NOLINT(build/unsigned)

  /**
   * @brief Marker bytes used to identify a TimeSliceHeader struct in a raw data stream
   */
  uint32_t timeslice_header_marker = s_timeslice_header_marker; // NOLINT(build/unsigned)

  /**
   * @brief Version of the TimeSliceHeader structure
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

  SourceID element_id;
};

/**
 * @brief Stream a TimeSliceHeader instance in human-readable form
 * @param o Stream to write to
 * @param hdr TimeSliceHeader to write
 * @return Stream instance for continued streaming
 */
std::ostream&
operator<<(std::ostream& o, TimeSliceHeader const& hdr);

} // namespace dunedaq::daqdataformats

#include "detail/TimeSliceHeader.hxx"

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TIMESLICEHEADER_HPP_
