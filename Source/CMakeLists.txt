cmake_minimum_required(VERSION 3.21)

add_subdirectory(CppUtils)

set(ALL_SOURCES_LIB "${PROJECT_NAME}" CACHE INTERNAL "")

add_library(${ALL_SOURCES_LIB} STATIC ${ALL_SOURCES_GLOBAL})
setup_target(TARGET ${ALL_SOURCES_LIB})
target_include_directories(${ALL_SOURCES_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})