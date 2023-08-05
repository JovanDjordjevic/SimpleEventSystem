#include <iostream>

#include "simpleEventSystem/event.hpp"
#include "simpleEventSystem/eventLoop.hpp"
#include "simpleEventSystem/eventListener.hpp"
#include "simpleEventSystem/eventGenerator.hpp"
#include "simpleEventSystem/eventDebug.hpp"

class MyEvent : public simpleEventSystem::Event {
    public:
        MyEvent(const std::string& eventData) : m_eventData(eventData) { std::cout << "MyEvent ctor" << std::endl; }
        ~MyEvent() { std::cout << "MyEvent dtor" << std::endl; }

        // copy ctor
        // copy = 
        // move ctor
        // move =

        std::string getEventData() const { return m_eventData; }
    private:
        std::string m_eventData;
};

class MyEventGenerator : public simpleEventSystem::EventGenerator {
    public:
        MyEventGenerator() {
            std::cout << "MyEventGenerator ctor" << std::endl;
        }

        ~MyEventGenerator() {
            std::cout << "MyEventGenerator dtor" << std::endl;
        }

    private:
};

class MyEventListener : public simpleEventSystem::EventListener {
    public:
        MyEventListener() {
            std::cout << "MyEventListener ctor" << std::endl;
        }

        ~MyEventListener() {
            std::cout << "MyEventListener dtor" << std::endl;
        }

        void onEvent(simpleEventSystem::Event* event) override {
            ON_EVENT(event, MyEvent, onMyEvent);
        }

        void onMyEvent(MyEvent* event) {
            std::cout << event->getEventData() << std::endl;
        }
    private:
};

int main() {
    simpleEventSystem::EventLoop& mainLoop = simpleEventSystem::EventLoop::getInstance();
    mainLoop.startLoop();

    MyEventGenerator meg{};
    MyEventListener mel{};

    meg.registerListener(&mel);

    meg.notifyListeners(new MyEvent{"Event 1"});
    std::cout << "==============" << std::endl;

    meg.postEvent(new MyEvent{"Event 2"});

    std::this_thread::sleep_for(std::chrono::seconds(2));
    mainLoop.stopLoop();
    
    return 0;
}