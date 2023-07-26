cmake_minimum_required(VERSION 3.21)
include(Compiler)
include(TestSetup)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Source)
set(THREADS_TARGET ${PROJECT_NAME}-threads)

add_executable(${THREADS_TARGET} Threads.cpp)
target_link_libraries(${THREADS_TARGET} ${ALL_SOURCES_LIB})
target_compile_definitions(${THREADS_TARGET} PUBLIC CAPACITY=100 THREADS_MAX=4)
target_include_directories(${THREADS_TARGET} PRIVATE ${SRC_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${THREADS_TARGET} ${ALL_SOURCES_LIB})

set(TSAN_THREADS_TARGET ${THREADS_TARGET}-tsan)
add_test_target(TARGET ${TSAN_THREADS_TARGET}
                COMPILER_FLAG -fsanitize=thread
                SOURCES "Threads.cpp;${ALL_SOURCES_GLOBAL}"
                INCLUDE_DIRS ${SRC_DIR})

add_valgrind_target(TARGET ${THREADS_TARGET})
add_helgrind_target(TARGET ${THREADS_TARGET})
