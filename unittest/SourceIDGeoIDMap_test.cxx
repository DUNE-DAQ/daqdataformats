/**
 * @file SourceIDGeoIDMap_test.cxx SourceIDGeoIDMap class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdataformats/SourceIDGeoIDMap.hpp"

#define BOOST_TEST_MODULE SourceIDGeoIDMap_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <cstdint>
#include <vector>

using namespace dunedaq::daqdataformats;
using dunedaq::detdataformats::GeoID;
using Subdetector = dunedaq::detdataformats::DetID::Subdetector;

namespace {

const SourceID tpc_0{ SourceID::Subsystem::kDetectorReadout, 0 };
const SourceID tpc_1{ SourceID::Subsystem::kDetectorReadout, 1 };
const SourceID pds_0{ SourceID::Subsystem::kDetectorReadout, 10 };

const GeoID tpc_0_a{ Subdetector::kHD_TPC, 1, 0, 0 };
const GeoID tpc_0_b{ Subdetector::kHD_TPC, 1, 0, 1 };
const GeoID tpc_1_a{ Subdetector::kHD_TPC, 1, 1, 0 };
const GeoID pds_0_a{ Subdetector::kHD_PDS, 2, 0, 0 };

SourceIDGeoIDMap
make_map()
{
  SourceIDGeoIDMap the_map;
  the_map.add(tpc_0, tpc_0_a);
  the_map.add(tpc_0, tpc_0_b);
  the_map.add(tpc_1, tpc_1_a);
  the_map.add(pds_0, pds_0_a);
  return the_map;
}

} // namespace

BOOST_AUTO_TEST_SUITE(SourceIDGeoIDMap_test)

BOOST_AUTO_TEST_CASE(Lookups)
{
  auto the_map = make_map();
  BOOST_REQUIRE(!the_map.empty());
  BOOST_REQUIRE((the_map.get_geo_ids(tpc_0) == SourceIDGeoIDMap::geo_id_list_t{ tpc_0_a, tpc_0_b }));
  BOOST_REQUIRE(the_map.get_geo_ids(SourceID(SourceID::Subsystem::kTrigger, 0)).empty());

  BOOST_REQUIRE(the_map.get_source_id(tpc_1_a) == tpc_1);
  BOOST_REQUIRE(!the_map.get_source_id(GeoID(Subdetector::kHD_CRT, 0, 0, 0)).has_value());

  BOOST_REQUIRE_EQUAL(the_map.get_all_geo_ids().size(), 4);
  BOOST_REQUIRE_EQUAL(the_map.get_geo_ids(Subdetector::kHD_TPC).size(), 3);
  BOOST_REQUIRE_EQUAL(the_map.get_geo_ids(Subdetector::kHD_PDS).size(), 1);
}

BOOST_AUTO_TEST_CASE(AddIsIdempotent)
{
  auto the_map = make_map();
  the_map.add(tpc_0, tpc_0_a);
  BOOST_REQUIRE_EQUAL(the_map.get_geo_ids(tpc_0).size(), 2);
}

BOOST_AUTO_TEST_CASE(EncodedRoundTrip)
{
  auto the_map = make_map();
  auto encoded = the_map.to_encoded();
  BOOST_REQUIRE_EQUAL(encoded.size(), 3);
  BOOST_REQUIRE_EQUAL(encoded[tpc_0].size(), 2);
  BOOST_REQUIRE_EQUAL(encoded[tpc_0][0], tpc_0_a.encode());
  BOOST_REQUIRE(SourceIDGeoIDMap::from_encoded(encoded) == the_map);
}

BOOST_AUTO_TEST_CASE(RestrictedToRecord)
{
  auto the_map = make_map();

  // a record in which the tpc_1 fragment is missing
  RecordIndex index(RecordID{ 5 }, SourceID(SourceID::Subsystem::kTRBuilder, 0));
  index.add_fragment(tpc_0, FragmentInfo{ FragmentType::kWIBEth, Subdetector::kHD_TPC });
  index.add_fragment(pds_0, FragmentInfo{ FragmentType::kDAPHNE, Subdetector::kHD_PDS });

  auto restricted = the_map.restricted_to(index);
  BOOST_REQUIRE_EQUAL(restricted.get_entries().size(), 2);
  BOOST_REQUIRE(restricted.get_geo_ids(tpc_1).empty());
  BOOST_REQUIRE_EQUAL(restricted.get_geo_ids(Subdetector::kHD_TPC).size(), 2);
  BOOST_REQUIRE(!restricted.get_source_id(tpc_1_a).has_value());
}

BOOST_AUTO_TEST_SUITE_END()
