target_sources(${CPP_UTILS_TARGET} PRIVATE
        ProgramOptions.cpp ProgramOptions.hpp
        Retriable.hpp
        StringUtils.cpp StringUtils.hpp
        Tokenizer.cpp Tokenizer.hpp
        ThreadSafeQueue.hpp)

if(DEFINED LINK_BOOST)
    target_sources(${CPP_UTILS_TARGET} PRIVATE FullEnum.hpp)
endif()
