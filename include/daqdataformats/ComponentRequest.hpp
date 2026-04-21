/**
 * @file ComponentRequest.hpp Represents the requested timestamp range for a single component
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_COMPONENTREQUEST_HPP_
#define DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_COMPONENTREQUEST_HPP_

#include "daqdataformats/SourceID.hpp"
#include "daqdataformats/Types.hpp"

#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>

namespace dunedaq::daqdataformats {

/**
 * @brief A request sent to a Component, including the SourceID of the component and the window offset and width
 */
struct ComponentRequest
{
  static constexpr uint32_t s_component_request_version = 2; // NOLINT(build/unsigned)

  uint32_t version{ s_component_request_version }; // NOLINT(build/unsigned)
  uint32_t unused{ 0xFFFFFFFF };                   ///< Padding to ensure 64b alignment // NOLINT(build/unsigned)

  SourceID component; ///< The ID of the Requested Component

  /// @brief Start of the data collection window
  timestamp_t window_begin{ TypeDefaults::s_invalid_timestamp };

  /// @brief End of the data collection window
  timestamp_t window_end{ TypeDefaults::s_invalid_timestamp };

  ComponentRequest() = default;
  ComponentRequest(SourceID const& comp, timestamp_t const& wbegin, timestamp_t const& wend);
};

std::ostream&
operator<<(std::ostream& o, ComponentRequest const& cr);

inline ComponentRequest::ComponentRequest(SourceID const& comp, timestamp_t const& wbegin, timestamp_t const& wend)
  : component(comp)
  , window_begin(wbegin)
  , window_end(wend)
{
}

} // namespace dunedaq::daqdataformats

#include "detail/ComponentRequest.hxx"

#endif // DAQDATAFORMATS_INCLUDE_DAQDATAFORMATS_COMPONENTREQUEST_HPP_
