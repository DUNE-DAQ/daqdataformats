/**
 * @file Fragment.hpp Data Fragment Interface
 *
 * This class respresents the data response of one link to a Dataflow DataRequest message.
 * This version is not serializable, and classes wanting to stream it/write it to disk must
 * also fetch the data associated with the Fragment.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef CDF_INCLUDE_CDF_FRAGMENT_HPP_
#define CDF_INCLUDE_CDF_FRAGMENT_HPP_

#include "cdf/FragmentHeader.hpp"
#include "cdf/GeoID.hpp"
#include "cdf/Types.hpp"

#include <bitset>
#include <cstdlib>
#include <cstring>
#include <numeric>
#include <utility>
#include <vector>

namespace dunedaq {
namespace cdf {

class Fragment
{
public:
  explicit Fragment(std::vector<std::pair<void*, size_t>> pieces)
  {
    size_ = std::accumulate(pieces.begin(), pieces.end(), 0ULL, [](auto& a, auto& b) { return a + b.second; });
    data_ = malloc(size_);

    size_t offset = 0;
    for (auto& piece : pieces) {
      memcpy(static_cast<uint8_t*>(data_) + offset, piece.first, piece.second); // NOLINT(build/unsigned)
      offset += piece.second;
    }
  }
  Fragment(void* buffer, size_t size)
    : Fragment({ std::make_pair(buffer, size) })
  {}

  FragmentHeader const& GetHeader() { return header_; }
  void SetHeader(FragmentHeader header) { header_ = header; }

  // Header setters and getters
  trigger_number_t GetTriggerNumber() { return header_.TriggerNumber; }
  void SetTriggerNumber(trigger_number_t triggerNumber) { header_.TriggerNumber = triggerNumber; }
  run_number_t GetRunNumber() { return header_.RunNumber; }
  void SetRunNumber(run_number_t runNumber) { header_.RunNumber = runNumber; }

  timestamp_t GetTriggerTimestamp() { return header_.TriggerTimestamp; }
  void SetTriggerTimestamp(timestamp_t triggerTimestamp) { header_.TriggerTimestamp = triggerTimestamp; }
  timestamp_t GetDataStart() { return header_.DataStart; }
  void SetDataStart(timestamp_t dataStart) { header_.DataStart = dataStart; }
  timestamp_t GetDataEnd() { return header_.DataEnd; }
  void SetDataEnd(timestamp_t dataEnd) { header_.DataEnd = dataEnd; }

  GeoID GetLinkID() { return header_.LinkID; }
  void SetLinkID(GeoID linkID) { header_.LinkID = linkID; }
  std::bitset<32> GetErrorBits() { return header_.ErrorBits; }
  void SetErrorBits(std::bitset<32> errorBits) { header_.ErrorBits = errorBits; }
  bool GetErrorBit(size_t bit) { return header_.ErrorBits[bit]; }
  void SetErrorBit(size_t bit, bool value) { header_.ErrorBits[bit] = value; }
  type_t GetType() { return header_.Type; }
  void SetType(type_t type) { header_.Type = type; }

  size_t size() { return size_; }
  void* data() { return data_; }

private:
  FragmentHeader header_;

  void* data_;
  size_t size_;
};
} // namespace cdf
} // namespace dunedaq

#endif // CDF_INCLUDE_CDF_FRAGMENT_HPP_