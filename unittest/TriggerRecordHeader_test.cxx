/**
 * @file TriggerRecordHeader_test.cxx TriggerRecordHeader class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "dataformats/TriggerRecordHeader.hpp"
#include "dataformats/TriggerRecordHeaderData.hpp"
#include <sstream>

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE TriggerRecordHeader_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::dataformats;

BOOST_AUTO_TEST_SUITE(TriggerRecordHeader_test)

/**
 * @brief Check that TriggerRecords have appropriate Copy/Move semantics
 */
BOOST_AUTO_TEST_CASE(CopyAndMoveSemantics)
{
  BOOST_REQUIRE(std::is_copy_constructible_v<TriggerRecordHeader>);
  BOOST_REQUIRE(std::is_copy_assignable_v<TriggerRecordHeader>);
  BOOST_REQUIRE(std::is_move_constructible_v<TriggerRecordHeader>);
  BOOST_REQUIRE(std::is_move_assignable_v<TriggerRecordHeader>);
}

/**
 * @brief Check that TriggerRecordHeader constructors function correctly
 */
BOOST_AUTO_TEST_CASE(ExistingHeader)
{
  std::vector<ComponentRequest> components;
  components.emplace_back();
  components.back().component.component_type = GeoIDComponentType::kTPC;
  components.back().component.region_id = 1;
  components.back().component.element_id = 2;
  components.back().window_begin = 3;
  components.back().window_end = 4;
  components.emplace_back();
  components.back().component.component_type = GeoIDComponentType::kTPC;
  components.back().component.region_id = 5;
  components.back().component.element_id = 6;
  components.back().window_begin = 7;
  components.back().window_end = 8;

  auto header = new TriggerRecordHeader(components);
  header->set_run_number(9);
  header->set_trigger_number(10);
  header->set_trigger_timestamp(11);
  header->set_trigger_type(12);
  header->set_error_bit(TriggerRecordErrorBits::kMismatch, true);
  header->set_error_bit(TriggerRecordErrorBits::kUnassigned3, true);

  BOOST_REQUIRE_THROW(header->at(header->get_header().num_requested_components),
                      dunedaq::dataformats::ComponentRequestIndexError);
  BOOST_REQUIRE_THROW((*header)[header->get_header().num_requested_components],
                      dunedaq::dataformats::ComponentRequestIndexError);

  void* buff = malloc(header->get_total_size_bytes());
  memcpy(buff, header->get_storage_location(), header->get_total_size_bytes());

  // Constructor should copy header
  TriggerRecordHeader copy_header(const_cast<void*>(header->get_storage_location()), true);
  delete header; // NOLINT(build/raw_ownership)

  BOOST_REQUIRE_EQUAL(copy_header.get_run_number(), 9);
  BOOST_REQUIRE_EQUAL(copy_header.get_error_bit(static_cast<TriggerRecordErrorBits>(0)), false);
  BOOST_REQUIRE_EQUAL(copy_header.get_error_bit(static_cast<TriggerRecordErrorBits>(1)), true);
  BOOST_REQUIRE_EQUAL(copy_header.get_header().error_bits, 10);
  BOOST_REQUIRE_EQUAL(copy_header.at(0).window_begin, 3);
  BOOST_REQUIRE_EQUAL(copy_header[1].window_begin, 7);

  {
    // Test copy constructor
    TriggerRecordHeader copy_copy_header(copy_header);
    BOOST_REQUIRE_EQUAL(copy_copy_header.get_run_number(), 9);
    BOOST_REQUIRE_EQUAL(copy_copy_header.get_error_bit(static_cast<TriggerRecordErrorBits>(0)), false);
    BOOST_REQUIRE_EQUAL(copy_copy_header.get_error_bit(static_cast<TriggerRecordErrorBits>(1)), true);
    BOOST_REQUIRE_EQUAL(copy_copy_header.get_header().error_bits, 10);
    BOOST_REQUIRE_EQUAL(copy_copy_header.at(0).window_begin, 3);
    BOOST_REQUIRE_EQUAL(copy_copy_header[1].window_begin, 7);
  }
  {
    // Test copy assignment
    TriggerRecordHeader copy_assign_header = copy_header;
    BOOST_REQUIRE_EQUAL(copy_assign_header.get_run_number(), 9);
    BOOST_REQUIRE_EQUAL(copy_assign_header.get_error_bit(static_cast<TriggerRecordErrorBits>(0)), false);
    BOOST_REQUIRE_EQUAL(copy_assign_header.get_error_bit(static_cast<TriggerRecordErrorBits>(1)), true);
    BOOST_REQUIRE_EQUAL(copy_assign_header.get_header().error_bits, 10);
    BOOST_REQUIRE_EQUAL(copy_assign_header.at(0).window_begin, 3);
    BOOST_REQUIRE_EQUAL(copy_assign_header[1].window_begin, 7);
  }

  {
    // Test Buffer adoption constructor
    TriggerRecordHeader buffer_header(buff, false);

    BOOST_REQUIRE_EQUAL(buffer_header.get_run_number(), 9);
    BOOST_REQUIRE_EQUAL(buffer_header.get_error_bit(static_cast<TriggerRecordErrorBits>(0)), false);
    BOOST_REQUIRE_EQUAL(buffer_header.get_error_bit(static_cast<TriggerRecordErrorBits>(1)), true);
    BOOST_REQUIRE_EQUAL(buffer_header.get_header().error_bits, 10);
    BOOST_REQUIRE_EQUAL(buffer_header.at(0).window_begin, 3);
    BOOST_REQUIRE_EQUAL(buffer_header[1].window_begin, 7);
  }

  BOOST_REQUIRE_EQUAL(*reinterpret_cast<uint32_t*>(buff), // NOLINT
                      TriggerRecordHeaderData::s_trigger_record_header_magic);

  free(buff);
}

