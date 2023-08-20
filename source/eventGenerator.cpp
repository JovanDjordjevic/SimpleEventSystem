#include <sstream>

#include "simpleEventSystem/eventDebug.hpp"
#include "simpleEventSystem/eventGenerator.hpp"
#include "simpleEventSystem/eventListener.hpp"
#include "simpleEventSystem/eventLoop.hpp"

namespace simpleEventSystem {
    EventGenerator::EventGenerator() {
        FUNCTRACE();
    }

    EventGenerator::~EventGenerator() {
        FUNCTRACE();
        for (auto listener : mListeners) {
            listener->unregisterEventGenerator(this, false);
        }
    }

    void EventGenerator::postEvent(Event* event, const EventPriority priority) {
        FUNCTRACE();
        if (!event) {
            return;
        }

        event->setEventGenerator(this);
        event->setPriority(priority);
        EventLoop::getInstance().queueEvent(event);
    }

    void EventGenerator::notifyListeners(Event* event, const EventPriority priority) {
        FUNCTRACE();
        if (!event) {
            return;
        }

        event->setEventGenerator(this);
        event->setPriority(priority);

        for (auto listener : mListeners) {
            std::ostringstream oss;
            oss << listener;
            EVENT_LOG("sending event to listener " + oss.str());

            listener->onEvent(event);

            if (event->isConsumed()) {
                break;
            }
        }

        delete event;
        event = nullptr;
    }

    void EventGenerator::registerListener(EventListener* listener) {
        FUNCTRACE();
        if (!listener) {
            return;
        }

        mListeners.emplace(listener);
        listener->registerEventGenerator(this);
    }

    void EventGenerator::unregisterListener(EventListener* listener, const bool mutual) {
        FUNCTRACE();
        if (!listener) {
            return;
        }
        
        auto it = mListeners.find(listener);
        if (it != mListeners.end()) {
            auto tmp = *it;
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

        return mListeners.find(listener) != mListeners.end();
    }
} // namespace simpleEventSystem 