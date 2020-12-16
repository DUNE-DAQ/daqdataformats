/**
 * @file ComponentRequest.hpp Represents the requested timestamp range for a single component
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef dataformats_INCLUDE_dataformats_COMPONENTREQUEST_HPP_
#define dataformats_INCLUDE_dataformats_COMPONENTREQUEST_HPP_

#include "dataformats/GeoID.hpp"
#include "dataformats/Types.hpp"

#include <ostream>

namespace dunedaq {
namespace dataformats {
struct ComponentRequest
{
  GeoID component;

  timestamp_diff_t window_offset;
  timestamp_diff_t window_width;
};

inline std::ostream&
operator<<(std::ostream& o, ComponentRequest const& cr)
{
  return o << cr.component << ", offset: " << cr.window_offset << ", width: " << cr.window_width;
}
} // namespace dataformats
} // namespace dunedaq

#endif // dataformats_INCLUDE_dataformats_COMPONENTREQUEST_HPP_