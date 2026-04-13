
namespace dunedaq::daqdataformats {

static_assert(std::is_trivially_copyable<FragmentHeader>::value, "FragmentHeader isn't trivially copyable and can't be safely std::memcpy'd");
  
static_assert(std::is_standard_layout<FragmentHeader>::value, "FragmentHeader isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");
  
static_assert(FragmentHeader::s_fragment_header_version == 6,
              "This is intentionally designed to tell the developer to update the static_assert checks (including this "
              "one) when the version is bumped");

static_assert(sizeof(FragmentHeader) == 72, "FragmentHeader struct size different than expected!");
static_assert(offsetof(FragmentHeader, fragment_header_marker) == 0,
              "FragmentHeader fragment_header_marker field not at expected offset!");
static_assert(offsetof(FragmentHeader, version) == 4, "FragmentHeader version field not at expected offset!");
static_assert(offsetof(FragmentHeader, size) == 8, "FragmentHeader size field not at expected offset!");
static_assert(offsetof(FragmentHeader, trigger_number) == 16,
              "FragmentHeader trigger_number field not at expected offset!");
static_assert(offsetof(FragmentHeader, trigger_timestamp) == 24,
              "FragmentHeader trigger_timestamp field not at expected offset!");
static_assert(offsetof(FragmentHeader, window_begin) == 32,
              "FragmentHeader window_begin field not at expected offset!");
static_assert(offsetof(FragmentHeader, window_end) == 40, "FragmentHeader window_end field not at expected offset!");
static_assert(offsetof(FragmentHeader, run_number) == 48, "FragmentHeader run_number field not at expected offset!");
static_assert(offsetof(FragmentHeader, status_bits) == 52, "FragmentHeader status_bits field not at expected offset!");
static_assert(offsetof(FragmentHeader, fragment_type) == 56,
              "FragmentHeader fragment_type field not at expected offset!");
static_assert(offsetof(FragmentHeader, sequence_number) == 60,
              "FragmentHeader sequence_number field not at expected offset!");
static_assert(offsetof(FragmentHeader, detector_id) == 62, "FragmentHeader detector_id field not at expected offset!");
static_assert(offsetof(FragmentHeader, element_id) == 64, "FragmentHeader element_id field not at expected offset!");

inline std::ostream&
operator<<(std::ostream& o, FragmentHeader const& hdr)
{
  return o << "check_word: " << std::hex << hdr.fragment_header_marker << std::dec << ", " << "version: " << hdr.version
           << ", " << "size: " << hdr.size << ", " << "trigger_number: " << hdr.trigger_number << ", "
           << "run_number: " << hdr.run_number << ", " << "trigger_timestamp: " << hdr.trigger_timestamp << ", "
           << "window_begin: " << hdr.window_begin << ", " << "window_end: " << hdr.window_end << ", "
           << "status_bits: " << hdr.status_bits << ", " << "fragment_type: " << hdr.fragment_type << ", "
           << "sequence_number: " << hdr.sequence_number << ", " << "detector_id: " << hdr.detector_id << ", "
           << "element_id: " << hdr.element_id;
}
  
} // namespace dunedaq::daqdataformats
