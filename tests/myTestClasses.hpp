#ifndef MY_TEST_CLASSES_HPP
#define MY_TEST_CLASSES_HPP

#include "simpleEventSystem/event.hpp"
#include "simpleEventSystem/eventLoop.hpp"
#include "simpleEventSystem/eventListener.hpp"
#include "simpleEventSystem/eventGenerator.hpp"
#include "simpleEventSystem/eventDebug.hpp"

class MyEvent : public simpleEventSystem::Event {
    public:
        MyEvent(const std::string& eventData) : m_eventData(eventData) { std::cout << "MyEvent ctor" << std::endl; }
        ~MyEvent() override { std::cout << "MyEvent dtor" << std::endl; }
        std::string getEventData() const { return m_eventData; }
    private:
        std::string m_eventData;
};

class MyEventGenerator : public simpleEventSystem::EventGenerator {
    public:
        MyEventGenerator() { std::cout << "MyEventGenerator ctor" << std::endl; }
        ~MyEventGenerator() override { std::cout << "MyEventGenerator dtor" << std::endl; }
    private:
};

class MyEventListener : public simpleEventSystem::EventListener {
    public:
        MyEventListener() { std::cout << "MyEventListener ctor" << std::endl; }
        ~MyEventListener() override { std::cout << "MyEventListener dtor" << std::endl; }

        void onEvent(simpleEventSystem::Event* event) override {
            ON_EVENT(event, MyEvent, onMyEvent)
        }

        void onMyEvent(MyEvent* event) {
            std::cout << event->getEventData() << std::endl;
            // std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    private:
};

#endif // MY_TEST_CLASSES_HPP
