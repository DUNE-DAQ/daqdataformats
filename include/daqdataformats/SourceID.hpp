/**
 * @file SourceID.hpp Identification for a DAQ component
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEID_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEID_HPP_

#include <cstddef>
#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include <tuple>

namespace dunedaq {
namespace daqdataformats {

/**
 * @brief SourceID is a generalized representation of the source of a
 * piece of data in the DAQ. That source could be a physical location
 * in the detector (i.e. detector data), a running process in the DAQ
 * (e.g. a dataflow app, the source of a data request), etc.
 */
struct SourceID
{
  /**
   * @brief The kind of source we're dealing with
   */
  enum class SystemType : uint16_t // NOLINT(build/unsigned)
  {
    kTPC = 1,
    kPDS = 2,
    kDataSelection = 3,
    kNDLArTPC = 4,
    kInvalid = 0
  };
  /**
   * @brief The current version of the SourceID
   */
  static constexpr uint32_t s_source_id_version = 1; // NOLINT(build/unsigned)
  /**
   * @brief An invalid region number, used for initialization
   */
  static constexpr uint16_t s_invalid_region_id = std::numeric_limits<uint16_t>::max(); // NOLINT(build/unsigned)
  /**
   * @brief An invalid element number, used for initialization
   */
  static constexpr uint32_t s_invalid_element_id = std::numeric_limits<uint32_t>::max(); // NOLINT(build/unsigned)

  /**
   * @brief Version number of the SourceID
   */
  uint32_t version{ s_source_id_version }; // NOLINT(build/unsigned)
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

  uint32_t unused{ 0xFFFFFFFF }; ///< Ensure 64bit alignment // NOLINT(build/unsigned)

  SourceID() {}
  inline SourceID(SystemType const& type, uint16_t const& region, uint32_t const& element); // NOLINT(build/unsigned)

  /**
   * @brief Comparison operator (to allow SourceID to be used in std::map)
   * @param other SourceID to compare
   * @return The result of std::tuple compare using SourceID fields
   */
  inline bool operator<(const SourceID& other) const noexcept;

  /**
   * @brief Comparison operator (to allow SourceID comparisons)
   * @param other SourceID to compare
   * @return The result of std::tuple compare using SourceID fields
   */
  bool operator!=(const SourceID& other) const noexcept { return (*this) < other || other < (*this); }

  /**
   * @brief Comparison operator (to allow SourceID comparisons)
   * @param other SourceID to compare
   * @return The result of std::tuple compare using SourceID fields
   */
  bool operator==(const SourceID& other) const noexcept { return !((*this) != other); }

  inline static std::string system_type_to_string(SystemType type);
  inline static SystemType string_to_system_type(std::string typestring);
};

static_assert(sizeof(SourceID) == 16, "SourceID struct size different than expected!");
static_assert(offsetof(SourceID, version) == 0, "SourceID version field not at expected offset");
static_assert(offsetof(SourceID, system_type) == 4, "SourceID system_type field not at expected offset");
static_assert(offsetof(SourceID, region_id) == 6, "SourceID region_id field not at expected offset");
static_assert(offsetof(SourceID, element_id) == 8, "SourceID element_id field not at expected offset");
static_assert(offsetof(SourceID, unused) == 12, "SourceID unused field not at expected offset");

/**
 * @brief Stream a SystemType instance in a human-readable form
 * @param o Stream to output to
 * @param id SystemType to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, SourceID::SystemType const& type)
{
  return o << SourceID::system_type_to_string(type);
}
/**
 * @brief Stream a SourceID instance in a human-readable form
 * @param o Stream to output to
 * @param id SourceID to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, SourceID const& id)
{
  return o << "type: " << id.system_type << ", region: " << id.region_id << ", element: " << id.element_id;
}

/**
 * @brief Read a SourceID::SystemType from a string stream
 * @param is Stream to read from
 * @param id SystemType to fill
 * @return Stream instance for further streaming
 */
inline std::istream&
operator>>(std::istream& is, SourceID::SystemType& t)
{
  std::string tmp;
  is >> tmp;

  t = SourceID::string_to_system_type(tmp);

  return is;
}
/**
 * @brief Read a SourceID from a string stream
 * @param is Stream to read from
 * @param id SourceID to fill
 * @return Stream instance for further streaming
 */
inline std::istream&
operator>>(std::istream& is, SourceID& id)
{
  std::string tmp;
  is >> tmp >> id.system_type >> tmp >> id.region_id >> tmp >> tmp >> id.element_id;

  return is;
}

SourceID::SourceID(SourceID::SystemType const& type, uint16_t const& region, uint32_t const& element) // NOLINT(build/unsigned)
  : version(s_source_id_version)
  , system_type(type)
  , region_id(region)
  , element_id(element)
{}

bool
SourceID::operator<(const SourceID& other) const noexcept
{
  return std::tuple(system_type, region_id, element_id) <
         std::tuple(other.system_type, other.region_id, other.element_id);
}

std::string
SourceID::system_type_to_string(SystemType type)
{
  switch (type) {
    case SystemType::kTPC:
      return "TPC";
    case SystemType::kPDS:
      return "PDS";
    case SystemType::kDataSelection:
      return "DataSelection";
    case SystemType::kNDLArTPC:
      return "NDLArTPC";
    case SystemType::kInvalid:
      return "Invalid";
  }
  return "Unknown";
}

SourceID::SystemType
SourceID::string_to_system_type(std::string typestring)
{
  if (typestring.find("TPC") == 0)
    return SystemType::kTPC;
  if (typestring.find("PDS") == 0)
    return SystemType::kPDS;
  if (typestring.find("DataSelection") == 0)
    return SystemType::kDataSelection;
  if (typestring.find("NDLArTPC") == 0)
    return SystemType::kNDLArTPC;
  return SystemType::kInvalid;
}

} // namespace daqdataformats
} // namespace dunedaq

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEID_HPP_
