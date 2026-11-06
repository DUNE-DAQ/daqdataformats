
#include <sstream>
#include <string>

namespace dunedaq::daqdataformats {

/**
 * @brief Stream a RecordID instance in a human-readable form ("number.sequence")
 * @param o Stream to output to
 * @param record_id RecordID to stream
 * @return Stream instance for further streaming
 */
inline std::ostream&
operator<<(std::ostream& o, RecordID const& record_id)
{
  return o << record_id.number << "." << record_id.sequence;
}

inline std::string
RecordID::to_string() const
{
  std::ostringstream oss;
  oss << *this;
  return oss.str();
}

} // namespace dunedaq::daqdataformats
