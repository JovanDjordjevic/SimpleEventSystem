#include <algorithm>
#include <sstream>

#include "simpleEventSystem/event.hpp"
#include "simpleEventSystem/eventDebug.hpp"
#include "simpleEventSystem/eventGenerator.hpp"
#include "simpleEventSystem/eventListener.hpp"
#include "simpleEventSystem/eventLoop.hpp"

namespace simpleEventSystem {
    EventLoop& EventLoop::getInstance() {
        FUNCTRACE();
        static EventLoop eventLoop;
        return eventLoop;
    }

    EventLoop::EventLoop() noexcept
        : mEventQueue{}, mShouldTerminateLoop{false}, mQueueEventsAllowed{true}, mLoopStopped{true}
    {
        FUNCTRACE();
        return;
    }
    
    EventLoop::~EventLoop() noexcept {
        FUNCTRACE();
        if (!mLoopStopped) {
            stopLoop();
        }

        return;
    }

    void EventLoop::startLoop() {
        FUNCTRACE();

        if (mLoopStopped) {
            mQueueEventsAllowed = true;
            mShouldTerminateLoop = false;

            mLoopThread = std::thread{&EventLoop::mainLoop, this};

            mLoopStopped = false;
        }

        return;
    }

    void EventLoop::stopLoop() {
        FUNCTRACE();
        if (!mLoopStopped) {
            mQueueEventsAllowed = false;

            {
                std::unique_lock<std::mutex> eventQueueLock(mEventQueueMutex);
                mCondEventPostFinished.wait(eventQueueLock, [this](){
                    return mEventQueue.size() == 0;
                });
            }

            mShouldTerminateLoop = true;
            mCondQueue.notify_one();

            mLoopThread.join();

            mLoopStopped = true;
        }

        return;
    }

    void EventLoop::mainLoop() {
        FUNCTRACE();
        Event* event = nullptr;
        
        while (true) {
            {
                std::unique_lock<std::mutex> eventQueueLock{mEventQueueMutex};
                
                mCondQueue.wait(eventQueueLock, [this](){
                    return mShouldTerminateLoop || !mEventQueue.empty();
                });

                if (mShouldTerminateLoop) {
                    return;
                }

                event = mEventQueue.top();
                mEventQueue.pop();
            }

            {
                std::unique_lock<std::mutex> listenerGroupLock{mListenerGroupAccessMutex};
                EVENT_LOG("mainLoop acquired group lock, dispatching events");
                        

                auto& listenerGroup = mListenerGroups[event->getRecevingGroup()];

                for (auto& listener : listenerGroup) {
                    std::ostringstream oss;
                    oss << listener.first;
                    EVENT_LOG("event loop sending event to listener " + oss.str());
                    listener.first->onEvent(event);

                    if (event->isConsumed()) {
                        break;
                    }
                }

                delete event;
                event = nullptr;
            }
            
            mCondEventPostFinished.notify_one();
        }

        return;
    }

    bool EventLoop::queueEvent(Event* event) {
        FUNCTRACE();
        if (!event) {
            return false;
        }

        {
            std::unique_lock<std::mutex> eventQueueLock(mEventQueueMutex);

            if(mQueueEventsAllowed) {
                mEventQueue.emplace(event);
            }
            else { 
                return false;
            }
        }

        mCondQueue.notify_one();

        return true;
    }

    void EventLoop::addListenerToGroup(EventListener* listener, const std::string& listenerGroupName, int listenerPriority) {
        FUNCTRACE();

        std::unique_lock<std::mutex> listenerGroupLock{mListenerGroupAccessMutex};

        auto& listenersInGroup = mListenerGroups[listenerGroupName];
        // listeners must not be repeated in a group, but std::multiset cannot be ordered such that listener is unique and all
        // listeners are ordered descending, so to enforce uniqueness, we traverse the set once here
        auto cond = [listener](const std::pair<EventListener*, int>& elem){ return elem.first == listener; };
        if (std::find_if(listenersInGroup.begin(), listenersInGroup.end(), cond) != listenersInGroup.end()) {
            return;
        }

        listenersInGroup.emplace(listener, listenerPriority);
        return;
    }

