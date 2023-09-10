#ifndef __SIMPLE_EVENT_LISTENER__
#define __SIMPLE_EVENT_LISTENER__

#include <string>
#include <unordered_set>

#define ON_EVENT(_eventPtr_, _eventClass_, _function_) \
    if (_eventClass_* _castedPtr_ = dynamic_cast<_eventClass_*>(_eventPtr_)) { \
        _function_(_castedPtr_); \
        _castedPtr_->consume(); \
        return; \
    } 

#define ON_EVENT_NO_CONSUME(_eventPtr_, _eventClass_, _function_) \
    if (_eventClass_* _castedPtr_ = dynamic_cast<_eventClass_*>(_eventPtr_)) { \
        _function_(_castedPtr_); \
        return; \
    }

namespace simpleEventSystem {
    class Event;
    class EventGenerator;

    class EventListener {
        public:
            EventListener() noexcept = default;
            EventListener(const EventListener& other) noexcept = default;
            EventListener(EventListener&& other) noexcept = default;
            virtual ~EventListener() noexcept;

            EventListener& operator=(const EventListener& other) noexcept = default;
            EventListener& operator=(EventListener&& other) noexcept = default;

            virtual void onEvent(Event*);

            void subscribeToListenerGroup(const std::string& listenerGroupName, int listenerPriority = 1000);
            void subscribeToListenerGroups(const std::unordered_set<std::string>& groups, int listenerPriority = 1000);
            void unsubscribeFromListenerGroup(const std::string& listenerGroupName);
            void unsubscribeFromListenerGroups(const std::unordered_set<std::string>& groups);

            bool isInListenerGroup(const std::string& listenerGroupName);
            std::unordered_set<std::string> getListenerGroups() const;
        private:
            // keeps track what groups this listener is subscribed to
            std::unordered_set<std::string> mListenerGroups;
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_LISTENER__
