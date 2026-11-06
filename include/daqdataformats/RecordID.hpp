/**
 * @file RecordID.hpp Identification of a record (TriggerRecord or TimeSlice) within a data stream
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_RECORDID_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_RECORDID_HPP_

#include "daqdataformats/Types.hpp"

#include <compare>
#include <cstdint>
#include <ostream>
#include <string>

namespace dunedaq::daqdataformats {

/**
 * @brief RecordID identifies a record: a TriggerRecord (trigger number, sequence number)
 * or a TimeSlice (timeslice number, sequence number 0).
 */
struct RecordID
{
  using number_t = uint64_t; // NOLINT(build/unsigned) holds a trigger_number_t or a timeslice_number_t

  number_t number{ 0 };
  sequence_number_t sequence{ 0 };

  constexpr RecordID() = default;
  constexpr explicit RecordID(number_t number_arg, sequence_number_t sequence_arg = 0)
    : number(number_arg)
    , sequence(sequence_arg)
  {
  }

  constexpr auto operator<=>(const RecordID&) const = default;

  std::string to_string() const;
};

std::ostream&
operator<<(std::ostream& o, RecordID const& record_id);

} // namespace dunedaq::daqdataformats

#include "detail/RecordID.hxx"

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_RECORDID_HPP_
