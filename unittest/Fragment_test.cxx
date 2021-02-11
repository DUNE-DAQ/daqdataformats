/**
 * @file Fragment_test.cxx Fragment class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "dataformats/Fragment.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE Fragment_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <string>
#include <utility>
#include <vector>

using namespace dunedaq::dataformats;

BOOST_AUTO_TEST_SUITE(Fragment_test)

/**
 * @brief Check that Fragments have appropriate Copy/Move semantics
 */
BOOST_AUTO_TEST_CASE(CopyAndMoveSemantics)
{
  BOOST_REQUIRE(!std::is_copy_constructible_v<Fragment>);
  BOOST_REQUIRE(!std::is_copy_assignable_v<Fragment>);
  BOOST_REQUIRE(std::is_move_constructible_v<Fragment>);
  BOOST_REQUIRE(std::is_move_assignable_v<Fragment>);
}

/**
 * @brief Test constructors that gather existing data buffers
*/
BOOST_AUTO_TEST_CASE(DataConstructors)
{
  auto buf1 = malloc(10);
  Fragment single_frag(buf1, size_t(10));
  BOOST_REQUIRE_EQUAL(single_frag.get_size(), sizeof(FragmentHeader) + 10);

  auto buf2 = malloc(20);
  Fragment collect_frag({ { buf1, 10 }, { buf2, 20 } });
  BOOST_REQUIRE_EQUAL(collect_frag.get_size(), sizeof(FragmentHeader) + 30);
}

/**
 * @brief Test construction of invalid Fragments
*/
BOOST_AUTO_TEST_CASE(BadConstructors)
{
  Fragment* fragment_ptr;
  BOOST_REQUIRE_EXCEPTION(fragment_ptr = new Fragment(nullptr, size_t(100)),
                          dunedaq::dataformats::FragmentBufferError,
                          [&](dunedaq::dataformats::FragmentBufferError) { return true; });
  BOOST_REQUIRE_EXCEPTION(fragment_ptr = new Fragment(nullptr, size_t(-1)),
                          dunedaq::dataformats::FragmentSizeError,
                          [&](dunedaq::dataformats::FragmentSizeError) { return true; });

  auto buf1 = malloc(10);
  fragment_ptr = new Fragment(buf1, size_t(10));
  BOOST_REQUIRE_EQUAL(fragment_ptr->get_size(), sizeof(FragmentHeader) + 10);
}

/**
 * @brief Check that Fragment constructors function correctly
 */
BOOST_AUTO_TEST_CASE(ExistingFragmentConstructor)
{
  FragmentHeader header;
  header.m_size = sizeof(FragmentHeader) + 4;
  header.m_trigger_number = 1;
  header.m_trigger_timestamp = 2;
  header.m_run_number = 3;

  auto frag = malloc(sizeof(FragmentHeader) + 4);
  memcpy(frag, &header, sizeof(FragmentHeader));

  uint8_t one = 1, two = 2, three = 3, four = 4;                               // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader), &one, 1);       // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 1, &two, 1);   // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 2, &three, 1); // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 3, &four, 1);  // NOLINT(build/unsigned)

  {
    Fragment test_frag(frag, Fragment::BufferAdoptionMode::kUseExistingBuffer);

    BOOST_REQUIRE_EQUAL(test_frag.get_storage_location(), frag);

    BOOST_REQUIRE_EQUAL(test_frag.get_trigger_number(), 1);
    BOOST_REQUIRE_EQUAL(test_frag.get_trigger_timestamp(), 2);
    BOOST_REQUIRE_EQUAL(test_frag.get_run_number(), 3);

    BOOST_REQUIRE_EQUAL(*static_cast<uint8_t*>(test_frag.get_data()), one);         // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(test_frag.get_data()) + 1), two);   // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(test_frag.get_data()) + 2), three); // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(test_frag.get_data()) + 3), four);  // NOLINT(build/unsigned)
    
  }
  free(frag); // Should not cause errors

  frag = malloc(sizeof(FragmentHeader) + 4);
  memcpy(frag, &header, sizeof(FragmentHeader));
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader), &four, 1);      // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 1, &three, 1); // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 2, &two, 1);   // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 3, &one, 1);   // NOLINT(build/unsigned)

  {
    Fragment test_frag(frag, Fragment::BufferAdoptionMode::kCopyFromBuffer);

    BOOST_REQUIRE(test_frag.get_storage_location() != frag);
    BOOST_REQUIRE_EQUAL(test_frag.get_trigger_number(), 1);
    BOOST_REQUIRE_EQUAL(test_frag.get_trigger_timestamp(), 2);
    BOOST_REQUIRE_EQUAL(test_frag.get_run_number(), 3);

    BOOST_REQUIRE_EQUAL(*static_cast<uint8_t*>(test_frag.get_data()), four);        // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(test_frag.get_data()) + 1), three); // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(test_frag.get_data()) + 2), two);   // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(test_frag.get_data()) + 3), one);   // NOLINT(build/unsigned)
  }

  {
    using blobtype_t = uint8_t;

    constexpr int blob1_num_elements = 123;
    constexpr int blob2_num_elements = 456;
    blobtype_t blob1[blob1_num_elements];
    blobtype_t blob2[blob2_num_elements];

    Fragment test_frag(
      std::vector<std::pair<void*, size_t>>({ { &blob1, blob1_num_elements }, { &blob2, blob2_num_elements } }));

    BOOST_REQUIRE_EQUAL(sizeof(FragmentHeader) + blob1_num_elements + blob2_num_elements, test_frag.get_size());
  }

  free(frag); // Should not cause errors
}

