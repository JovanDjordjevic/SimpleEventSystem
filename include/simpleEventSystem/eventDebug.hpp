#ifndef __SIMPLE_EVENT_DEBUG__
#define __SIMPLE_EVENT_DEBUG__

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
// for windows
// #define __FILENAME__ (strrchr(__FILE__, '\') ? strrchr(__FILE__, '\') + 1 : __FILE__)

#define ALLOW_DEBUG_LOGS
#define ALLOW_FUNCTRACE

#ifdef ALLOW_DEBUG_LOGS
    template<typename... Args>
    inline void EVENT_LOG(Args&&... args) {   
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%a, %d %b %Y %T");

        std::cerr << "[timestamp: " << oss.str() << "] ";
        ((std::cerr << args), ...);
        std::cerr << std::endl;
    }

    #ifdef ALLOW_FUNCTRACE
        #define FUNCTRACE() { EVENT_LOG("file: [", __FILENAME__, "] func: [", __FUNCTION__, "] line: [", __LINE__, "]"); }
    #else
        #define FUNCTRACE() {}
    #endif
#else
    template<typename... Args>
    inline void EVENT_LOG(Args&&...) {}

    #define FUNCTRACE() {}
#endif //ALLOW_DEBUG_LOGS

// namespace simpleEventSystem {
// } // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_DEBUG__