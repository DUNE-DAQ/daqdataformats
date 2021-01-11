/**
 * @file TriggerRecordHeader.hpp  TriggerRecordHeader struct definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORDHEADER_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORDHEADER_HPP_

#include "dataformats/ComponentRequest.hpp"
#include "dataformats/TriggerRecordHeaderData.hpp"
#include "dataformats/Types.hpp"

#include <ostream>
#include <string>
#include <vector>

namespace dunedaq {

namespace dataformats {

/**
 * @brief C++ representation of a TriggerRecordHeader, which wraps a flat array that is the TriggerRecordHeader's
 * "actual" form
 */
class TriggerRecordHeader
{
public:
  /**
   * @brief Construct a TriggerRecordHeader using a vector of ComponentRequest objects
   * @param components Vector of ComponentRequests to copy into TriggerRecordHeader
   */
  explicit TriggerRecordHeader(std::vector<ComponentRequest> components)
  {
    size_t size = sizeof(TriggerRecordHeaderData) + components.size() * sizeof(ComponentRequest);

    data_arr_ = malloc(size); // NOLINT(build/unsigned)
    alloc_ = true;

    TriggerRecordHeaderData header;
    header.num_requested_components = components.size();
    memcpy(data_arr_, &header, sizeof(header));

    size_t offset = sizeof(header);
    for (auto const& component : components) {
      memcpy(static_cast<uint8_t*>(data_arr_) + offset, &component, sizeof(ComponentRequest)); // NOLINT
      offset += sizeof(ComponentRequest);
    }
  }

  /**
   * @brief Construct a TriggerRecordHeader using an existing TriggerRecordHeader data array
   * @param existing_trigger_record_header_buffer Pointer to existing TriggerRecordHeader array
   * @param copy_from_buffer Whether to create a copy of the exiting buffer (true) or use that memory without taking
   * ownership (false)
   */
  explicit TriggerRecordHeader(void* existing_trigger_record_header_buffer, bool copy_from_buffer = false)
  {
    if (!copy_from_buffer) {
      data_arr_ = existing_trigger_record_header_buffer;
    } else {
      auto header = reinterpret_cast<TriggerRecordHeaderData*>(existing_trigger_record_header_buffer); // NOLINT
      size_t size = header->num_requested_components * sizeof(ComponentRequest) + sizeof(TriggerRecordHeaderData);

      data_arr_ = malloc(size);
      alloc_ = true;
      memcpy(data_arr_, existing_trigger_record_header_buffer, size);
    }
  }

  /**
   * @brief TriggerRecordHeader Copy Constructor
   * @param other TriggerRecordHeader to copy
   */
  TriggerRecordHeader(TriggerRecordHeader const& other)
    : TriggerRecordHeader(other.data_arr_, true)
  {}
  /**
   * @brief TriggerRecordHeader copy assignment operator
   * @param other TriggerRecordHeader to copy
   * @return Reference to TriggerRecordHeader copy
   */
  TriggerRecordHeader& operator=(TriggerRecordHeader const& other)
  {
    if (&other == this)
      return *this;

    data_arr_ = malloc(other.get_total_size_bytes());
    alloc_ = true;
    memcpy(data_arr_, other.data_arr_, other.get_total_size_bytes());
    return *this;
  }

  TriggerRecordHeader(TriggerRecordHeader&&) = default;            ///< Default move constructor
  TriggerRecordHeader& operator=(TriggerRecordHeader&&) = default; ///< Default move assignment operator

  /**
   * @brief TriggerRecordHeader destructor
   */
  ~TriggerRecordHeader()
  {
    if (alloc_)
      free(data_arr_);
  }

  /**
   * @brief Get a copy of the TriggerRecordHeaderData struct
   * @return A copy of the TriggerRecordHeaderData struct stored in this TriggerRecordHeader
   */
  TriggerRecordHeaderData get_header() const { return *header_(); }

  /**
   * @brief Get the trigger number for this TriggerRecordHeader
   * @return The trigger_number TriggerRecordHeaderData field
   */
  trigger_number_t get_trigger_number() const { return header_()->trigger_number; }
  /**
   * @brief Set the trigger number for this TriggerRecordHeader
   * @param trigger_number Trigger nunmber to set
   */
  void set_trigger_number(trigger_number_t trigger_number) { header_()->trigger_number = trigger_number; }
  /**
   * @brief Get the trigger_timestamp stored in this TriggerRecordHeader
   * @return The trigger_timestamp TriggerRecordHeaderData field
   */
  timestamp_t get_trigger_timestamp() const { return header_()->trigger_timestamp; }
  /**
   * @brief Set the trigger timestamp for this TriggerRecordHeader
   * @param trigger_timestamp Trigger timestamp to set
   */
  void set_trigger_timestamp(timestamp_t trigger_timestamp) { header_()->trigger_timestamp = trigger_timestamp; }

