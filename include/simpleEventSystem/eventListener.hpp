#ifndef __SIMPLE_EVENT_LISTENER__
#define __SIMPLE_EVENT_LISTENER__

#include <vector>

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
            EventListener();
            virtual ~EventListener();
            virtual void onEvent(Event* event) = 0;

            void registerEventGenerator(EventGenerator* generator);
            void unregisterEventGenerator(EventGenerator* generator);

            bool isListenerOf(EventGenerator* generator);

        private:
            // list of generators this listener will receive events from
            std::vector<EventGenerator*> mGenerators;
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_LISTENER__