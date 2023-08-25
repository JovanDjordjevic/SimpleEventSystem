#ifndef __SIMPLE_EVENT_LOOP__
#define __SIMPLE_EVENT_LOOP__

#include <atomic>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <queue>

namespace simpleEventSystem {
    class Event;
    struct EventPirorityComparator;
    class EventGenerator;
    class EventListener;

    class EventLoop {
        public:
            static EventLoop& getInstance();

            EventLoop(const EventLoop& other) = delete;
            EventLoop(EventLoop&& other) = delete;

            ~EventLoop() noexcept;

            EventLoop& operator=(const EventLoop& other) = delete;
            EventLoop& operator=(EventLoop&& other) = delete;

            void startLoop();
            void stopLoop();

            void queueEvent(Event* event);
        private:
            EventLoop() noexcept;
            void mainLoop();       
                 
        private:
            std::priority_queue<Event*, std::vector<Event*>, EventPirorityComparator> mEventQueue;
            std::thread mLoopThread;

            std::mutex mEventQueueMutex;
            std::condition_variable mCondQueue;
            std::condition_variable mCondEventPostFinished;
            std::atomic<bool> mShouldTerminateLoop;
            std::atomic<bool> mQueueEventsAllowed;
            std::atomic<bool> mLoopStopped;
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_LOOP__