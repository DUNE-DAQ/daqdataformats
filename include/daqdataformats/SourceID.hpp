/**
 * @file SourceID.hpp Identification for the origin of a dunedaq::daqdataformats::Fragment instance in the DAQ 
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEID_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEID_HPP_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>

namespace dunedaq::daqdataformats {

/**
 * @brief SourceID is a generalized representation of the source of a
 * piece of data in the DAQ. That source could be a physical location
 * in the detector, a running process in the DAQ
 * (e.g. a dataflow app, the source of a data request), etc.
 */
struct SourceID
{

  using Version_t = uint16_t;  // NOLINT(build/unsigned)
  using Subsystem_t = uint16_t; // NOLINT(build/unsigned)
  using ID_t = uint32_t;       // NOLINT(build/unsigned)

  /**
   * @brief The Subsystem enum describes the kind of source we're dealing with
   */

  enum class Subsystem : Subsystem_t 
  {
    kDRO = 0,
    kHSI = 1,
    kTRG = 2,
    kTRB = 3,
    kUNDEFINED = 4,
    kCountOfEnums   // Add any new Subsystem enums above this line. This is the count of enums besides itself.
  };

  /**
   * @brief The version of this SourceID struct. 
   */
  static constexpr Version_t s_source_id_version = 2; // Taking SourceID as the direct successor of GeoID which had version 1

  /**
   * @brief A value for the id meant to convey that it doesn't identify a data source and shouldn't be worked with
   */
  static constexpr ID_t s_invalid_id = std::numeric_limits<ID_t>::max(); 

 
 /**
   * @brief Version number of the SourceID
   */
  Version_t version{ s_source_id_version }; 
  /**
   * @brief The general subsystem of the source of the data
   */
  Subsystem subsystem { Subsystem::kUNDEFINED };

  /**
   * @brief Unique identifier of the source of the data
   */
  ID_t id{ s_invalid_id }; 

  SourceID() = default;

  SourceID(const Subsystem& subsystem_arg, const ID_t& id_arg)
    : subsystem(subsystem_arg)
    , id(id_arg)
  {}

  std::string to_string() const { 
    std::ostringstream ostr;
    ostr << subsystem_to_string(subsystem) << "_0x" << std::hex << std::setfill('0') << std::setw(2*sizeof(id)) << id;
    return ostr.str(); 
  }

  bool is_in_valid_state() const noexcept { return subsystem != Subsystem::kUNDEFINED && id != s_invalid_id ; }

  /**
   * @brief Comparison operators to allow SourceID to be used in std::map
   */
  inline bool operator<(const SourceID& other) const noexcept;
  inline bool operator!=(const SourceID& other) const noexcept; 
  inline bool operator==(const SourceID& other) const noexcept; 

  inline static std::string subsystem_to_string(const Subsystem& type);
  inline static Subsystem string_to_subsystem(const std::string& typestring);
};

static_assert(SourceID::s_source_id_version == 2, "This is intentionally designed to tell the developer to update the static_assert checks (including this one) when the version is bumped"); 
  static_assert(static_cast<int>(SourceID::Subsystem::kCountOfEnums) == 5, "Unexpected number of SourceID::Subsystem enums found");
static_assert(sizeof(SourceID) == 8, "SourceID struct size different than expected!");
static_assert(offsetof(SourceID, version) == 0, "SourceID version field not at expected offset");
static_assert(offsetof(SourceID, subsystem) == 2, "SourceID subsystem field not at expected offset");
static_assert(offsetof(SourceID, id) == 4, "SourceID id field not at expected offset");


/**
 * @brief Stream a Subsystem instance in a human-readable form
 * @param o Stream to output to
 * @param id Subsystem to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, SourceID::Subsystem const& type)
{
  return o << SourceID::subsystem_to_string(type);
}

/**
 * @brief Stream a SourceID instance in a human-readable form
 * @param o Stream to output to
 * @param id SourceID to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, SourceID const& source_id)
{
  return o << "subsystem: " << source_id.subsystem << " id: " << source_id.id ;
}

/**
 * @brief Read a SourceID::Subsystem from a string stream
 * @param is Stream to read from
 * @param id Subsystem to fill
 * @return Stream instance for further streaming
 */
inline std::istream&
operator>>(std::istream& is, SourceID::Subsystem& t)
{
  std::string tmp;
  is >> tmp;

  t = SourceID::string_to_subsystem(tmp);

  return is;
}
/**
 * @brief Read a SourceID from a string stream
 * @param is Stream to read from
 * @param id SourceID to fill
 * @return Stream instance for further streaming
 */
inline std::istream&
operator>>(std::istream& is, SourceID& source_id)
{
  std::string tmp;
  is >> tmp >> source_id.subsystem >> tmp >> source_id.id ; // Eat last three tokens, e.g. "-> (314, 159)"

  return is;
}

bool
SourceID::operator<(const SourceID& other) const noexcept
{
  return std::tuple(subsystem, id) <
         std::tuple(other.subsystem, other.id);
}

  bool SourceID::operator!=(const SourceID& other) const noexcept
 { return (*this) < other || other < (*this); 
}

  bool SourceID::operator==(const SourceID& other) const noexcept { return !((*this) != other); }

std::string
SourceID::subsystem_to_string(const Subsystem& type)
{
  switch (type) {
    case Subsystem::kDRO:
      return "DRO";
    case Subsystem::kHSI:
      return "HSI";
    case Subsystem::kTRG:
      return "TRG";
    case Subsystem::kTRB:
      return "TRB";
  case Subsystem::kUNDEFINED:
    return "UNDEFINED";
  default:
    // See https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html. Possibly too severe.
    __builtin_unreachable();
  }
}

SourceID::Subsystem
SourceID::string_to_subsystem(const std::string& typestring)
{
  if (typestring == "DRO")
    return Subsystem::kDRO;
  if (typestring == "HSI")
    return Subsystem::kHSI;
  if (typestring == "TRG")
    return Subsystem::kTRG;
  if (typestring == "TRB")
    return Subsystem::kTRB;

  return Subsystem::kUNDEFINED;
}

} // namespace dunedaq::daqdataformats

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEID_HPP_
