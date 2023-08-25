#ifndef __SIMPLE_EVENT_LISTENER__
#define __SIMPLE_EVENT_LISTENER__

#include <set>

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

            virtual void onEvent(Event* event) = 0;

            void registerEventGenerator(EventGenerator* generator);
            void unregisterEventGenerator(EventGenerator* generator, const bool mutual = true);

            std::size_t getNumberOfGenerators() const;

            bool isListenerOf(EventGenerator* generator);

        private:
            // list of generators this listener will receive events from
            std::set<EventGenerator*> mGenerators;
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_LISTENER__