    void EventLoop::addListenerToGroups(EventListener* listener, const std::unordered_set<std::string>& groups, int listenerPriority) {
        FUNCTRACE();

        if (!listener) {
            return;
        }

        std::unique_lock<std::mutex> listenerGroupLock{mListenerGroupAccessMutex};

        for (auto& group : groups) {
            auto& listenersInGroup = mListenerGroups[group];
            // listeners must not be repeated in a group, but std::multiset cannot be ordered such that listener is unique and all
            // listeners are ordered descending, so to enforce uniqueness, we traverse the set once here
            auto cond = [listener](const std::pair<EventListener*, int>& elem){ return elem.first == listener; };
            if (std::find_if(listenersInGroup.begin(), listenersInGroup.end(), cond) != listenersInGroup.end()) {
                return;
            }

            listenersInGroup.emplace(listener, listenerPriority);
        }

        return;
    }

    void EventLoop::removeListenerFromGroup(EventListener* listener, const std::string& listenerGroupName) {
        FUNCTRACE();
        if (!listener) {
            return;
        }

        std::unique_lock<std::mutex> listenerGroupLock{mListenerGroupAccessMutex};

        auto& listenersInGroup = mListenerGroups[listenerGroupName];
        
        auto cond = [listener](const std::pair<EventListener*, int>& elem){ return elem.first == listener; };
        auto it = std::find_if(listenersInGroup.begin(), listenersInGroup.end(), cond);
        if (it != listenersInGroup.end()) {
            listenersInGroup.erase(it);
        }

        return;
    }

    void EventLoop::removeListenerFromGroups(EventListener* listener, const std::unordered_set<std::string>& groups) {
        FUNCTRACE();

        if (!listener) {
            return;
        }

        std::unique_lock<std::mutex> listenerGroupLock{mListenerGroupAccessMutex};
        
        for (auto& group : groups) {
            auto& listenersInGroup = mListenerGroups[group];
        
            auto cond = [listener](const std::pair<EventListener*, int>& elem){ return elem.first == listener; };
            auto it = std::find_if(listenersInGroup.begin(), listenersInGroup.end(), cond);
            if (it != listenersInGroup.end()) {
                listenersInGroup.erase(it);
            }
        }

        return;
    }

    void EventLoop::removeEventsPostedBy(EventGenerator* generator) {
        FUNCTRACE()
        std::priority_queue<Event*, std::vector<Event*>, EventPirorityComparator> newQueue;
        Event* event = nullptr;

        std::unique_lock<std::mutex> eventQueueLock{mEventQueueMutex};

        while(!mEventQueue.empty()) {
            event = mEventQueue.top();

            if (event->getEventGenerator() != generator) {
                newQueue.emplace(event);
            }

            mEventQueue.pop();
        }

        mEventQueue.swap(newQueue);
        return;
    }

    std::unordered_map<std::string, std::multiset<std::pair<EventListener*, int>, ListenerPriorityComparator>> EventLoop::getListenerGroups() {
        FUNCTRACE();
        std::unique_lock<std::mutex> listenerGroupLock{mListenerGroupAccessMutex};
        return mListenerGroups;
    }

    const std::multiset<std::pair<EventListener*, int>, ListenerPriorityComparator>& EventLoop::getListenersInGroup(const std::string& listenerGroupName) {
        FUNCTRACE();
        std::unique_lock<std::mutex> listenerGroupLock{mListenerGroupAccessMutex};

        // !!!!!

        return mListenerGroups[listenerGroupName];
    }


    void EventLoop::eraseEmptyGroups() {
        FUNCTRACE();
        std::unique_lock<std::mutex> listenerGroupLock{mListenerGroupAccessMutex};

        auto it = mListenerGroups.begin();
        while (it !=  mListenerGroups.end()) {
            if (it->second.empty()) {
                it = mListenerGroups.erase(it);
            }
            else {
                ++it;
            }
        }

        return;
    }
} // namespace simpleEventSystem
