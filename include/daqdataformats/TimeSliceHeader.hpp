/**
 * @file TimeSliceHeader.hpp  TimeSliceHeader struct definition
 *
 * An instance of this header provides metadata for the TimeSlice instance in which it's contained
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
 * @brief Data fields associated with a TimeSliceHeader
 */
struct TimeSliceHeader
{
  /// @brief Marker bytes to identify a TimeSliceHeader entry in a raw data stream
  static constexpr uint32_t s_timeslice_header_marker = 0x55556666; // NOLINT(build/unsigned)

  static constexpr uint32_t s_timeslice_header_version = 2; // NOLINT(build/unsigned)

  uint32_t timeslice_header_marker = s_timeslice_header_marker; // NOLINT(build/unsigned)
  uint32_t version = s_timeslice_header_version;                // NOLINT(build/unsigned)

  /// @brief Slice number of this TimeSlice within the stream
  timeslice_number_t timeslice_number{ TypeDefaults::s_invalid_timeslice_number };

  run_number_t run_number{ TypeDefaults::s_invalid_run_number };

  uint32_t unused{ 0xFFFFFFFF }; // NOLINT(build/unsigned) Padding to ensure 64-bit alignment

  SourceID element_id;
};

std::ostream&
operator<<(std::ostream& o, TimeSliceHeader const& hdr);

} // namespace dunedaq::daqdataformats

#include "detail/TimeSliceHeader.hxx"

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TIMESLICEHEADER_HPP_
