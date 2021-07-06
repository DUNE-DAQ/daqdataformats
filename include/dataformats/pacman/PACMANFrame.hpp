/**
 * @file PACMANFrame.hpp PACMAN bit fields and accessors (ND LAr).
 *
 * This is part of the DUNE DAQ , copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_PACMANFRAME_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_PACMANFRAME_HPP_

#include <bitset>
#include <iostream>
#include <vector>
#include <stdint.h>

namespace dunedaq {
namespace dataformats {

using word_t = uint32_t; // NOLINT(build/unsigned)

/**
 * @brief PACMAN frame
 */
class PACMANFrame
{
  public:
  static constexpr int WORD_LEN = 8;  // bytes
  static constexpr int HEADER_LEN = 16; // bytes
  
  struct PACMANMessageHeader
  {
    // Can be implemented on request, not currently necessary.
  };
  
  enum msg_type { // message type declarations
    DATA_MSG = 0x44,
    REQ_MSG  = 0x3F,
    REP_MSG  = 0x21
  };

  enum word_type { // word type declarations
    DATA_WORD  = 0x44,
    TRIG_WORD  = 0x54,
    SYNC_WORD  = 0x53,
    PING_WORD  = 0x50,
    WRITE_WORD = 0x57,
    READ_WORD  = 0x52,
    TX_WORD    = 0x44,
    ERR_WORD   = 0x45
  };
  enum packet_type { // packet type declarations
    DATA_PACKET         = 0x0,
    CONFIG_WRITE_PACKET = 0x2,
    CONFIG_READ_PACKET  = 0x3
  };


  static constexpr int MSG_TYPE_OFFSET= 0; // bytes
  static constexpr int MSG_WORDS_OFFSET = 6; // bytes
  static constexpr int UNIX_TS_OFFSET = 1; // bytes
  static constexpr int WORD_TYPE_OFFSET = 0; // bytes
  static constexpr int IO_CHANNEL_OFFSET = 1; // bytes
  static constexpr int RECEIPT_TIMESTAMP_OFFSET = 4; // bytes
  static constexpr int PACKET_OFFSET = 8; // bytes


  /* ~~~ Access into message header and message contents ~~~ */

  uint32_t get_msg_bytes(void* msg) {
    // get total number of bytes in message, including header
    return HEADER_LEN + WORD_LEN * (*get_msg_words(msg));
  }

  uint8_t* get_msg_type(void* msg) {
    // get pointer to message type
    return static_cast<uint8_t*>(msg) + MSG_TYPE_OFFSET;
  }

  uint16_t* get_msg_words(void* msg) {
    // get pointer to number of message words
    return reinterpret_cast<uint16_t*>(static_cast<uint8_t*>(msg) + MSG_WORDS_OFFSET);
  }

