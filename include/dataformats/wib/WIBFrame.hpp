/**
 * @file WIBFrame.hpp WIB1 bit fields and accessors
 * Originally FelixFrame.hpp from protodune.
 * Original authors M. Vermeulen, R.Sipos 2018
 * Modified by P. Rodrigues on June 2020
 *
 * This is part of the DUNE DAQ , copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_WIB_WIBFRAME_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_WIB_WIBFRAME_HPP_

#include <bitset>
#include <iostream>
#include <vector>

namespace dunedaq {
namespace dataformats {

typedef uint32_t word_t;
typedef uint16_t adc_t;
typedef std::vector<uint16_t> adc_v;

//===================
// WIB header struct
//===================
struct WIBHeader
{
  word_t sof : 8, version : 5, fiber_no : 3, crate_no : 5, slot_no : 3, reserved_1 : 8;
  word_t mm : 1, oos : 1, reserved_2 : 14, wib_errors : 16;
  word_t timestamp_1;
  word_t timestamp_2 : 16, wib_counter_1 : 15, z : 1;

  uint64_t timestamp() const
  {
    uint64_t timestamp = timestamp_1 | ((uint64_t)timestamp_2 << 32);
    if (!z) {
      timestamp |= (uint64_t)wib_counter_1 << 48;
    }
    return timestamp;
  }
  uint16_t wib_counter() const { return z ? wib_counter_1 : 0; }

  void set_timestamp(const uint64_t new_timestamp)
  {
    timestamp_1 = new_timestamp;
    timestamp_2 = new_timestamp >> 32;
    if (!z) {
      wib_counter_1 = new_timestamp >> 48;
    }
  }
  void set_wib_counter(const uint16_t new_wib_counter)
  {
    if (z) {
      wib_counter_1 = new_wib_counter;
    }
  }

  // Print functions for debugging.
  void print() const
  {
    std::cout << "SOF:" << unsigned(sof) << " version:" << unsigned(version) << " fiber:" << unsigned(fiber_no)
              << " slot:" << unsigned(slot_no) << " crate:" << unsigned(crate_no) << " mm:" << unsigned(mm)
              << " oos:" << unsigned(oos) << " wib_errors:" << unsigned(wib_errors) << " timestamp: " << timestamp()
              << '\n';
  }
  void printHex() const
  {
    std::cout << std::hex << "SOF:" << sof << " version:" << version << " fiber:" << fiber_no << " slot:" << slot_no
              << " crate:" << crate_no << " mm:" << mm << " oos:" << oos << " wib_errors:" << wib_errors
              << " timestamp: " << timestamp() << std::dec << '\n';
  }

  void printBits() const
  {
    std::cout << "SOF:" << std::bitset<8>(sof) << " version:" << std::bitset<5>(version)
              << " fiber:" << std::bitset<3>(fiber_no) << " slot:" << std::bitset<5>(slot_no)
              << " crate:" << std::bitset<3>(crate_no) << " mm:" << bool(mm) << " oos:" << bool(oos)
              << " wib_errors:" << std::bitset<16>(wib_errors) << " timestamp: " << timestamp() << '\n'
              << " Z: " << z << '\n';
  }
};

//=======================
// COLDATA header struct
//=======================
struct ColdataHeader
{
  word_t s1_error : 4, s2_error : 4, reserved_1 : 8, checksum_a_1 : 8, checksum_b_1 : 8;
  word_t checksum_a_2 : 8, checksum_b_2 : 8, coldata_convert_count : 16;
  word_t error_register : 16, reserved_2 : 16;
  word_t hdr_1 : 4, hdr_3 : 4, hdr_2 : 4, hdr_4 : 4, hdr_5 : 4, hdr_7 : 4, hdr_6 : 4, hdr_8 : 4;

  uint16_t checksum_a() const { return (uint16_t)checksum_a_1 | (checksum_a_2 << 8); }
  uint16_t checksum_b() const { return (uint16_t)checksum_b_1 | (checksum_b_2 << 8); }
  uint8_t hdr(const uint8_t i) const
  {
    switch (i) {
      case 1:
        return hdr_1;
      case 2:
        return hdr_2;
      case 3:
        return hdr_3;
      case 4:
        return hdr_4;
      case 5:
        return hdr_5;
      case 6:
        return hdr_6;
      case 7:
        return hdr_7;
      case 8:
        return hdr_8;
    }
    return 0;
  }

  void set_checksum_a(const uint16_t new_checksum_a)
  {
    checksum_a_1 = new_checksum_a;
    checksum_a_2 = new_checksum_a >> 8;
  }
  void set_checksum_b(const uint16_t new_checksum_b)
  {
    checksum_b_1 = new_checksum_b;
    checksum_b_2 = new_checksum_b >> 8;
  }
  void set_hdr(const uint8_t i, const uint8_t new_hdr)
  {
    switch (i) {
      case 1:
        hdr_1 = new_hdr;
        break;
      case 2:
        hdr_2 = new_hdr;
        break;
      case 3:
        hdr_3 = new_hdr;
        break;
      case 4:
        hdr_4 = new_hdr;
        break;
      case 5:
        hdr_5 = new_hdr;
        break;
      case 6:
        hdr_6 = new_hdr;
        break;
      case 7:
        hdr_7 = new_hdr;
        break;
      case 8:
        hdr_8 = new_hdr;
        break;
    }
  }

  // Print functions for debugging.
  void print() const
  {
    std::cout << "s1_error:" << unsigned(s1_error) << " s2_error:" << unsigned(s2_error)
              << " checksum_a1:" << unsigned(checksum_a_1) << " checksum_b1:" << unsigned(checksum_b_1)
              << " checksum_a2:" << unsigned(checksum_a_2) << " checksum_b1:" << unsigned(checksum_b_2)
              << " coldata_convert_count:" << unsigned(coldata_convert_count)
              << " error_register:" << unsigned(error_register) << " hdr_1:" << unsigned(hdr_1)
              << " hdr_2:" << unsigned(hdr_2) << " hdr_3:" << unsigned(hdr_3) << " hdr_4:" << unsigned(hdr_4)
              << " hdr_5:" << unsigned(hdr_5) << " hdr_6:" << unsigned(hdr_6) << " hdr_7:" << unsigned(hdr_7)
              << " hdr_8:" << unsigned(hdr_8);
    std::cout << '\n';
  }
  void printHex() const
  {
    std::cout << std::hex << "s1_error:" << s1_error << " s2_error:" << s2_error << " checksum_a1:" << checksum_a_1
              << " checksum_b1:" << checksum_b_1 << " checksum_a2:" << checksum_a_2 << " checksum_b1:" << checksum_b_2
              << " coldata_convert_count:" << coldata_convert_count << " error_register:" << error_register
              << " hdr_1:" << hdr_1 << " hdr_2:" << hdr_2 << " hdr_3:" << hdr_3 << " hdr_4:" << hdr_4
              << " hdr_5:" << hdr_5 << " hdr_6:" << hdr_6 << " hdr_7:" << hdr_7 << " hdr_8:" << hdr_8;
    std::cout << '\n';
  }
  void printBits() const
  {
    std::cout << "s1_error:" << std::bitset<4>(s1_error) << " s2_error:" << std::bitset<4>(s2_error)
              << " checksum_a1:" << std::bitset<8>(checksum_a_1) << " checksum_b1:" << std::bitset<8>(checksum_b_1)
              << " checksum_a2:" << std::bitset<8>(checksum_a_2) << " checksum_b2:" << std::bitset<8>(checksum_b_2)
              << " coldata_convert_count:" << std::bitset<16>(coldata_convert_count)
              << " error_register:" << std::bitset<16>(error_register) << " hdr_1:" << std::bitset<8>(hdr_1)
              << " hdr_2:" << std::bitset<8>(hdr_2) << " hdr_3:" << std::bitset<8>(hdr_3)
              << " hdr_4:" << std::bitset<8>(hdr_4) << " hdr_5:" << std::bitset<8>(hdr_5)
              << " hdr_6:" << std::bitset<8>(hdr_6) << " hdr_7:" << std::bitset<8>(hdr_7)
              << " hdr_8:" << std::bitset<8>(hdr_8);
    std::cout << '\n';
  }
};

//========================
// COLDATA segment struct
//========================
struct ColdataSegment
{
  // This struct contains three words of ADC values that form the main repeating
  // pattern in the COLDATA block.
  word_t adc0ch0_1 : 8, adc1ch0_1 : 8, adc0ch0_2 : 4, adc0ch1_1 : 4, adc1ch0_2 : 4, adc1ch1_1 : 4;
  word_t adc0ch1_2 : 8, adc1ch1_2 : 8, adc0ch2_1 : 8, adc1ch2_1 : 8;
  word_t adc0ch2_2 : 4, adc0ch3_1 : 4, adc1ch2_2 : 4, adc1ch3_1 : 4, adc0ch3_2 : 8, adc1ch3_2 : 8;

  uint16_t channel(const uint8_t adc, const uint8_t ch) const
  {
    if (adc % 2 == 0) {
      switch (ch % 4) {
        case 0:
          return adc0ch0_1 | adc0ch0_2 << 8;
        case 1:
          return adc0ch1_1 | adc0ch1_2 << 4;
        case 2:
          return adc0ch2_1 | adc0ch2_2 << 8;
        case 3:
          return adc0ch3_1 | adc0ch3_2 << 4;
      }
    } else if (adc % 2 == 1) {
      switch (ch % 4) {
        case 0:
          return adc1ch0_1 | adc1ch0_2 << 8;
        case 1:
          return adc1ch1_1 | adc1ch1_2 << 4;
        case 2:
          return adc1ch2_1 | adc1ch2_2 << 8;
        case 3:
          return adc1ch3_1 | adc1ch3_2 << 4;
      }
    }
    return 0;
  }

  void set_channel(const uint8_t adc, const uint8_t ch, const uint16_t new_val)
  {
    if (adc % 2 == 0) {
      switch (ch % 4) {
        case 0:
          adc0ch0_1 = new_val;
          adc0ch0_2 = new_val >> 8;
          break;
        case 1:
          adc0ch1_1 = new_val;
          adc0ch1_2 = new_val >> 4;
          break;
        case 2:
          adc0ch2_1 = new_val;
          adc0ch2_2 = new_val >> 8;
          break;
        case 3:
          adc0ch3_1 = new_val;
          adc0ch3_2 = new_val >> 4;
          break;
      }
    } else if (adc % 2 == 1) {
      switch (ch % 4) {
        case 0:
          adc1ch0_1 = new_val;
          adc1ch0_2 = new_val >> 8;
          break;
        case 1:
          adc1ch1_1 = new_val;
          adc1ch1_2 = new_val >> 4;
          break;
        case 2:
          adc1ch2_1 = new_val;
          adc1ch2_2 = new_val >> 8;
          break;
        case 3:
          adc1ch3_1 = new_val;
          adc1ch3_2 = new_val >> 4;
          break;
      }
    }
  }
};

//======================
// COLDATA block struct
//======================
struct ColdataBlock
{
  ColdataHeader head;

  ColdataSegment segments[8];

  uint16_t channel(const uint8_t adc, const uint8_t ch) const
  {
    // Each segment houses one half (four channels) of two subsequent ADCs.
    return segments[(adc / 2) * 2 + ch / 4].channel(adc, ch);
  }

  void set_channel(const uint8_t adc, const uint8_t ch, uint16_t new_val)
  {
    segments[(adc / 2) * 2 + ch / 4].set_channel(adc, ch, new_val);
  }

  void printADCs() const
  {
    std::cout << "\t\t0\t1\t2\t3\t4\t5\t6\t7\n";
    for (int i = 0; i < 8; i++) {
      std::cout << "Stream " << i << ":\t";
      for (int j = 0; j < 8; j++) {
        std::cout << std::hex << channel(i, j) << '\t';
      }
      std::cout << std::dec << '\n';
    }
  }
};

//=============
// FELIX frame
//=============
class WIBFrame
{
private:
  WIBHeader head;
  ColdataBlock blocks[4];

public:
  // Constant expressions
  static constexpr size_t num_frame_hdr_words = 4;
  static constexpr size_t num_COLDATA_hdr_words = 4;
  static constexpr size_t num_frame_words = 116;
  static constexpr size_t num_frame_bytes = num_frame_words * sizeof(word_t);
  static constexpr size_t num_COLDATA_words = 28;

  static constexpr size_t num_block_per_frame = 4;
  static constexpr size_t num_ch_per_frame = 256;
  static constexpr size_t num_ch_per_block = 64;
  static constexpr size_t num_seg_per_block = 8;
  static constexpr size_t num_ch_per_seg = 8;

  // WIB header accessors
  uint8_t sof() const { return head.sof; }
  uint8_t version() const { return head.version; }
  uint8_t fiber_no() const { return head.fiber_no; }
  uint8_t crate_no() const { return head.crate_no; }
  uint8_t slot_no() const { return head.slot_no; }
  uint8_t mm() const { return head.mm; }
  uint8_t oos() const { return head.oos; }
  uint16_t wib_errors() const { return head.wib_errors; }
  uint64_t timestamp() const { return head.timestamp(); }
  uint16_t wib_counter() const { return head.wib_counter(); }
  uint8_t z() const { return head.z; }
  // WIB header mutators
  void set_sof(const uint8_t new_sof) { head.sof = new_sof; }
  void set_version(const uint8_t new_version) { head.version = new_version; }
  void set_fiber_no(const uint8_t new_fiber_no) { head.fiber_no = new_fiber_no; }
  void set_crate_no(const uint8_t new_crate_no) { head.crate_no = new_crate_no; }
  void set_slot_no(const uint8_t new_slot_no) { head.slot_no = new_slot_no; }
  void set_mm(const uint8_t new_mm) { head.mm = new_mm; }
  void set_oos(const uint8_t new_oos) { head.oos = new_oos; }
  void set_wib_errors(const uint16_t new_wib_errors) { head.wib_errors = new_wib_errors; }
  void set_timestamp(uint64_t new_timestamp) { head.set_timestamp(new_timestamp); }
  void set_wib_counter(uint16_t new_wib_counter) { head.wib_counter_1 = new_wib_counter; }
  void set_z(uint8_t new_z) { head.z = new_z; }

  // COLDATA header accessors
  uint8_t s1_error(const uint8_t block_num) const { return blocks[block_num].head.s1_error; }
  uint8_t s2_error(const uint8_t block_num) const { return blocks[block_num].head.s2_error; }
  uint16_t checksum_a(const uint8_t block_num) const { return blocks[block_num].head.checksum_a(); }
  uint16_t checksum_b(const uint8_t block_num) const { return blocks[block_num].head.checksum_b(); }
  uint16_t coldata_convert_count(const uint8_t block_num) const { return blocks[block_num].head.coldata_convert_count; }
  uint16_t error_register(const uint8_t block_num) const { return blocks[block_num].head.error_register; }
  uint8_t hdr(const uint8_t block_num, const uint8_t i) const { return blocks[block_num].head.hdr(i); }
  // COLDATA header mutators
  void set_s1_error(const uint8_t block_num, const uint8_t new_s1_error)
  {
    blocks[block_num].head.s1_error = new_s1_error;
  }
  void set_s2_error(const uint8_t block_num, const uint8_t new_s2_error)
  {
    blocks[block_num].head.s2_error = new_s2_error;
  }
  void set_checksum_a(const uint8_t block_num, const uint16_t new_checksum_a)
  {
    blocks[block_num].head.set_checksum_a(new_checksum_a);
  }
  void set_checksum_b(const uint8_t block_num, const uint16_t new_checksum_b)
  {
    blocks[block_num].head.set_checksum_b(new_checksum_b);
  }
  void set_coldata_convert_count(const uint8_t block_num, const uint16_t new_coldata_convert_count)
  {
    blocks[block_num].head.coldata_convert_count = new_coldata_convert_count;
  }
  void set_error_register(const uint8_t block_num, const uint16_t new_error_register)
  {
    blocks[block_num].head.error_register = new_error_register;
  }
  void set_hdr(const uint8_t block_num, const uint8_t i, const uint8_t new_hdr)
  {
    blocks[block_num].head.set_hdr(i, new_hdr);
  }

  // Channel accessors
  uint16_t channel(const uint8_t block_num, const uint8_t adc, const uint8_t ch) const
  {
    return blocks[block_num].channel(adc, ch);
  }
  uint16_t channel(const uint8_t block_num, const uint8_t ch) const { return channel(block_num, ch / 8, ch % 8); }
  uint16_t channel(const uint8_t ch) const { return channel(ch / 64, ch % 64); }

  const ColdataBlock& block(const uint8_t b) const { return blocks[b]; }
  ColdataBlock& block(const uint8_t b) { return blocks[b]; }

  // Channel mutators
  void set_channel(const uint8_t block_num, const uint8_t adc, const uint8_t ch, const uint16_t new_val)
  {
    blocks[block_num].set_channel(adc, ch, new_val);
  }
  void set_channel(const uint8_t block_num, const uint8_t ch, const uint16_t new_val)
  {
    set_channel(block_num, ch / 8, ch % 8, new_val);
  }
  void set_channel(const uint8_t ch, const uint16_t new_val) { set_channel(ch / 64, ch % 64, new_val); }

  // Const struct accessors.
  const WIBHeader* wib_header() const { return &head; }
  const ColdataHeader* coldata_header(const unsigned& block = 0) const { return &blocks[block % 4].head; }

  // Utility functions
  void print() const
  {
    std::cout << "Printing frame:\n";
    head.print();
    for (auto b : blocks) {
      b.head.print();
      b.printADCs();
    }
  }
};

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_WIB_WIBFRAME_HPP_
