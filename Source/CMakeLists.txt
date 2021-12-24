cmake_minimum_required(VERSION 3.21)

set(CPP_UTILS_TARGET "cpp-utils")

add_library(${CPP_UTILS_TARGET} STATIC)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_include_directories(${CPP_UTILS_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED LINK_BOOST)
    set(DUMMY ${LINK_BOOST})
    message("Linking Boost...")
    find_package(Boost)
    target_include_directories(${CPP_UTILS_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR})
    target_link_libraries(${CPP_UTILS_TARGET} LINK_PUBLIC ${Boost_LIBRARIES})
endif()

add_subdirectory(Utility)