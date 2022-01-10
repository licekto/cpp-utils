cmake_minimum_required(VERSION 3.21)

set(CPP_UTILS_TARGET "cpp-utils-lib")

add_subdirectory(CppUtils)

add_library(${CPP_UTILS_TARGET} STATIC ${ALL_SOURCES_GLOBAL})
setup_target(TARGET ${CPP_UTILS_TARGET})
target_include_directories(${CPP_UTILS_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})