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


/**
 * @brief Represents a coordinate point in the DAQ's logical coordinate system (i.e. not physical coordinates)
 */
struct GeoID
{
  /**
   * @brief The readout system to which the component belongs
   */
  enum class SystemType : uint16_t
  {
    kTPC = 1,
    kPDS = 2,
    kDataSelection = 3,
    kInvalid = 0
  };
  /**
   * @brief The current version of the GeoID
   */
  static constexpr uint32_t s_geo_id_version = 1; // NOLINT(build/unsigned)
  /**
   * @brief An invalid region number, used for initialization
   */
  static constexpr uint16_t s_invalid_region_id = std::numeric_limits<uint16_t>::max(); // NOLINT(build/unsigned)
  /**
   * @brief An invalid element number, used for initialization
   */
  static constexpr uint32_t s_invalid_element_id = std::numeric_limits<uint32_t>::max(); // NOLINT(build/unsigned)

  /**
   * @brief Version number of the GeoID
   */
  uint32_t version{ s_geo_id_version };
  /**
   * @brief The type of the component (i.e. which system it belongs to)
  */
  SystemType system_type{ SystemType::kInvalid };

  /**
   * @brief Region number of the component
   */
  uint16_t region_id{ s_invalid_region_id }; // NOLINT(build/unsigned)
  /**
   * @brief Number of the component within the region
   */
  uint32_t element_id{ s_invalid_element_id }; // NOLINT(build/unsigned)

  uint32_t unused {0xFFFFFFFF}; ///< Ensure 64bit alignment

  GeoID() {}
  GeoID(SystemType const& type, uint16_t const& region, uint32_t const& element)
    : version(s_geo_id_version)
    , system_type(type)
    , region_id(region)
    , element_id(element)
  {}

  /**
   * @brief Comparison operator (to allow GeoID to be used in std::map)
   * @param other GeoID to compare
   * @return The result of std::tuple compare using GeoID fields
   */
  bool operator<(const GeoID& other) const noexcept
  {
    return std::tuple(system_type, region_id, element_id) <
           std::tuple(other.system_type, other.region_id, other.element_id);
  }

  /**
   * @brief Comparison operator (to allow GeoID comparisons)
   * @param other GeoID to compare
   * @return The result of std::tuple compare using GeoID fields
   */
  bool operator!=(const GeoID& other) const noexcept { return (*this) < other || other < (*this); }

  /**
   * @brief Comparison operator (to allow GeoID comparisons)
   * @param other GeoID to compare
   * @return The result of std::tuple compare using GeoID fields
   */
  bool operator==(const GeoID& other) const noexcept { return !((*this) != other); }

  static std::string system_type_to_string(SystemType type)
  {
    switch (type) {
      case SystemType::kTPC:
        return "TPC";
      case SystemType::kPDS:
        return "PDS";
      case SystemType::kDataSelection:
        return "DataSelection";
      case SystemType::kInvalid:
        return "Invalid";
    }
    return "Unknown";
  }
  static SystemType string_to_system_type(std::string typestring)
  {
    if (typestring.find("TPC") == 0)
      return SystemType::kTPC;
    if (typestring.find("PDS") == 0)
      return SystemType::kPDS;
    if (typestring.find("DataSelection") == 0)
      return SystemType::kDataSelection;
    return SystemType::kInvalid;
  }
};

/**
 * @brief Stream a SystemType instance in a human-readable form
 * @param o Stream to output to
 * @param id SystemType to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, GeoID::SystemType const& type)
{
  return o << GeoID::system_type_to_string(type);
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
  return o << "type: " << id.system_type << ", region: " << id.region_id << ", element: " << id.element_id;
}

/**
 * @brief Read a GeoID::SystemType from a string stream
 * @param is Stream to read from
 * @param id SystemType to fill
 * @return Stream instance for further streaming
 */
inline std::istream&
operator>>(std::istream& is, GeoID::SystemType& t)
{
  std::string tmp;
  is >> tmp;

  t = GeoID::string_to_system_type(tmp);

  return is;
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
  is >> tmp >> id.system_type >> tmp >> id.region_id >> tmp >> tmp >> id.element_id;

  return is;
}

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_GEOID_HPP_
