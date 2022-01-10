cmake_minimum_required(VERSION 3.16)

set(TARGET "cpp-utils-unit-tests")
include(CTest)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../Source/)

set(SRCTEST
    ${CMAKE_CURRENT_LIST_DIR}/FullEnumTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ProgramOptionsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RetriableTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/StringUtilsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestMain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ThreadSafeQueueTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TokenizerTest.cpp)

add_executable(${TARGET} ${SRCTEST} ${ALL_SOURCES_GLOBAL})
setup_target(TARGET ${TARGET})
target_include_directories(${TARGET} PRIVATE ${SRC_DIR})

add_test(NAME ${TARGET} COMMAND ${TARGET})