/**
 * @brief Test header field manipulation methods
 */
BOOST_AUTO_TEST_CASE(HeaderFields)
{
  std::vector<ComponentRequest> components;
  components.emplace_back();
  components.back().component.component_type = GeoIDComponentType::kTPC;
  components.back().component.region_id = 1;
  components.back().component.element_id = 2;
  components.back().window_begin = 3;
  components.back().window_end = 4;
  components.emplace_back();
  components.back().component.component_type = GeoIDComponentType::kTPC;
  components.back().component.region_id = 5;
  components.back().component.element_id = 6;
  components.back().window_begin = 7;
  components.back().window_end = 8;

  auto header = new TriggerRecordHeader(components);
  header->set_run_number(9);
  header->set_trigger_number(10);
  header->set_trigger_timestamp(11);
  header->set_trigger_type(12);
  header->set_error_bit(TriggerRecordErrorBits::kMismatch, true);
  header->set_error_bit(TriggerRecordErrorBits::kUnassigned3, true);

  auto header_data = header->get_header();
  BOOST_REQUIRE_EQUAL(header->get_run_number(), header_data.run_number);
  BOOST_REQUIRE_EQUAL(header->get_trigger_number(), header_data.trigger_number);
  BOOST_REQUIRE_EQUAL(header->get_trigger_timestamp(), header_data.trigger_timestamp);
  BOOST_REQUIRE_EQUAL(header->get_trigger_type(), header_data.trigger_type);
  BOOST_REQUIRE_EQUAL(header->get_num_requested_components(), 2);
  BOOST_REQUIRE_EQUAL(header->get_num_requested_components(), header_data.num_requested_components);

  auto header_ptr = reinterpret_cast<const TriggerRecordHeaderData*>(header->get_storage_location());
  BOOST_REQUIRE_EQUAL(header_ptr->run_number, header_data.run_number);
  header->set_run_number(10);
  BOOST_REQUIRE(header_ptr->run_number != header_data.run_number);
  BOOST_REQUIRE_EQUAL(header_ptr->run_number, 10);

  std::ostringstream oss;
  oss << header_data;
  std::istringstream iss(oss.str());
  TriggerRecordHeaderData trhd;
  iss >> trhd;
  BOOST_REQUIRE_EQUAL(trhd.run_number, header_data.run_number);
  BOOST_REQUIRE_EQUAL(trhd.trigger_number, header_data.trigger_number);
  BOOST_REQUIRE_EQUAL(trhd.trigger_timestamp, header_data.trigger_timestamp);
  BOOST_REQUIRE_EQUAL(trhd.trigger_type, header_data.trigger_type);
  BOOST_REQUIRE_EQUAL(trhd.num_requested_components, header_data.num_requested_components);
}

BOOST_AUTO_TEST_SUITE_END()
