#include <cassert>
#include <iostream>

#include "myTestClasses.hpp"

int main() {
    simpleEventSystem::EventLoop& mainLoop = simpleEventSystem::EventLoop::getInstance();
    mainLoop.startLoop();

    MyEventGenerator meg{};
    MyEventListener mel{};

    mel.subscribeToListenerGroup("megListeners");
    meg.directNotifyListenerGroup(new MyEvent{"Event 1"}, "megListeners");
    meg.directNotifyListenerGroup(nullptr, "megListeners"); // should have no effect

    bool ret1 = meg.postEventToListenerGroup(new MyEvent{"Event 2"}, "megListeners");
    assert(ret1 == true);

    bool ret2 = meg.postEventToListenerGroup(nullptr, "megListeners"); // should have no effect
    assert(ret2 == false);

    bool ret3 = meg.postEventToListenerGroup(new MyEvent{"Event 3"}, "megListeners", simpleEventSystem::EventPriority::LOW);
    assert(ret3 == true);
    bool ret4 = meg.postEventToListenerGroup(new MyEvent{"Event 4"}, "megListeners");   // default priority
    assert(ret4 == true);
    bool ret5 = meg.postEventToListenerGroup(new MyEvent{"Event 5"}, "megListeners", simpleEventSystem::EventPriority::HIGH);
    assert(ret5 == true);

    // std::this_thread::sleep_for(std::chrono::seconds(2));

    mainLoop.stopLoop();

    MyEvent* event6 = new MyEvent{"Event 6"};
    bool ret6 = meg.postEventToListenerGroup(event6, "megListeners"); // should fail
    if (!ret6) {
        delete event6;
    }
    assert(ret6 == false);


    mainLoop.startLoop();

    bool ret7 = meg.postEventToListenerGroup(new MyEvent{"Event 7"}, "megListeners"); // should be valid again
    assert(ret7 == true);

    // stopLoop should be called here as part of the EventLoop destructor

    return 0;
}
