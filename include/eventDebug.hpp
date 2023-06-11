#ifndef __SIMPLE_EVENT_DEBUG__
#define __SIMPLE_EVENT_DEBUG__

#include <iostream>

#define ENABLE_SERVER_LOGS

#ifdef ENABLE_SERVER_LOGS
    template<typename... Args>
    inline void EVENT_LOOP_LOG(Args&&... args) {   
        ((std::cerr << args), ...);
        std::cerr << std::endl;
    }
#else
    template<typename... Args>
    inline void EVENT_LOOP_LOG(Args&&...) {}
#endif //ENABLE_SERVER_LOGS

// namespace simpleEventSystem {
// } // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_DEBUG__