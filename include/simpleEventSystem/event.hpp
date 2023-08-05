#ifndef __SIMPLE_EVENT__
#define __SIMPLE_EVENT__

#include "eventGenerator.hpp"

namespace simpleEventSystem {
    enum class EventPriority {
        DEFAULT,
        HIGH,
        LOW
    };

    class EventGenerator;
    
    class Event {
        public:
            Event();
            virtual ~Event();
            
            void setEventGenerator(EventGenerator* generator);
            EventGenerator* getEventGenerator() const;
            
            void consume();
            bool isConsumed() const;

        private:
            bool mIsConsumed;
            EventGenerator* mEventGenerator;
    };
} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT__