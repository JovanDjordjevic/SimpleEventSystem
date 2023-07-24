#ifndef __SIMPLE_EVENT_LISTENER__
#define __SIMPLE_EVENT_LISTENER__

#include <vector>

#include "event.hpp"
#include "eventGenerator.hpp"

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