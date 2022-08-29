/**
 * @file geoid.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdataformats/GeoID.hpp"

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <sstream>

namespace py = pybind11;

namespace dunedaq {
namespace daqdataformats {
namespace python {

void
register_geoid(py::module& m)
{

  py::class_<GeoID> py_geoid(m, "GeoID");
  py_geoid.def(py::self < py::self).def("__repr__", [](const GeoID& gid) {
    std::ostringstream oss;
    oss << "<daqdataformats::GeoID " << gid << ">";
    return oss.str();
  });

  py::enum_<GeoID::SystemType>(py_geoid, "SystemType")
    .value("kTPC", GeoID::SystemType::kTPC)
    .value("kPDS", GeoID::SystemType::kPDS)
    .value("kDataSelection", GeoID::SystemType::kDataSelection)
    .value("kNDLArTPC", GeoID::SystemType::kNDLArTPC)
    .value("kHSI", GeoID::SystemType::kHSI)
    .value("kInvalid", GeoID::SystemType::kInvalid)
    .export_values();

  py_geoid.def_readwrite("version", &GeoID::version)
    .def_readwrite("system_type", &GeoID::system_type)
    .def_readwrite("region_id", &GeoID::region_id)
    .def_readwrite("element_id", &GeoID::element_id);
}

} // namespace python
} // namespace daqdataformats
} // namespace dunedaq
