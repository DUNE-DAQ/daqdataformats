/**
 * @file TriggerRecord.hpp  TriggerRecord class definition
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TRIGGERRECORD_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TRIGGERRECORD_HPP_

#include "daqdataformats/Fragment.hpp"
#include "daqdataformats/TriggerRecordHeader.hpp"
#include "daqdataformats/Types.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace dunedaq::daqdataformats {

/**
 * @brief C++ Representation of a DUNE TriggerRecord, consisting of a TriggerRecordHeader object and a vector of
 * pointers to Fragment objects
 */
class TriggerRecord
{
public:
  /**
   * @brief Construct a TriggerRecord using the given vector of components to initialize the TriggerRecordHeader
   * @param components List of components requested for this TriggerRecord
   */
  explicit TriggerRecord(std::vector<ComponentRequest> const& components);

  /**
   * @brief Construct a TriggerRecord using the given TriggerRecordHeader
   * @param header TriggerRecordHeader to *copy* into the TriggerRecord
   */
  explicit TriggerRecord(TriggerRecordHeader const& header);

  /**
   * @brief Get a handle to the TriggerRecordHeader
   * @return A reference to the TriggerRecordHeader
   */
  const TriggerRecordHeader& get_header_ref() const { return m_header; }
  TriggerRecordHeader& get_header_ref() { return m_header; }

  TriggerRecordHeaderData get_header_data() const { return m_header.get_header(); }

  /**
   * @brief Get a handle to the Fragments
   * @return A reference to the Fragments vector
   */
  const std::vector<std::unique_ptr<Fragment>>& get_fragments_ref() const { return m_fragments; }
  std::vector<std::unique_ptr<Fragment>>& get_fragments_ref() { return m_fragments; }

  /**
   * @brief Move a unique_ptr owning a fragment to the Fragments vector
   * @param fragment The unique_ptr
   */
  void add_fragment(std::unique_ptr<Fragment>&& fragment) { m_fragments.emplace_back(std::move(fragment)); }

  /**
   * @brief Get size of trigger record from underlying TriggerRecordHeader and Fragments
   */
  size_t get_total_size_bytes() const
  {
    size_t total_size = get_header_ref().get_total_size_bytes();

    for (auto const& frag_ptr : m_fragments)
      total_size += frag_ptr->get_size();

    return total_size;
  }

  TriggerRecord(TriggerRecord const&) = delete;
  TriggerRecord& operator=(TriggerRecord const&) = delete;

  TriggerRecord(TriggerRecord&&) = default;
  TriggerRecord& operator=(TriggerRecord&&) = default;

  ~TriggerRecord() = default;

private:
  TriggerRecordHeader m_header;
  std::vector<std::unique_ptr<Fragment>> m_fragments;
};

inline TriggerRecord::TriggerRecord(std::vector<ComponentRequest> const& components)
  : m_header(components)
  , m_fragments()
{
}

inline TriggerRecord::TriggerRecord(TriggerRecordHeader const& header)
  : m_header(header)
  , m_fragments()
{
}

} // namespace dunedaq::daqdataformats

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_TRIGGERRECORD_HPP_
