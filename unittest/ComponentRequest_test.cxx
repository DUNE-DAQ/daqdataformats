/**
 * @file ComponentRequest_test.cxx ComponentRequest class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "dataformats/ComponentRequest.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE ComponentRequest_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::dataformats;

BOOST_AUTO_TEST_SUITE(ComponentRequest_test)

/**
 * @brief Test that ComponentRequest::operator<< functions as expected
 */
BOOST_AUTO_TEST_CASE(StreamOperator)
{
  ComponentRequest component;
  GeoID test;
  test.apa_number = 1;
  test.link_number = 2;
  component.component = test;
  component.window_begin = 3;
  component.window_end = 4;

  std::ostringstream ostr;
  ostr << component;

  std::string output = ostr.str();
  std::cout << "Stream operator: " << output << std::endl;

  BOOST_REQUIRE(!output.empty());
  auto pos = output.find("APA: 1,");
  BOOST_REQUIRE(pos != std::string::npos);
  pos = output.find("link: 2,");
  BOOST_REQUIRE(pos != std::string::npos);
  pos = output.find("offset: 3,");
  BOOST_REQUIRE(pos != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()