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

#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <stdexcept>
#include <vector>

namespace dunedaq::daqdataformats {

/**
 * @brief One SourceID-to-GeoID pair: the element of the flat array that a SourceIDGeoIDMap wraps
 */
struct SourceIDGeoIDMapEntry
{
  static constexpr uint32_t s_source_id_geo_id_map_entry_version = 1; // NOLINT(build/unsigned)

  uint32_t version{ s_source_id_geo_id_map_entry_version }; // NOLINT(build/unsigned)
  uint32_t unused{ 0xFFFFFFFF }; ///< Padding to ensure 64b alignment // NOLINT(build/unsigned)

  SourceID source_id;
  detdataformats::GeoID geo_id;

  SourceIDGeoIDMapEntry() = default;
  SourceIDGeoIDMapEntry(const SourceID& source_id_arg, const detdataformats::GeoID& geo_id_arg)
    : source_id(source_id_arg)
    , geo_id(geo_id_arg)
  {
  }
};

/**
 * @brief Data fields at the start of a SourceIDGeoIDMap's flat array, followed by num_entries SourceIDGeoIDMapEntry
 */
struct SourceIDGeoIDMapHeader
{
  /// @brief Magic bytes used to identify a SourceIDGeoIDMapHeader struct in a raw data stream
  static constexpr uint32_t s_source_id_geo_id_map_magic = 0x77778888; // NOLINT(build/unsigned)

  static constexpr uint32_t s_source_id_geo_id_map_version = 1; // NOLINT(build/unsigned)

  uint32_t source_id_geo_id_map_marker = s_source_id_geo_id_map_magic; // NOLINT(build/unsigned)
  uint32_t version = s_source_id_geo_id_map_version;                   // NOLINT(build/unsigned)

  /// @brief Number of SourceIDGeoIDMapEntry objects following the header
  uint64_t num_entries{ 0 }; // NOLINT(build/unsigned)
};

/**
 * @brief SourceIDGeoIDMap records which detector GeoIDs feed each SourceID. It comes from the
 * configuration and is carried along with the data, because it is needed to interpret raw data
 * files on their own. One SourceID may carry several GeoIDs; each GeoID belongs to one SourceID.
 * Its stored form is a flat array: a SourceIDGeoIDMapHeader followed by one SourceIDGeoIDMapEntry
 * per pair, grouped by SourceID in SourceID order.
 *
 * The one-SourceID-per-GeoID rule is enforced when a map is built (add()), so a configuration that
 * gives one GeoID to two SourceIDs is caught before any data is written. Reading a stored map is
 * lenient, so an existing file that breaks the rule can still be read: get_shared_geo_ids() then
 * lists the GeoIDs that have more than one SourceID, and get_source_id() gives no answer for them.
 */
class SourceIDGeoIDMap
{
public:
  using geo_id_list_t = std::vector<detdataformats::GeoID>;
  using encoded_map_t = std::map<SourceID, std::vector<uint64_t>>; // NOLINT(build/unsigned)

  SourceIDGeoIDMap();

  // Copies the entries as given, without checks or regrouping.
  explicit SourceIDGeoIDMap(const std::vector<SourceIDGeoIDMapEntry>& entries);

  // Copies the flat array (header and entries) at the pointer, which must hold all of it. Throws
  // std::invalid_argument if it does not start with a SourceIDGeoIDMapHeader of a known version.
  explicit SourceIDGeoIDMap(const void* existing_source_id_geo_id_map_buffer);

  // Conversion from/to the form with packed 64-bit GeoIDs, as stored in raw data files.
  // from_encoded() keeps every stored pair, even a GeoID listed under more than one SourceID.
  static SourceIDGeoIDMap from_encoded(const encoded_map_t& encoded_map);
  encoded_map_t to_encoded() const;

  // Adds a GeoID to a SourceID's list (no-op if that pair is already present). Throws
  // std::invalid_argument, leaving the map unchanged, if the GeoID already belongs to another SourceID.
  void add(const SourceID& source_id, const detdataformats::GeoID& geo_id);

  bool empty() const noexcept { return get_num_entries() == 0; }

  // The map's content grouped by SourceID
  std::map<SourceID, geo_id_list_t> get_entries() const;

  uint64_t get_num_entries() const noexcept { return header_()->num_entries; } // NOLINT(build/unsigned)

  // Throws std::range_error if idx is out of range.
  const SourceIDGeoIDMapEntry& at(size_t idx) const;

  // The flat array (header and entries), as stored in raw data files
  size_t get_total_size_bytes() const noexcept { return m_data_arr.size(); }
  const void* get_storage_location() const noexcept { return m_data_arr.data(); }

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

  const SourceIDGeoIDMapHeader* header_() const noexcept;
  SourceIDGeoIDMapHeader* header_() noexcept;
  std::span<const SourceIDGeoIDMapEntry> entries_() const noexcept;

  std::vector<uint8_t> m_data_arr; // NOLINT(build/unsigned) a SourceIDGeoIDMapHeader, then the entries
};

} // namespace dunedaq::daqdataformats

#include "detail/SourceIDGeoIDMap.hxx"

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_SOURCEIDGEOIDMAP_HPP_
