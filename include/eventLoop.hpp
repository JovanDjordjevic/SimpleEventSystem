#ifndef __SIMPLE_EVENT_LOOP__
#define __SIMPLE_EVENT_LOOP__

#include "event.hpp"

#include <map>
#include <memory>
#include <thread>
#include <queue>

namespace simpleEventSystem {
    class EventLoop {
        public:
            static EventLoop& getInstance();

            EventLoop(const EventLoop& other) = delete;
            EventLoop(EventLoop&& other) = delete;

            void startLoop();
            void stopLoop();
        
        private:
            EventLoop();
            
            void mainLoop();
            void queueEvent(const std::shared_ptr<Event>& event, EventPriority priority);

        private:
            std::map<EventPriority, std::queue<std::shared_ptr<Event>>> m_eventQueues;
            std::thread m_loopThread;

    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_LOOP__