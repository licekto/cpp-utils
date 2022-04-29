add_sources_to_list(SOURCES Logger.cpp Logger.hpp
                            ProgramOptions.cpp ProgramOptions.hpp
                            Retriable.hpp
                            SimpleLogger.hpp
                            StringUtils.cpp StringUtils.hpp
                            Tokenizer.cpp Tokenizer.hpp
                            ThreadSafeQueue.hpp)

if(DEFINED LINK_BOOST)
    add_sources_to_list(SOURCES FullEnum.cpp FullEnum.hpp)
endif()