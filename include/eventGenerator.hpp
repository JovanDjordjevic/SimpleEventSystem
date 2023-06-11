#ifndef __SIMPLE_EVENT_GENERATOR__
#define __SIMPLE_EVENT_GENERATOR__

#include "event.hpp"

namespace simpleEventSystem {
    class EventGenerator {
        public:
            void postEvent(const Event& event, const EventPriority& eventPriority = EventPriority::DEFAULT);

        private:

    };

} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_GENERATOR__