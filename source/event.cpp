#include "simpleEventSystem/event.hpp"
#include "simpleEventSystem/eventDebug.hpp"

namespace simpleEventSystem {
    Event::Event() noexcept
        : mIsConsumed{false}, mPriority{EventPriority::DEFAULT}, mEventGenerator{nullptr}, mReceivingGroup{}
    {
        FUNCTRACE()
        return;
    }

    void Event::setEventGenerator(EventGenerator* generator) {
        // FUNCTRACE()
        if (!generator) {
            return;
        }

        mEventGenerator = generator;
        return;
    }

    EventGenerator* Event::getEventGenerator() const {
        // FUNCTRACE()
        return mEventGenerator;
    }

    void Event::setPriority(const EventPriority priority) {
        // FUNCTRACE()
        mPriority = priority;
        return;
    }
    
    EventPriority Event::getPriority() const {
        // FUNCTRACE()
        return mPriority;
    }

    void Event::setRecevingGroup(const std::string& listenerGroupName) {
        // FUNCTRACE()
        mReceivingGroup = listenerGroupName;
        return;
    }
    std::string Event::getRecevingGroup() const {
        // FUNCTRACE()
        return mReceivingGroup;
    }

    void Event::consume() {
        FUNCTRACE()
        mIsConsumed = true;
        return;
    }

    bool Event::isConsumed() const {
        FUNCTRACE()
        return mIsConsumed;
    }

    bool EventPirorityComparator::operator()(const Event* lhs, const Event* rhs) {
        return static_cast<int>(lhs->getPriority()) < static_cast<int>(rhs->getPriority());
    }
} // namespace simpleEventSystem
