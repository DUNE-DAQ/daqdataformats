/**
 * @file TriggerRecordHeader_test.cxx TriggerRecordHeader class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "cdf/TriggerRecordHeader.hpp"

#define BOOST_TEST_MODULE TriggerRecordHeader_test // NOLINT

#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>

using namespace dunedaq::cdf;

BOOST_AUTO_TEST_SUITE(TriggerRecordHeader_test)

BOOST_AUTO_TEST_CASE(CopyAndMoveSemantics)
{
  BOOST_REQUIRE(std::is_copy_constructible_v<TriggerRecordHeader>);
  BOOST_REQUIRE(std::is_copy_assignable_v<TriggerRecordHeader>);
  BOOST_REQUIRE(std::is_move_constructible_v<TriggerRecordHeader>);
  BOOST_REQUIRE(std::is_move_assignable_v<TriggerRecordHeader>);
}

BOOST_AUTO_TEST_SUITE_END()