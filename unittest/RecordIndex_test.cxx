/**
 * @file RecordIndex_test.cxx RecordID, SourceIDSelector and RecordIndex Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdataformats/RecordID.hpp"
#include "daqdataformats/RecordIndex.hpp"

#define BOOST_TEST_MODULE RecordIndex_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace dunedaq::daqdataformats;
using Subdetector = dunedaq::detdataformats::DetID::Subdetector;

namespace {

std::unique_ptr<Fragment>
make_fragment(const SourceID& source_id, FragmentType fragment_type, Subdetector subdetector)
{
  std::vector<char> payload(10);
  auto frag = std::make_unique<Fragment>(payload.data(), payload.size());
  FragmentHeader fh = frag->get_header();
  fh.element_id = source_id;
  fh.fragment_type = static_cast<fragment_type_t>(fragment_type);
  fh.detector_id = static_cast<uint16_t>(subdetector); // NOLINT(build/unsigned)
  frag->set_header_fields(fh);
  return frag;
}

const SourceID tpc_0{ SourceID::Subsystem::kDetectorReadout, 0 };
const SourceID tpc_1{ SourceID::Subsystem::kDetectorReadout, 1 };
const SourceID pds_0{ SourceID::Subsystem::kDetectorReadout, 10 };
const SourceID ta_0{ SourceID::Subsystem::kTrigger, 0 };
const SourceID trb_header{ SourceID::Subsystem::kTRBuilder, 3 };

TriggerRecord
make_trigger_record()
{
  std::vector<ComponentRequest> components(4);
  TriggerRecordHeader header(components);
  header.set_trigger_number(42);
  header.set_sequence_number(2);
  header.set_element_id(trb_header);

  TriggerRecord record(header);
  record.add_fragment(make_fragment(tpc_0, FragmentType::kWIBEth, Subdetector::kHD_TPC));
  record.add_fragment(make_fragment(tpc_1, FragmentType::kWIBEth, Subdetector::kHD_TPC));
  record.add_fragment(make_fragment(pds_0, FragmentType::kDAPHNE, Subdetector::kHD_PDS));
  record.add_fragment(make_fragment(ta_0, FragmentType::kTriggerActivity, Subdetector::kDAQ));
  return record;
}

} // namespace ""

BOOST_AUTO_TEST_SUITE(RecordIndex_test)

BOOST_AUTO_TEST_CASE(RecordIDOrdering)
{
  BOOST_REQUIRE((RecordID{ 1, 0 } < RecordID{ 1, 1 }));
  BOOST_REQUIRE((RecordID{ 1, 5 } < RecordID{ 2, 0 }));
  BOOST_REQUIRE(RecordID{ 7 } == RecordID(7, 0));
  BOOST_REQUIRE_EQUAL(RecordID(7, 3).to_string(), "7.3");
}

BOOST_AUTO_TEST_CASE(FromTriggerRecord)
{
  auto record = make_trigger_record();
  auto index = RecordIndex::from(record);

  BOOST_REQUIRE(index.get_record_id() == RecordID(42, 2));
  BOOST_REQUIRE(index.get_header_source_id() == trb_header);
  BOOST_REQUIRE_EQUAL(index.get_fragments().size(), 4);
  BOOST_REQUIRE(index.contains_fragment(pds_0));
  BOOST_REQUIRE(!index.contains_fragment(trb_header)); // the header is not a fragment

  auto info = index.get_fragment_info(pds_0);
  BOOST_REQUIRE(info.has_value());
  BOOST_REQUIRE(info->fragment_type == FragmentType::kDAPHNE);
  BOOST_REQUIRE(info->subdetector == Subdetector::kHD_PDS);
  BOOST_REQUIRE(!index.get_fragment_info(trb_header).has_value());
}

BOOST_AUTO_TEST_CASE(FromTimeSlice)
{
  TimeSlice timeslice(17, 1);
  timeslice.set_element_id(trb_header);
  timeslice.add_fragment(make_fragment(ta_0, FragmentType::kTriggerPrimitive, Subdetector::kDAQ));

  auto index = RecordIndex::from(timeslice);
  BOOST_REQUIRE(index.get_record_id() == RecordID(17, 0));
  BOOST_REQUIRE(index.get_header_source_id() == trb_header);
  BOOST_REQUIRE(index.get_fragment_source_ids() == std::set<SourceID>{ ta_0 });
}

BOOST_AUTO_TEST_CASE(Selection)
{
  auto index = RecordIndex::from(make_trigger_record());

  BOOST_REQUIRE_EQUAL(index.get_fragment_source_ids().size(), 4);

  SourceIDSelector by_subsystem;
  by_subsystem.subsystem = SourceID::Subsystem::kTrigger;
  BOOST_REQUIRE(index.get_fragment_source_ids(by_subsystem) == std::set<SourceID>{ ta_0 });

  SourceIDSelector by_type;
  by_type.fragment_type = FragmentType::kWIBEth;
  BOOST_REQUIRE((index.get_fragment_source_ids(by_type) == std::set<SourceID>{ tpc_0, tpc_1 }));

  SourceIDSelector by_type_and_subdet;
  by_type_and_subdet.fragment_type = FragmentType::kWIBEth;
  by_type_and_subdet.subdetector = Subdetector::kHD_PDS;
  BOOST_REQUIRE(index.get_fragment_source_ids(by_type_and_subdet).empty());

  by_type_and_subdet.subdetector = Subdetector::kHD_TPC;
  BOOST_REQUIRE_EQUAL(index.get_fragment_source_ids(by_type_and_subdet).size(), 2);
}

BOOST_AUTO_TEST_CASE(Groupings)
{
  auto index = RecordIndex::from(make_trigger_record());

  auto by_type = index.get_source_ids_by_fragment_type();
  BOOST_REQUIRE_EQUAL(by_type.size(), 3);
  BOOST_REQUIRE_EQUAL(by_type[FragmentType::kWIBEth].size(), 2);

  auto by_subdet = index.get_source_ids_by_subdetector();
  BOOST_REQUIRE_EQUAL(by_subdet[Subdetector::kHD_TPC].size(), 2);
  BOOST_REQUIRE_EQUAL(by_subdet[Subdetector::kDAQ].size(), 1);

  auto by_subsystem = index.get_source_ids_by_subsystem();
  BOOST_REQUIRE_EQUAL(by_subsystem[SourceID::Subsystem::kDetectorReadout].size(), 3);
  BOOST_REQUIRE_EQUAL(by_subsystem.count(SourceID::Subsystem::kTRBuilder), 0); // header not included
}

BOOST_AUTO_TEST_CASE(RebuildFromStoredEntries)
{
  // a reader rebuilds an index entry by entry from what a file stores; it must equal the original
  auto original = RecordIndex::from(make_trigger_record());
  RecordIndex rebuilt(original.get_record_id(), original.get_header_source_id());
  for (auto const& [source_id, info] : original.get_fragments()) {
    rebuilt.add_fragment(source_id, info);
  }
  BOOST_REQUIRE(rebuilt == original);
}

BOOST_AUTO_TEST_SUITE_END()
