#include "event.hpp"
#include "eventDebug.hpp"

namespace simpleEventSystem {
    Event::Event() 
        : mIsConsumed{false}, mEventGenerator{nullptr}
    {
        FUNCTRACE();
    }

    Event::~Event() {
        FUNCTRACE();
    }

    void Event::setEventGenerator(EventGenerator* generator) {
        mEventGenerator = generator;
    }

    EventGenerator* Event::getEventGenerator() const {
        FUNCTRACE();
        return mEventGenerator;
    }

    void Event::consume() {
        FUNCTRACE();
        mIsConsumed = true;
    }

    bool Event::isConsumed() const {
        FUNCTRACE();
        return mIsConsumed;
    }

} // namespace simpleEventSystem 