#ifndef __SIMPLE_EVENT__
#define __SIMPLE_EVENT__

namespace simpleEventSystem {
    enum class EventPriority {
        DEFAULT,
        HIGH,
        LOW
    };
    
    class Event {
        public:
            Event();

        private:
    };

} // namespace simpleEventSystem 

#endif // __SIMPLE_EVENT__