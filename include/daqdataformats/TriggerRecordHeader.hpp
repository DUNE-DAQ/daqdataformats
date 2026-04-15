/**
 * @file TriggerRecordHeader.hpp  TriggerRecordHeader struct definition
 *
 * Conceptually, this is actually a header containing metadata about the trigger 
 * record followed by a collection of ComponentRequest instances
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TRIGGERRECORDHEADER_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TRIGGERRECORDHEADER_HPP_

#include "daqdataformats/ComponentRequest.hpp"
#include "daqdataformats/SourceID.hpp"
#include "daqdataformats/TriggerRecordHeaderData.hpp"
#include "daqdataformats/Types.hpp"

#include <bitset>
#include <cstddef>
#include <cstring>
#include <format>
#include <new>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace dunedaq::daqdataformats {

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
  explicit TriggerRecordHeader(const std::vector<ComponentRequest>& components);

  /**
   * @brief Construct a TriggerRecordHeader using an existing TriggerRecordHeader data array
   * @param existing_trigger_record_header_buffer Pointer to existing TriggerRecordHeader array
   * @param copy_from_buffer Whether to create a copy of the exiting buffer (true) or use that memory without taking
   * ownership (false)
   */
  explicit TriggerRecordHeader(void* existing_trigger_record_header_buffer, bool copy_from_buffer = false);

  TriggerRecordHeaderData get_header() const { return *header_(); }

  trigger_number_t get_trigger_number() const { return header_()->trigger_number; }
  void set_trigger_number(trigger_number_t trigger_number) { header_()->trigger_number = trigger_number; }

  timestamp_t get_trigger_timestamp() const { return header_()->trigger_timestamp; }
  void set_trigger_timestamp(timestamp_t trigger_timestamp) { header_()->trigger_timestamp = trigger_timestamp; }

  uint64_t get_num_requested_components() const // NOLINT(build/unsigned)
  {
    return header_()->num_requested_components;
  }

  run_number_t get_run_number() const { return header_()->run_number; }
  void set_run_number(run_number_t run_number) { header_()->run_number = run_number; }

  std::bitset<32> get_status_bits() const { return header_()->status_bits; }
  void set_status_bits(std::bitset<32> bits) { header_()->status_bits = bits.to_ulong(); }

  /**
   * @brief Get the value of the given status bit
   * @param bit Bit to get
   * @return Value of bit (true/false)
   */
  bool get_status_bit(TriggerRecordStatusBits bit) const { return get_status_bits()[static_cast<size_t>(bit)]; }
  /**
   * @brief Set the given status bit to the given value
   * @param bit Bit to set
   * @param value Value to set (true/false)
   */
  void set_status_bit(TriggerRecordStatusBits bit, bool value)
  {
    auto bits = get_status_bits();
    bits[static_cast<size_t>(bit)] = value;
    set_status_bits(bits);
  }

  trigger_type_t get_trigger_type() const { return header_()->trigger_type; }
  void set_trigger_type(trigger_type_t trigger_type) { header_()->trigger_type = trigger_type; }

  trigger_number_t get_sequence_number() const { return header_()->sequence_number; }
  void set_sequence_number(sequence_number_t number) { header_()->sequence_number = number; }

  trigger_number_t get_max_sequence_number() const { return header_()->max_sequence_number; }
  void set_max_sequence_number(sequence_number_t number) { header_()->max_sequence_number = number; }

  /**
   * @brief Get the SourceID for this TriggerRecordHeader
   * @return The element_id TriggerRecordHeaderData field
   */
  SourceID get_element_id() const { return header_()->element_id; }
  /**
   * @brief Set the SourceID for this TriggerRecordHeader
   * @param source_id SourceID value to set
   */
  void set_element_id(SourceID source_id) { header_()->element_id = source_id; }

  /// @brief Get the total size of the TriggerRecordHeader, including header and all component requests
  size_t get_total_size_bytes() const
  {
    return header_()->num_requested_components * sizeof(ComponentRequest) + sizeof(TriggerRecordHeaderData);
  }

  /// @brief Get read-only access to the underlying flat data array
  const void* get_storage_location() const { return m_data_arr; }

  /**
   * @brief Access ComponentRequest and copy result
   * @param idx Index to access
   * @return Copy of ComponentRequest at index
   * @throws std::range_error exception if idx is outside of allowable range
   */
  ComponentRequest at(size_t idx) const;

  /**
   * @brief Access ComponentRequest by SourceID
   * @param source_id SourceID to access
   * @return ComponentRequest constant reference
   * @throws std::invalid_argument exception if source_id is not in ComponentRequest list
   */
  ComponentRequest const& get_component_for_source_id(SourceID const& source_id) const;


  TriggerRecordHeader(TriggerRecordHeader const& other);
  TriggerRecordHeader& operator=(TriggerRecordHeader const& other);

  TriggerRecordHeader(TriggerRecordHeader&& other)
  {
    m_alloc = other.m_alloc;
    other.m_alloc = false;
    m_data_arr = other.m_data_arr;
  }
  TriggerRecordHeader& operator=(TriggerRecordHeader&& other)
  {
    m_alloc = other.m_alloc;
    other.m_alloc = false;
    m_data_arr = other.m_data_arr;
    return *this;
  }

  ~TriggerRecordHeader()
  {
    if (m_alloc)
      free(m_data_arr); // NOLINT
  }

