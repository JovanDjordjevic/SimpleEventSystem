#include <sstream>
#include <algorithm>

#include "simpleEventSystem/eventDebug.hpp"
#include "simpleEventSystem/eventGenerator.hpp"
#include "simpleEventSystem/eventListener.hpp"
#include "simpleEventSystem/eventLoop.hpp"

namespace simpleEventSystem {
    bool ListenerPriorityComparator::operator()(const std::pair<EventListener*, int>& lhs, const std::pair<EventListener*, int>& rhs) const {
        return lhs.second > rhs.second;
    }

    EventGenerator::~EventGenerator() noexcept {
        FUNCTRACE()
        EventLoop::getInstance().removeEventsPostedBy(this);
        return;
    }

    bool EventGenerator::postEventToListenerGroup(Event* event, const std::string& listenerGroupName, const EventPriority priority) {
        FUNCTRACE()
        if (!event) {
            return false;
        }

        event->setEventGenerator(this);
        event->setPriority(priority);
        event->setRecevingGroup(listenerGroupName);
        return EventLoop::getInstance().queueEvent(event);
    }

    void EventGenerator::directNotifyListenerGroup(Event* event, const std::string& listenerGroupName, const EventPriority priority) {
        FUNCTRACE()
        if (!event) {
            return;
        }

        event->setEventGenerator(this);
        event->setPriority(priority);
        event->setRecevingGroup(listenerGroupName);

        // TODO sync somehow to avoid listeners getting destroyed in another thread after we get them here
        auto listenersInGroup = EventLoop::getInstance().getListenersInGroup(listenerGroupName);

        for (auto& listener : listenersInGroup) {
            std::ostringstream oss;
            oss << listener.first;
            EVENT_LOG("sending event to listener " + oss.str());

            listener.first->onEvent(event);

            if (event->isConsumed()) {
                break;
            }
        }

        delete event;
        event = nullptr;

        return;
    }
} // namespace simpleEventSystem
