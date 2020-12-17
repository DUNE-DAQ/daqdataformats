/**
 * @file Fragment_test.cxx Fragment class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "dataformats/Fragment.hpp"

#define BOOST_TEST_MODULE Fragment_test // NOLINT

#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>

using namespace dunedaq::dataformats;

BOOST_AUTO_TEST_SUITE(Fragment_test)

BOOST_AUTO_TEST_CASE(CopyAndMoveSemantics)
{
  BOOST_REQUIRE(std::is_copy_constructible_v<Fragment>);
  BOOST_REQUIRE(std::is_copy_assignable_v<Fragment>);
  BOOST_REQUIRE(std::is_move_constructible_v<Fragment>);
  BOOST_REQUIRE(std::is_move_assignable_v<Fragment>);
}

BOOST_AUTO_TEST_CASE(ExistingFragmentConstructor)
{
  FragmentHeader header;
  header.size = sizeof(FragmentHeader);
  header.trigger_number = 1;
  header.trigger_timestamp = 2;
  header.run_number = 3;
  auto frag = malloc(sizeof(FragmentHeader));
  memcpy(frag, &header, sizeof(FragmentHeader));
  auto testFrag = Fragment(frag); // frag memory now owned by Fragment

  BOOST_REQUIRE_EQUAL(testFrag.get_storage_location(), frag);
  
  BOOST_REQUIRE_EQUAL(testFrag.get_trigger_number(), 1);
  BOOST_REQUIRE_EQUAL(testFrag.get_trigger_timestamp(), 2);
  BOOST_REQUIRE_EQUAL(testFrag.get_run_number(), 3);

  testFrag = Fragment(frag, true);

  BOOST_REQUIRE(testFrag.get_storage_location() != frag);
  BOOST_REQUIRE_EQUAL(testFrag.get_trigger_number(), 1);
  BOOST_REQUIRE_EQUAL(testFrag.get_trigger_timestamp(), 2);
  BOOST_REQUIRE_EQUAL(testFrag.get_run_number(), 3);

}

BOOST_AUTO_TEST_SUITE_END()