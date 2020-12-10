/**
 * @file GeoID.hpp Geometric Identification for a link
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <cstdint>

namespace dunedaq {
namespace cdf {
struct GeoID
{
  uint32_t APANumber;
  uint32_t LinkNumber;
};
} // namespace dfmessages
} // namespace dunedaq