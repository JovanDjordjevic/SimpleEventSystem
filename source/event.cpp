#include "simpleEventSystem/event.hpp"
#include "simpleEventSystem/eventDebug.hpp"

namespace simpleEventSystem {
    Event::Event() 
        : mIsConsumed{false}, mPriority{EventPriority::DEFAULT}, mEventGenerator{nullptr}
    {
        FUNCTRACE();
    }

    Event::~Event() {
        FUNCTRACE();
    }

    void Event::setEventGenerator(EventGenerator* generator) {
        FUNCTRACE();
        if (!generator) {
            return;
        }

        mEventGenerator = generator;
    }

    EventGenerator* Event::getEventGenerator() const {
        FUNCTRACE();
        return mEventGenerator;
    }

    void Event::setPriority(const EventPriority priority) {
        FUNCTRACE();
        mPriority = priority;
    }
    
    EventPriority Event::getPriority() const {
        FUNCTRACE();
        return mPriority;
    }   

    void Event::consume() {
        FUNCTRACE();
        mIsConsumed = true;
    }

    bool Event::isConsumed() const {
        FUNCTRACE();
        return mIsConsumed;
    }

    bool EventPirorityComparator::operator()(const Event* lhs, const Event* rhs) {
        return static_cast<int>(lhs->getPriority()) < static_cast<int>(rhs->getPriority());
    }
} // namespace simpleEventSystem 