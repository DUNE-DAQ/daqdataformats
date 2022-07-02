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
#include <istream>
#include <limits>
#include <ostream>
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

  using Version_t = uint32_t;
  using Category_t = uint16_t;
  using ID_t = uint32_t;

  // Break the ID_t into two parts users can examine separately
  using ID_upper_t = uint16_t; 
  using ID_lower_t = ID_upper_t;

  /**
   * @brief The Category enum describes the kind of source we're dealing with
   */

  enum class Category : Category_t 
  {
    kInvalid = 0,
    kTPC = 1,
    kPDS = 2,
    kDataSelection = 3,
    kNDLArTPC = 4,
    kTriggerPrimitive = 5,
    kTriggerActivity = 6,
    kTriggerCandidate = 7,
    kCountOfEnums   // Add any new Category enums above this line. This is the count of enums besides itself.
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
   * @brief The general category of the source of the data
   */
  Category category { Category::kInvalid };

  /**
   * @brief Unique identifier of the source of the data
   */
  ID_t id{ s_invalid_id }; 

  uint32_t unused{ 0xFFFFFFFF }; ///< Ensure 64bit alignment // NOLINT(build/unsigned)

  SourceID() = default;

  SourceID(const Category& category_arg, const ID_t& id_arg)
    : category(category_arg)
    , id(id_arg)
  {}

  static ID_t compose_id(const ID_upper_t& upper, const ID_lower_t& lower);
  static void decompose_id(const ID_t& id_arg, ID_upper_t& upper, ID_lower_t& lower);

  inline bool is_in_valid_state() const noexcept { return category != Category::kInvalid && id != s_invalid_id ; }

  /**
   * @brief Comparison operators to allow SourceID to be used in std::map
   */
  inline bool operator<(const SourceID& other) const noexcept;
  inline bool operator!=(const SourceID& other) const noexcept; 
  inline bool operator==(const SourceID& other) const noexcept; 

  inline static std::string category_to_string(const Category& type);
  inline static Category string_to_category(const std::string& typestring);
};

static_assert(SourceID::s_source_id_version == 2, "This is intentionally designed to tell the developer to update the static_assert checks (including this one) when the version is bumped"); 
  static_assert(static_cast<int>(SourceID::Category::kCountOfEnums) == 8, "Unexpected number of SourceID::Category enums found");
static_assert(sizeof(SourceID) == 16, "SourceID struct size different than expected!");
static_assert(offsetof(SourceID, version) == 0, "SourceID version field not at expected offset");
static_assert(offsetof(SourceID, category) == 4, "SourceID category field not at expected offset");
static_assert(offsetof(SourceID, id) == 8, "SourceID id field not at expected offset");
static_assert(offsetof(SourceID, unused) == 12, "SourceID unused field not at expected offset");
  static_assert(sizeof(SourceID::ID_upper_t) == sizeof(SourceID::ID_lower_t));
  static_assert(sizeof(SourceID::ID_upper_t) + sizeof(SourceID::ID_lower_t) == sizeof(SourceID::ID_t));

  // We could consider adding logic s.t. the compose/decompose
  // functions only works for certain categories which are
  // meaningfully described by two elements (e.g. TPC) and not for
  // those which aren't (e.g. TriggerCandidate)

  SourceID::ID_t SourceID::compose_id(const ID_upper_t& upper, const ID_lower_t& lower) {
    static_assert(std::is_unsigned<ID_t>::value, "In SourceID::compose_id, code currently only supports an unsigned type for SourceID::ID_t");
    return ( (static_cast<ID_t>(upper) << 8*sizeof(ID_upper_t)) + static_cast<ID_t>(lower));
  }

  void SourceID::decompose_id(const ID_t& id_arg, ID_upper_t& upper, ID_lower_t& lower) {
    
    static_assert(std::is_unsigned<ID_t>::value, "In SourceID::decompose_id, code currently only supports an unsigned type for SourceID::ID_t");

    upper = static_cast<ID_upper_t>(id_arg >> 8*sizeof(ID_t)/2 );
    ID_t mask_upper = std::numeric_limits<ID_t>::max();
    mask_upper = mask_upper >> 8*sizeof(ID_t)/2; 
    
    lower = static_cast<ID_lower_t>(id_arg & mask_upper );
  }



/**
 * @brief Stream a Category instance in a human-readable form
 * @param o Stream to output to
 * @param id Category to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, SourceID::Category const& type)
{
  return o << SourceID::category_to_string(type);
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
  SourceID::ID_upper_t upper { 0 };
  SourceID::ID_lower_t lower { 0 };

  SourceID::decompose_id(source_id.id, upper, lower);

  return o << "category: " << source_id.category << " id: " << source_id.id  << " -> (" << upper << ", " << lower << ")";
}

/**
 * @brief Read a SourceID::Category from a string stream
 * @param is Stream to read from
 * @param id Category to fill
 * @return Stream instance for further streaming
 */
inline std::istream&
operator>>(std::istream& is, SourceID::Category& t)
{
  std::string tmp;
  is >> tmp;

  t = SourceID::string_to_category(tmp);

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
  is >> tmp >> source_id.category >> tmp >> source_id.id;

  return is;
}

bool
SourceID::operator<(const SourceID& other) const noexcept
{
  return std::tuple(category, id) <
         std::tuple(other.category, other.id);
}

  bool SourceID::operator!=(const SourceID& other) const noexcept
 { return (*this) < other || other < (*this); 
}

  bool SourceID::operator==(const SourceID& other) const noexcept { return !((*this) != other); }

std::string
SourceID::category_to_string(const Category& type)
{
  switch (type) {
    case Category::kTPC:
      return "TPC";
    case Category::kPDS:
      return "PDS";
    case Category::kDataSelection:
      return "DataSelection";
    case Category::kNDLArTPC:
      return "NDLArTPC";
  case Category::kTriggerPrimitive:
    return "TriggerPrimitive";
  case Category::kTriggerActivity:
    return "TriggerActivity";
  case Category::kTriggerCandidate:
    return "TriggerCandidate";
  case Category::kInvalid:
    return "Invalid";
  default:
    // See https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html. Possibly too severe.
    __builtin_unreachable();
  }
}

SourceID::Category
SourceID::string_to_category(const std::string& typestring)
{
  if (typestring == "TPC")
    return Category::kTPC;
  if (typestring == "PDS")
    return Category::kPDS;
  if (typestring == "DataSelection")
    return Category::kDataSelection;
  if (typestring == "NDLArTPC")
    return Category::kNDLArTPC;
  if (typestring == "TriggerPrimitive")
    return Category::kTriggerPrimitive;
  if (typestring == "TriggerActivity")
    return Category::kTriggerActivity;
  if (typestring == "TriggerCandidate")
    return Category::kTriggerCandidate;

  return Category::kInvalid;
}

} // namespace dunedaq::daqdataformats

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEID_HPP_
