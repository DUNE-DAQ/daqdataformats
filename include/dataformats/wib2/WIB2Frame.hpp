#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_WIB2_WIB2FRAME_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_WIB2_WIB2FRAME_HPP_

#include <algorithm> // For std::min
#include <cassert>   // For assert()
#include <cstdio>
#include <cstdlib>
#include <stdexcept> // For std::out_of_range
#include <stdint.h>  // For uint32_t etc

namespace dunedaq::dataformats {

/**
 *  @brief Class for accessing raw WIB v2 frames, as used in ProtoDUNE-SP-II
 *
 *  The canonical definition of the WIB format is given in EDMS document 2088713:
 *  https://edms.cern.ch/document/2088713/4
 */
class WIB2Frame
{
public:
  // ===============================================================
  // Preliminaries
  // ===============================================================

  // The definition of the format is in terms of 32-bit words
  typedef uint32_t word_t;

  static const int BITS_PER_ADC = 14;
  static const int BITS_PER_WORD = 8 * sizeof(word_t);

  struct Header
  {
    word_t start_frame;
    word_t crate : 8, frame_version : 4, slot : 3, fiber : 1, femb_valid : 2, wib_code_1 : 14;
    word_t wib_code_2 : 32;
    word_t timestamp_1 : 32;
    word_t timestamp_2 : 32;
  };

  struct Trailer
  {
    word_t crc20 : 20;
    word_t flex_word_12 : 12;
    word_t eof : 8;
    word_t flex_word_24 : 24;
  };

  // ===============================================================
  // Data members
  // ===============================================================
  Header header;
  word_t adc_words[112];
  Trailer trailer;

  // ===============================================================
  // Accessors
  // ===============================================================

  /**
   * @brief Get the ith ADC value in the frame
   *
   * The ADC words are 14 bits long, stored packed in the data structure. The order is:
   *
   * - 40 values from FEMB0 U channels
   * - 40 values from FEMB0 V channels
   * - 48 values from FEMB0 X channels (collection)
   * - 40 values from FEMB1 U channels
   * - 40 values from FEMB1 V channels
   * - 48 values from FEMB1 X channels (collection)
   */
  uint16_t get_adc(int i) const
  {
    if (i < 0 || i > 255)
      throw std::out_of_range("ADC index out of range");

    // The index of the first (and sometimes only) word containing the required ADC value
    int word_index = BITS_PER_ADC * i / BITS_PER_WORD;
    assert(word_index < 112);
    // Where in the word the lowest bit of our ADC value is located
    int first_bit_position = (BITS_PER_ADC * i) % BITS_PER_WORD;
    // How many bits of our desired ADC are located in the `word_index`th word
    int bits_from_first_word = std::min(BITS_PER_ADC, BITS_PER_WORD - first_bit_position);
    uint16_t adc = adc_words[word_index] >> first_bit_position;
    // If we didn't get the full 14 bits from this word, we need the rest from the next word
    if (bits_from_first_word < BITS_PER_ADC) {
      assert(word_index + 1 < 112);
      adc |= adc_words[word_index + 1] << bits_from_first_word;
    }
    // Mask out all but the lowest 14 bits;
    return adc & 0x3FFFu;
  }

  /**
   * @brief Get the ith ADC value in the frame to val
   */
  void set_adc(int i, uint16_t val)
  {
    if (i < 0 || i > 255)
      throw std::out_of_range("ADC index out of range");
    if (val >= (1 << 14))
      throw std::out_of_range("ADC value out of range");

    // The index of the first (and sometimes only) word containing the required ADC value
    int word_index = BITS_PER_ADC * i / BITS_PER_WORD;
    assert(word_index < 112);
    // Where in the word the lowest bit of our ADC value is located
    int first_bit_position = (BITS_PER_ADC * i) % BITS_PER_WORD;
    // How many bits of our desired ADC are located in the `word_index`th word
    int bits_in_first_word = std::min(BITS_PER_ADC, BITS_PER_WORD - first_bit_position);
    adc_words[word_index] |= (val << first_bit_position);
    // If we didn't put the full 14 bits in this word, we need to put the rest in the next word
    if (bits_in_first_word < BITS_PER_ADC) {
      assert(word_index + 1 < 112);
      adc_words[word_index + 1] |= val >> bits_in_first_word;
    }
  }

  /** @brief Get the ith U-channel ADC in the given femb
   */
  uint16_t get_u(int femb, int i) const
  {
    if (femb < 0 || femb > 1)
      throw std::out_of_range("FEMB index out of range");
    if (i < 0 || i >= 40)
      throw std::out_of_range("U channel index out of range");

    return get_adc(128 * femb + i);
  }

  /** @brief Get the ith V-channel ADC in the given femb
   */
  uint16_t get_v(int femb, int i) const
  {
    if (femb < 0 || femb > 1)
      throw std::out_of_range("FEMB index out of range");
    if (i < 0 || i >= 40)
      throw std::out_of_range("V channel index out of range");

    return get_adc(128 * femb + 40 + i);
  }

  /** @brief Get the ith X-channel (ie, collection) ADC in the given femb
   */
  uint16_t get_x(int femb, int i) const
  {
    if (femb < 0 || femb > 1)
      throw std::out_of_range("FEMB index out of range");
    if (i < 0 || i >= 48)
      throw std::out_of_range("X channel index out of range");

    return get_adc(128 * femb + 40 + 40 + i);
  }

  /** @brief Set the ith U-channel ADC in the given femb to val
   */
  void set_u(int femb, int i, uint16_t val)
  {
    if (femb < 0 || femb > 1)
      throw std::out_of_range("FEMB index out of range");
    if (i < 0 || i >= 40)
      throw std::out_of_range("U channel index out of range");

    return set_adc(128 * femb + i, val);
  }

  /** @brief Set the ith V-channel ADC in the given femb to val
   */
  void set_v(int femb, int i, uint16_t val)
  {
    if (femb < 0 || femb > 1)
      throw std::out_of_range("FEMB index out of range");
    if (i < 0 || i >= 40)
      throw std::out_of_range("V channel index out of range");

    return set_adc(128 * femb + 40 + i, val);
  }

  /** @brief Set the ith X-channel (ie, collection) ADC in the given femb to val
   */
  void set_x(int femb, int i, uint16_t val)
  {
    if (femb < 0 || femb > 1)
      throw std::out_of_range("FEMB index out of range");
    if (i < 0 || i >= 48)
      throw std::out_of_range("X channel index out of range");

    return set_adc(128 * femb + 40 + 40 + i, val);
  }

  /** @brief Get the 64-bit timestamp of the frame
   */
  uint64_t get_timestamp() const { return (uint64_t)header.timestamp_1 | ((uint64_t)header.timestamp_2 << 32); }
};

// Define the static constants from WIB2Frame to avoid a linker error
const int WIB2Frame::BITS_PER_ADC;
const int WIB2Frame::BITS_PER_WORD;

}

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_WIB2_WIB2FRAME_HPP_

// Local Variables:
// c-basic-offset: 2
// End:
