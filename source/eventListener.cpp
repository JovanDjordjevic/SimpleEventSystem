#include <iostream>
#include <algorithm>

#include "eventDebug.hpp"
#include "eventListener.hpp"
#include "eventLoop.hpp"

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
        mGenerators.emplace_back(generator);
    }

    void EventListener::unregisterEventGenerator(EventGenerator* generator) {
        FUNCTRACE();
        auto it = std::remove(std::begin(mGenerators), std::end(mGenerators), generator);
        mGenerators.erase(it, std::end(mGenerators));
    }
} // namespace simpleEventSystem 