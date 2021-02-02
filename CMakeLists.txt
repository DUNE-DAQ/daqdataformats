cmake_minimum_required(VERSION 3.12)
project(dataformats VERSION 1.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

#find_package(cetlib REQUIRED)
find_package(ers REQUIRED)
find_package(Boost 1.70.0 COMPONENTS unit_test_framework REQUIRED)
find_package(serialization REQUIRED)

set(RAWDATA_DEPENDENCIES ers::ers serialization::serialization)

##############################################################################
# Main library

daq_add_library(*.cpp LINK_LIBRARIES ${RAWDATA_DEPENDENCIES})

##############################################################################
# Plugins

#daq_add_plugin(... ... LINK_LIBRARIES daq-rawdata)

##############################################################################
# Integration tests

#daq_add_application(test_raw_app test_raw_app.cxx TEST LINK_LIBRARIES daq-rawdata)

##############################################################################
# Unit Tests

daq_add_unit_test(Fragment_test       LINK_LIBRARIES dataformats)
daq_add_unit_test(TriggerRecord_test  LINK_LIBRARIES dataformats)

##############################################################################

daq_install()
