cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0076 NEW)

enable_testing()

project("cpp-utils")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

set(LOG_LEVEL Info)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LOG_LEVEL Trace)
endif()

if(DEFINED CUSTOM_LOG_LEVEL)
    message("LOG_LEVEL set to '" ${CUSTOM_LOG_LEVEL} "'")
    set(LOG_LEVEL ${CUSTOM_LOG_LEVEL})
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Source/CppUtils/LogLevel.hpp.in ${CMAKE_BINARY_DIR}/generated/LogLevel.hpp)

include(Compiler)
include(TargetSetup)

add_subdirectory(Source)

option(BUILD_TESTS "BUILD_TESTS" OFF)

message("BUILD_TESTS: " ${BUILD_TESTS})

if (${BUILD_TESTS})
    add_subdirectory(Tests)
endif()
