/**
 * @file Types.hpp  Types used in Data Structures
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef CDF_INCLUDE_CDF_TYPES_HPP_
#define CDF_INCLUDE_CDF_TYPES_HPP_

#include <cstdint>

namespace dunedaq {
namespace cdf {

using run_number_t = uint32_t;     // NOLINT(build/unsigned)
using trigger_number_t = uint64_t; // NOLINT(build/unsigned)
using fragment_type_t = uint32_t;  // NOLINT(build/unsigned)
using timestamp_t = uint64_t;      // NOLINT(build/unsigned)
using timestamp_diff_t = int64_t;
using trigger_type_t = uint16_t; // NOLINT(build/unsigned)
} // namespace cdf
} // namespace dunedaq

#endif // CDF_INCLUDE_CDF_TYPES_HPP_