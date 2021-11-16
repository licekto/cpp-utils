cmake_minimum_required(VERSION 3.21)
include(CMake/Compiler.cmake)

project("cpp-utils")

add_subdirectory(Source)
include(Tests/UnittestsTarget.cmake)
