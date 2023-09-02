#ifndef __SIMPLE_EVENT_DEBUG__
#define __SIMPLE_EVENT_DEBUG__

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#define __LAST_FILENAME_COMPONENT__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))
#define __FUNCTION_NAME__ (strrchr(__FUNCTION__, ':') ? strrchr(__FUNCTION__, ':') + 1 : __FUNCTION__)

#define ALLOW_DEBUG_LOGS
#define ALLOW_FUNCTRACE
#define TIMESTAMP_IN_LOGS
#define THREAD_ID_IN_LOGS

#ifdef ALLOW_DEBUG_LOGS
    inline void EVENT_LOG(const std::string& str) {   
        #ifdef TIMESTAMP_IN_LOGS
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            std::cerr << "[timestamp: " << std::put_time(&tm, "%a, %d %b %Y %T") << "] ";
        #endif

        #ifdef THREAD_ID_IN_LOGS
            std::cerr << "[thread ID: " << std::this_thread::get_id() << "] ";
        #endif

        std::cerr << str << std::endl;
    }

    #ifdef ALLOW_FUNCTRACE
        #define FUNCTRACE() { EVENT_LOG("file: [" + std::string{__LAST_FILENAME_COMPONENT__} + "] func: [" + std::string{__FUNCTION_NAME__} + "] line: [" + std::to_string(__LINE__) + "]"); }
    #else
        #define FUNCTRACE() {}
    #endif
#else
    inline void EVENT_LOG(const std::string& str) {}

    #define FUNCTRACE() {}
#endif //ALLOW_DEBUG_LOGS

#endif // __SIMPLE_EVENT_DEBUG__