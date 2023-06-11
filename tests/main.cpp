#include <iostream>

#include "eventLoop.hpp"

int main() {

    simpleEventSystem::EventLoop& mainLoop = simpleEventSystem::EventLoop::getInstance();

    std::cout << &mainLoop << std::endl;
    
    return 0;
}