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

    meg.postEvent(new MyEvent{"Event 6"}); // should fail

    mainLoop.startLoop();

    meg.postEvent(new MyEvent{"Event 7"}); // should be valid again

    // FIXME: If this sleep is removed, we get a seg fault because listener destructor is called before it receives the event !!!
    // Check unregistering logic again
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // stopLoop should be called here as part of the EventLoop destructor

    return 0;
}