
#include <algorithm>

namespace dunedaq::daqdataformats {

inline SourceIDGeoIDMap
SourceIDGeoIDMap::from_encoded(const encoded_map_t& encoded_map)
{
  SourceIDGeoIDMap the_map;
  for (auto const& [source_id, encoded_geo_ids] : encoded_map) {
    for (auto const& encoded_geo_id : encoded_geo_ids) {
      the_map.add(source_id, detdataformats::GeoID::decode(encoded_geo_id));
    }
  }
  return the_map;
}

inline SourceIDGeoIDMap::encoded_map_t
SourceIDGeoIDMap::to_encoded() const
{
  encoded_map_t encoded_map;
  for (auto const& [source_id, geo_ids] : m_geo_ids) {
    auto& encoded_geo_ids = encoded_map[source_id];
    for (auto const& geo_id : geo_ids) {
      encoded_geo_ids.push_back(geo_id.encode());
    }
  }
  return encoded_map;
}

inline void
SourceIDGeoIDMap::add(const SourceID& source_id, const detdataformats::GeoID& geo_id)
{
  auto& geo_ids = m_geo_ids[source_id];
  if (std::find(geo_ids.begin(), geo_ids.end(), geo_id) == geo_ids.end()) {
    geo_ids.push_back(geo_id);
  }
}

inline SourceIDGeoIDMap::geo_id_list_t
SourceIDGeoIDMap::get_geo_ids(const SourceID& source_id) const
{
  auto iter = m_geo_ids.find(source_id);
  return (iter == m_geo_ids.end()) ? geo_id_list_t{} : iter->second;
}

inline std::optional<SourceID>
SourceIDGeoIDMap::get_source_id(const detdataformats::GeoID& geo_id) const
{
  for (auto const& [source_id, geo_ids] : m_geo_ids) {
    if (std::find(geo_ids.begin(), geo_ids.end(), geo_id) != geo_ids.end()) {
      return source_id;
    }
  }
  return std::nullopt;
}

inline std::set<detdataformats::GeoID>
SourceIDGeoIDMap::get_all_geo_ids() const
{
  std::set<detdataformats::GeoID> all_geo_ids;
  for (auto const& [source_id, geo_ids] : m_geo_ids) {
    all_geo_ids.insert(geo_ids.begin(), geo_ids.end());
  }
  return all_geo_ids;
}

inline std::set<detdataformats::GeoID>
SourceIDGeoIDMap::get_geo_ids(detdataformats::DetID::Subdetector subdetector) const
{
  std::set<detdataformats::GeoID> matching_geo_ids;
  for (auto const& [source_id, geo_ids] : m_geo_ids) {
    for (auto const& geo_id : geo_ids) {
      if (geo_id.subdetector() == subdetector) {
        matching_geo_ids.insert(geo_id);
      }
    }
  }
  return matching_geo_ids;
}

inline SourceIDGeoIDMap
SourceIDGeoIDMap::restricted_to(const RecordIndex& record_index) const
{
  SourceIDGeoIDMap restricted;
  for (auto const& [source_id, geo_ids] : m_geo_ids) {
    if (record_index.contains_fragment(source_id)) {
      restricted.m_geo_ids[source_id] = geo_ids;
    }
  }
  return restricted;
}

} // namespace dunedaq::daqdataformats
