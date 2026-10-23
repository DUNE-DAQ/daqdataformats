
namespace dunedaq::daqdataformats {

inline bool
SourceIDSelector::matches(const SourceID& source_id, const FragmentInfo& info) const
{
  return (!subsystem || source_id.subsystem == *subsystem) &&
         (!fragment_type || info.fragment_type == *fragment_type) && (!subdetector || info.subdetector == *subdetector);
}

namespace detail {
template<typename RecordType>
void
add_fragments_to_index(RecordIndex& index, const RecordType& record)
{
  for (auto const& frag_ptr : record.get_fragments_ref()) {
    index.add_fragment(frag_ptr->get_element_id(),
                       FragmentInfo{ frag_ptr->get_fragment_type(),
                                     static_cast<detdataformats::DetID::Subdetector>(frag_ptr->get_detector_id()) });
  }
}
} // namespace detail

inline RecordIndex
RecordIndex::from(const TriggerRecord& trigger_record)
{
  auto const& header = trigger_record.get_header_ref();
  RecordIndex index(
    RecordID{ header.get_trigger_number(), static_cast<sequence_number_t>(header.get_sequence_number()) },
    header.get_element_id());
  detail::add_fragments_to_index(index, trigger_record);
  return index;
}

inline RecordIndex
RecordIndex::from(const TimeSlice& timeslice)
{
  auto const header = timeslice.get_header();
  RecordIndex index(RecordID{ header.timeslice_number }, header.element_id);
  detail::add_fragments_to_index(index, timeslice);
  return index;
}

inline std::optional<FragmentInfo>
RecordIndex::get_fragment_info(const SourceID& source_id) const
{
  auto iter = m_fragments.find(source_id);
  if (iter == m_fragments.end()) {
    return std::nullopt;
  }
  return iter->second;
}

inline std::set<SourceID>
RecordIndex::get_fragment_source_ids(const SourceIDSelector& selector) const
{
  std::set<SourceID> source_ids;
  for (auto const& [source_id, info] : m_fragments) {
    if (selector.matches(source_id, info)) {
      source_ids.insert(source_id);
    }
  }
  return source_ids;
}

inline std::map<FragmentType, std::set<SourceID>>
RecordIndex::get_source_ids_by_fragment_type() const
{
  std::map<FragmentType, std::set<SourceID>> grouped;
  for (auto const& [source_id, info] : m_fragments) {
    grouped[info.fragment_type].insert(source_id);
  }
  return grouped;
}

inline std::map<detdataformats::DetID::Subdetector, std::set<SourceID>>
RecordIndex::get_source_ids_by_subdetector() const
{
  std::map<detdataformats::DetID::Subdetector, std::set<SourceID>> grouped;
  for (auto const& [source_id, info] : m_fragments) {
    grouped[info.subdetector].insert(source_id);
  }
  return grouped;
}

inline std::map<SourceID::Subsystem, std::set<SourceID>>
RecordIndex::get_source_ids_by_subsystem() const
{
  std::map<SourceID::Subsystem, std::set<SourceID>> grouped;
  for (auto const& [source_id, info] : m_fragments) {
    grouped[source_id.subsystem].insert(source_id);
  }
  return grouped;
}

} // namespace dunedaq::daqdataformats
