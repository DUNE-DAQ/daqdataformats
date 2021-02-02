/**
 * @file GeoID.hpp Geometric Identification for a link
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_GEOID_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_GEOID_HPP_

#include "serialization/Serialization.hpp"

#include <cstdint>
#include <limits>
#include <ostream>
#include <tuple>

namespace dunedaq {
namespace dataformats {

/**
 * @brief Represents a coordinate point in the DAQ's logical coordinate system (i.e. not physical coordinates)
 */
struct GeoID
{
  /**
   * @brief An invalid APA number, used for initialization
   */
  static constexpr uint32_t s_invalid_apa_number = std::numeric_limits<uint32_t>::max(); // NOLINT(build/unsigned)
  /**
   * @brief An invalid link number, used for initialization
   */
  static constexpr uint32_t s_invalid_link_number = std::numeric_limits<uint32_t>::max(); // NOLINT(build/unsigned)

  /**
   * @brief APA Number of the component
   */
  uint32_t m_apa_number{ s_invalid_apa_number }; // NOLINT(build/unsigned)
  /**
   * @brief Link Number of the component
   */
  uint32_t m_link_number{ s_invalid_link_number }; // NOLINT(build/unsigned)

  /**
   * @brief Comparison operator (to allow GeoID to be used in std::map)
   * @param other GeoID to compare
   * @return The result of std::tuple compare using apa_number and link_number
   */
  bool operator<(const GeoID& other) const
  {
    return std::tuple(m_apa_number, m_link_number) < std::tuple(other.m_apa_number, other.m_link_number);
  }

  MSGPACK_DEFINE(m_apa_number, m_link_number)
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GeoID, m_apa_number, m_link_number);
};

/**
 * @brief Stream a GeoID instance in a human-readable form
 * @param o Stream to output to
 * @param id GeoID to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, GeoID const& id)
{
  return o << "APA: " << id.m_apa_number << ", link: " << id.m_link_number;
}
} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_GEOID_HPP_
