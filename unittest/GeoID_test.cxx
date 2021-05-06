/**
 * @file GeoID_test.cxx GeoID struct Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "dataformats/GeoID.hpp"
#include <sstream>

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE GeoID_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::dataformats;

BOOST_AUTO_TEST_SUITE(GeoID_test)

/**
 * @brief Test that GeoID::operator<< functions as expected 
*/
BOOST_AUTO_TEST_CASE(StreamOperator) 
{
  GeoID test;
  test.system_type = GeoID::SystemType::kTPC;
  test.region_id = 1;
  test.element_id = 2;

  std::ostringstream ostr;
  ostr << test;

  std::string output = ostr.str();
  std::cout << "Stream operator: " << output << std::endl;

  BOOST_REQUIRE(!output.empty());
  auto pos = output.find("region: 1,");
  BOOST_REQUIRE(pos != std::string::npos);

  std::istringstream istr(output);
  GeoID test2;
  istr >> test2;
  BOOST_REQUIRE_EQUAL(test2, test);
}

/**
 * @brief Test that GeoID::operator< functions as expected
 */
BOOST_AUTO_TEST_CASE(ComparisonOperator)
{
  GeoID lesser, greater;
  lesser.system_type = GeoID::SystemType::kTPC;
  lesser.region_id = 1;
  lesser.element_id = 2;
  greater.system_type = GeoID::SystemType::kTPC;
  greater.region_id = 3;
  greater.element_id = 4;

  BOOST_REQUIRE(lesser != greater);
  BOOST_REQUIRE(lesser == lesser);
  BOOST_REQUIRE(greater == greater);
  BOOST_REQUIRE(lesser < greater);
  BOOST_REQUIRE(!(greater < lesser));
}

BOOST_AUTO_TEST_SUITE_END()
