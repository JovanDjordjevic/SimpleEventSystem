#ifndef __SIMPLE_EVENT_GENERATOR__
#define __SIMPLE_EVENT_GENERATOR__

#include <set>
#include <vector>

#include "event.hpp"

namespace simpleEventSystem {
    class EventListener;

    // traversing of std set is from lowest to highest, so we do highest priority to be first (lowest)
    struct ListenerPriorityComparator {
        bool operator()(const std::pair<EventListener*, int>& lhs, const std::pair<EventListener*, int>& rhs) const;
    };
    
    enum class EventPriority;
    class Event;

    class EventGenerator {
        public:
            EventGenerator();
            virtual ~EventGenerator();

            // Posts the event to the event loop. It just calls notifyListeners from the event loop thread
            void postEvent(Event* event, const EventPriority priority = EventPriority::DEFAULT);

            // Notifies all listeners that are registered to receive events from this event generator
            // This method will free the memory occupied by the event, no need for user to delete event manually
            void notifyListeners(Event* event, const EventPriority priority = EventPriority::DEFAULT);

            void registerListener(EventListener* listener, int listenerPriority = 1000);
            void unregisterListener(EventListener* listener, const bool mutual = true);

            std::size_t getNumberOfListeners() const;

            bool isGeneratorFor(EventListener* listener);

            std::vector<EventListener*> getListenersWithPriority(const int priority) const;
            std::vector<std::pair<EventListener*, int>> getListenersAndTheirPriorities() const;
        private:
            // multiset is chosen for this because MSVC debug builds will throw errors that strict weak ordering is not enforced if std::set is used
            std::multiset<std::pair<EventListener*, int>, ListenerPriorityComparator> mListeners;
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT_GENERATOR__