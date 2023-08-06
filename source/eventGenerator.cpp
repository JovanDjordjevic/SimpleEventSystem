#include <algorithm>
#include <sstream>

#include "simpleEventSystem/eventDebug.hpp"
#include "simpleEventSystem/eventGenerator.hpp"
#include "simpleEventSystem/eventLoop.hpp"

namespace simpleEventSystem {
    EventGenerator::EventGenerator() {
        FUNCTRACE();
    }

    EventGenerator::~EventGenerator() {
        FUNCTRACE();
        for (auto listener : mListeners) {
            listener->unregisterEventGenerator(this);
        }
    }

    void EventGenerator::postEvent(Event* event) {
        FUNCTRACE();
        event->setEventGenerator(this);
        EventLoop::getInstance().queueEvent(event);
    }

    void EventGenerator::notifyListeners(Event* event) {
        FUNCTRACE();
        event->setEventGenerator(this);

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
    }

    void EventGenerator::registerListener(EventListener* listener) {
        FUNCTRACE();
        mListeners.emplace_back(listener);
        listener->registerEventGenerator(this);
    }

    void EventGenerator::unregisterListener(EventListener* listener) {
        FUNCTRACE();
        auto it = std::remove(mListeners.begin(), mListeners.end(), listener);
        mListeners.erase(it, mListeners.end());
    }

    std::size_t EventGenerator::getNumberOfListeners() const {
        FUNCTRACE();
        return mListeners.size();
    }

    bool EventGenerator::isGeneratorFor(EventListener* listener) {
        FUNCTRACE();
        return std::find(mListeners.begin(), mListeners.end(), listener) != mListeners.end();
    }
} // namespace simpleEventSystem 