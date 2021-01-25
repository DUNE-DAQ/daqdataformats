/**
 * @file Types.hpp  Types used in Data Structures
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_TYPES_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_TYPES_HPP_

#include <cstdint>

/**
 * @brief The DUNE-DAQ namespace
 */
namespace dunedaq {
/**
 * @brief Dataformats code
 */
namespace dataformats {

/**
 * @brief Type used to represent run number
 */
using run_number_t = uint32_t; // NOLINT(build/unsigned)
/**
 * @brief Type used to represent trigger number
 */
using trigger_number_t = uint64_t; // NOLINT(build/unsigned)
/**
 * @brief Type used to represent Fragment type ID
 */
using fragment_type_t = uint32_t; // NOLINT(build/unsigned)
/**
 * @brief Type used to represent Fragment size
 */
using fragment_size_t = uint64_t; // NOLINT(build/unsigned)
/**
 * @brief Type used to represent DUNE timing system timestamps
 */
using timestamp_t = uint64_t; // NOLINT(build/unsigned)
/**
 * @brief Type used to represent differences between DUNE system timestamps
 */
using timestamp_diff_t = int64_t;
/**
 * @brief Type used to represent Trigger Decision trigger types
 */
using trigger_type_t = uint16_t; // NOLINT(build/unsigned)
} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_TYPES_HPP_