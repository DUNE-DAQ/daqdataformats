cmake_minimum_required(VERSION 3.12)
project(dataformats VERSION 0.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

#find_package(cetlib REQUIRED)
#find_package(ers REQUIRED)
find_package(Boost 1.70.0 COMPONENTS unit_test_framework REQUIRED)

#set(RAWDATA_DEPENDENCIES ${CETLIB} ${CETLIB_EXCEPT} ers::ers cmdlib::cmdlib nlohmann_json::nlohmann_json pistache_shared)

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

daq_add_unit_test(Fragment_test)
daq_add_unit_test(TriggerRecord_test)

##############################################################################

daq_install()
