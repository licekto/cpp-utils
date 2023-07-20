cmake_minimum_required(VERSION 3.21)

include(TestSetup)

set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/../Source/)

set(SRCTEST
    ${CMAKE_CURRENT_LIST_DIR}/FullEnumTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ProgramOptionsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RetriableTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/StringUtilsTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TestMain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ThreadSafeQueueTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TokenizerTest.cpp)

SET(UNIT_TEST_TARGET cpp-utils-unit-tests)

add_test_target(TARGET ${UNIT_TEST_TARGET}
                COMPILER_FLAG ""
                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
                INCLUDE_DIRS ${SRC_DIR})

add_valgrind_target(TARGET ${UNIT_TEST_TARGET})

add_test_target(TARGET cpp-utils-asan-tests
                COMPILER_FLAG -fsanitize=address
                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
                INCLUDE_DIRS ${SRC_DIR})

add_test_target(TARGET cpp-utils-lsan-tests
                COMPILER_FLAG -fsanitize=leak
                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
                INCLUDE_DIRS ${SRC_DIR})

add_test_target(TARGET cpp-utils-ubsan-tests
                COMPILER_FLAG -fsanitize=undefined
                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
                INCLUDE_DIRS ${SRC_DIR})

# Catch2 is not thread safe and therefore tsan generates a lot of errors
#add_test_target(TARGET cpp-utils-tsan-tests
#                COMPILER_FLAG -fsanitize=thread -O1
#                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
#                INCLUDE_DIRS ${SRC_DIR})

#add_test_target(TARGET cpp-utils-msan-tests
#                COMPILER_FLAG -fsanitize=memory -fsanitize-memory-track-origins=2 -fPIE -fno-omit-frame-pointer -g -O1 -fno-optimize-sibling-calls
#                SOURCES "${SRCTEST};${ALL_SOURCES_GLOBAL}"
#                INCLUDE_DIRS ${SRC_DIR})