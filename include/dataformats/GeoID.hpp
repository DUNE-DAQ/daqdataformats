/**
 * @file GeoID.hpp Geometric Identification for a link
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_GEOID_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_GEOID_HPP_

#include <cstdint>
#include <ostream>
#include <tuple>

namespace dunedaq {
namespace dataformats {
struct GeoID
{
  uint32_t APA_number;  // NOLINT(build/unsigned)
  uint32_t link_number; // NOLINT(build/unsigned)

  bool operator<(const GeoID& other) const
  {
    return std::tuple(APA_number, link_number) < std::tuple(other.APA_number, other.link_number);
  }

};

inline std::ostream&
operator<<(std::ostream& o, GeoID const& id)
{
  return o << "APA: " << id.APA_number << ", link: " << id.link_number;
}
} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_GEOID_HPP_
