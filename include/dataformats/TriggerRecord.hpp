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
#include <utility>
#include <vector>

namespace dunedaq {
namespace dataformats {

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
  explicit TriggerRecord(std::vector<ComponentRequest> components)
    : m_header_(components)
    , m_fragments_()
  {}

  /**
   * @brief Construct a TriggerRecord using the given TriggerRecordHeader
   * @param header TriggerRecordHeader to *copy* into the TriggerRecord
   */
  explicit TriggerRecord(TriggerRecordHeader const& header)
    : m_header_(header)
    , m_fragments_()
  {}
  virtual ~TriggerRecord() = default; ///< TriggerRecord default destructor

  TriggerRecord(TriggerRecord const&) = delete;            ///< TriggerRecords are not copy-constructible
  TriggerRecord(TriggerRecord&&) = default;                ///< Default TriggerRecord move constructor
  TriggerRecord& operator=(TriggerRecord const&) = delete; ///< TriggerRecords are not copy-assignable
  TriggerRecord& operator=(TriggerRecord&&) = default;     ///< Default TriggerRecord move assignment operator

  /**
   * @brief Get a handle to the TriggerRecordHeader
   * @return A reference to the TriggerRecordHeader
   */
  TriggerRecordHeader& header_ref() { return m_header_; }
  /**
   * @brief Set the TriggerRecordHeader to the given TriggerRecordHeader object
   * @param header new TriggerRecordHeader to use
   */
  void set_header(TriggerRecordHeader header) { m_header_ = header; }
  /**
   * @brief Get a copy of the TriggerRecordHeaderData from the TriggerRecordHeader
   * @return Copy of the TriggerRecordHeaderData struct from the TriggerRecordHeader
   */
  TriggerRecordHeaderData get_header_data() const { return m_header_.get_header(); }

  /**
   * @brief Get a handle to the Fragments
   * @return A reference to the Fragments vector
   */
  std::vector<std::unique_ptr<Fragment>>& fragments_ref() { return m_fragments_; }
  /**
   * @brief Set the Fragments vector to the given vector of Fragments
   * @param fragments Fragments vector to use
   */
  void set_fragments(std::vector<std::unique_ptr<Fragment>>&& fragments) { m_fragments_ = std::move(fragments); }
  /**
   * @brief Add a Fragment pointer to the Fragments vector
   * @param fragment Fragment to add
   */
  void add_fragment(std::unique_ptr<Fragment>&& fragment) { m_fragments_.emplace_back(std::move(fragment)); }

private:
  TriggerRecordHeader m_header_;                       ///< TriggerRecordHeader object
  std::vector<std::unique_ptr<Fragment>> m_fragments_; ///< Vector of unique_ptrs to Fragment objects
};
} // namespace dataformats
} // namespace dunedaq

#endif // DATAFORMATS_INCLUDE_DATAFORMATS_TRIGGERRECORD_HPP_