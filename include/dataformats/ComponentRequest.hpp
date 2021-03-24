/**
 * @file ComponentRequest.hpp Represents the requested timestamp range for a single component
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_COMPONENTREQUEST_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_COMPONENTREQUEST_HPP_

#include "dataformats/GeoID.hpp"
#include "dataformats/Types.hpp"

#include <ostream>

namespace dunedaq {
namespace dataformats {

/**
 * @brief A request sent to a Component, including the GeoID of the component and the window offset and width
 */
struct ComponentRequest
{
  GeoID component; ///< The Requested Component

  /**
   * @brief Start of the data collection window
   */
  timestamp_t window_begin{ TypeDefaults::s_invalid_timestamp };
  /**
   * @brief End of the data collection window
   */
  timestamp_t window_end{ TypeDefaults::s_invalid_timestamp };
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
} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_COMPONENTREQUEST_HPP_
