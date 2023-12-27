#ifndef SIMPLE_EVENT_DEBUG_HPP
#define SIMPLE_EVENT_DEBUG_HPP

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

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
        inline std::string getLastFileNameComponent(const std::string& fullFileName) {
            auto lastIndex = fullFileName.rfind('/');
            if (lastIndex != std::string::npos) {
                return fullFileName.substr(lastIndex + 1);
            }

            lastIndex = fullFileName.rfind('\\');
            if (lastIndex != std::string::npos) {
                return fullFileName.substr(lastIndex + 1);
            }

            return fullFileName;
        }

        inline std::string getFunctionName(const std::string& fullFunctionName) {
            auto lastIndex = fullFunctionName.rfind(':');
            if (lastIndex != std::string::npos) {
                return fullFunctionName.substr(lastIndex + 1);
            }

            return fullFunctionName;
        }

        #define FUNCTRACE() { EVENT_LOG("file: [" + getLastFileNameComponent(__FILE__) + "] func: [" + getFunctionName(__func__) + "] line: [" + std::to_string(__LINE__) + "]"); }
    #else
        #define FUNCTRACE() {}
    #endif
#else
    inline void EVENT_LOG(const std::string& str) {}

    #define FUNCTRACE() {}
#endif //ALLOW_DEBUG_LOGS

#endif // SIMPLE_EVENT_DEBUG_HPP
