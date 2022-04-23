cmake_minimum_required(VERSION 3.21)

set(ALL_SOURCES_GLOBAL "" CACHE INTERNAL "")

function(add_sources_to_list)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    list(TRANSFORM ARG_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
    set(ALL_SOURCES_GLOBAL ${ALL_SOURCES_GLOBAL} ${ARG_SOURCES} CACHE INTERNAL "")
endfunction(add_sources_to_list)

function(setup_target)
    set(oneValueArgs TARGET)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    message("Setting up target " ${ARG_TARGET})
    target_precompile_headers(${ARG_TARGET} PRIVATE ${PRECOMPILED_HEADERS})
    set_property(TARGET ${ARG_TARGET} PROPERTY CXX_STANDARD 17)
    target_link_libraries(${ARG_TARGET} ${Protobuf_LIBRARIES} Threads::Threads proto)
endfunction()