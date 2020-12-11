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
  uint32_t APANumber;  // NOLINT(build/unsigned)
  uint32_t LinkNumber; // NOLINT(build/unsigned)

  bool operator<(const GeoID& other) const
  {
    return std::tuple(APANumber, LinkNumber) < std::tuple(other.APANumber, other.LinkNumber);
  }
};
} // namespace cdf
} // namespace dunedaq

#endif // CDF_INCLUDE_CDF_GEOID_HPP_
