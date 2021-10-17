/**
 * @file FragmentHeader_test.cxx FragmentHeader class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdataformats/FragmentHeader.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE FragmentHeader_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::daqdataformats;

BOOST_AUTO_TEST_SUITE(FragmentHeader_test)

/**
 * @brief Test that FragmentHeader::operator<< functions as expected
 */
BOOST_AUTO_TEST_CASE(StreamOperator)
{
  FragmentHeader header;
  header.size = sizeof(FragmentHeader) + 4;
  header.trigger_number = 1;
  header.trigger_timestamp = 2;
  header.run_number = 3;
  header.sequence_number = 4;

  std::ostringstream ostr;
  ostr << header;
  std::string output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);

  BOOST_REQUIRE(!output.empty());
  auto pos = output.find("trigger_number: 1,");
  BOOST_REQUIRE(pos != std::string::npos);
  pos = output.find("trigger_timestamp: 2,");
  BOOST_REQUIRE(pos != std::string::npos);
  pos = output.find("run_number: 3,");
  BOOST_REQUIRE(pos != std::string::npos);

  std::istringstream iss(ostr.str());
  FragmentHeader header_from_stream;
  iss >> header_from_stream;
  BOOST_REQUIRE_EQUAL(header_from_stream.run_number, header.run_number);
  BOOST_REQUIRE_EQUAL(header_from_stream.trigger_number, header.trigger_number);
  BOOST_REQUIRE_EQUAL(header_from_stream.trigger_timestamp, header.trigger_timestamp);
  BOOST_REQUIRE_EQUAL(header_from_stream.sequence_number, header.sequence_number);
}

BOOST_AUTO_TEST_SUITE_END()