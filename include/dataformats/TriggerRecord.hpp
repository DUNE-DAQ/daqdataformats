/**
 * @file TriggerRecord.hpp  TriggerRecord class definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORD_HPP_
#define DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORD_HPP_

#include "dataformats/Fragment.hpp"
#include "dataformats/TriggerRecordHeader.hpp"
#include "dataformats/Types.hpp"

#include <memory>
#include <vector>

namespace dunedaq {
namespace dataformats {

class TriggerRecord
{
public:
  TriggerRecord() = default;
  virtual ~TriggerRecord() = default;
  
  TriggerRecord(TriggerRecord const&) = delete;
  TriggerRecord(TriggerRecord&&) = default;
  TriggerRecord& operator=(TriggerRecord const&) = delete;
  TriggerRecord& operator=(TriggerRecord&&) = default;

  TriggerRecordHeader const& get_header() { return header_; }
  void set_header(TriggerRecordHeader header) { header_ = header; }
  std::vector<ComponentRequest> const& get_requested_components() { return requested_components_; }
  void set_requested_components(std::vector<ComponentRequest> components)
  {
    header_.num_requested_components = components.size();
    requested_components_ = components;
  }
  std::vector<std::unique_ptr<Fragment>>& get_fragments() { return fragments_; }
  void set_fragments(std::vector<std::unique_ptr<Fragment>>&& fragments) { fragments_ = std::move(fragments); }
  void add_fragment(std::unique_ptr<Fragment>&& fragment) { fragments_.emplace_back(std::move(fragment)); }

  trigger_number_t get_trigger_number() { return header_.trigger_number; }
  void set_trigger_number(trigger_number_t trigger_number) { header_.trigger_number = trigger_number; }
  run_number_t get_run_number() { return header_.run_number; }
  void set_run_number(run_number_t run_number) { header_.run_number = run_number; }
  timestamp_t get_trigger_timestamp() { return header_.trigger_timestamp; }
  void set_trigger_timestamp(timestamp_t trigger_timestamp) { header_.trigger_timestamp = trigger_timestamp; }
  trigger_type_t get_trigger_type() { return header_.trigger_type; }
  void set_trigger_type(trigger_type_t trigger_type) { header_.trigger_type = trigger_type; }

  std::bitset<32> get_error_bits() { return header_.error_bits; }
  void set_error_bits(std::bitset<32> bits) { header_.error_bits = bits.to_ulong(); }
  bool get_error_bit(size_t bit) { return get_error_bits()[bit]; }
  void set_error_bit(size_t bit, bool value)
  {
    auto bits = get_error_bits();
    bits[bit] = value;
    set_error_bits(bits);
  }

  uint64_t get_num_requested_components() { return header_.num_requested_components; } // NOLINT(build/unsigned)

private:
  TriggerRecordHeader header_;
  std::vector<ComponentRequest> requested_components_;
  std::vector<std::unique_ptr<Fragment>> fragments_;
};
} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORD_HPP_