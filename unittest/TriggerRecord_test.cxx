/**
 * @file TriggerRecord_test.cxx TriggerRecord class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "cdf/TriggerRecord.hpp"

#define BOOST_TEST_MODULE TriggerRecord_test // NOLINT

#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>

using namespace dunedaq::cdf;

BOOST_AUTO_TEST_SUITE(TriggerRecord_test)

BOOST_AUTO_TEST_CASE(CopyAndMoveSemantics)
{
  BOOST_REQUIRE(std::is_copy_constructible_v<TriggerRecord>);
  BOOST_REQUIRE(std::is_copy_assignable_v<TriggerRecord>);
  BOOST_REQUIRE(std::is_move_constructible_v<TriggerRecord>);
  BOOST_REQUIRE(std::is_move_assignable_v<TriggerRecord>);
}

BOOST_AUTO_TEST_SUITE_END()