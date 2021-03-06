cmake_minimum_required(VERSION 3.21)
find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)

function(add_test_target)
    set(oneValueArgs TARGET INCLUDE_DIR)
    set(multiValueArgs SOURCES COMPILER_FLAG)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${ARG_TARGET} ${ARG_SOURCES})
    target_link_libraries(${ARG_TARGET} Catch2::Catch2)
    setup_target(TARGET ${ARG_TARGET})
    target_compile_definitions(${ARG_TARGET} PRIVATE LINK_BOOST)
    target_compile_options(${ARG_TARGET} PRIVATE ${ARG_COMPILER_FLAG})
    target_link_options(${ARG_TARGET} PRIVATE "${ARG_COMPILER_FLAG}")
    target_include_directories(${ARG_TARGET} PRIVATE ${ARG_INCLUDE_DIR})

    add_test(NAME ${ARG_TARGET} COMMAND ${ARG_TARGET})
endfunction()