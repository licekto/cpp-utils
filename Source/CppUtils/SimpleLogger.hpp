#pragma once

#ifdef DEBUG
    #include <string.h>

    #if defined(_WIN32) || defined(_WIN64)
        #define DELIM_ '\\'
    #else
        #define DELIM_ '/'
    #endif

    #define __FILENAME__ (strrchr(__FILE__, DELIM_) ? strrchr(__FILE__, DELIM_) + 1 : __FILE__)

    #define source_info __FILENAME__ << "::" << __FUNCTION__ << "::" << __LINE__ << " "

    #define log(msg) do { std::cout << source_info << msg << "\n"; } while(0);
#else
    #define log(msg) do {} while(0);
#endif