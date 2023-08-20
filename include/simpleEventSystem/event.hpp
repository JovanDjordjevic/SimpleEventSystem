#ifndef __SIMPLE_EVENT__
#define __SIMPLE_EVENT__

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
            Event();
            virtual ~Event();
            
            void setEventGenerator(EventGenerator* generator);
            EventGenerator* getEventGenerator() const;

            void setPriority(const EventPriority priority);
            EventPriority getPriority() const;
            
            void consume();
            bool isConsumed() const;

        private:
            bool mIsConsumed;
            EventPriority mPriority;
            EventGenerator* mEventGenerator;
    };

    struct EventPirorityComparator {
        bool operator()(const Event* lhs, const Event* rhs);
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT__