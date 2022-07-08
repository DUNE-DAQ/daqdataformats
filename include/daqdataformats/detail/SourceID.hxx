
namespace dunedaq::daqdataformats {

static_assert(SourceID::s_source_id_version == 2,
              "This is intentionally designed to tell the developer to update the static_assert checks (including this "
              "one) when the version is bumped");
static_assert(static_cast<int>(SourceID::Subsystem::kCountOfEnums) == 5,
              "Unexpected number of SourceID::Subsystem enums found");
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
  return o << "subsystem: " << source_id.subsystem << " id: " << source_id.id;
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
  is >> tmp >> source_id.subsystem >> tmp >> source_id.id; // Eat last three tokens, e.g. "-> (314, 159)"

  return is;
}

bool
SourceID::operator<(const SourceID& other) const noexcept
{
  return std::tuple(subsystem, id) < std::tuple(other.subsystem, other.id);
}

bool
SourceID::operator!=(const SourceID& other) const noexcept
{
  return (*this) < other || other < (*this);
}

bool
SourceID::operator==(const SourceID& other) const noexcept
{
  return !((*this) != other);
}

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
