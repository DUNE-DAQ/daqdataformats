#include <algorithm>
#include <cstring>
#include <format>
#include <span>
#include <type_traits>

namespace dunedaq::daqdataformats {

static_assert(std::is_trivially_copyable<SourceIDGeoIDMapEntry>::value,
              "SourceIDGeoIDMapEntry isn't trivially copyable and can't be safely std::memcpy'd");

static_assert(
  std::is_standard_layout<SourceIDGeoIDMapEntry>::value,
  "SourceIDGeoIDMapEntry isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");

static_assert(SourceIDGeoIDMapEntry::s_source_id_geo_id_map_entry_version == 1,
              "This is intentionally designed to tell the developer to update the static_assert checks (including this "
              "one) when the version is bumped");

static_assert(sizeof(SourceIDGeoIDMapEntry) == 24, "SourceIDGeoIDMapEntry struct size different than expected!");
static_assert(offsetof(SourceIDGeoIDMapEntry, version) == 0,
              "SourceIDGeoIDMapEntry version field not at expected offset");
static_assert(offsetof(SourceIDGeoIDMapEntry, unused) == 4,
              "SourceIDGeoIDMapEntry unused field not at expected offset");
static_assert(offsetof(SourceIDGeoIDMapEntry, source_id) == 8,
              "SourceIDGeoIDMapEntry source_id field not at expected offset");
static_assert(offsetof(SourceIDGeoIDMapEntry, geo_id) == 16,
              "SourceIDGeoIDMapEntry geo_id field not at expected offset");

static_assert(std::is_trivially_copyable<SourceIDGeoIDMapHeader>::value,
              "SourceIDGeoIDMapHeader isn't trivially copyable and can't be safely std::memcpy'd");

static_assert(
  std::is_standard_layout<SourceIDGeoIDMapHeader>::value,
  "SourceIDGeoIDMapHeader isn't standard layout; reinterpret_cast and offsetof can't safely be used with it");

static_assert(SourceIDGeoIDMapHeader::s_source_id_geo_id_map_version == 1,
              "This is intentionally designed to tell the developer to update the static_assert checks (including this "
              "one) when the version is bumped");

static_assert(sizeof(SourceIDGeoIDMapHeader) == 16, "SourceIDGeoIDMapHeader struct size different than expected!");
static_assert(offsetof(SourceIDGeoIDMapHeader, source_id_geo_id_map_marker) == 0,
              "SourceIDGeoIDMapHeader marker field not at expected offset");
static_assert(offsetof(SourceIDGeoIDMapHeader, version) == 4,
              "SourceIDGeoIDMapHeader version field not at expected offset");
static_assert(offsetof(SourceIDGeoIDMapHeader, num_entries) == 8,
              "SourceIDGeoIDMapHeader num_entries field not at expected offset");

inline const SourceIDGeoIDMapHeader*
SourceIDGeoIDMap::header_() const noexcept
{
  return reinterpret_cast<const SourceIDGeoIDMapHeader*>(m_data_arr.data()); // NOLINT
}

inline SourceIDGeoIDMapHeader*
SourceIDGeoIDMap::header_() noexcept
{
  return reinterpret_cast<SourceIDGeoIDMapHeader*>(m_data_arr.data()); // NOLINT
}

inline std::span<const SourceIDGeoIDMapEntry>
SourceIDGeoIDMap::entries_() const noexcept
{
  auto first =
    reinterpret_cast<const SourceIDGeoIDMapEntry*>(m_data_arr.data() + sizeof(SourceIDGeoIDMapHeader)); // NOLINT
  return { first, get_num_entries() };
}

inline SourceIDGeoIDMap::SourceIDGeoIDMap()
  : m_data_arr(sizeof(SourceIDGeoIDMapHeader))
{
  SourceIDGeoIDMapHeader header;
  std::memcpy(m_data_arr.data(), &header, sizeof(header));
}

inline SourceIDGeoIDMap::SourceIDGeoIDMap(const std::vector<SourceIDGeoIDMapEntry>& entries)
  : m_data_arr(sizeof(SourceIDGeoIDMapHeader) + entries.size() * sizeof(SourceIDGeoIDMapEntry))
{
  SourceIDGeoIDMapHeader header;
  header.num_entries = entries.size();
  std::memcpy(m_data_arr.data(), &header, sizeof(header));
  if (!entries.empty()) {
    std::memcpy(&m_data_arr[sizeof(header)], entries.data(), entries.size() * sizeof(SourceIDGeoIDMapEntry));
  }
}

inline SourceIDGeoIDMap::SourceIDGeoIDMap(const void* existing_source_id_geo_id_map_buffer)
{
  SourceIDGeoIDMapHeader header;
  std::memcpy(&header, existing_source_id_geo_id_map_buffer, sizeof(header));
  if (header.source_id_geo_id_map_marker != SourceIDGeoIDMapHeader::s_source_id_geo_id_map_magic ||
      header.version != SourceIDGeoIDMapHeader::s_source_id_geo_id_map_version) {
    throw std::invalid_argument(std::format("Buffer does not hold a SourceIDGeoIDMap (marker 0x{:x}, version {})",
                                            header.source_id_geo_id_map_marker,
                                            header.version));
  }
  std::span<const uint8_t> bytes(
    static_cast<const uint8_t*>(existing_source_id_geo_id_map_buffer), // NOLINT(build/unsigned)
    sizeof(header) + header.num_entries * sizeof(SourceIDGeoIDMapEntry));
  m_data_arr.assign(bytes.begin(), bytes.end());
}

inline SourceIDGeoIDMap
SourceIDGeoIDMap::from_encoded(const encoded_map_t& encoded_map)
{
  SourceIDGeoIDMap the_map;
  for (auto const& [source_id, encoded_geo_ids] : encoded_map) {
    for (auto const& encoded_geo_id : encoded_geo_ids) {
      the_map.add_unchecked(source_id, detdataformats::GeoID::decode(encoded_geo_id));
    }
  }
  return the_map;
}

inline SourceIDGeoIDMap::encoded_map_t
SourceIDGeoIDMap::to_encoded() const
{
  encoded_map_t encoded_map;
  for (auto const& entry : entries_()) {
    encoded_map[entry.source_id].push_back(entry.geo_id.encode());
  }
  return encoded_map;
}

inline void
SourceIDGeoIDMap::add(const SourceID& source_id, const detdataformats::GeoID& geo_id)
{
  for (auto const& owner : get_source_ids(geo_id)) {
    if (owner != source_id) {
      throw std::invalid_argument("GeoID " + geo_id.to_string() + " can't be added to SourceID " +
                                  source_id.to_string() + ": it already belongs to SourceID " + owner.to_string());
    }
  }
  add_unchecked(source_id, geo_id);
}

inline void
SourceIDGeoIDMap::add_unchecked(const SourceID& source_id, const detdataformats::GeoID& geo_id)
{
  // The new entry goes at the end of its SourceID's group, or where that group would start.
  auto entries = entries_();
  size_t idx = 0;
  for (; idx < entries.size() && !(source_id < entries[idx].source_id); ++idx) {
    if (entries[idx].source_id == source_id && entries[idx].geo_id == geo_id) {
      return;
    }
  }
  SourceIDGeoIDMapEntry entry(source_id, geo_id);
  std::span<const uint8_t> entry_bytes(reinterpret_cast<const uint8_t*>(&entry), sizeof(entry)); // NOLINT
  m_data_arr.insert(
    m_data_arr.begin() + sizeof(SourceIDGeoIDMapHeader) + idx * sizeof(entry), entry_bytes.begin(), entry_bytes.end());
  header_()->num_entries += 1;
}

inline std::map<SourceID, SourceIDGeoIDMap::geo_id_list_t>
SourceIDGeoIDMap::get_entries() const
{
  std::map<SourceID, geo_id_list_t> entries;
  for (auto const& entry : entries_()) {
    entries[entry.source_id].push_back(entry.geo_id);
  }
  return entries;
}

inline const SourceIDGeoIDMapEntry&
SourceIDGeoIDMap::at(size_t idx) const
{
  if (idx >= get_num_entries()) {
    throw std::range_error(
      std::format("Supplied SourceIDGeoIDMapEntry index {} out of range (size: {})", idx, get_num_entries()));
  }
  return entries_()[idx];
}

inline SourceIDGeoIDMap::geo_id_list_t
SourceIDGeoIDMap::get_geo_ids(const SourceID& source_id) const
{
  geo_id_list_t geo_ids;
  for (auto const& entry : entries_()) {
    if (entry.source_id == source_id) {
      geo_ids.push_back(entry.geo_id);
    }
  }
  return geo_ids;
}

inline std::optional<SourceID>
SourceIDGeoIDMap::get_source_id(const detdataformats::GeoID& geo_id) const
{
  auto source_ids = get_source_ids(geo_id);
  if (source_ids.size() != 1) {
    return std::nullopt;
  }
  return source_ids.front();
}

inline std::vector<SourceID>
SourceIDGeoIDMap::get_source_ids(const detdataformats::GeoID& geo_id) const
{
  std::vector<SourceID> source_ids;
  for (auto const& entry : entries_()) {
    if (entry.geo_id == geo_id &&
        std::find(source_ids.begin(), source_ids.end(), entry.source_id) == source_ids.end()) {
      source_ids.push_back(entry.source_id);
    }
  }
  return source_ids;
}

inline std::set<detdataformats::GeoID>
SourceIDGeoIDMap::get_shared_geo_ids() const
{
  std::set<detdataformats::GeoID> seen;
  std::set<detdataformats::GeoID> shared;
  for (auto const& entry : entries_()) {
    if (!seen.insert(entry.geo_id).second) {
      shared.insert(entry.geo_id);
    }
  }
  return shared;
}

inline std::set<detdataformats::GeoID>
SourceIDGeoIDMap::get_all_geo_ids() const
{
  std::set<detdataformats::GeoID> all_geo_ids;
  for (auto const& entry : entries_()) {
    all_geo_ids.insert(entry.geo_id);
  }
  return all_geo_ids;
}

inline std::set<detdataformats::GeoID>
SourceIDGeoIDMap::get_geo_ids(detdataformats::DetID::Subdetector subdetector) const
{
  std::set<detdataformats::GeoID> matching_geo_ids;
  for (auto const& entry : entries_()) {
    if (entry.geo_id.subdetector() == subdetector) {
      matching_geo_ids.insert(entry.geo_id);
    }
  }
  return matching_geo_ids;
}

inline SourceIDGeoIDMap
SourceIDGeoIDMap::restricted_to(const RecordIndex& record_index) const
{
  std::vector<SourceIDGeoIDMapEntry> entries;
  for (auto const& entry : entries_()) {
    if (record_index.contains_fragment(entry.source_id)) {
      entries.push_back(entry);
    }
  }
  return SourceIDGeoIDMap(entries);
}

} // namespace dunedaq::daqdataformats
