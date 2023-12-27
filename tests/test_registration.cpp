#include <algorithm>
#include <iostream>
#include <cassert>

#include "myTestClasses.hpp"

int main() {
    auto& eventLoopInstance = simpleEventSystem::EventLoop::getInstance();

    assert(eventLoopInstance.getListenersInGroup("nonExistantGroup").size() == 0); // this query will create an empty group called "nonExistantGroup"
    assert(eventLoopInstance.getListenerGroups().size() == 1);
    eventLoopInstance.eraseEmptyGroups();
    assert(eventLoopInstance.getListenerGroups().size() == 0);

    MyEventGenerator meg1{};
    MyEventListener mel1{};

    assert(mel1.getListenerGroups().size() == 0);

    mel1.subscribeToListenerGroup("meg1Listeners");
    mel1.subscribeToListenerGroup("meg1Listeners"); // adding the same listener more than once will have no effect

    assert(mel1.getListenerGroups().size() == 1);
    assert(!mel1.isInListenerGroup("nonExistantGroup"));
    assert(mel1.isInListenerGroup("meg1Listeners"));

    MyEventListener mel2{};

    assert(mel2.getListenerGroups().size() == 0);
    mel2.subscribeToListenerGroup("meg1Listeners");
    assert(mel2.getListenerGroups().size() == 1);

    assert(eventLoopInstance.getListenersInGroup("meg1Listeners").size() == 2);
    
    mel1.unsubscribeFromListenerGroup("meg1Listeners");
    mel1.unsubscribeFromListenerGroup("meg1Listeners");  // removing a nonexistant listener should have no effect
    assert(mel1.getListenerGroups().size() == 0);
    assert(eventLoopInstance.getListenersInGroup("meg1Listeners").size() == 1);

    mel2.unsubscribeFromListenerGroup("meg1Listeners");
    assert(mel2.getListenerGroups().size() == 0);
    assert(eventLoopInstance.getListenersInGroup("meg1Listeners").size() == 0);

    // ==================================================================================

    auto currentListenerGroupsCount = eventLoopInstance.getListenerGroups().size();

    MyEventGenerator meg2;
    {
        MyEventListener mel3;
        mel3.subscribeToListenerGroup("meg2Listeners");

        assert(eventLoopInstance.getListenersInGroup("meg2Listeners").size() == 1);
    }
    assert(eventLoopInstance.getListenersInGroup("meg2Listeners").size() == 0);
    eventLoopInstance.eraseEmptyGroups();
    assert(eventLoopInstance.getListenerGroups().size() == currentListenerGroupsCount - 1);

    // ==================================================================================

    MyEventGenerator meg3;
    MyEventListener mel4;
    MyEventListener mel5;
    
    mel4.subscribeToListenerGroup("meg3Listeners");
    mel5.subscribeToListenerGroup("meg3Listeners", 500);
    mel5.subscribeToListenerGroup("meg3Listeners", 1500);   // should be ignored

    auto& meg3Listeners = eventLoopInstance.getListenersInGroup("meg3Listeners");
    assert(meg3Listeners.size() == 2);

    assert(1 == std::count_if(meg3Listeners.begin(), meg3Listeners.end(), [](const std::pair<simpleEventSystem::EventListener*, int>& elem) { return elem.second == 1000; }));
    assert(1 == std::count_if(meg3Listeners.begin(), meg3Listeners.end(), [](const std::pair<simpleEventSystem::EventListener*, int>& elem) { return elem.second == 500; }));
    assert(0 == std::count_if(meg3Listeners.begin(), meg3Listeners.end(), [](const std::pair<simpleEventSystem::EventListener*, int>& elem) { return elem.second == 1500; }));

    size_t i = 0;
    std::vector<simpleEventSystem::EventListener*> expectedListenerOrder = {&mel4, &mel5};
    std::vector<int> expectedPriorityOrder = {1000, 500};

    for (auto& elem : meg3Listeners) {
        assert(elem.first == expectedListenerOrder[i]);
        assert(elem.second == expectedPriorityOrder[i]);
        ++i;
    }

    // ==================================================================================

    return 0;
}
