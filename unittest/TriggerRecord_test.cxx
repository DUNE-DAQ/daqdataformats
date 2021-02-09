/**
 * @file TriggerRecord_test.cxx TriggerRecord class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "dataformats/TriggerRecord.hpp"
#include "serialization/Serialization.hpp"

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
 * @brief Check that TriggerRecordHeader constructors function correctly
 */
BOOST_AUTO_TEST_CASE(ExistingHeader)
{
  std::vector<ComponentRequest> components;
  components.emplace_back();
  components.back().m_component.m_apa_number = 1;
  components.back().m_component.m_link_number = 2;
  components.back().m_window_offset = 3;
  components.back().m_window_width = 4;
  components.emplace_back();
  components.back().m_component.m_apa_number = 5;
  components.back().m_component.m_link_number = 6;
  components.back().m_window_offset = 7;
  components.back().m_window_width = 8;

  auto header = new TriggerRecordHeader(components);
  header->set_run_number(9);
  header->set_trigger_number(10);
  header->set_trigger_timestamp(11);
  header->set_trigger_type(12);
  header->set_error_bit(TriggerRecordErrorBits::kMismatch, true);
  header->set_error_bit(TriggerRecordErrorBits::kUnassigned3, true);

  BOOST_REQUIRE_THROW(header->at(header->get_header().m_num_requested_components),
                      dunedaq::dataformats::ComponentRequestIndexError);
  BOOST_REQUIRE_THROW((*header)[header->get_header().m_num_requested_components],
                      dunedaq::dataformats::ComponentRequestIndexError);

  void* buff = malloc(header->get_total_size_bytes());
  memcpy(buff, header->get_storage_location(), header->get_total_size_bytes());

  // Constructor should copy header
  TriggerRecord record(*header);
  delete header; // NOLINT(build/raw_ownership)

  BOOST_REQUIRE_EQUAL(record.get_header_ref().get_run_number(), 9);
  BOOST_REQUIRE_EQUAL(record.get_header_ref().get_error_bit(static_cast<TriggerRecordErrorBits>(0)), false);
  BOOST_REQUIRE_EQUAL(record.get_header_ref().get_error_bit(static_cast<TriggerRecordErrorBits>(1)), true);
  BOOST_REQUIRE_EQUAL(record.get_header_data().m_error_bits, 10);
  BOOST_REQUIRE_EQUAL(record.get_header_ref().at(0).m_window_offset, 3);
  BOOST_REQUIRE_EQUAL(record.get_header_ref()[1].m_window_offset, 7);

  {
    TriggerRecordHeader bufferHeader(buff, false);

    BOOST_REQUIRE_EQUAL(bufferHeader.get_run_number(), 9);
    BOOST_REQUIRE_EQUAL(bufferHeader.get_error_bit(static_cast<TriggerRecordErrorBits>(0)), false);
    BOOST_REQUIRE_EQUAL(bufferHeader.get_error_bit(static_cast<TriggerRecordErrorBits>(1)), true);
    BOOST_REQUIRE_EQUAL(bufferHeader.get_header().m_error_bits, 10);
    BOOST_REQUIRE_EQUAL(bufferHeader.at(0).m_window_offset, 3);
    BOOST_REQUIRE_EQUAL(bufferHeader[1].m_window_offset, 7);
  }

  BOOST_REQUIRE_EQUAL(*reinterpret_cast<uint32_t*>(buff), // NOLINT
                      TriggerRecordHeaderData::s_trigger_record_header_magic);

  free(buff);
}

/**
 * @brief Check that TriggerRecordHeader serialization via MsgPack works
 */
