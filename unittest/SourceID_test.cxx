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

BOOST_AUTO_TEST_CASE(CategoryConversion)
{
  BOOST_REQUIRE_EQUAL(SourceID::category_to_string(SourceID::Category::kTPC), "TPC");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_category("TPC"), SourceID::Category::kTPC);

  BOOST_REQUIRE_EQUAL(SourceID::category_to_string(SourceID::Category::kPDS), "PDS");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_category("PDS"), SourceID::Category::kPDS);

  BOOST_REQUIRE_EQUAL(SourceID::category_to_string(SourceID::Category::kDataSelection), "DataSelection");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_category("DataSelection"), SourceID::Category::kDataSelection);

  BOOST_REQUIRE_EQUAL(SourceID::category_to_string(SourceID::Category::kNDLArTPC), "NDLArTPC");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_category("NDLArTPC"), SourceID::Category::kNDLArTPC);

  BOOST_REQUIRE_EQUAL(SourceID::category_to_string(SourceID::Category::kInvalid), "Invalid");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_category("Invalid"), SourceID::Category::kInvalid);
}

/**
 * @brief Test that SourceID::operator<< functions as expected
 */
BOOST_AUTO_TEST_CASE(StreamOperator)
{
  SourceID test;
  const SourceID::ID_upper_t upper = 314;
  const SourceID::ID_lower_t lower = 159;

  test.category = SourceID::Category::kTPC;
  test.id = SourceID::compose_id(upper, lower);

  std::ostringstream ostr;
  ostr << test;

  std::string output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);

  BOOST_REQUIRE(!output.empty());

  // Test that the "id -> (upper, lower)" output of the stream operator holds
  auto pos = output.find(std::to_string(test.id));
  BOOST_REQUIRE(pos != std::string::npos);  

  pos = output.find(std::to_string(upper));
  BOOST_REQUIRE(pos != std::string::npos);  

  pos = output.find(std::to_string(lower));
  BOOST_REQUIRE(pos != std::string::npos);  

  BOOST_TEST_MESSAGE("About to try to input from \"" << output << "\"");
  std::istringstream istr(output);
  SourceID test2;
  istr >> test2;
  BOOST_TEST_MESSAGE("Looks like the output-from-the-input is \"" << test2) << "\"";
  BOOST_REQUIRE_EQUAL(test, test2); // Recall that output was generated from streaming out a SourceID instance


}

BOOST_AUTO_TEST_CASE(ComposeDecompose)
{
  static_assert(sizeof(SourceID::ID_t) == 4); // Otherwise the DEADBEEF below should change
  SourceID test { SourceID::Category::kTPC, 0xDEADBEEF };

  // Test the splitting / combining of the upper and lower half of SourceID's id member
  SourceID::ID_upper_t upper;
  SourceID::ID_lower_t lower;
  SourceID::decompose_id(test.id, upper, lower);
  auto recomposed_id = SourceID::compose_id(upper, lower);
  BOOST_REQUIRE_EQUAL(test.id, recomposed_id);
}

/**
 * @brief Test that SourceID::operator< functions as expected
 */
BOOST_AUTO_TEST_CASE(ComparisonOperator)
{
  SourceID lesser, greater;
  lesser.category = SourceID::Category::kTPC;
  lesser.id = 1;
  greater.category = SourceID::Category::kTPC;
  greater.id = 3;

  BOOST_REQUIRE(lesser != greater);
  BOOST_REQUIRE(lesser == lesser);
  BOOST_REQUIRE(greater == greater);
  BOOST_REQUIRE(lesser < greater);
  BOOST_REQUIRE(!(greater < lesser));
}

BOOST_AUTO_TEST_CASE(Validity)
{
  SourceID test;
  BOOST_REQUIRE( ! test.is_in_valid_state() );

  test = { SourceID::Category::kPDS, 3141592 }; 
  BOOST_REQUIRE( test.is_in_valid_state() );
  
  test.id = SourceID::s_invalid_id;
  BOOST_REQUIRE( ! test.is_in_valid_state() );
  
}

BOOST_AUTO_TEST_SUITE_END()
