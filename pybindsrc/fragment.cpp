/**
 * @file fragment.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdataformats/Fragment.hpp"
#include "daqdataformats/FragmentHeader.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq {
namespace daqdataformats {
namespace python {

void
register_fragment(py::module& m)
{

  py::class_<Fragment> py_fragment(m, "Fragment", py::buffer_protocol());

  py_fragment
    .def("get_header", &Fragment::get_header, py::return_value_policy::reference_internal)
    .def("get_storage_location", &Fragment::get_storage_location, py::return_value_policy::reference_internal)
    .def("get_trigger_number", &Fragment::get_trigger_number)
    .def("get_run_number", &Fragment::get_run_number)
    .def("get_trigger_timestamp", &Fragment::get_trigger_timestamp)
    .def("get_window_begin", &Fragment::get_window_begin)
    .def("get_window_end", &Fragment::get_window_end)
    .def("get_element_id", &Fragment::get_element_id)
    .def("get_error_bits", &Fragment::get_error_bits)
    .def("get_error_bit", &Fragment::get_error_bit)
    .def("get_fragment_type_code", &Fragment::get_fragment_type_code)
    .def("get_fragment_type", &Fragment::get_fragment_type)
    .def("get_sequence_number", &Fragment::get_sequence_number)
    .def("get_size", &Fragment::get_size)
    .def("get_data", &Fragment::get_data, py::return_value_policy::reference_internal)
    .def("get_data", [](Fragment& self, size_t offset){ return self.get_data()+offset; }, py::return_value_policy::reference_internal)
  ;

  py::enum_<Fragment::BufferAdoptionMode>(py_fragment, "BufferAdoptionMode")
    .value("kReadOnlyMode", Fragment::BufferAdoptionMode::kReadOnlyMode)
    .value("kTakeOverBuffer", Fragment::BufferAdoptionMode::kTakeOverBuffer)
    .value("kCopyFromBuffer", Fragment::BufferAdoptionMode::kCopyFromBuffer)
    .export_values();

  py::class_<FragmentHeader>(m, "FragmentHeader")
    .def_readwrite("fragment_header_marker", &FragmentHeader::fragment_header_marker)
    .def_readwrite("version", &FragmentHeader::version)
    .def_readwrite("size", &FragmentHeader::size)
    .def_readwrite("trigger_number", &FragmentHeader::trigger_number)
    .def_readwrite("trigger_timestamp", &FragmentHeader::trigger_timestamp)
    .def_readwrite("window_begin", &FragmentHeader::window_begin)
    .def_readwrite("window_end", &FragmentHeader::window_end)
    .def_readwrite("run_number", &FragmentHeader::run_number)
    .def_readwrite("error_bits", &FragmentHeader::error_bits)
    .def_readwrite("fragment_type", &FragmentHeader::fragment_type)
    .def_readwrite("sequence_number", &FragmentHeader::sequence_number)
    .def_readwrite("element_id", &FragmentHeader::element_id)
    .def_static("sizeof", [](){ return sizeof(FragmentHeader); })
  ;

  py::enum_<FragmentErrorBits>(m, "FragmentErrorBits")
    .value("kDataNotFound", FragmentErrorBits::kDataNotFound)
    .value("kIncomplete", FragmentErrorBits::kIncomplete)
    .value("kInvalidWindow", FragmentErrorBits::kInvalidWindow)
    // TODO:  Add unassigned
    .export_values();

  py::enum_<FragmentType>(m, "FragmentType")
    .value("kFakeData", FragmentType::kFakeData)
    .value("kTPCData", FragmentType::kTPCData)
    .value("kPDSData", FragmentType::kPDSData)
    .value("kNDLArTPC", FragmentType::kNDLArTPC)
    .value("kTriggerPrimitives", FragmentType::kTriggerPrimitives)
    .value("kTriggerActivities", FragmentType::kTriggerActivities)
    .value("kTriggerCandidates", FragmentType::kTriggerCandidates)
    .value("kUnknown", FragmentType::kUnknown)
    .export_values();

}

} // namespace python
} // namespace daqdataformats
} // namespace dunedaq