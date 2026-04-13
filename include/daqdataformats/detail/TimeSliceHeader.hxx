
namespace dunedaq::daqdataformats {

  static_assert(std::is_trivially_copyable<TimeSliceHeader>::value, "TimeSliceHeader isn't trivially copyable and can't be safely std::memcpy'd");
  
  static_assert(std::is_standard_layout<TimeSliceHeader>::value, "TimeSliceHeader isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");
  static_assert(TimeSliceHeader::s_timeslice_header_version == 2,
              "This is intentionally designed to tell the developer to update the static_assert checks (including this "
              "one) when the version is bumped");
  static_assert(sizeof(TimeSliceHeader) == 32, "TimeSliceHeader struct size different than expected!");
  static_assert(offsetof(TimeSliceHeader, timeslice_header_marker) == 0,
		"TimeSliceHeader timeslice_header_marker field not at expected offset!");
  static_assert(offsetof(TimeSliceHeader, version) == 4, "TimeSliceHeader version field not at expected offset!");
  static_assert(offsetof(TimeSliceHeader, timeslice_number) == 8,
		"TimeSliceHeader timeslice_number field not at expected offset!");
  static_assert(offsetof(TimeSliceHeader, run_number) == 16, "TimeSliceHeader run_number field not at expected offset!");
  static_assert(offsetof(TimeSliceHeader, unused) == 20, "TimeSliceHeader unused field not at expected offset!");
  static_assert(offsetof(TimeSliceHeader, element_id) == 24, "TimeSliceHeader source_id field not at expected offset!");
  
} // namespace dunedaq::daqdataformats
