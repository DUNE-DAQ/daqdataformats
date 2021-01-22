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

#include <boost/test/unit_test.hpp>
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
 * @brief Check that TriggerRecordHeader constructors function correctly
*/
BOOST_AUTO_TEST_CASE(ExistingHeader)
{
  auto components = std::vector<ComponentRequest>();
  components.emplace_back();
  components.back().component.apa_number = 1;
  components.back().component.link_number = 2;
  components.back().window_offset = 3;
  components.back().window_width = 4;
  components.emplace_back();
  components.back().component.apa_number = 5;
  components.back().component.link_number = 6;
  components.back().window_offset = 7;
  components.back().window_width = 8;

  auto header = new TriggerRecordHeader(components);
  header->set_run_number(9);
  header->set_trigger_number(10);
  header->set_trigger_timestamp(11);
  header->set_trigger_type(12);
  header->set_error_bit(1, true);
  header->set_error_bit(3, true);

  void* buff = malloc(header->get_total_size_bytes());
  memcpy(buff, header->get_storage_location(), header->get_total_size_bytes());

  // Constructor should copy header
  TriggerRecord record(*header);
  delete header; // NOLINT(build/raw_ownership)

  BOOST_REQUIRE_EQUAL(record.get_header().get_run_number(), 9);
  BOOST_REQUIRE_EQUAL(record.get_header().get_error_bit(0), false);
  BOOST_REQUIRE_EQUAL(record.get_header().get_error_bit(1), true);
  BOOST_REQUIRE_EQUAL(record.get_header_data().error_bits, 10);
  BOOST_REQUIRE_EQUAL(record.get_header().at(0).window_offset, 3);
  BOOST_REQUIRE_EQUAL(record.get_header()[1].window_offset, 7);

  {
  TriggerRecordHeader bufferHeader(buff, false);

  BOOST_REQUIRE_EQUAL(bufferHeader.get_run_number(), 9);
  BOOST_REQUIRE_EQUAL(bufferHeader.get_error_bit(0), false);
  BOOST_REQUIRE_EQUAL(bufferHeader.get_error_bit(1), true);
  BOOST_REQUIRE_EQUAL(bufferHeader.get_header().error_bits, 10);
  BOOST_REQUIRE_EQUAL(bufferHeader.at(0).window_offset, 3);
  BOOST_REQUIRE_EQUAL(bufferHeader[1].window_offset, 7);
  }

  BOOST_REQUIRE_EQUAL(*reinterpret_cast<uint32_t*>(buff), TRIGGER_RECORD_HEADER_MAGIC); // NOLINT
  free(buff);
}
BOOST_AUTO_TEST_SUITE_END()