  /**
   * @brief Get the number of ComponentRequest objects stored in this TriggerRecordHeader
   * @return The num_requested_components TriggerRecordHeaderData field
   */
  uint64_t get_num_requested_components() const // NOLINT(build/unsigned)
  {
    return header_()->num_requested_components;
  }

  /**
   * @brief Get the run_number stored in this TriggerRecordHeader
   * @return The run_number TriggerRecordHeaderData field
   */
  run_number_t get_run_number() const { return header_()->run_number; }
  /**
   * @brief Set the run number for this TriggerRecordHeader
   * @param run_number Run number to set
   */
  void set_run_number(run_number_t run_number) { header_()->run_number = run_number; }
  /**
   * @brief Get the error_bits header field as a bitset
   * @return bitset containing error_bits header field
   */
  std::bitset<32> get_error_bits() const { return header_()->error_bits; }
  /**
   * @brief Overwrite error bits using the given bitset
   * @param bits Bitset of error bits to set
   */
  void set_error_bits(std::bitset<32> bits) { header_()->error_bits = bits.to_ulong(); }
  /**
   * @brief Get the value of the given error bit
   * @param bit Bit to get
   * @return Value of bit (true/false)
   */
  bool get_error_bit(size_t bit) const { return get_error_bits()[bit]; }
  /**
   * @brief Set the given error bit to the given value
   * @param bit Bit to set
   * @param value Value to set (true/false)
   */
  void set_error_bit(size_t bit, bool value)
  {
    auto bits = get_error_bits();
    bits[bit] = value;
    set_error_bits(bits);
  }

  /**
   * @brief Get the trigger_type field from the data struct
   * @return The trigger_type field from the TriggerRecordHeaderData struct
   */
  trigger_type_t get_trigger_type() const { return header_()->trigger_type; }
  /**
   * @brief Set the trigger_type header field to the given value
   * @param trigger_type Value of trigger_type to set
   */
  void set_trigger_type(trigger_type_t trigger_type) { header_()->trigger_type = trigger_type; }

  /**
   * @brief Get the total size of the TriggerRecordHeader
   * @return The size of the TriggerRecordHeader, including header and all component requests
   */
  size_t get_total_size_bytes() const
  {
    return header_()->num_requested_components * sizeof(ComponentRequest) + sizeof(TriggerRecordHeaderData);
  }
  /**
   * @brief Get the location of the flat data array for I/O
   * @return Pointer to the TriggerRecordHeader data array
   */
  void* get_storage_location() const { return data_arr_; }

  /**
   * @brief Access ComponentRequest and copy result
   * @param idx Index to access
   * @return Copy of ComponentRequest at index
   * @throws std::out_of_range exception if idx is outside of allowable range
   */
  ComponentRequest at(size_t idx) const
  {
    if (idx >= header_()->num_requested_components) {
      /** @todo Eric Flumerfelt <eflumerf@fnal.gov>, 01/07/2021: Discuss whether throwing an ERS exception here would be
       * more correct, given that this code may be shared with the Offline
       */
      throw std::out_of_range(std::string("Supplied ComponentRequest index ") + std::to_string(idx) +
                              " is greater than the maximum index " +
                              std::to_string(header_()->num_requested_components - 1));
    }
    // Increment header pointer by one to skip header
    return *(reinterpret_cast<ComponentRequest*>(header_() + 1) + idx); // NOLINT
  }

  /**
   * @brief Operator[] to access ComponentRequests by index
   * @param idx Index to access
   * @return ComponentRequest reference
   * @throws std::out_of_range exception if idx is outside of allowable range
   */
  ComponentRequest& operator[](size_t idx)
  {
    if (idx >= header_()->num_requested_components) {
      /** @todo Eric Flumerfelt <eflumerf@fnal.gov>, 01/07/2021: Discuss whether throwing an ERS exception here would be
       * more correct, given that this code may be shared with the Offline
       */
      throw std::out_of_range(std::string("Supplied ComponentRequest index ") + std::to_string(idx) +
                              " is greater than the maximum index " +
                              std::to_string(header_()->num_requested_components - 1));
    }
    // Increment header pointer by one to skip header
    return *(reinterpret_cast<ComponentRequest*>(header_() + 1) + idx); // NOLINT
  }

private:
  /**
   * @brief Get the TriggerRecordHeaderData from the data_arr_ array
   * @return Pointer to the TriggerRecordHeaderData
   */
  TriggerRecordHeaderData* header_() const { return static_cast<TriggerRecordHeaderData*>(data_arr_); } // NOLINT
  void* data_arr_{
    nullptr
  };                    ///< Flat memory containing a TriggerRecordHeaderData header and an array of ComponentRequests
  bool alloc_{ false }; ///< Whether the TriggerRecordHeader owns the memory pointed by data_arr_
};

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORDHEADER_HPP_
