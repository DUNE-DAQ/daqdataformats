/**
 * @file Fragment_test.cxx Fragment class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "dataformats/Fragment.hpp"
#include "serialization/Serialization.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE Fragment_test // NOLINT

#include "boost/test/unit_test.hpp"

#include <string>
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
    Fragment test_frag(frag); // frag memory now owned by Fragment

    BOOST_REQUIRE_EQUAL(test_frag.get_storage_location(), frag);

    BOOST_REQUIRE_EQUAL(test_frag.get_trigger_number(), 1);
    BOOST_REQUIRE_EQUAL(test_frag.get_trigger_timestamp(), 2);
    BOOST_REQUIRE_EQUAL(test_frag.get_run_number(), 3);

    BOOST_REQUIRE_EQUAL(*static_cast<uint8_t*>(test_frag.get_data()), one);         // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(test_frag.get_data()) + 1), two);   // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(test_frag.get_data()) + 2), three); // NOLINT(build/unsigned)
    BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(test_frag.get_data()) + 3), four);  // NOLINT(build/unsigned)
  }

  frag = malloc(sizeof(FragmentHeader) + 4);
  memcpy(frag, &header, sizeof(FragmentHeader));
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader), &four, 1);      // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 1, &three, 1); // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 2, &two, 1);   // NOLINT(build/unsigned)
  memcpy(static_cast<uint8_t*>(frag) + sizeof(FragmentHeader) + 3, &one, 1);   // NOLINT(build/unsigned)

  {
    Fragment test_frag(frag, true);

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
    blobtype_t blob1[ blob1_num_elements ];
    blobtype_t blob2[ blob2_num_elements ];

    Fragment test_frag(
    		      std::vector<std::pair<void*, size_t>>( { { &blob1, blob1_num_elements}, { &blob2, blob2_num_elements} } )
    		      );

    BOOST_REQUIRE_EQUAL(sizeof(FragmentHeader) + blob1_num_elements + blob2_num_elements,
    			test_frag.get_size());

  }

  free(frag); // Should not cause errors
}

BOOST_AUTO_TEST_CASE(SerDes_MsgPack)
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

  Fragment test_frag(frag); // frag memory now owned by Fragment
  auto bytes = dunedaq::serialization::serialize(test_frag, dunedaq::serialization::kMsgPack);

  Fragment frag_deserialized = dunedaq::serialization::deserialize<Fragment>(bytes);

  BOOST_REQUIRE_EQUAL(frag_deserialized.get_trigger_number(), test_frag.get_trigger_number());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_run_number(), test_frag.get_run_number());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_trigger_timestamp(), test_frag.get_trigger_timestamp());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_window_offset(), test_frag.get_window_offset());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_window_width(), test_frag.get_window_width());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_link_id().m_link_number, test_frag.get_link_id().m_link_number);
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_link_id().m_apa_number, test_frag.get_link_id().m_apa_number);
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_error_bits(), test_frag.get_error_bits());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_fragment_type(), test_frag.get_fragment_type());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_size(), test_frag.get_size());

  BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(frag_deserialized.get_data()) + 0),
                      *(static_cast<uint8_t*>(test_frag.get_data())+ 0));
  BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(frag_deserialized.get_data()) + 1),
                      *(static_cast<uint8_t*>(test_frag.get_data())+ 1));
  BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(frag_deserialized.get_data()) + 2),
                      *(static_cast<uint8_t*>(test_frag.get_data())+ 2));
}

BOOST_AUTO_TEST_CASE(SerDes_JSON)
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

  Fragment test_frag(frag); // frag memory now owned by Fragment

  auto bytes = dunedaq::serialization::serialize(test_frag, dunedaq::serialization::kJSON);

  Fragment frag_deserialized = dunedaq::serialization::deserialize<Fragment>(bytes);

  BOOST_REQUIRE_EQUAL(frag_deserialized.get_size(), test_frag.get_size());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_trigger_number(), test_frag.get_trigger_number());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_run_number(), test_frag.get_run_number());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_trigger_timestamp(), test_frag.get_trigger_timestamp());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_window_offset(), test_frag.get_window_offset());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_window_width(), test_frag.get_window_width());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_link_id().m_link_number, test_frag.get_link_id().m_link_number);
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_link_id().m_apa_number, test_frag.get_link_id().m_apa_number);
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_error_bits(), test_frag.get_error_bits());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_fragment_type(), test_frag.get_fragment_type());
  BOOST_REQUIRE_EQUAL(frag_deserialized.get_size(), test_frag.get_size());

  BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(frag_deserialized.get_data()) + 0),
                      *(static_cast<uint8_t*>(test_frag.get_data())+ 0));
  BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(frag_deserialized.get_data()) + 1),
                      *(static_cast<uint8_t*>(test_frag.get_data())+ 1));
  BOOST_REQUIRE_EQUAL(*(static_cast<uint8_t*>(frag_deserialized.get_data()) + 2),
                      *(static_cast<uint8_t*>(test_frag.get_data())+ 2));
}

BOOST_AUTO_TEST_SUITE_END()
