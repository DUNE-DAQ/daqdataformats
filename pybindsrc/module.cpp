/**
 * @file timing_wrapper.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace dunedaq {
namespace daqdataformats {
namespace python {

    extern void register_fragment(py::module &);

PYBIND11_MODULE(_daq_daqdataformats_py, top_module) {

    top_module.doc() = "c++ implementation of the dunedaq daqdataformats modules"; // optional module docstring

    register_fragment(top_module);
}

} // namespace python
} // namespace daqdataformats
} // namespace dunedaq