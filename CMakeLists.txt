cmake_minimum_required(VERSION 3.12)
project(daqdataformats VERSION 3.1.0)

find_package(daq-cmake REQUIRED)
find_package(Boost 1.70.0 COMPONENTS unit_test_framework REQUIRED)

daq_setup_environment()


##############################################################################
# Main library

daq_add_library(LINK_LIBRARIES )

##############################################################################
daq_add_python_bindings(*.cpp LINK_LIBRARIES ${PROJECT_NAME})

##############################################################################
# Plugins

##############################################################################
# Integration tests

##############################################################################
# Unit Tests
daq_add_unit_test(ComponentRequest_test        LINK_LIBRARIES ${PROJECT_NAME})
daq_add_unit_test(Fragment_test                LINK_LIBRARIES ${PROJECT_NAME})
daq_add_unit_test(FragmentHeader_test          LINK_LIBRARIES ${PROJECT_NAME})
daq_add_unit_test(GeoID_test                   LINK_LIBRARIES ${PROJECT_NAME})
daq_add_unit_test(TriggerRecord_test           LINK_LIBRARIES ${PROJECT_NAME})
daq_add_unit_test(TriggerRecordHeader_test     LINK_LIBRARIES ${PROJECT_NAME})
daq_add_unit_test(TriggerRecordHeaderData_test LINK_LIBRARIES ${PROJECT_NAME})

##############################################################################

daq_install()
