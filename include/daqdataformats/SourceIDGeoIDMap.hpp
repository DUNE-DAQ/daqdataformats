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

#include "detdataformats/DetID.hpp"
#include "detdataformats/GeoID.hpp"

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

namespace dunedaq::daqdataformats {

/**
 * @brief SourceIDGeoIDMap records which detector GeoIDs feed each SourceID. It comes from the
 * configuration and is carried along with the data, because it is needed to interpret raw data
 * files on their own. One SourceID may carry several GeoIDs; each GeoID belongs to one SourceID.
 *
 * That rule is enforced when a map is built (add()), so a configuration that gives one GeoID to two
 * SourceIDs is caught before any data is written. Reading a stored map (from_encoded()) is lenient,
 * so an existing file that breaks the rule can still be read: get_shared_geo_ids() then lists the
 * GeoIDs that have more than one SourceID, and get_source_id() gives no answer for them.
 */
class SourceIDGeoIDMap
{
public:
  using geo_id_list_t = std::vector<detdataformats::GeoID>;
  using encoded_map_t = std::map<SourceID, std::vector<uint64_t>>; // NOLINT(build/unsigned)

  SourceIDGeoIDMap() = default;

  // Conversion from/to the form with packed 64-bit GeoIDs, as stored in raw data files.
  // from_encoded() keeps every stored pair, even a GeoID listed under more than one SourceID.
  static SourceIDGeoIDMap from_encoded(const encoded_map_t& encoded_map);
  encoded_map_t to_encoded() const;

  // Adds a GeoID to a SourceID's list (no-op if that pair is already present). Throws
  // std::invalid_argument, leaving the map unchanged, if the GeoID already belongs to another SourceID.
  void add(const SourceID& source_id, const detdataformats::GeoID& geo_id);

  bool empty() const noexcept { return m_geo_ids.empty(); }
  const std::map<SourceID, geo_id_list_t>& get_entries() const noexcept { return m_geo_ids; }

  // GeoIDs for one SourceID; empty if the SourceID is not in the map
  geo_id_list_t get_geo_ids(const SourceID& source_id) const;

  // The SourceID that a GeoID belongs to; none if the GeoID isn't in the map, or if it has more
  // than one SourceID (possible only in a map read with from_encoded())
  std::optional<SourceID> get_source_id(const detdataformats::GeoID& geo_id) const;

  // Every SourceID that a GeoID is listed under (at most one, except in a map read with from_encoded())
  std::vector<SourceID> get_source_ids(const detdataformats::GeoID& geo_id) const;

  // The GeoIDs listed under more than one SourceID; empty for any map built with add()
  std::set<detdataformats::GeoID> get_shared_geo_ids() const;

  std::set<detdataformats::GeoID> get_all_geo_ids() const;
  std::set<detdataformats::GeoID> get_geo_ids(detdataformats::DetID::Subdetector subdetector) const;

  // The part of this map covering only the fragments actually present in a record, so that
  // per-record answers never report GeoIDs whose fragment is missing
  SourceIDGeoIDMap restricted_to(const RecordIndex& record_index) const;

  bool operator==(const SourceIDGeoIDMap&) const = default;

private:
  // add() without the one-SourceID-per-GeoID check, for reading stored maps
  void add_unchecked(const SourceID& source_id, const detdataformats::GeoID& geo_id);

  std::map<SourceID, geo_id_list_t> m_geo_ids;
};

} // namespace dunedaq::daqdataformats

#include "detail/SourceIDGeoIDMap.hxx"

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEIDGEOIDMAP_HPP_
