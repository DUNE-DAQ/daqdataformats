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
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace dunedaq::daqdataformats;

BOOST_AUTO_TEST_SUITE(SourceID_test)

BOOST_AUTO_TEST_CASE(SubsystemConversion)
{
  BOOST_REQUIRE_EQUAL(SourceID::subsystem_to_string(SourceID::Subsystem::kUNDEFINED), "UNDEFINED");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_subsystem("UNDEFINED"), SourceID::Subsystem::kUNDEFINED);

  BOOST_REQUIRE_EQUAL(SourceID::subsystem_to_string(SourceID::Subsystem::kDRO), "DRO");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_subsystem("DRO"), SourceID::Subsystem::kDRO);

  BOOST_REQUIRE_EQUAL(SourceID::subsystem_to_string(SourceID::Subsystem::kHSI), "HSI");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_subsystem("HSI"), SourceID::Subsystem::kHSI);

  BOOST_REQUIRE_EQUAL(SourceID::subsystem_to_string(SourceID::Subsystem::kTRG), "TRG");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_subsystem("TRG"), SourceID::Subsystem::kTRG);

  BOOST_REQUIRE_EQUAL(SourceID::subsystem_to_string(SourceID::Subsystem::kTRB), "TRB");
  BOOST_REQUIRE_EQUAL(SourceID::string_to_subsystem("TRB"), SourceID::Subsystem::kTRB);
}

/**
 * @brief Test that SourceID::operator<< functions as expected
 */
BOOST_AUTO_TEST_CASE(StreamOperator)
{
  SourceID test = { SourceID::Subsystem::kDRO, 314159 };

  std::ostringstream ostr;
  ostr << test;

  std::string output = ostr.str();
  BOOST_TEST_MESSAGE("Stream operator: " << output);

  BOOST_REQUIRE(!output.empty());

  auto pos = output.find(std::to_string(test.id));
  BOOST_REQUIRE(pos != std::string::npos);

  BOOST_TEST_MESSAGE("About to try to input from \"" << output << "\"");
  std::istringstream istr(output);
  SourceID test2;
  istr >> test2;
  BOOST_TEST_MESSAGE("Looks like the output-from-the-input is \"" << test2) << "\"";
  BOOST_REQUIRE_EQUAL(test, test2); // Recall that output was generated from streaming out a SourceID instance

  SourceID::Subsystem cat{ SourceID::Subsystem::kTRB };
  std::ostringstream cat_ostr;
  cat_ostr << cat;
  std::istringstream cat_istr(cat_ostr.str());
  SourceID::Subsystem cat2{ SourceID::Subsystem::kUNDEFINED };
  cat_istr >> cat2;

  BOOST_REQUIRE_EQUAL(cat, cat2);
}

/**
 * @brief Test that SourceID::operator< functions as expected
 */
BOOST_AUTO_TEST_CASE(ComparisonOperator)
{
  SourceID lesser{ SourceID::Subsystem::kDRO, 1 };
  SourceID greater{ SourceID::Subsystem::kDRO, 2 };

  BOOST_REQUIRE(lesser != greater);
  BOOST_REQUIRE(lesser == lesser);
  BOOST_REQUIRE(greater == greater);
  BOOST_REQUIRE(lesser < greater);
  BOOST_REQUIRE(!(greater < lesser));
}

BOOST_AUTO_TEST_CASE(Validity)
{
  SourceID test;
  BOOST_REQUIRE(!test.is_in_valid_state());

  test = { SourceID::Subsystem::kHSI, 3141592 };
  BOOST_REQUIRE(test.is_in_valid_state());

  test.id = SourceID::s_invalid_id;
  BOOST_REQUIRE(!test.is_in_valid_state());
}

BOOST_AUTO_TEST_SUITE_END()
