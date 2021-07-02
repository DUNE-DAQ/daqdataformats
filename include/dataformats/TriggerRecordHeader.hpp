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

#include "ers/Issue.hpp"

#include <bitset>
#include <ostream>
#include <string>
#include <vector>

namespace dunedaq {

/**
 * @brief An ERS Error indicating that the requested index is out of range
 * @param cri_index_supplied Index that caused the error
 * @param cri_index_max Maximum valid index for this function
 * @cond Doxygen doesn't like ERS macros LCOV_EXCL_START
 */
ERS_DECLARE_ISSUE(dataformats,
                  ComponentRequestIndexError,
                  "Supplied ComponentRequest index " << cri_index_supplied << " is greater than the maximum index "
                                                     << cri_index_max,
                  ((int)cri_index_supplied)((int)cri_index_max)) // NOLINT
                                                                 /// @endcond LCOV_EXCL_STOP

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
  explicit TriggerRecordHeader(const std::vector<ComponentRequest>& components)
  {
    size_t size = sizeof(TriggerRecordHeaderData) + components.size() * sizeof(ComponentRequest);

    m_data_arr = malloc(size); // NOLINT(build/unsigned)
    if (m_data_arr == nullptr) {
      throw MemoryAllocationFailed(ERS_HERE, size);
    }
    m_alloc = true;

    TriggerRecordHeaderData header;
    header.num_requested_components = components.size();
    memcpy(m_data_arr, &header, sizeof(header));

    size_t offset = sizeof(header);
    for (auto const& component : components) {
      memcpy(static_cast<uint8_t*>(m_data_arr) + offset, &component, sizeof(component)); // NOLINT
      offset += sizeof(component);
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
      m_data_arr = existing_trigger_record_header_buffer;
    } else {
      auto header = reinterpret_cast<TriggerRecordHeaderData*>(existing_trigger_record_header_buffer); // NOLINT
      size_t size = header->num_requested_components * sizeof(ComponentRequest) + sizeof(TriggerRecordHeaderData);

      m_data_arr = malloc(size);
      if (m_data_arr == nullptr) {
        throw MemoryAllocationFailed(ERS_HERE, size);
      }
      m_alloc = true;
      memcpy(m_data_arr, existing_trigger_record_header_buffer, size);
    }
  }

  /**
   * @brief TriggerRecordHeader Copy Constructor
   * @param other TriggerRecordHeader to copy
   */
  TriggerRecordHeader(TriggerRecordHeader const& other)
    : TriggerRecordHeader(other.m_data_arr, true)
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

    if (m_alloc) {
      free(m_data_arr);
    }
    m_data_arr = malloc(other.get_total_size_bytes());
    if (m_data_arr == nullptr) {
      throw MemoryAllocationFailed(ERS_HERE, other.get_total_size_bytes());
    }
    m_alloc = true;
    memcpy(m_data_arr, other.m_data_arr, other.get_total_size_bytes());
    return *this;
  }

  TriggerRecordHeader(TriggerRecordHeader&&) = default;            ///< Default move constructor
  TriggerRecordHeader& operator=(TriggerRecordHeader&&) = default; ///< Default move assignment operator

  /**
   * @brief TriggerRecordHeader destructor
   */
  ~TriggerRecordHeader()
  {
    if (m_alloc)
      free(m_data_arr);
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
  bool get_error_bit(TriggerRecordErrorBits bit) const { return get_error_bits()[static_cast<size_t>(bit)]; }
  /**
   * @brief Set the given error bit to the given value
   * @param bit Bit to set
   * @param value Value to set (true/false)
   */
  void set_error_bit(TriggerRecordErrorBits bit, bool value)
  {
    auto bits = get_error_bits();
    bits[static_cast<size_t>(bit)] = value;
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
   * @brief Get the sequence number for this TriggerRecordHeader
   * @return The sequence_number TriggerRecordHeaderData field
   */
  trigger_number_t get_sequence_number() const { return header_()->sequence_number; }
  /**
   * @brief Set the sequence number for this TriggerRecordHeader
   * @param sequence_number Sequence number to set
   */
  void set_sequence_number(sequence_number_t number) { header_()->sequence_number = number; }

  /**
   * @brief Get the maximum sequence number for this TriggerRecordHeader
   * @return The max_sequence_number TriggerRecordHeaderData field
   */
  trigger_number_t get_max_sequence_number() const { return header_()->max_sequence_number; }
  /**
   * @brief Set the maxiumum sequence number for this TriggerRecordHeader
   * @param max_sequence_number Maximum sequence number to set
   */
  void set_max_sequence_number(sequence_number_t number) { header_()->max_sequence_number = number; }

  /**
   * @brief Get the total size of the TriggerRecordHeader
   * @return The size of the TriggerRecordHeader, including header and all component requests
   */
  size_t get_total_size_bytes() const
  {
    return header_()->num_requested_components * sizeof(ComponentRequest) + sizeof(TriggerRecordHeaderData);
  }
  /**
   * @brief Get the location of the flat data array for output
   * @return Pointer to the TriggerRecordHeader data array
   */

  const void* get_storage_location() const { return m_data_arr; }

  /**
   * @brief Access ComponentRequest and copy result
   * @param idx Index to access
   * @return Copy of ComponentRequest at index
   * @throws ComponentRequestIndexError exception if idx is outside of allowable range
   */
  ComponentRequest at(size_t idx) const
  {
    if (idx >= header_()->num_requested_components) {
      throw ComponentRequestIndexError(ERS_HERE, idx, header_()->num_requested_components - 1);
    }
    // Increment header pointer by one to skip header
    return *(reinterpret_cast<ComponentRequest*>(header_() + 1) + idx); // NOLINT
  }

  /**
   * @brief Operator[] to access ComponentRequests by index
   * @param idx Index to access
   * @return ComponentRequest reference
   * @throws ComponentRequestIndexError exception if idx is outside of allowable range
   */
  ComponentRequest& operator[](size_t idx)
  {
    if (idx >= header_()->num_requested_components) {
      throw ComponentRequestIndexError(ERS_HERE, idx, header_()->num_requested_components - 1);
    }
    // Increment header pointer by one to skip header
    return *(reinterpret_cast<ComponentRequest*>(header_() + 1) + idx); // NOLINT
  }

private:
  /**
   * @brief Get the TriggerRecordHeaderData from the m_data_arr array
   * @return Pointer to the TriggerRecordHeaderData
   */
  TriggerRecordHeaderData* header_() const { return static_cast<TriggerRecordHeaderData*>(m_data_arr); }

  void* m_data_arr{
    nullptr
  };                     ///< Flat memory containing a TriggerRecordHeaderData header and an array of ComponentRequests
  bool m_alloc{ false }; ///< Whether the TriggerRecordHeader owns the memory pointed by m_data_arr
};

} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORDHEADER_HPP_
