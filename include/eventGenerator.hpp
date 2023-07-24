#ifndef __SIMPLE_EVENT_GENERATOR__
#define __SIMPLE_EVENT_GENERATOR__

#include <vector>

#include "event.hpp"
#include "eventListener.hpp"

namespace simpleEventSystem {
    class Event;
    class EventListener;

    class EventGenerator {
        public:
            EventGenerator();
            virtual ~EventGenerator();

            // Posts the event to the event loop. It just calls notifyListeners from the event loop thread
            void postEvent(Event* event);

            // Notifies all listeners that are registered to receive events from this event generator
            // This method will free the memory occupied by the event, no need for user to delete event manually
            void notifyListeners(Event* event);

            void registerListener(EventListener* listener);
            void unregisterListener(EventListener* listener);

            size_t getNumberOfListeners() const;

            bool isGeneratorFor(EventListener* listener);

        private:
            std::vector<EventListener*> mListeners;
    };

} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_GENERATOR__