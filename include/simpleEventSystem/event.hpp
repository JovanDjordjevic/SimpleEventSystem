#ifndef SIMPLE_EVENT_HPP
#define SIMPLE_EVENT_HPP

#include <string>

namespace simpleEventSystem {
    class EventGenerator;

    enum class EventPriority {
        VERY_LOW = 0,
        LOW = 1,
        DEFAULT = 2,
        HIGH = 3,
        VERY_HIGH = 4
    };
    
    class Event {
        public:
            Event() noexcept;
            Event(const Event& other) noexcept = default;
            Event(Event&& other) noexcept = default;
            virtual ~Event() noexcept = default;

            Event& operator=(const Event& other) noexcept = default;
            Event& operator=(Event&& other) noexcept = default;
            
            void setEventGenerator(EventGenerator* generator);
            EventGenerator* getEventGenerator() const;

            void setPriority(const EventPriority priority);
            EventPriority getPriority() const;

            void setRecevingGroup(const std::string& listenerGroupName);
            std::string getRecevingGroup() const;
            
            void consume();
            bool isConsumed() const;

        private:
            bool mIsConsumed;
            EventPriority mPriority;
            EventGenerator* mEventGenerator;
            std::string mReceivingGroup;
    };

    struct EventPirorityComparator {
        bool operator()(const Event* lhs, const Event* rhs);
    };
} // namespace simpleEventSystem 

#endif // SIMPLE_EVENT_HPP
