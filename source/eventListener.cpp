#include <iostream>
#include <algorithm>

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
            generator->unregisterListener(this);
        }
    }

    void EventListener::registerEventGenerator(EventGenerator* generator) {
        FUNCTRACE();
        if (!generator) {
            return;
        }

        mGenerators.emplace_back(generator);
    }

    void EventListener::unregisterEventGenerator(EventGenerator* generator) {
        FUNCTRACE();
        if (!generator) {
            return;
        }

        auto it = std::remove(std::begin(mGenerators), std::end(mGenerators), generator);
        mGenerators.erase(it, std::end(mGenerators));
    }

    bool EventListener::isListenerOf(EventGenerator* generator) {
        FUNCTRACE();
        if (!generator) {
            return false;
        }

        return std::find(mGenerators.begin(), mGenerators.end(), generator) != mGenerators.end();
    }
} // namespace simpleEventSystem 