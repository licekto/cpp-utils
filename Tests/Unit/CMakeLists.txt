cmake_minimum_required(VERSION 3.21)
find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)
include(TestSetup)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../../Source/)

set(SRCTEST
    ${CMAKE_CURRENT_LIST_DIR}/FullEnumTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjectPoolTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/LoggerTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/LRUCacheTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ProgramOptionsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RetriableTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/StringUtilsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestMain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ThreadSafeQueueTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TokenizerTest.cpp)

SET(UNIT_TEST_TARGET ${PROJECT_NAME}-unit-tests)

add_test_target(TARGET ${UNIT_TEST_TARGET}
                COMPILER_FLAG --coverage
                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
                INCLUDE_DIRS ${SRC_DIR})

add_valgrind_target(TARGET ${UNIT_TEST_TARGET})

set(UNIT_TEST_ASAN_TARGET ${PROJECT_NAME}-unit-asan-test)

add_test_target(TARGET ${UNIT_TEST_ASAN_TARGET}
                COMPILER_FLAG -fsanitize=address
                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
                INCLUDE_DIRS ${SRC_DIR})

set(UNIT_TEST_LSAN_TARGET ${PROJECT_NAME}-unit-lsan-test)

add_test_target(TARGET ${UNIT_TEST_LSAN_TARGET}
                COMPILER_FLAG -fsanitize=leak
                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
                INCLUDE_DIRS ${SRC_DIR})

set(UNIT_TEST_UBSAN_TARGET ${PROJECT_NAME}-unit-ubsan-test)

add_test_target(TARGET ${UNIT_TEST_UBSAN_TARGET}
                COMPILER_FLAG -fsanitize=undefined
                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
                INCLUDE_DIRS ${SRC_DIR})

# Not available in GCC
#add_test_target(TARGET cpp-utils-cfi-tests
#                COMPILER_FLAG -fsanitize=cfi
#                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
#                INCLUDE_DIRS ${SRC_DIR})

#add_test_target(TARGET cpp-utils-msan-tests
#                COMPILER_FLAG -fsanitize=memory -fsanitize-memory-track-origins=2 -fPIE -fno-omit-frame-pointer -g -O1 -fno-optimize-sibling-calls
#                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
#                INCLUDE_DIRS ${SRC_DIR})
