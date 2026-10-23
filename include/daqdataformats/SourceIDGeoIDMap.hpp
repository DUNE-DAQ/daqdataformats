/**
 * @file SourceIDGeoIDMap.hpp The mapping between SourceIDs and detector GeoIDs
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEIDGEOIDMAP_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEIDGEOIDMAP_HPP_

#include "daqdataformats/RecordIndex.hpp"
#include "daqdataformats/SourceID.hpp"

// REVIEW: see the note in RecordIndex.hpp about the daqdataformats -> detdataformats dependency.
#include "detdataformats/DetID.hpp"
#include "detdataformats/GeoID.hpp"

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <vector>

namespace dunedaq::daqdataformats {

/**
 * @brief SourceIDGeoIDMap records which detector GeoIDs feed each SourceID. It comes from the
 * configuration and is carried along with the data, because it is needed to interpret raw data
 * files on their own. One SourceID may carry several GeoIDs; each GeoID belongs to one SourceID.
 */
class SourceIDGeoIDMap
{
public:
  using geo_id_list_t = std::vector<detdataformats::GeoID>;
  using encoded_map_t = std::map<SourceID, std::vector<uint64_t>>; // NOLINT(build/unsigned)

  SourceIDGeoIDMap() = default;

  // Conversion from/to the form with packed 64-bit GeoIDs, as stored in raw data files
  static SourceIDGeoIDMap from_encoded(const encoded_map_t& encoded_map);
  encoded_map_t to_encoded() const;

  // Adds a GeoID to a SourceID's list (no-op if that pair is already present)
  void add(const SourceID& source_id, const detdataformats::GeoID& geo_id);

  bool empty() const noexcept { return m_geo_ids.empty(); }
  const std::map<SourceID, geo_id_list_t>& get_entries() const noexcept { return m_geo_ids; }

  // GeoIDs for one SourceID; empty if the SourceID is not in the map
  geo_id_list_t get_geo_ids(const SourceID& source_id) const;

  // The SourceID that a GeoID belongs to, if any
  std::optional<SourceID> get_source_id(const detdataformats::GeoID& geo_id) const;

  std::set<detdataformats::GeoID> get_all_geo_ids() const;
  std::set<detdataformats::GeoID> get_geo_ids(detdataformats::DetID::Subdetector subdetector) const;

  // The part of this map covering only the fragments actually present in a record, so that
  // per-record answers never report GeoIDs whose fragment is missing
  SourceIDGeoIDMap restricted_to(const RecordIndex& record_index) const;

  bool operator==(const SourceIDGeoIDMap&) const = default;

private:
  std::map<SourceID, geo_id_list_t> m_geo_ids;
};

} // namespace dunedaq::daqdataformats

#include "detail/SourceIDGeoIDMap.hxx"

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEIDGEOIDMAP_HPP_
