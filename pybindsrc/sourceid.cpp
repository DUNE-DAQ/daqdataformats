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

namespace dunedaq::daqdataformats::python {

void
register_sourceid(py::module& m)
{

  py::class_<SourceID> py_sourceid(m, "SourceID");
  py_sourceid.def(py::self < py::self).def("__repr__", [](const SourceID& gid) {
    std::ostringstream oss;
    oss << "<daqdataformats::SourceID " << gid << ">";
    return oss.str();
  });

  py::enum_<SourceID::Category>(py_sourceid, "Category")
    .value("kInvalid", SourceID::Category::kInvalid)
    .value("kTPC", SourceID::Category::kTPC)
    .value("kPDS", SourceID::Category::kPDS)
    .value("kDataSelection", SourceID::Category::kDataSelection)
    .value("kNDLArTPC", SourceID::Category::kNDLArTPC)
    .value("kTriggerPrimitive", SourceID::Category::kTriggerPrimitive)
    .value("kTriggerActivity", SourceID::Category::kTriggerActivity)
    .value("kTriggerCandidate", SourceID::Category::kTriggerCandidate)
    .export_values();

  py_sourceid.def_readwrite("version", &SourceID::version)
    .def_readwrite("category", &SourceID::category)
    .def_readwrite("id", &SourceID::id);
}

} // namespace dunedaq::daqdataformats::python
