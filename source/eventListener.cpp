#include "simpleEventSystem/eventDebug.hpp"
#include "simpleEventSystem/eventGenerator.hpp"
#include "simpleEventSystem/eventListener.hpp"
#include "simpleEventSystem/eventLoop.hpp"

namespace simpleEventSystem {
    EventListener::EventListener() {
        FUNCTRACE();
    }

    EventListener::~EventListener() {
        FUNCTRACE();
        for (auto generator : mGenerators) {
            generator->unregisterListener(this, false);
        }
    }

    void EventListener::registerEventGenerator(EventGenerator* generator) {
        FUNCTRACE();
        if (!generator) {
            return;
        }

        mGenerators.emplace(generator);
    }

    void EventListener::unregisterEventGenerator(EventGenerator* generator, const bool mutual) {
        FUNCTRACE();
        if (!generator) {
            return;
        }

        auto it = mGenerators.find(generator);
        if (it != mGenerators.end()) {
            auto tmp = *it;
            mGenerators.erase(it);
            if (mutual) {
                tmp->unregisterListener(this);
            }
        }
    }

    std::size_t EventListener::getNumberOfGenerators() const {
        return mGenerators.size();
    }

    bool EventListener::isListenerOf(EventGenerator* generator) {
        FUNCTRACE();
        if (!generator) {
            return false;
        }

        return mGenerators.find(generator) != mGenerators.end();
    }
} // namespace simpleEventSystem 