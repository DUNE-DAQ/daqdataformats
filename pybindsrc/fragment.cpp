/**
 * @file wib.cpp
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
register_wib(py::module& m)
{

  py::class_<daqdataformats::Fragment>(m, "Fragment", py::buffer_protocol())
    // .def(py::init([](py::buffer b) {
    //     /* Request a buffer descriptor from Python */
    //     py::buffer_info info = b.request();
    //     return Fragment(info.ptr, Fragment::BufferAdoptionMode::kReadOnlyMode);
    // } ))
    .def(py::init([](py::buffer b) {
        /* Request a buffer descriptor from Python */
        py::buffer_info info = b.request();
        return *static_cast<Fragment*>(info.ptr);
    } ))
    .def("get_header", &Fragment::get_header)
    .def("get_storage_location", &Fragment::get_storage_location)
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
    .def("get_data", &Fragment::get_data)
  ;


}

} // namespace python
} // namespace timing
} // namespace dunedaq