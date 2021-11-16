cmake_minimum_required(VERSION 3.16)
set(TARGET "cpp-utils-unittests")
include(CTest)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../Source/Utility)

set(SRCTEST
    ${CMAKE_CURRENT_LIST_DIR}/TestMain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ProgramOptionsTest.cpp
    ${SRC_DIR}/ProgramOptions.hpp
    ${SRC_DIR}/ProgramOptions.cpp)

add_executable(${TARGET} ${SRCTEST})
target_include_directories(${TARGET} PRIVATE ${SRC_DIR})
set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 17)

add_test(NAME ${TARGET}
         COMMAND ${TARGET})