/**
 * @brief Test header field manipulation methods
*/
BOOST_AUTO_TEST_CASE(HeaderFields)
{

  FragmentHeader header;
  header.m_size = sizeof(FragmentHeader) + 4;
  header.m_trigger_number = 1;
  header.m_trigger_timestamp = 2;
  header.m_run_number = 3;
  header.m_window_offset = 4;
  header.m_window_width = 5;

  GeoID component;
  component.m_apa_number = 6;
  component.m_link_number = 7;
  header.m_link_id = component;

  header.m_error_bits = 0x12345678;
  header.m_fragment_type = 8;

  auto buf1 = malloc(10);
  Fragment frag(buf1, size_t(10));
  BOOST_REQUIRE_EQUAL(frag.get_size(), sizeof(FragmentHeader) + 10);

  frag.set_header_fields(header);
  BOOST_REQUIRE_EQUAL(frag.get_size(), sizeof(FragmentHeader) + 10);
  BOOST_REQUIRE_EQUAL(frag.get_header().m_run_number, header.m_run_number);
  BOOST_REQUIRE_EQUAL(frag.get_trigger_number(), header.m_trigger_number);
  BOOST_REQUIRE_EQUAL(frag.get_run_number(), header.m_run_number);
  BOOST_REQUIRE_EQUAL(frag.get_trigger_timestamp(), header.m_trigger_timestamp);
  BOOST_REQUIRE_EQUAL(frag.get_window_offset(), header.m_window_offset);
  BOOST_REQUIRE_EQUAL(frag.get_window_width(), header.m_window_width);
  BOOST_REQUIRE_EQUAL(frag.get_link_id().m_apa_number, header.m_link_id.m_apa_number);
  BOOST_REQUIRE_EQUAL(frag.get_link_id().m_link_number, header.m_link_id.m_link_number);

  BOOST_REQUIRE_EQUAL(frag.get_error_bits().to_ulong(), header.m_error_bits);
  BOOST_REQUIRE_EQUAL(frag.get_error_bit(static_cast<FragmentErrorBits>(3)), true);

  BOOST_REQUIRE_EQUAL(frag.get_fragment_type(), header.m_fragment_type);

  auto theHeader = reinterpret_cast<const FragmentHeader*>(frag.get_storage_location());
  frag.set_trigger_number(0x11);
  BOOST_REQUIRE_EQUAL(theHeader->m_trigger_number, 0x11);
  frag.set_run_number(0x33);
  BOOST_REQUIRE_EQUAL(theHeader->m_run_number, 0x33);
  frag.set_trigger_timestamp(0x22);
  BOOST_REQUIRE_EQUAL(theHeader->m_trigger_timestamp, 0x22);
  frag.set_window_offset(0x44);
  BOOST_REQUIRE_EQUAL(theHeader->m_window_offset, 0x44);
  frag.set_window_width(0x55);
  BOOST_REQUIRE_EQUAL(theHeader->m_window_width, 0x55);
  frag.set_type(0x88);
  BOOST_REQUIRE_EQUAL(theHeader->m_fragment_type, 0x88);

  GeoID new_component;
  new_component.m_apa_number = 0x66;
  new_component.m_link_number = 0x77;
  frag.set_link_id(new_component);
  BOOST_REQUIRE_EQUAL(theHeader->m_link_id.m_apa_number, 0x66);
  BOOST_REQUIRE_EQUAL(theHeader->m_link_id.m_link_number, 0x77);

  std::bitset<32> no_errors(0);
  frag.set_error_bits(no_errors);
  BOOST_REQUIRE_EQUAL(theHeader->m_error_bits, 0);
  frag.set_error_bit(static_cast<FragmentErrorBits>(1), true);
  BOOST_REQUIRE_EQUAL(theHeader->m_error_bits, 2);
  frag.set_error_bit(static_cast<FragmentErrorBits>(1), false);
  BOOST_REQUIRE_EQUAL(theHeader->m_error_bits, 0);
}

BOOST_AUTO_TEST_SUITE_END()
