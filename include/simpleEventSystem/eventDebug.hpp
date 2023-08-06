#ifndef __SIMPLE_EVENT_DEBUG__
#define __SIMPLE_EVENT_DEBUG__

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

#define __FILE_NAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))
#define __FUNCTION_NAME__ (strrchr(__FUNCTION__, ':') ? strrchr(__FUNCTION__, ':') + 1 : __FUNCTION__)

#define ALLOW_DEBUG_LOGS
#define ALLOW_FUNCTRACE

#ifdef ALLOW_DEBUG_LOGS
    inline void EVENT_LOG(std::string&& str) {   
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::cerr << "[timestamp: " << std::put_time(&tm, "%a, %d %b %Y %T") << "] " << str << std::endl;
    }

    #ifdef ALLOW_FUNCTRACE
        #define FUNCTRACE() { EVENT_LOG("file: [" + std::string{__FILE_NAME__} + "] func: [" + std::string{__FUNCTION_NAME__} + "] line: [" + std::to_string(__LINE__) + "]"); }
    #else
        #define FUNCTRACE() {}
    #endif
#else
    inline void EVENT_LOG(std::string&& str) {}

    #define FUNCTRACE() {}
#endif //ALLOW_DEBUG_LOGS

// namespace simpleEventSystem {
// } // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_DEBUG__