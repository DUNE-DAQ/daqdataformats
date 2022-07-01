/**
 * @file sourceid.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdataformats/SourceID.hpp"

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <sstream>

namespace py = pybind11;

namespace dunedaq {
namespace daqdataformats {
namespace python {

void
register_sourceid(py::module& m)
{

  py::class_<SourceID> py_sourceid(m, "SourceID");
  py_sourceid.def(py::self < py::self).def("__repr__", [](const SourceID& gid) {
    std::ostringstream oss;
    oss << "<daqdataformats::SourceID " << gid << ">";
    return oss.str();
  });

  py::enum_<SourceID::SystemType>(py_sourceid, "SystemType")
    .value("kTPC", SourceID::SystemType::kTPC)
    .value("kPDS", SourceID::SystemType::kPDS)
    .value("kDataSelection", SourceID::SystemType::kDataSelection)
    .value("kNDLArTPC", SourceID::SystemType::kNDLArTPC)
    .value("kInvalid", SourceID::SystemType::kInvalid)
    .export_values();

  py_sourceid.def_readwrite("version", &SourceID::version)
    .def_readwrite("system_type", &SourceID::system_type)
    .def_readwrite("region_id", &SourceID::region_id)
    .def_readwrite("element_id", &SourceID::element_id);
}

} // namespace python
} // namespace daqdataformats
} // namespace dunedaq
