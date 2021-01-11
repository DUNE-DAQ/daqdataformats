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
  BOOST_REQUIRE(!std::is_copy_constructible_v<Fragment>);
  BOOST_REQUIRE(!std::is_copy_assignable_v<Fragment>);
  BOOST_REQUIRE(std::is_move_constructible_v<Fragment>);
  BOOST_REQUIRE(std::is_move_assignable_v<Fragment>);
}

BOOST_AUTO_TEST_CASE(ExistingFragmentConstructor)
{
  FragmentHeader header;
  header.size = sizeof(FragmentHeader) + 4;
  header.trigger_number = 1;
  header.trigger_timestamp = 2;
  header.run_number = 3;

  auto frag = malloc(sizeof(FragmentHeader) + 4);
  memcpy(frag, &header, sizeof(FragmentHeader));

  uint8_t one = 1, two = 2, three = 3, four = 4; // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader), &one, 1); // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 1, &two, 1); // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 2, &three, 1); // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 3, &four, 1);  // NOLINT(build/unsigned)

  {
    auto testFrag = Fragment(frag); // frag memory now owned by Fragment

    BOOST_REQUIRE_EQUAL(testFrag.get_storage_location(), frag);

    BOOST_REQUIRE_EQUAL(testFrag.get_trigger_number(), 1);
    BOOST_REQUIRE_EQUAL(testFrag.get_trigger_timestamp(), 2);
    BOOST_REQUIRE_EQUAL(testFrag.get_run_number(), 3);

    BOOST_REQUIRE_EQUAL(*static_cast<uint8_t*>(testFrag.data()), one); // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(testFrag.data()) + 1), two); // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(testFrag.data()) + 2), three); // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(testFrag.data()) + 3), four);  // NOLINT(build/unsigned)
  }

  frag = malloc(sizeof(FragmentHeader) + 4);
  memcpy(frag, &header, sizeof(FragmentHeader));
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader), &four, 1); // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 1, &three, 1); // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 2, &two, 1);   // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 3, &one, 1);   // NOLINT(build/unsigned)

  {
    auto testFrag = Fragment(frag, true);

    BOOST_REQUIRE(testFrag.get_storage_location() != frag);
    BOOST_REQUIRE_EQUAL(testFrag.get_trigger_number(), 1);
    BOOST_REQUIRE_EQUAL(testFrag.get_trigger_timestamp(), 2);
    BOOST_REQUIRE_EQUAL(testFrag.get_run_number(), 3);

    BOOST_REQUIRE_EQUAL(*static_cast<uint8_t*>(testFrag.data()), four); // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(testFrag.data()) + 1), three); // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(testFrag.data()) + 2), two);   // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(testFrag.data()) + 3), one); // NOLINT(build/unsigned)
  }
  free(frag); // Should not cause errors
}

BOOST_AUTO_TEST_SUITE_END()