BOOST_AUTO_TEST_CASE(Header_SerDes_MsgPack)
{
  std::vector<ComponentRequest> components;
  components.emplace_back();
  components.back().m_component.m_apa_number = 1;
  components.back().m_component.m_link_number = 2;
  components.back().m_window_offset = 3;
  components.back().m_window_width = 4;
  components.emplace_back();
  components.back().m_component.m_apa_number = 5;
  components.back().m_component.m_link_number = 6;
  components.back().m_window_offset = 7;
  components.back().m_window_width = 8;

  auto header = new TriggerRecordHeader(components);
  header->set_run_number(9);
  header->set_trigger_number(10);
  header->set_trigger_timestamp(11);
  header->set_trigger_type(12);
  header->set_error_bit(TriggerRecordErrorBits::kMismatch, true);
  header->set_error_bit(TriggerRecordErrorBits::kUnassigned3, true);

  auto bytes = dunedaq::serialization::serialize(*header, dunedaq::serialization::kMsgPack);
  TriggerRecordHeader& header_orig=*header;
  TriggerRecordHeader header_deserialized = dunedaq::serialization::deserialize<TriggerRecordHeader>(bytes);


  BOOST_REQUIRE_EQUAL(header_orig.get_trigger_number(), header_deserialized.get_trigger_number());
  BOOST_REQUIRE_EQUAL(header_orig.get_trigger_timestamp(), header_deserialized.get_trigger_timestamp());
  BOOST_REQUIRE_EQUAL(header_orig.get_num_requested_components(), header_deserialized.get_num_requested_components());
  BOOST_REQUIRE_EQUAL(header_orig.get_run_number(), header_deserialized.get_run_number());
  BOOST_REQUIRE_EQUAL(header_orig.get_error_bits(), header_deserialized.get_error_bits());
  BOOST_REQUIRE_EQUAL(header_orig.get_trigger_type(), header_deserialized.get_trigger_type());
  BOOST_REQUIRE_EQUAL(header_orig.get_total_size_bytes(), header_deserialized.get_total_size_bytes());
  BOOST_REQUIRE_EQUAL(header_orig.at(0).m_window_offset, header_deserialized.at(0).m_window_offset);
  BOOST_REQUIRE_EQUAL(header_orig.at(1).m_window_offset, header_deserialized.at(1).m_window_offset);
  
}

/**
 * @brief Check that TriggerRecordHeader serialization via MsgPack works
 */
BOOST_AUTO_TEST_CASE(Header_SerDes_JSON)
{
  std::vector<ComponentRequest> components;
  components.emplace_back();
  components.back().m_component.m_apa_number = 1;
  components.back().m_component.m_link_number = 2;
  components.back().m_window_offset = 3;
  components.back().m_window_width = 4;
  components.emplace_back();
  components.back().m_component.m_apa_number = 5;
  components.back().m_component.m_link_number = 6;
  components.back().m_window_offset = 7;
  components.back().m_window_width = 8;

  auto header = new TriggerRecordHeader(components);
  header->set_run_number(9);
  header->set_trigger_number(10);
  header->set_trigger_timestamp(11);
  header->set_trigger_type(12);
  header->set_error_bit(TriggerRecordErrorBits::kMismatch, true);
  header->set_error_bit(TriggerRecordErrorBits::kUnassigned3, true);

  auto bytes = dunedaq::serialization::serialize(*header, dunedaq::serialization::kJSON);
  TriggerRecordHeader& header_orig=*header;
  TriggerRecordHeader header_deserialized = dunedaq::serialization::deserialize<TriggerRecordHeader>(bytes);


  BOOST_REQUIRE_EQUAL(header_orig.get_trigger_number(), header_deserialized.get_trigger_number());
  BOOST_REQUIRE_EQUAL(header_orig.get_trigger_timestamp(), header_deserialized.get_trigger_timestamp());
  BOOST_REQUIRE_EQUAL(header_orig.get_num_requested_components(), header_deserialized.get_num_requested_components());
  BOOST_REQUIRE_EQUAL(header_orig.get_run_number(), header_deserialized.get_run_number());
  BOOST_REQUIRE_EQUAL(header_orig.get_error_bits(), header_deserialized.get_error_bits());
  BOOST_REQUIRE_EQUAL(header_orig.get_trigger_type(), header_deserialized.get_trigger_type());
  BOOST_REQUIRE_EQUAL(header_orig.get_total_size_bytes(), header_deserialized.get_total_size_bytes());
  BOOST_REQUIRE_EQUAL(header_orig.at(0).m_window_offset, header_deserialized.at(0).m_window_offset);
  BOOST_REQUIRE_EQUAL(header_orig.at(1).m_window_offset, header_deserialized.at(1).m_window_offset);
  
}

BOOST_AUTO_TEST_SUITE_END()
