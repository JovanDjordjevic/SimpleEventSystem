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

    EventLoop::EventLoop() 
        : mEventQueue{}, mShouldTerminateLoop{false}, mQueueEventsAllowed{true}
    {
        FUNCTRACE();
    }
    
    EventLoop::~EventLoop() {
        FUNCTRACE();
    }

    void EventLoop::startLoop() {
        FUNCTRACE();
        mLoopThread = std::thread{&EventLoop::mainLoop, this};
    }

    void EventLoop::stopLoop() {
        FUNCTRACE();
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

    void EventLoop::queueEvent(Event* event) {
        FUNCTRACE();
        if (!event) {
            return;
        }

        {
            std::unique_lock<std::mutex> eventQueueLock(mEventQueueMutex);

            if(mQueueEventsAllowed) {
                mEventQueue.emplace(event);
            }
        }

        mCondQueue.notify_one();
    }
} // namespace simpleEventSystem 