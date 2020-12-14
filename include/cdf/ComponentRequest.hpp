/**
 * @file ComponentRequest.hpp Represents the requested timestamp range for a single component
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef CDF_INCLUDE_CDF_COMPONENTREQUEST_HPP_
#define CDF_INCLUDE_CDF_COMPONENTREQUEST_HPP_

#include "cdf/GeoID.hpp"
#include "cdf/Types.hpp"

namespace dunedaq {
namespace cdf {
struct ComponentRequest
{
  GeoID Component;

  timestamp_t RequestTimestamp;
  timestamp_diff_t RequestOffset;
  timestamp_t RequestWidth;
};
} // namespace cdf
} // namespace dunedaq

#endif // CDF_INCLUDE_CDF_COMPONENTREQUEST_HPP_