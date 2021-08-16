cmake_minimum_required(VERSION 3.12)
project(dataformats VERSION 3.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

#find_package(cetlib REQUIRED)
find_package(ers REQUIRED)
find_package(Boost 1.70.0 COMPONENTS unit_test_framework REQUIRED)
find_package(logging REQUIRED)

set(RAWDATA_DEPENDENCIES ers::ers logging::logging)

##############################################################################
# Main library

daq_add_library(*.cpp LINK_LIBRARIES ${RAWDATA_DEPENDENCIES})

##############################################################################
# Plugins

#daq_add_plugin(... ... LINK_LIBRARIES daq-rawdata)

##############################################################################
# Integration tests



##############################################################################
# Unit Tests

daq_add_unit_test(ComponentRequest_test        LINK_LIBRARIES dataformats)
daq_add_unit_test(Fragment_test                LINK_LIBRARIES dataformats)
daq_add_unit_test(FragmentHeader_test          LINK_LIBRARIES dataformats)
daq_add_unit_test(GeoID_test                   LINK_LIBRARIES dataformats)
daq_add_unit_test(TriggerRecord_test           LINK_LIBRARIES dataformats)
daq_add_unit_test(TriggerRecordHeader_test     LINK_LIBRARIES dataformats)
daq_add_unit_test(TriggerRecordHeaderData_test LINK_LIBRARIES dataformats)
daq_add_unit_test(WIBFrame_test                LINK_LIBRARIES dataformats)
daq_add_unit_test(WIB2Frame_test                LINK_LIBRARIES dataformats)

##############################################################################

daq_install()
