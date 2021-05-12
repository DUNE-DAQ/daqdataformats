/**
 * @file TriggerRecord_test.cxx TriggerRecord class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "dataformats/TriggerRecord.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE TriggerRecord_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::dataformats;

BOOST_AUTO_TEST_SUITE(TriggerRecord_test)

/**
 * @brief Check that TriggerRecords have appropriate Copy/Move semantics
 */
BOOST_AUTO_TEST_CASE(CopyAndMoveSemantics)
{
  BOOST_REQUIRE(!std::is_copy_constructible_v<TriggerRecord>);
  BOOST_REQUIRE(!std::is_copy_assignable_v<TriggerRecord>);
  BOOST_REQUIRE(std::is_move_constructible_v<TriggerRecord>);
  BOOST_REQUIRE(std::is_move_assignable_v<TriggerRecord>);
}

/**
 * @brief Test constructor that takes a vector of ComponentRequests
 */
BOOST_AUTO_TEST_CASE(ComponentsConstructor)
{
  std::vector<ComponentRequest> components;
  components.emplace_back();
  components.back().component.system_type = GeoID::SystemType::kTPC;
  components.back().component.region_id = 1;
  components.back().component.element_id = 2;
  components.back().window_begin = 3;
  components.back().window_end = 4;
  components.emplace_back();
  components.back().component.system_type = GeoID::SystemType::kTPC;
  components.back().component.region_id = 5;
  components.back().component.element_id = 6;
  components.back().window_begin = 7;
  components.back().window_end = 8;

  auto record = new TriggerRecord(components);
  BOOST_REQUIRE_EQUAL(record->get_header_data().num_requested_components, 2);
  delete record;
}

/**
 * @brief Test constructor that uses and existing TriggerRecordHeader
 */
BOOST_AUTO_TEST_CASE(HeaderConstructor)
{
  std::vector<ComponentRequest> components;
  components.emplace_back();
  components.back().component.system_type = GeoID::SystemType::kTPC;
  components.back().component.region_id = 1;
  components.back().component.element_id = 2;
  components.back().window_begin = 3;
  components.back().window_end = 4;
  components.emplace_back();
  components.back().component.system_type = GeoID::SystemType::kTPC;
  components.back().component.region_id = 5;
  components.back().component.element_id = 6;
  components.back().window_begin = 7;
  components.back().window_end = 8;

  auto header = new TriggerRecordHeader(components);
  auto record = new TriggerRecord(*header);
  BOOST_REQUIRE_EQUAL(record->get_header_data().num_requested_components, 2);
  delete record;
  delete header;
}

/**
 *@brief Test TriggerRecordHeader manipulation methods
 */
BOOST_AUTO_TEST_CASE(HeaderManipulation)
{

  std::vector<ComponentRequest> components;
  components.emplace_back();
  components.back().component.system_type = GeoID::SystemType::kTPC;
  components.back().component.region_id = 1;
  components.back().component.element_id = 2;
  components.back().window_begin = 3;
  components.back().window_end = 4;
  components.emplace_back();
  components.back().component.system_type = GeoID::SystemType::kTPC;
  components.back().component.region_id = 5;
  components.back().component.element_id = 6;
  components.back().window_begin = 7;
  components.back().window_end = 8;

  TriggerRecord record(components);

  components.emplace_back();
  components.back().component.system_type = GeoID::SystemType::kTPC;
  components.back().component.region_id = 9;
  components.back().component.element_id = 10;
  components.back().window_begin = 11;
  components.back().window_end = 12;

  TriggerRecordHeader new_header(components);
  record.set_header(new_header);
  BOOST_REQUIRE_EQUAL(record.get_header_ref().get_num_requested_components(), 3);

  record.get_header_ref().set_trigger_timestamp(100);
  BOOST_REQUIRE_EQUAL(record.get_header_data().trigger_timestamp, 100);
}

/**
 * @brief Test Fragment vector manipulation methods
 */
BOOST_AUTO_TEST_CASE(FragmentManipulation)
{

  std::vector<ComponentRequest> components;
  components.emplace_back();
  components.back().component.system_type = GeoID::SystemType::kTPC;
  components.back().component.region_id = 1;
  components.back().component.element_id = 2;
  components.back().window_begin = 3;
  components.back().window_end = 4;
  components.emplace_back();
  components.back().component.system_type = GeoID::SystemType::kTPC;
  components.back().component.region_id = 5;
  components.back().component.element_id = 6;
  components.back().window_begin = 7;
  components.back().window_end = 8;

  TriggerRecord record(components);

  BOOST_REQUIRE_EQUAL(record.get_fragments_ref().size(), 0);

  auto buf1 = malloc(10);
  auto frag = std::make_unique<Fragment>(buf1, size_t(10));
  record.add_fragment(std::move(frag));
  BOOST_REQUIRE_EQUAL(record.get_fragments_ref().size(), 1);
  BOOST_REQUIRE_EQUAL(record.get_fragments_ref()[0]->get_size(), sizeof(FragmentHeader) + 10);

  std::vector<std::unique_ptr<Fragment>> new_vector;
  record.set_fragments(std::move(new_vector));
  BOOST_REQUIRE_EQUAL(record.get_fragments_ref().size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
