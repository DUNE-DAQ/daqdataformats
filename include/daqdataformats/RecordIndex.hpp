/**
 * @file RecordIndex.hpp Format-independent index of the contents of one record
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_RECORDINDEX_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_RECORDINDEX_HPP_

#include "daqdataformats/FragmentHeader.hpp"
#include "daqdataformats/RecordID.hpp"
#include "daqdataformats/SourceID.hpp"
#include "daqdataformats/TimeSlice.hpp"
#include "daqdataformats/TriggerRecord.hpp"

#include "detdataformats/DetID.hpp"

#include <map>
#include <optional>
#include <set>

namespace dunedaq::daqdataformats {

/**
 * @brief Per-fragment properties recorded in a RecordIndex, so that fragments can be
 * selected without reading them.
 */
struct FragmentInfo
{
  FragmentType fragment_type{ FragmentType::kUnknown };
  detdataformats::DetID::Subdetector subdetector{ detdataformats::DetID::Subdetector::kUnknown };

  bool operator==(const FragmentInfo&) const = default;
};

/**
 * @brief Criteria for selecting fragment SourceIDs from a RecordIndex. Unset criteria match
 * everything; set criteria must all match.
 */
struct SourceIDSelector
{
  std::optional<SourceID::Subsystem> subsystem;
  std::optional<FragmentType> fragment_type;
  std::optional<detdataformats::DetID::Subdetector> subdetector;

  bool matches(const SourceID& source_id, const FragmentInfo& info) const;
};

/**
 * @brief RecordIndex describes what one record (TriggerRecord or TimeSlice) contains: its
 * RecordID, the SourceID of its header, and the SourceID and FragmentInfo of each fragment.
 * It holds no data and nothing about where the data is stored, so it is independent of
 * any file format. Writers build it from a record; readers rebuild it from what a file stores.
 */
class RecordIndex
{
public:
  using fragment_map_t = std::map<SourceID, FragmentInfo>;

  RecordIndex() = default;
  RecordIndex(const RecordID& record_id, const SourceID& header_source_id)
    : m_record_id(record_id)
    , m_header_source_id(header_source_id)
  {
  }

  static RecordIndex from(const TriggerRecord& trigger_record);
  static RecordIndex from(const TimeSlice& timeslice);

  // Adds (or replaces) a fragment entry
  void add_fragment(const SourceID& source_id, const FragmentInfo& info) { m_fragments[source_id] = info; }

  const RecordID& get_record_id() const noexcept { return m_record_id; }
  const SourceID& get_header_source_id() const noexcept { return m_header_source_id; }
  const fragment_map_t& get_fragments() const noexcept { return m_fragments; }

  bool contains_fragment(const SourceID& source_id) const { return m_fragments.count(source_id) != 0; }
  std::optional<FragmentInfo> get_fragment_info(const SourceID& source_id) const;

  // Fragment SourceIDs (the header SourceID is not included) matching the selector
  std::set<SourceID> get_fragment_source_ids(const SourceIDSelector& selector = {}) const;

  // Groupings of the fragment SourceIDs, e.g. for storage in formats that keep these maps
  std::map<FragmentType, std::set<SourceID>> get_source_ids_by_fragment_type() const;
  std::map<detdataformats::DetID::Subdetector, std::set<SourceID>> get_source_ids_by_subdetector() const;
  std::map<SourceID::Subsystem, std::set<SourceID>> get_source_ids_by_subsystem() const;

  bool operator==(const RecordIndex&) const = default;

private:
  RecordID m_record_id;
  SourceID m_header_source_id;
  fragment_map_t m_fragments;
};

} // namespace dunedaq::daqdataformats

#include "detail/RecordIndex.hxx"

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_RECORDINDEX_HPP_
