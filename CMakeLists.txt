cmake_minimum_required(VERSION 3.12)
project(cdf VERSION 0.0.0)

find_package(daq-cmake 1.1.0 REQUIRED)


daq_setup_environment()

find_package(Boost 1.70.0 COMPONENTS unit_test_framework REQUIRED)

daq_add_unit_test(Fragment_test)
daq_add_unit_test(TriggerRecordHeader_test)

daq_install()
