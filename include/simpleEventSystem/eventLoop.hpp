#ifndef __SIMPLE_EVENT_LOOP__
#define __SIMPLE_EVENT_LOOP__

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <queue>

namespace simpleEventSystem {
    class Event;
    struct EventPirorityComparator;
    class EventGenerator;
    class EventListener;

    // traversing of std set is from lowest to highest by specified order, and we need to traverse from highest priority to lowest priority
    struct ListenerPriorityComparator {
        bool operator()(const std::pair<EventListener*, int>& lhs, const std::pair<EventListener*, int>& rhs) const;
    };

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

            // returns true if event was enqueued successfully
            bool queueEvent(Event* event);

            void addListenerToGroup(EventListener* listener, const std::string& listenerGroupName, int listenerPriority = 1000);
            void addListenerToGroups(EventListener* listener, const std::unordered_set<std::string>& groups, int listenerPriority = 1000);
            void removeListenerFromGroup(EventListener* listener, const std::string& listenerGroupName);
            void removeListenerFromGroups(EventListener* listener, const std::unordered_set<std::string>& groups);

            void removeEventsPostedBy(EventGenerator* generator);

            // returns how the listeners are currently organized in groups
            std::unordered_map<std::string, std::multiset<std::pair<EventListener*, int>, ListenerPriorityComparator>> getListenerGroups();
            // ref to member, be careful!! TODO: maybe reorganize
            const std::multiset<std::pair<EventListener*, int>, ListenerPriorityComparator>& getListenersInGroup(const std::string& listenerGroupName);
        
            // most be done so group map cannot grow indefinitely
            void eraseEmptyGroups();
        
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

            // multiset is chosen for this because MSVC debug builds will throw errors that strict weak ordering is not enforced if std::set is used
            std::unordered_map<std::string, std::multiset<std::pair<EventListener*, int>, ListenerPriorityComparator>> mListenerGroups;
            std::mutex mListenerGroupAccessMutex;
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_LOOP__
