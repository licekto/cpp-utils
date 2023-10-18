cmake_minimum_required(VERSION 3.21)

add_subdirectory(CppUtils)

# List of all source files, which can be later used to compilation of multiple
# testing targets (asan, ubsan, lsan...). In case of using CMake's target_sources(),
# the sources would be added directly to a target, disabling generation of
# any other targets using the same list of source files.
set(ALL_SOURCES_LIB "${PROJECT_NAME}" CACHE INTERNAL "")

add_library(${ALL_SOURCES_LIB} STATIC ${ALL_SOURCES_GLOBAL})
setup_target(TARGET ${ALL_SOURCES_LIB})
target_include_directories(${ALL_SOURCES_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/generated)
