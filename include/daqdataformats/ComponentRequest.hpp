/**
 * @file ComponentRequest.hpp Represents the requested timestamp range for a single component
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_COMPONENTREQUEST_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_COMPONENTREQUEST_HPP_

#include "daqdataformats/GeoID.hpp"
#include "daqdataformats/Types.hpp"

#include <ostream>
#include <string>

namespace dunedaq {
namespace daqdataformats {

/**
 * @brief A request sent to a Component, including the GeoID of the component and the window offset and width
 */
struct ComponentRequest
{
  /**
   * @brief The current version of the ComponentRequest
   */
  static constexpr uint32_t s_component_request_version = 1; // NOLINT(build/unsigned)

  /**
   * @brief The version number of this ComponentRequest
   */
  uint32_t version{ s_component_request_version }; // NOLINT(build/unsigned)
  uint32_t unused{ 0xFFFFFFFF };                   ///< Padding to ensure 64b alignment // NOLINT(build/unsigned)

  GeoID component; ///< The Requested Component

  /**
   * @brief Start of the data collection window
   */
  timestamp_t window_begin{ TypeDefaults::s_invalid_timestamp };

  /**
   * @brief End of the data collection window
   */
  timestamp_t window_end{ TypeDefaults::s_invalid_timestamp };

  ComponentRequest() {}
  ComponentRequest(GeoID const& comp, timestamp_t const& wbegin, timestamp_t const& wend);
};

/**
 * @brief Write out a ComponentRequest in human-readable form
 * @param o Output stream
 * @param cr ComponentRequest to write
 * @return Stream instance for continued streaming
 */
inline std::ostream&
operator<<(std::ostream& o, ComponentRequest const& cr)
{
  return o << cr.component << ", begin: " << cr.window_begin << ", end: " << cr.window_end;
}

/**
 * @brief Read a ComponentRequest from a string stream
 * @param is Input stream
 * @param cr ComponentRequest to read
 * @return Stream instance for continued streaming
 */
inline std::istream&
operator>>(std::istream& is, ComponentRequest& cr)
{
  std::string tmp;
  return is >> cr.component >> tmp >> tmp >> cr.window_begin >> tmp >> tmp >> cr.window_end;
}



ComponentRequest::ComponentRequest(GeoID const& comp, timestamp_t const& wbegin, timestamp_t const& wend)
  : version(s_component_request_version)
  , component(comp)
  , window_begin(wbegin)
  , window_end(wend)
{}
} // namespace daqdataformats
} // namespace dunedaq

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_COMPONENTREQUEST_HPP_
