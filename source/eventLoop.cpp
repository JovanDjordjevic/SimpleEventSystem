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
    }
    
    EventLoop::~EventLoop() noexcept {
        FUNCTRACE();
        if (!mLoopStopped) {
            stopLoop();
        }
    }

    void EventLoop::startLoop() {
        FUNCTRACE();

        if (mLoopStopped) {
            mQueueEventsAllowed = true;
            mShouldTerminateLoop = false;

            mLoopThread = std::thread{&EventLoop::mainLoop, this};

            mLoopStopped = false;
        }
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
    }

    void EventLoop::mainLoop() {
        FUNCTRACE();
        Event* event = nullptr;
        
        while (true) {
            {
                std::unique_lock<std::mutex> eventQueueLock(mEventQueueMutex);
                
                mCondQueue.wait(eventQueueLock, [this](){
                    return mShouldTerminateLoop || !mEventQueue.empty();
                });

                if (mShouldTerminateLoop) {
                    return;
                }

                event = mEventQueue.top();
                mEventQueue.pop();
            }

            event->getEventGenerator()->notifyListeners(event, event->getPriority());
            
            mCondEventPostFinished.notify_one();
        }
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
} // namespace simpleEventSystem 