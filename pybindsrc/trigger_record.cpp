/**
 * @file trigger_record.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdataformats/TriggerRecord.hpp"
#include "daqdataformats/TriggerRecordHeader.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq {
namespace daqdataformats {
namespace python {

void
register_trigger_record(py::module& m)
{
  py::class_<TriggerRecordHeader> (m, "TriggerRecordHeader", pybind11::buffer_protocol())
    .def(py::init<std::vector<ComponentRequest> const&>())
    .def(py::init([](py::capsule capsule, bool copy_from_buffer) {
        return std::unique_ptr<TriggerRecordHeader>(new TriggerRecordHeader(capsule.get_pointer(), copy_from_buffer));
    }))
    .def(py::init<TriggerRecordHeader const&>())
    .def("get_header", &TriggerRecordHeader::get_header)
    .def("get_trigger_number", &TriggerRecordHeader::get_trigger_number)
    //.def("set_trigger_number", &TriggerRecordHeader::set_trigger_number)
    .def("get_trigger_timestamp", &TriggerRecordHeader::get_trigger_timestamp)
    //.def("set_trigger_timestamp", &TriggerRecordHeader::set_trigger_timestamp)
    .def("get_num_requested_components", &TriggerRecordHeader::get_num_requested_components)
    .def("get_run_number", &TriggerRecordHeader::get_run_number)
    //.def("set_run_number", &TriggerRecordHeader::set_run_number)
    .def("get_error_bits", &TriggerRecordHeader::get_error_bits)
    //.def("set_error_bits", &TriggerRecordHeader::set_error_bits)
    .def("get_error_bit", &TriggerRecordHeader::get_error_bit)
    //.def("set_error_bit", &TriggerRecordHeader::set_error_bit)
    .def("get_trigger_type", &TriggerRecordHeader::get_trigger_type)
    //.def("set_trigger_type", &TriggerRecordHeader::set_trigger_type)
    .def("get_sequence_number", &TriggerRecordHeader::get_sequence_number)
    //.def("set_sequence_number", &TriggerRecordHeader::set_sequence_number)
    .def("get_max_sequence_number", &TriggerRecordHeader::get_max_sequence_number)
    //.def("set_max_sequence_number", &TriggerRecordHeader::set_max_sequence_number)
    .def("get_total_size_bytes", &TriggerRecordHeader::get_total_size_bytes)
    .def("get_storage_location", &TriggerRecordHeader::get_storage_location, py::return_value_policy::reference_internal)
    .def("at", &TriggerRecordHeader::at)
    .def("__getitem__", &TriggerRecordHeader::operator[], py::return_value_policy::reference_internal)
  ;

  py::class_<TriggerRecordHeaderData> (m, "TriggerRecordHeaderData")
    .def_readonly_static("s_trigger_record_header_magic", &TriggerRecordHeaderData::s_trigger_record_header_magic)
    .def_readonly_static("s_trigger_record_header_version", &TriggerRecordHeaderData::s_trigger_record_header_version)
    .def_readonly_static("s_invalid_number_components", &TriggerRecordHeaderData::s_invalid_number_components)
    .def_readonly_static("s_default_error_bits", &TriggerRecordHeaderData::s_default_error_bits)
    .def_readonly("trigger_record_header_marker", &TriggerRecordHeaderData::trigger_record_header_marker)
    .def_readonly("version", &TriggerRecordHeaderData::version)
    .def_readonly("trigger_number", &TriggerRecordHeaderData::trigger_number)
    .def_readonly("trigger_timestamp", &TriggerRecordHeaderData::trigger_timestamp)
    .def_readonly("num_requested_components", &TriggerRecordHeaderData::num_requested_components)
    .def_readonly("run_number", &TriggerRecordHeaderData::run_number)
    .def_readonly("error_bits", &TriggerRecordHeaderData::error_bits)
    .def_readonly("trigger_type", &TriggerRecordHeaderData::trigger_type)
    .def_readonly("sequence_number", &TriggerRecordHeaderData::sequence_number)
    .def_readonly("max_sequence_number", &TriggerRecordHeaderData::max_sequence_number)
    .def_readonly("unused", &TriggerRecordHeaderData::unused)
  ;

  py::enum_<TriggerRecordErrorBits>(m, "TriggerRecordErrorBits")
    .value("kIncomplete", TriggerRecordErrorBits::kIncomplete)
    .value("kMismatch", TriggerRecordErrorBits::kMismatch)
    .value("kUnassigned2", TriggerRecordErrorBits::kUnassigned2)
    .value("kUnassigned3", TriggerRecordErrorBits::kIncomplete)
    .value("kUnassigned4", TriggerRecordErrorBits::kUnassigned4)
    .value("kUnassigned5", TriggerRecordErrorBits::kUnassigned5)
    .value("kUnassigned6", TriggerRecordErrorBits::kUnassigned6)
    .value("kUnassigned7", TriggerRecordErrorBits::kUnassigned7)
    .value("kUnassigned8", TriggerRecordErrorBits::kUnassigned8)
    .value("kUnassigned9", TriggerRecordErrorBits::kUnassigned9)
    .value("kUnassigned10", TriggerRecordErrorBits::kUnassigned10)
    .value("kUnassigned11", TriggerRecordErrorBits::kUnassigned11)
    .value("kUnassigned12", TriggerRecordErrorBits::kUnassigned12)
    .value("kUnassigned13", TriggerRecordErrorBits::kUnassigned13)
    .value("kUnassigned14", TriggerRecordErrorBits::kUnassigned14)
    .value("kUnassigned15", TriggerRecordErrorBits::kUnassigned15)
    .value("kUnassigned16", TriggerRecordErrorBits::kUnassigned16)
    .value("kUnassigned17", TriggerRecordErrorBits::kUnassigned17)
    .value("kUnassigned18", TriggerRecordErrorBits::kUnassigned18)
    .value("kUnassigned19", TriggerRecordErrorBits::kUnassigned19)
    .value("kUnassigned20", TriggerRecordErrorBits::kUnassigned20)
    .value("kUnassigned21", TriggerRecordErrorBits::kUnassigned21)
    .value("kUnassigned22", TriggerRecordErrorBits::kUnassigned22)
    .value("kUnassigned23", TriggerRecordErrorBits::kUnassigned23)
    .value("kUnassigned24", TriggerRecordErrorBits::kUnassigned24)
    .value("kUnassigned25", TriggerRecordErrorBits::kUnassigned25)
    .value("kUnassigned26", TriggerRecordErrorBits::kUnassigned26)
    .value("kUnassigned27", TriggerRecordErrorBits::kUnassigned27)
    .value("kUnassigned28", TriggerRecordErrorBits::kUnassigned28)
    .value("kUnassigned29", TriggerRecordErrorBits::kUnassigned29)
    .value("kUnassigned30", TriggerRecordErrorBits::kUnassigned30)
    .value("kUnassigned31", TriggerRecordErrorBits::kUnassigned31)
    .value("kInvalid", TriggerRecordErrorBits::kInvalid)
  ;

  py::class_<TriggerRecord> py_trigger_record(m, "TriggerRecord", pybind11::buffer_protocol());
  py_trigger_record
    .def(py::init<TriggerRecordHeader const&>())
    .def(py::init<std::vector<ComponentRequest> const&>())
    .def("get_header_ref", &TriggerRecord::get_header_ref, py::return_value_policy::reference_internal)
    .def("set_header", &TriggerRecord::set_header)
    .def("get_header_data", &TriggerRecord::get_header_data)
    .def("get_fragments_ref", [](TriggerRecord& self)
    {
      auto fragments = py::list();
      for (auto& fragment : self.get_fragments_ref())
      {
        auto py_fragment = py::cast(*fragment, py::return_value_policy::reference);
        fragments.append(py_fragment);
      }
      return fragments;
    }, py::return_value_policy::reference_internal)
  ;

}

} // namespace python
} // namespace daqdataformats
} // namespace dunedaq