cmake_minimum_required(VERSION 3.21)
find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)

function(add_test_target)
    set(oneValueArgs TARGET)
    set(multiValueArgs SOURCES COMPILER_FLAGS INCLUDE_DIRS LIBRARIES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${ARG_TARGET} ${ARG_SOURCES})
    target_link_libraries(${ARG_TARGET} Catch2::Catch2 ${ARG_LIBRARIES})
    setup_target(TARGET ${ARG_TARGET})
    target_compile_options(${ARG_TARGET} PRIVATE ${ARG_COMPILER_FLAGS})
    target_link_options(${ARG_TARGET} PRIVATE "${ARG_COMPILER_FLAGS}")
    target_include_directories(${ARG_TARGET} PRIVATE ${ARG_INCLUDE_DIRS})

    add_test(NAME ${ARG_TARGET} COMMAND ${ARG_TARGET})
endfunction()

function(add_valgrind_target)
    set(oneValueArgs TARGET)
    set(multiValueArgs)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_test(NAME "${ARG_TARGET}-valgrind" COMMAND valgrind --verbose --leak-check=full --show-reachable=yes --show-leak-kinds=all --track-origins=yes ./${ARG_TARGET})
endfunction()

function(add_helgrind_target)
    set(oneValueArgs TARGET)
    set(multiValueArgs)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_test(NAME "${ARG_TARGET}-helgrind" COMMAND valgrind --tool=helgrind ./${ARG_TARGET})
endfunction()
