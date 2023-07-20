cmake_minimum_required(VERSION 3.21)

project("cpp-utils")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

include(Compiler)
include(TargetSetup)

add_subdirectory(Source)
add_subdirectory(Tests)
#include(Tests/UnittestsTarget.cmake)
