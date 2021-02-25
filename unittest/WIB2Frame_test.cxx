/**
 * @file WIBFrame_test.cxx WIBFrame class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "dataformats/wib2/WIB2Frame.hpp"
#include "dataformats/wib2/WIB2Unpack.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE WIB2Frame_test // NOLINT

#include "boost/test/data/test_case.hpp"
#include "boost/test/unit_test.hpp"

#include <string>
#include <vector>

using namespace dunedaq::dataformats;

typedef std::array<uint16_t, 256> vals_type;
BOOST_TEST_DONT_PRINT_LOG_VALUE(vals_type)

BOOST_AUTO_TEST_SUITE(WIB2Frame_test)

// inline std::ostream&
// operator<<(std::ostream& os, std::array<uint16_t, 256> const& arr)
// {
//   for(size_t i=0; i<arr.size(); ++i){
//     os << arr[i] << ", ";
//   }
//   return os;
// }

std::vector<vals_type>
make_vals()
{
  std::vector<vals_type> ret;

  std::default_random_engine e1(10);
  std::uniform_int_distribution<int> uniform_dist(0, (1 << 14) - 1);

  const int n_fuzz = 100;
  for (int i = 0; i < n_fuzz; ++i) {
    std::array<uint16_t, 256> vals;
    for (size_t j = 0; j < vals.size(); ++j) {
      vals[j] = uniform_dist(e1);
    }
    ret.push_back(vals);
  }

  return ret;
}

BOOST_DATA_TEST_CASE(CompareToUnpack, boost::unit_test::data::make(make_vals()), vals)
{
  // Check the WIB2 frame overlay code in WIB2Frame.hpp by comparing to
  // the output of the (independently-written) code in WIB2Unpack.hpp
  dunedaq::dataformats::wib2::frame14_unpacked unpacked;
  // Zero it out just to be on the safe side
  std::memset(&unpacked, 0, sizeof(dunedaq::dataformats::wib2::frame14_unpacked));

  dunedaq::dataformats::wib2::frame14 packed;
  // Zero it out just to be on the safe side
  std::memset(&packed, 0, sizeof(dunedaq::dataformats::wib2::frame14));

  // Fill the unpacked frame from the input `vals` array
  int counter = 0;
  for (int femb = 0; femb < 2; ++femb) {
    for (int i = 0; i < 40; ++i) {
      unpacked.femb[femb].u[i] = vals[counter++];
    }
    for (int i = 0; i < 40; ++i) {
      unpacked.femb[femb].v[i] = vals[counter++];
    }
    for (int i = 0; i < 48; ++i) {
      unpacked.femb[femb].x[i] = vals[counter++];
    }
  }

  // Create the packed array from the unpacked array
  dunedaq::dataformats::wib2::repack_frame(&unpacked, &packed);

  dunedaq::dataformats::WIB2Frame* wib2frame = reinterpret_cast<dunedaq::dataformats::WIB2Frame*>(&packed);

  for (int femb = 0; femb < 2; ++femb) {
    for (int i = 0; i < 40; ++i) {
      uint16_t gold = unpacked.femb[femb].u[i];
      uint16_t test = wib2frame->get_u(femb, i);
      BOOST_CHECK_EQUAL(gold, test);
    }
    for (int i = 0; i < 40; ++i) {
      uint16_t gold = unpacked.femb[femb].v[i];
      uint16_t test = wib2frame->get_v(femb, i);
      BOOST_CHECK_EQUAL(gold, test);
    }
    for (int i = 0; i < 48; ++i) {
      uint16_t gold = unpacked.femb[femb].x[i];
      uint16_t test = wib2frame->get_x(femb, i);
      BOOST_CHECK_EQUAL(gold, test);
    }
  } // loop over femb
}

BOOST_AUTO_TEST_SUITE_END()
