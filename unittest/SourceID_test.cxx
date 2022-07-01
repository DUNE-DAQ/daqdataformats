/**
 * @file SourceID_test.cxx SourceID struct Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdataformats/SourceID.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE SourceID_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <functional>
#include <sstream>
#include <string>
#include <vector>

using namespace dunedaq::daqdataformats;

BOOST_AUTO_TEST_SUITE(SourceID_test)

BOOST_AUTO_TEST_CASE(SystemTypeConversion)
{
  BOOST_REQUIRE_EQUAL(SourceID::system_type_to_string(SourceID::SystemType::kTPC), "TPC");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_system_type("TPC"), SourceID::SystemType::kTPC);

  BOOST_REQUIRE_EQUAL(SourceID::system_type_to_string(SourceID::SystemType::kPDS), "PDS");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_system_type("PDS"), SourceID::SystemType::kPDS);

  BOOST_REQUIRE_EQUAL(SourceID::system_type_to_string(SourceID::SystemType::kDataSelection), "DataSelection");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_system_type("DataSelection"), SourceID::SystemType::kDataSelection);

  BOOST_REQUIRE_EQUAL(SourceID::system_type_to_string(SourceID::SystemType::kNDLArTPC), "NDLArTPC");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_system_type("NDLArTPC"), SourceID::SystemType::kNDLArTPC);

  BOOST_REQUIRE_EQUAL(SourceID::system_type_to_string(SourceID::SystemType::kInvalid), "Invalid");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_system_type("Invalid"), SourceID::SystemType::kInvalid);

  auto test_type = static_cast<SourceID::SystemType>(0x1234);
  BOOST_REQUIRE_EQUAL(SourceID::system_type_to_string(test_type), "Unknown");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_system_type("Unknown"), SourceID::SystemType::kInvalid);
}

/**
 * @brief Test that SourceID::operator<< functions as expected
 */
BOOST_AUTO_TEST_CASE(StreamOperator)
{
  SourceID test;
  test.system_type = SourceID::SystemType::kTPC;
  test.region_id = 1;
  test.element_id = 2;

  std::ostringstream ostr;
  ostr << test;

  std::string output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);

  BOOST_REQUIRE(!output.empty());
  auto pos = output.find("region: 1,");
  BOOST_REQUIRE(pos != std::string::npos);

  std::istringstream istr(output);
  SourceID test2;
  istr >> test2;
  BOOST_REQUIRE_EQUAL(test2, test);
}

/**
 * @brief Test that SourceID::operator< functions as expected
 */
BOOST_AUTO_TEST_CASE(ComparisonOperator)
{
  SourceID lesser, greater;
  lesser.system_type = SourceID::SystemType::kTPC;
  lesser.region_id = 1;
  lesser.element_id = 2;
  greater.system_type = SourceID::SystemType::kTPC;
  greater.region_id = 3;
  greater.element_id = 4;

  BOOST_REQUIRE(lesser != greater);
  BOOST_REQUIRE(lesser == lesser);
  BOOST_REQUIRE(greater == greater);
  BOOST_REQUIRE(lesser < greater);
  BOOST_REQUIRE(!(greater < lesser));
}

BOOST_AUTO_TEST_SUITE_END()
