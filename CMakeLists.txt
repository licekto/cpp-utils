cmake_minimum_required(VERSION 3.21)

project("cpp-utils")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

set(LOG_LEVEL Info)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LOG_LEVEL Trace)
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Source/CppUtils/LogLevel.hpp.in ${CMAKE_BINARY_DIR}/generated/LogLevel.hpp)

include(Compiler)
include(TargetSetup)

add_subdirectory(Source)
add_subdirectory(Tests)
