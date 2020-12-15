/**
 * @file GeoID.hpp Geometric Identification for a link
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef CDF_INCLUDE_CDF_GEOID_HPP_
#define CDF_INCLUDE_CDF_GEOID_HPP_

#include <cstdint>
#include <tuple>

namespace dunedaq {
namespace cdf {
struct GeoID
{
  uint32_t APA_number;  // NOLINT(build/unsigned)
  uint32_t link_number; // NOLINT(build/unsigned)

  bool operator<(const GeoID& other) const
  {
    return std::tuple(APA_number, link_number) < std::tuple(other.APA_number, other.link_number);
  }
};
} // namespace cdf
} // namespace dunedaq

#endif // CDF_INCLUDE_CDF_GEOID_HPP_
