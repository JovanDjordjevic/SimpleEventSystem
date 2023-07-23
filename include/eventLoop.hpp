#ifndef __SIMPLE_EVENT_LOOP__
#define __SIMPLE_EVENT_LOOP__

#include <atomic>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <queue>

#include "event.hpp"
#include "eventGenerator.hpp"
#include "eventListener.hpp"

namespace simpleEventSystem {
    class EventLoop {
        public:
            static EventLoop& getInstance();

            ~EventLoop();

            EventLoop(const EventLoop& other) = delete;
            EventLoop(EventLoop&& other) = delete;

            void startLoop();
            void stopLoop();

            void queueEvent(Event* event);
        
        private:
            EventLoop();
            
            void mainLoop();
            
        private:
            std::queue<Event*> mEventQueue;
            std::thread mLoopThread;

            std::mutex mEventQueueMutex;
            std::condition_variable mCondQueue;
            std::condition_variable mCondEventPostFinished;
            std::atomic<bool> mShouldTerminateLoop;
            std::atomic<bool> mQueueEventsAllowed;
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_LOOP__