  uint32_t* get_msg_unix_ts(void* msg) {
    // get pointer to message unix timestamp
    return reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(msg) + UNIX_TS_OFFSET);
  }

  void* get_msg_word(void* msg, const uint32_t i) {
    // get pointer to word within message
    return static_cast<void*>((static_cast<uint8_t*>(msg) + HEADER_LEN + WORD_LEN * i));
  }

  
  /* ~~~ Access into message words ~~~ */

  uint8_t* get_word_type(void* word) {
    // get pointer to word type
    return static_cast<uint8_t*>(word) + WORD_TYPE_OFFSET;
  }
  uint8_t* get_word_io_channel(void* word) {
    // only valid for DATA type words, get pointer to the PACMAN io channel a word arrived on
    return static_cast<uint8_t*>(word) + IO_CHANNEL_OFFSET;
  }
  uint32_t* get_word_receipt_timestamp(void* word) {
    // only valid for DATA type words, get pointer to the timestamp when word was received at the PACMAN
    return reinterpret_cast<uint32_t*>((static_cast<uint8_t*>(word) + RECEIPT_TIMESTAMP_OFFSET);
  }
  uint64_t* get_word_packet(void* word) {
    // only valid for DATA type words, get pointer to the LArPix word
    return reinterpret_cast<uint64_t*>((static_cast<uint8_t*>(word) + PACKET_OFFSET);
  }


  /* ~~~ Access into LArPix packets ~~~ */

  uint64_t get_packet_data(uint64_t* packet, const uint8_t bit_offset, const uint64_t bit_mask) {
    // right-shift packet bits by bit_offset and then grab bits specified by bit_mask
    return (*packet >> bit_offset) & bit_mask;
  }

  static constexpr int PACKET_TYPE_OFFSET = 0; // bits
  static constexpr std::uint_fast8_t PACKET_TYPE_MASK{0x3}; // bitmask
  uint64_t get_packet_type(uint64_t* packet) {
   // bits [0:1]
   return get_packet_data(packet, PACKET_TYPE_OFFSET, PACKET_TYPE_MASK);
  }

  static constexpr int PACKET_CHIPID_OFFSET = 8; // bits
  static constexpr std::uint_fast8_t PACKET_CHIPID_MASK{0xFF}; // bitmask
  uint64_t get_packet_chipid(uint64_t* packet) {
    // bits [2:9]
    return get_packet_data(packet, PACKET_CHIPID_OFFSET, PACKET_CHIPID_MASK);
  }

  static constexpr int PACKET_CHANNELID_OFFSET = 10; // bits
  static constexpr std::uint_fast8_t PACKET_CHANNELID_MASK{0x3F}; // bitmask
  uint64_t get_packet_channelid(uint64_t* packet) {
    // bits [10:15], only valid for data packets
    return get_packet_data(packet, PACKET_CHANNELID_OFFSET, PACKET_CHANNELID_MASK);
  }

  static constexpr int PACKET_TIMESTAMP_OFFSET = 16; // bits
  static constexpr std::uint_fast32_t PACKET_TIMESTAMP_MASK{0x7FFFFFFF}; // bitmask
  uint64_t get_packet_timestamp(uint64_t* packet) {
    // bits [46:16], only valid for data packets
    return get_packet_data(packet, PACKET_TIMESTAMP_OFFSET, PACKET_TIMESTAMP_MASK);
  }

  static constexpr int PACKET_FIRST_PACKET_OFFSET = 47; // bits
  static constexpr std::uint_fast8_t PACKET_FIRST_PACKET_MASK{0x1}; // bitmask
  uint64_t get_packet_first_packet(uint64_t* packet) {
    // bits [47], only valid for data packets
    return get_packet_data(packet, PACKET_FIRST_PACKET_OFFSET, PACKET_FIRST_PACKET_MASK);
  }

  static constexpr int PACKET_DATAWORD_OFFSET = 48; // bits
  static constexpr std::uint_fast8_t PACKET_DATAWORD_MASK{0xFF}; // bitmask
  uint64_t get_packet_dataword(uint64_t* packet) {
    // bits [48:55], only valid for data packets
    return get_packet_data(packet, PACKET_DATAWORD_OFFSET, PACKET_DATAWORD_MASK);
  }

  static constexpr int PACKET_TRIGGER_TYPE_OFFSET = 56; // bits
  static constexpr std::uint_fast8_t PACKET_TRIGGER_TYPE_MASK{0x3}; // bitmask
  uint64_t get_packet_trigger_type(uint64_t* packet) {
    // bits [56:57], only valid for data packets
    return get_packet_data(packet, PACKET_TRIGGER_TYPE_OFFSET, PACKET_TRIGGER_TYPE_MASK);
  }

  static constexpr int PACKET_LOCAL_FIFO_STATUS_OFFSET = 58;  // bits
  static constexpr std::uint_fast8_t PACKET_LOCAL_FIFO_STATUS_MASK{0x3}; // bitmask
  uint64_t get_packet_local_fifo_status(uint64_t* packet) {
    // bits [58:59], only valid for data packets
    return get_packet_data(packet, PACKET_LOCAL_FIFO_STATUS_OFFSET, PACKET_LOCAL_FIFO_STATUS_MASK);
  }

  static constexpr int PACKET_SHARED_FIFO_STATUS_OFFSET = 60; // bits
  static constexpr std::uint_fast8_t PACKET_SHARED_FIFO_STATUS_MASK{0x3}; // bitmask
  uint64_t get_packet_shared_fifo_status(uint64_t* packet) {
    // bits [60:61], only valid for data packets
    return get_packet_data(packet, PACKET_SHARED_FIFO_STATUS_OFFSET, PACKET_SHARED_FIFO_STATUS_MASK);
  }

  static constexpr int PACKET_DOWNSTREAM_MARKER_OFFSET = 62; // bits
  static constexpr std::uint_fast8_t PACKET_DOWNSTREAM_MARKER_MASK{0x1}; // bitmask
  uint64_t get_packet_downstream_marker(uint64_t* packet) {
    // bits [62], only valid for data packets
    return get_packet_data(packet, PACKET_DOWNSTREAM_MARKER_OFFSET, PACKET_DOWNSTREAM_MARKER_MASK);
  }

  static constexpr int PACKET_PARITY_BIT_MARKER_OFFSET = 63;  // bits
  static constexpr std::uint_fast8_t PACKET_PARITY_BIT_MARKER_MASK{0x1}; // bitmask
  uint64_t get_packet_parity_bit(uint64_t* packet) {
    // bits [63], only valid for data packets
    return get_packet_data(packet, PACKET_PARITY_BIT_MARKER_OFFSET, PACKET_PARITY_BIT_MARKER_MASK);
  }

  friend std::ostream& operator<<(std::ostream& o, PACMANFrame const& frame);

private:
  
};

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_PACMANFRAME_HPP_