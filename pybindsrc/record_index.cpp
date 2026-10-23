/**
 * @file record_index.cpp Python bindings for RecordID, RecordIndex, SourceIDSelector and SourceIDGeoIDMap
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "daqdataformats/RecordID.hpp"
#include "daqdataformats/RecordIndex.hpp"
#include "daqdataformats/SourceIDGeoIDMap.hpp"

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional>
#include <sstream>

namespace py = pybind11;

namespace dunedaq::daqdataformats::python {

void
register_record_index(py::module& m)
{
  py::class_<RecordID>(m, "RecordID")
    .def(py::init())
    .def(py::init<RecordID::number_t, sequence_number_t>(), py::arg("number"), py::arg("sequence") = 0)
    .def_readwrite("number", &RecordID::number)
    .def_readwrite("sequence", &RecordID::sequence)
    .def("to_string", &RecordID::to_string)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def("__hash__", [](const RecordID& rid) { return py::hash(py::make_tuple(rid.number, rid.sequence)); })
    .def("__repr__", [](const RecordID& rid) { return "<daqdataformats::RecordID " + rid.to_string() + ">"; });

  py::class_<FragmentInfo>(m, "FragmentInfo")
    .def(py::init())
    .def(py::init([](FragmentType fragment_type, detdataformats::DetID::Subdetector subdetector) {
           return FragmentInfo{ fragment_type, subdetector };
         }),
         py::arg("fragment_type"),
         py::arg("subdetector"))
    .def_readwrite("fragment_type", &FragmentInfo::fragment_type)
    .def_readwrite("subdetector", &FragmentInfo::subdetector)
    .def(py::self == py::self);

  py::class_<SourceIDSelector>(m, "SourceIDSelector")
    .def(py::init([](std::optional<SourceID::Subsystem> subsystem,
                     std::optional<FragmentType> fragment_type,
                     std::optional<detdataformats::DetID::Subdetector> subdetector) {
           return SourceIDSelector{ subsystem, fragment_type, subdetector };
         }),
         py::arg("subsystem") = py::none(),
         py::arg("fragment_type") = py::none(),
         py::arg("subdetector") = py::none())
    .def_readwrite("subsystem", &SourceIDSelector::subsystem)
    .def_readwrite("fragment_type", &SourceIDSelector::fragment_type)
    .def_readwrite("subdetector", &SourceIDSelector::subdetector)
    .def("matches", &SourceIDSelector::matches, py::arg("source_id"), py::arg("info"));

  py::class_<RecordIndex>(m, "RecordIndex")
    .def(py::init())
    .def(py::init<const RecordID&, const SourceID&>(), py::arg("record_id"), py::arg("header_source_id"))
    // "from" is a Python keyword, hence from_record
    .def_static("from_record", py::overload_cast<const TriggerRecord&>(&RecordIndex::from), py::arg("record"))
    .def_static("from_record", py::overload_cast<const TimeSlice&>(&RecordIndex::from), py::arg("record"))
    .def("add_fragment", &RecordIndex::add_fragment, py::arg("source_id"), py::arg("info"))
    .def("get_record_id", &RecordIndex::get_record_id)
    .def("get_header_source_id", &RecordIndex::get_header_source_id)
    .def("get_fragments", &RecordIndex::get_fragments)
    .def("contains_fragment", &RecordIndex::contains_fragment, py::arg("source_id"))
    .def("get_fragment_info", &RecordIndex::get_fragment_info, py::arg("source_id"))
    .def("get_fragment_source_ids", &RecordIndex::get_fragment_source_ids, py::arg("selector") = SourceIDSelector{})
    .def("get_source_ids_by_fragment_type", &RecordIndex::get_source_ids_by_fragment_type)
    .def("get_source_ids_by_subdetector", &RecordIndex::get_source_ids_by_subdetector)
    .def("get_source_ids_by_subsystem", &RecordIndex::get_source_ids_by_subsystem)
    .def(py::self == py::self);

  py::class_<SourceIDGeoIDMap>(m, "SourceIDGeoIDMap")
    .def(py::init())
    .def_static("from_encoded", &SourceIDGeoIDMap::from_encoded, py::arg("encoded_map"))
    .def("to_encoded", &SourceIDGeoIDMap::to_encoded)
    .def("add", &SourceIDGeoIDMap::add, py::arg("source_id"), py::arg("geo_id"))
    .def("empty", &SourceIDGeoIDMap::empty)
    .def("get_entries", &SourceIDGeoIDMap::get_entries)
    .def("get_geo_ids",
         py::overload_cast<const SourceID&>(&SourceIDGeoIDMap::get_geo_ids, py::const_),
         py::arg("source_id"))
    .def("get_geo_ids",
         py::overload_cast<detdataformats::DetID::Subdetector>(&SourceIDGeoIDMap::get_geo_ids, py::const_),
         py::arg("subdetector"))
    .def("get_source_id", &SourceIDGeoIDMap::get_source_id, py::arg("geo_id"))
    .def("get_all_geo_ids", &SourceIDGeoIDMap::get_all_geo_ids)
    .def("restricted_to", &SourceIDGeoIDMap::restricted_to, py::arg("record_index"))
    .def(py::self == py::self);
}

} // namespace dunedaq::daqdataformats::python
