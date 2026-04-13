
namespace dunedaq::daqdataformats {

  static_assert(std::is_trivially_copyable<TriggerRecordHeaderData>::value, "TriggerRecordHeaderData isn't trivially copyable and can't be safely std::memcpy'd");
  
  static_assert(std::is_standard_layout<TriggerRecordHeaderData>::value, "TriggerRecordHeaderData isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");

  
static_assert(TriggerRecordHeaderData::s_trigger_record_header_version == 5,
              "This is intentionally designed to tell the developer to update the static_assert checks (including this "
              "one) when the version is bumped");

static_assert(sizeof(TriggerRecordHeaderData) == 64, "TriggerRecordHeaderData struct size different than expected!");
static_assert(offsetof(TriggerRecordHeaderData, trigger_record_header_marker) == 0,
              "TriggerRecordHeaderData trigger_record_header_marker field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, version) == 4,
              "TriggerRecordHeaderData version field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, trigger_number) == 8,
              "TriggerRecordHeaderData trigger_number field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, trigger_timestamp) == 16,
              "TriggerRecordHeaderData trigger_timestamp field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, num_requested_components) == 24,
              "TriggerRecordHeaderData num_requested_components field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, run_number) == 32,
              "TriggerRecordHeaderData run_number field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, status_bits) == 36,
              "TriggerRecordHeaderData status_bits field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, trigger_type) == 40,
              "TriggerRecordHeaderData trigger_type field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, sequence_number) == 48,
              "TriggerRecordHeaderData sequence_number field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, max_sequence_number) == 50,
              "TriggerRecordHeaderData max_sequence_number field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, unused) == 52,
              "TriggerRecordHeaderData unused field not at expected offset!");
static_assert(offsetof(TriggerRecordHeaderData, element_id) == 56,
              "TriggerRecordHeaderData source_id field not at expected offset!");
  
} // namespace dunedaq::daqdataformats
