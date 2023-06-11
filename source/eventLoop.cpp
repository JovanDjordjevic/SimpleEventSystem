#include "eventLoop.hpp"
#include "eventDebug.hpp"

#include <thread>

namespace simpleEventSystem {
    EventLoop& EventLoop::getInstance() {
        static EventLoop eventLoop;
        return eventLoop;
    }

    EventLoop::EventLoop() 
        : m_eventQueues{
            {EventPriority::DEFAULT, std::queue<std::shared_ptr<Event>>{}},
            {EventPriority::HIGH, std::queue<std::shared_ptr<Event>>{}},
            {EventPriority::LOW, std::queue<std::shared_ptr<Event>>{}}
        }
    {}

    void EventLoop::startLoop() {
        m_loopThread = std::thread{&EventLoop::mainLoop, this};
    }

    void EventLoop::stopLoop() {
        //
        m_loopThread.join();
    }

    void EventLoop::mainLoop() {
        auto& highPriorityQueue = m_eventQueues[EventPriority::HIGH];
        auto& defaultPriorityQueue = m_eventQueues[EventPriority::DEFAULT];
        auto& lowPriorityQueue = m_eventQueues[EventPriority::LOW];

        while (true) {
            while (!highPriorityQueue.empty()) {
                const auto& event = highPriorityQueue.front();
                highPriorityQueue.pop();

                EVENT_LOOP_LOG("Event popped");
            }

            // for (const auto&& queuedEvent : highPriorityQueue) {}
            // for (const auto&& queuedEvent : defaultPriorityQueue) {}
            // for (const auto&& queuedEvent : lowPriorityQueue) {}
        }
    }
    
    void EventLoop::queueEvent(const std::shared_ptr<Event>& event, EventPriority priority) {
        m_eventQueues[priority].emplace(event);
    }
} // namespace simpleEventSystem 