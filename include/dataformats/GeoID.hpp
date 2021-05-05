/**
 * @file GeoID.hpp Geographic Identification for a DAQ component
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_GEOID_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_GEOID_HPP_

#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>
#include <tuple>

namespace dunedaq {
namespace dataformats {

enum class GeoIDComponentType : uint16_t
{
  kTPC = 1,
  kPDS = 2,
  kDataSelection = 3,
  kInvalid = 0
};

/**
 * @brief Represents a coordinate point in the DAQ's logical coordinate system (i.e. not physical coordinates)
 */
struct GeoID
{
  /**
   * @brief An invalid region number, used for initialization
   */
  static constexpr uint16_t s_invalid_region_id = std::numeric_limits<uint16_t>::max(); // NOLINT(build/unsigned)
  /**
   * @brief An invalid element number, used for initialization
   */
  static constexpr uint32_t s_invalid_element_id = std::numeric_limits<uint32_t>::max(); // NOLINT(build/unsigned)

  /**
   * @brief The type of the component (i.e. which subsystem it belongs to)
  */
  GeoIDComponentType component_type{ GeoIDComponentType::kInvalid };

  /**
   * @brief Region number of the component
   */
  uint16_t region_id{ s_invalid_region_id }; // NOLINT(build/unsigned)
  /**
   * @brief Number of the component within the region
   */
  uint32_t element_id{ s_invalid_element_id }; // NOLINT(build/unsigned)

  /**
   * @brief Comparison operator (to allow GeoID to be used in std::map)
   * @param other GeoID to compare
   * @return The result of std::tuple compare using GeoID fields
   */
  bool operator<(const GeoID& other) const
  {
    return std::tuple(component_type, region_id, element_id) <
           std::tuple(other.component_type, other.region_id, other.element_id);
  }

  /**
   * @brief Comparison operator (to allow GeoID comparisons)
   * @param other GeoID to compare
   * @return The result of std::tuple compare using GeoID fields
   */
  bool operator!=(const GeoID& other) const { return (*this) < other || other < (*this); }

  /**
   * @brief Comparison operator (to allow GeoID comparisons)
   * @param other GeoID to compare
   * @return The result of std::tuple compare using GeoID fields
   */
  bool operator==(const GeoID& other) const { return !((*this) != other); }
};

/**
 * @brief Stream a GeoIDComponentType instance in a human-readable form
 * @param o Stream to output to
 * @param id GeoIDComponentType to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, GeoIDComponentType const& type)
{
  return o << static_cast<uint16_t>(type);
}
/**
 * @brief Stream a GeoID instance in a human-readable form
 * @param o Stream to output to
 * @param id GeoID to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, GeoID const& id)
{
  return o << "type: " << id.component_type << ", region:" << id.region_id << ", element: " << id.element_id;
}

/**
 * @brief Read a GeoID from a string stream
 * @param is Stream to read from
 * @param id GeoID to fill
 * @return Stream instance for further streaming
 */
inline std::istream&
operator>>(std::istream& is, GeoID& id)
{
  std::string tmp;
  uint16_t type_temp;
  is >> tmp >> type_temp >> tmp >> tmp >> id.region_id >> tmp >> tmp >> id.element_id;

  id.component_type = static_cast<GeoIDComponentType>(type_temp);

  return is;
}

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_GEOID_HPP_
