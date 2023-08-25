#include <cassert>
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

    bool ret1 = meg.postEvent(new MyEvent{"Event 2"});
    assert(ret1 == true);

    bool ret2 = meg.postEvent(nullptr); // should have no effect
    assert(ret2 == false);

    bool ret3 = meg.postEvent(new MyEvent{"Event 3"}, simpleEventSystem::EventPriority::LOW);
    assert(ret3 == true);
    bool ret4 = meg.postEvent(new MyEvent{"Event 4"});
    assert(ret4 == true);
    bool ret5 = meg.postEvent(new MyEvent{"Event 5"}, simpleEventSystem::EventPriority::HIGH);
    assert(ret5 == true);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    mainLoop.stopLoop();

    bool ret6 = meg.postEvent(new MyEvent{"Event 6"}); // should fail
    assert(ret6 == false);

    mainLoop.startLoop();

    bool ret7 = meg.postEvent(new MyEvent{"Event 7"}); // should be valid again
    assert(ret7 == true);

    // FIXME: If this sleep is removed, we get a seg fault because listener destructor is called before it receives the event !!!
    // Check unregistering logic again
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // stopLoop should be called here as part of the EventLoop destructor

    return 0;
}