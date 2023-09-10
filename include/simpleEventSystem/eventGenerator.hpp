#ifndef __SIMPLE_EVENT_GENERATOR__
#define __SIMPLE_EVENT_GENERATOR__

#include <string>

#include "event.hpp"

namespace simpleEventSystem {
    class Event;
    enum class EventPriority;
    
    class EventGenerator {
        public:
            EventGenerator() noexcept = default;
            EventGenerator(const EventGenerator& other) noexcept = default;
            EventGenerator(EventGenerator&& other) noexcept = default;
            virtual ~EventGenerator() noexcept;

            EventGenerator& operator=(const EventGenerator& other) noexcept = default;
            EventGenerator& operator=(EventGenerator&& other) noexcept = default;

            bool postEventToListenerGroup(Event* event, const std::string& listenerGroupName, const EventPriority priority = EventPriority::DEFAULT);
            void directNotifyListenerGroup(Event* event, const std::string& listenerGroupName, const EventPriority priority = EventPriority::DEFAULT);
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_GENERATOR__
