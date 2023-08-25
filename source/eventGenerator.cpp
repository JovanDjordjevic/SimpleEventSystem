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
        FUNCTRACE();
        for (auto&& listener : mListeners) {
            listener.first->unregisterEventGenerator(this, false);
        }
    }

    bool EventGenerator::postEvent(Event* event, const EventPriority priority) {
        FUNCTRACE();
        if (!event) {
            return false;
        }

        event->setEventGenerator(this);
        event->setPriority(priority);
        return EventLoop::getInstance().queueEvent(event);
    }

    void EventGenerator::notifyListeners(Event* event, const EventPriority priority) {
        FUNCTRACE();
        if (!event) {
            return;
        }

        event->setEventGenerator(this);
        event->setPriority(priority);

        for (auto&& listener : mListeners) {
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
    }

    void EventGenerator::registerListener(EventListener* listener, int listenerPriority) {
        FUNCTRACE();
        if (!listener) {
            return;
        }

        // listeners must not be repeated in mListeners, but std::multiset cannot be ordered such that listener is unique and all
        // listeners are ordered descending, so to enforce uniqueness, we traverse the set once here
        auto cond = [listener](const std::pair<EventListener*, int>& elem){ return elem.first == listener; };
        if (std::find_if(mListeners.begin(), mListeners.end(), cond) != mListeners.end()) {
            return;
        }

        mListeners.emplace(listener, listenerPriority);
        listener->registerEventGenerator(this);
    }

    void EventGenerator::unregisterListener(EventListener* listener, const bool mutual) {
        FUNCTRACE();
        if (!listener) {
            return;
        }
        
        auto cond = [listener](const std::pair<EventListener*, int>& elem){ return elem.first == listener; };
        auto it = std::find_if(mListeners.begin(), mListeners.end(), cond);
        if (it != mListeners.end()) {
            auto tmp = (*it).first;
            mListeners.erase(it);
            if (mutual) {
                tmp->unregisterEventGenerator(this);
            }
        }
    }

    std::size_t EventGenerator::getNumberOfListeners() const {
        FUNCTRACE();
        return mListeners.size();
    }

    bool EventGenerator::isGeneratorFor(EventListener* listener) {
        FUNCTRACE();
        if (!listener) {
            return false;
        }

        auto cond = [listener](const std::pair<EventListener*, int>& elem){ return elem.first == listener; };
        return std::find_if(mListeners.begin(), mListeners.end(), cond) != mListeners.end();
    }

    std::vector<EventListener*> EventGenerator::getListenersWithPriority(const int priority) const {
        std::vector<EventListener*> res;

        for (auto&& listener : mListeners) {
            if (listener.second == priority) {
                res.emplace_back(listener.first);
            }
        }

        return res;
    }

    std::vector<std::pair<EventListener*, int>> EventGenerator::getListenersAndTheirPriorities() const {
        return std::vector<std::pair<EventListener*, int>>(mListeners.begin(), mListeners.end());
    }
} // namespace simpleEventSystem 