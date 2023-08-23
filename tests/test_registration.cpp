#include <iostream>
#include <cassert>

#include "myTestClasses.hpp"

int main() {
    MyEventGenerator meg1{};
    MyEventListener mel1{};

    assert(meg1.getNumberOfListeners() == 0);
    assert(meg1.isGeneratorFor(&mel1) == false);
    assert(mel1.getNumberOfGenerators() == 0);
    assert(mel1.isListenerOf(&meg1) == false);

    meg1.registerListener(&mel1);
    meg1.registerListener(&mel1); // adding the same listener more than once will have no effect
    meg1.registerListener(nullptr); // should have no effect

    assert(meg1.getNumberOfListeners() == 1);
    assert(meg1.isGeneratorFor(&mel1) == true);
    assert(mel1.getNumberOfGenerators() == 1);
    assert(mel1.isListenerOf(&meg1) == true);

    assert(meg1.isGeneratorFor(nullptr) == false);
    assert(mel1.isListenerOf(nullptr) == false);

    MyEventListener mel2{};

    assert(mel2.getNumberOfGenerators() == 0);
    assert(mel2.isListenerOf(&meg1) == false);

    meg1.registerListener(&mel2);
    
    assert(meg1.getNumberOfListeners() == 2);
    assert(meg1.isGeneratorFor(&mel1) == true);
    assert(mel2.getNumberOfGenerators() == 1);
    assert(mel2.isListenerOf(&meg1) == true);

    meg1.unregisterListener(&mel1);
    meg1.unregisterListener(nullptr); // should have no effect

    assert(meg1.getNumberOfListeners() == 1);
    assert(meg1.isGeneratorFor(&mel1) == false);
    assert(mel1.getNumberOfGenerators() == 0);
    assert(mel1.isListenerOf(&meg1) == false);

    meg1.unregisterListener(&mel2);

    assert(meg1.getNumberOfListeners() == 0);
    assert(meg1.isGeneratorFor(&mel1) == false);
    assert(mel2.getNumberOfGenerators() == 0);
    assert(mel2.isListenerOf(&meg1) == false);

    // ==================================================================================

    MyEventGenerator meg3;
    MyEventListener* mel3Ptr;
    {
        MyEventListener mel3;
        mel3Ptr = &mel3;
        meg3.registerListener(&mel3);

        assert(meg3.getNumberOfListeners() == 1);
        assert(meg3.isGeneratorFor(&mel3) == true);
        assert(mel3.getNumberOfGenerators() == 1);
        assert(mel3.isListenerOf(&meg3) == true);
    }
    assert(meg3.getNumberOfListeners() == 0);
    assert(meg3.isGeneratorFor(mel3Ptr) == false);

    MyEventListener mel4;
    MyEventGenerator* meg4Ptr;
    {
        MyEventGenerator meg4;
        meg4Ptr = &meg4;
        meg4.registerListener(&mel4);

        assert(meg4.getNumberOfListeners() == 1);
        assert(meg4.isGeneratorFor(&mel4) == true);
        assert(mel4.getNumberOfGenerators() == 1);
        assert(mel4.isListenerOf(&meg4) == true);
    }
    assert(mel4.getNumberOfGenerators() == 0);
    assert(mel4.isListenerOf(meg4Ptr) == false);

    // ==================================================================================

    // think about if this even makes sense
    MyEventGenerator meg5;
    MyEventListener mel5;

    mel5.registerEventGenerator(&meg5);
    mel5.registerEventGenerator(nullptr); // should have no effect

    assert(meg5.getNumberOfListeners() == 0);       // this is why it will not make sense, either fully allow or fully disallow this
    assert(meg5.isGeneratorFor(&mel5) == false);    // this is why it will not make sense, either fully allow or fully disallow this
    assert(mel5.getNumberOfGenerators() == 1);
    assert(mel5.isListenerOf(&meg5) == true);

    mel5.unregisterEventGenerator(&meg5);
    mel5.unregisterEventGenerator(nullptr); // should have no effect

    assert(meg5.getNumberOfListeners() == 0);
    assert(meg5.isGeneratorFor(&mel5) == false);
    assert(mel5.getNumberOfGenerators() == 0);
    assert(mel5.isListenerOf(&meg5) == false);

    // ==================================================================================

    MyEventGenerator meg6;
    MyEventListener mel6;
    MyEventListener mel7;

    meg6.registerListener(&mel6);
    meg6.registerListener(&mel7, 500);
    meg6.registerListener(&mel7, 1500);

    assert(meg6.getNumberOfListeners() == 2);
    assert(meg6.isGeneratorFor(&mel6) == true);
    assert(meg6.isGeneratorFor(&mel7) == true);
    assert(mel6.getNumberOfGenerators() == 1);
    assert(mel6.isListenerOf(&meg6) == true);
    assert(mel7.getNumberOfGenerators() == 1);
    assert(mel7.isListenerOf(&meg6) == true);
    assert(meg6.getListenersWithPriority(1000).size() == 1);
    assert(meg6.getListenersWithPriority(500).size() == 1);
    assert(meg6.getListenersWithPriority(1500).size() == 0);

    auto meg6ListenersOrdered = meg6.getListenersAndTheirPriorities();
    assert(meg6ListenersOrdered.size() == 2);
    assert(meg6ListenersOrdered[0].first == &mel6);
    assert(meg6ListenersOrdered[1].first == &mel7);
    assert(meg6ListenersOrdered[0].second == 1000);
    assert(meg6ListenersOrdered[1].second == 500);

    return 0;
}