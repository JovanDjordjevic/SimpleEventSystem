#ifndef __SIMPLE_EVENT_GENERATOR__
#define __SIMPLE_EVENT_GENERATOR__

#include <set>

#include "event.hpp"

namespace simpleEventSystem {
    enum class EventPriority;
    class Event;
    class EventListener;

    class EventGenerator {
        public:
            EventGenerator();
            virtual ~EventGenerator();

            // Posts the event to the event loop. It just calls notifyListeners from the event loop thread
            void postEvent(Event* event, const EventPriority priority = EventPriority::DEFAULT);

            // Notifies all listeners that are registered to receive events from this event generator
            // This method will free the memory occupied by the event, no need for user to delete event manually
            void notifyListeners(Event* event, const EventPriority priority = EventPriority::DEFAULT);

            void registerListener(EventListener* listener);
            void unregisterListener(EventListener* listener, const bool mutual = true);

            std::size_t getNumberOfListeners() const;

            bool isGeneratorFor(EventListener* listener);

        private:
            std::set<EventListener*> mListeners;
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_GENERATOR__