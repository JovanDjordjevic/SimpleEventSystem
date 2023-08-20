#include <iostream>

#include "myTestClasses.hpp"

int main() {
    simpleEventSystem::EventLoop& mainLoop = simpleEventSystem::EventLoop::getInstance();
    mainLoop.startLoop();

    MyEventGenerator meg{};
    MyEventListener mel{};

    meg.registerListener(&mel);

    meg.notifyListeners(new MyEvent{"Event 1"});
    meg.notifyListeners(nullptr); // should have no effect

    meg.postEvent(new MyEvent{"Event 2"});
    meg.postEvent(nullptr); // should have no effect

    meg.postEvent(new MyEvent{"Event 3"}, simpleEventSystem::EventPriority::LOW);
    meg.postEvent(new MyEvent{"Event 4"});
    meg.postEvent(new MyEvent{"Event 5"}, simpleEventSystem::EventPriority::HIGH);

    std::this_thread::sleep_for(std::chrono::seconds(2));
    mainLoop.stopLoop();
    
    return 0;
}