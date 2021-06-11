/**
 * @file Types.hpp  Types used in Data Structures
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_TYPES_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_TYPES_HPP_

#include "ers/Issue.hpp"

#include <cstdint>
#include <limits>

/**
 * @brief The DUNE-DAQ namespace
 */
namespace dunedaq {
/**
 * @brief An ERS Error indicating that a malloc failed
 * @param maf_bytes Bytes requested from malloc
 * @cond Doxygen doesn't like ERS macros
 */
ERS_DECLARE_ISSUE(dataformats,
                  MemoryAllocationFailed,
                  "Malloc of " << maf_bytes << " bytes failed",
                  ((size_t)maf_bytes)) // NOLINT
                                       /// @endcond
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
using timestamp_t = uint64_t;     // NOLINT(build/unsigned)
using timestamp_diff_t = int64_t; ///< Used to represent differences between timestamps
/**
 * @brief Type used to represent Trigger Decision trigger types
 */
using trigger_type_t = uint16_t; // NOLINT(build/unsigned)
/**
 * @brief Type used to represent sequence within a trigger record
 */
using sequence_number_t = uint16_t; // NOLINT(build/unsigned)

/**
 * @brief Default values for dataformats types
 */
class TypeDefaults
{
public:
  static constexpr run_number_t s_invalid_run_number =
    std::numeric_limits<run_number_t>::max(); ///< An invalid run number
  static constexpr trigger_number_t s_invalid_trigger_number =
    std::numeric_limits<trigger_number_t>::max(); ///< An invalid trigger number
  static constexpr fragment_type_t s_invalid_fragment_type =
    std::numeric_limits<fragment_type_t>::max(); ///< An invalid fragment type
  static constexpr fragment_size_t s_invalid_fragment_size =
    0; ///< Invalid size for a Fragment (as FragmentHeader is counted as well)
  static constexpr timestamp_t s_invalid_timestamp = std::numeric_limits<timestamp_t>::max(); ///< An invalid timestamp
  static constexpr timestamp_diff_t s_invalid_timestamp_diff =
    std::numeric_limits<timestamp_diff_t>::max(); ///< An invalid timestamp difference

  static constexpr trigger_type_t s_invalid_trigger_type =
    std::numeric_limits<trigger_type_t>::max(); ///< An invalid trigger type
  static constexpr sequence_number_t s_invalid_sequence_number =
    std::numeric_limits<sequence_number_t>::max(); // An invalid sequence number
};

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_TYPES_HPP_