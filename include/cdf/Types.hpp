/**
 * @file Types.hpp  Types used in Data Structures
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef dataformats_INCLUDE_dataformats_TYPES_HPP_
#define dataformats_INCLUDE_dataformats_TYPES_HPP_

#include <cstdint>

namespace dunedaq {
namespace dataformats {

using run_number_t = uint32_t;     // NOLINT(build/unsigned)
using trigger_number_t = uint64_t; // NOLINT(build/unsigned)
using fragment_type_t = uint32_t;  // NOLINT(build/unsigned)
using fragment_size_t = uint64_t;  // NOLINT(build/unsigned)
using timestamp_t = uint64_t;      // NOLINT(build/unsigned)
using timestamp_diff_t = int64_t;
using trigger_type_t = uint16_t; // NOLINT(build/unsigned)
} // namespace dataformats
} // namespace dunedaq

#endif // dataformats_INCLUDE_dataformats_TYPES_HPP_