include(../../CMake/AddSourcesToProject.cmake)

add_sources_to_project(TARGET ${CPP_UTILS_TARGET} SOURCES
        ProgramOptions.cpp ProgramOptions.hpp
        StringUtils.cpp StringUtils.hpp)
