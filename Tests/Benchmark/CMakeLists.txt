cmake_minimum_required(VERSION 3.21)
include(TestSetup)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Source)
set(BENCHMARK_TARGET ${PROJECT_NAME}-benchmark)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(BENCHMARK_SOURCES Benchmark.cpp BoostObjectPool.hpp DefaultAllocator.hpp)

add_executable(${BENCHMARK_TARGET} ${BENCHMARK_SOURCES})

target_link_libraries(${BENCHMARK_TARGET} ${ALL_SOURCES_LIB})

target_include_directories(${BENCHMARK_TARGET} PRIVATE ${SRC_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${BENCHMARK_TARGET} ${ALL_SOURCES_LIB} ${Boost_LIBRARIES})