private:
  /**
   * @brief Get the TriggerRecordHeaderData from the m_data_arr array
   * @return Pointer to the TriggerRecordHeaderData
   */
  TriggerRecordHeaderData* header_() const { return static_cast<TriggerRecordHeaderData*>(m_data_arr); }

  void* m_data_arr{
    nullptr
  }; ///< Flat memory containing a TriggerRecordHeaderData header and an array of ComponentRequests
  bool m_alloc{ false }; ///< Whether the TriggerRecordHeader owns the memory pointed by m_data_arr
};

//------

inline TriggerRecordHeader::TriggerRecordHeader(const std::vector<ComponentRequest>& components)
{
  size_t size = sizeof(TriggerRecordHeaderData) + components.size() * sizeof(ComponentRequest);

  m_data_arr = malloc(size); // NOLINT
  if (m_data_arr == nullptr) {
    throw std::bad_alloc();
  }
  m_alloc = true;

  TriggerRecordHeaderData header;
  header.num_requested_components = components.size();
  std::memcpy(m_data_arr, &header, sizeof(header));
  std::memcpy(static_cast<uint8_t*>(m_data_arr) + sizeof(header), components.data(), sizeof(ComponentRequest)*components.size()); // NOLINT
}

inline TriggerRecordHeader::TriggerRecordHeader(void* existing_trigger_record_header_buffer, bool copy_from_buffer)
{
  if (!copy_from_buffer) {
    m_data_arr = existing_trigger_record_header_buffer;
  } else {
    auto header = reinterpret_cast<TriggerRecordHeaderData*>(existing_trigger_record_header_buffer); // NOLINT
    size_t size = header->num_requested_components * sizeof(ComponentRequest) + sizeof(TriggerRecordHeaderData);

    m_data_arr = malloc(size); // NOLINT
    if (m_data_arr == nullptr) {
      throw std::bad_alloc();
    }
    m_alloc = true;
    std::memcpy(m_data_arr, existing_trigger_record_header_buffer, size);
  }
}

inline TriggerRecordHeader::TriggerRecordHeader(TriggerRecordHeader const& other)
  : TriggerRecordHeader(other.m_data_arr, true)
{
}

inline TriggerRecordHeader&
TriggerRecordHeader::operator=(TriggerRecordHeader const& other)
{
  if (&other == this)
    return *this;

  if (m_alloc) {
    free(m_data_arr); // NOLINT
  }
  m_data_arr = malloc(other.get_total_size_bytes()); // NOLINT
  if (m_data_arr == nullptr) {
    throw std::bad_alloc();
  }
  m_alloc = true;
  std::memcpy(m_data_arr, other.m_data_arr, other.get_total_size_bytes());
  return *this;
}

inline ComponentRequest
TriggerRecordHeader::at(size_t idx) const
{
  if (idx >= header_()->num_requested_components) {
    throw std::range_error(
			   std::format("Supplied ComponentRequest index {} out of range (size: {})",
				       idx, header_()->num_requested_components));
  }

  // Increment header pointer by one to skip header
  return *(reinterpret_cast<ComponentRequest*>(header_() + 1) + idx); // NOLINT
}

inline ComponentRequest const&
TriggerRecordHeader::get_component_for_source_id(SourceID const& source_id) const
{
  for (uint64_t idx = 0; idx < get_num_requested_components(); ++idx) { // NOLINT(build/unsigned)
    ComponentRequest const& component_obj = *(reinterpret_cast<ComponentRequest*>(header_() + 1) + idx); // NOLINT
    if (source_id == component_obj.component) {
      return component_obj;
    }
  }
  throw std::invalid_argument("Supplied SourceID (" + source_id.to_string() +
                              ") was not found in the ComponentRequest list.");
}

} // namespace dunedaq::daqdataformats

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TRIGGERRECORDHEADER_HPP_
