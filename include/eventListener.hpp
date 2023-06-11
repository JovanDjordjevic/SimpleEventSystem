#ifndef __SIMPLE_EVENT_LISTENER__
#define __SIMPLE_EVENT_LISTENER__

#include "event.hpp"

namespace simpleEventSystem {
    class EventListener {
        public:
            virtual void onEvent(const Event& event) = 0;

        private:
    };

} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_LISTENER__