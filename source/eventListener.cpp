#include "simpleEventSystem/eventDebug.hpp"
#include "simpleEventSystem/eventGenerator.hpp"
#include "simpleEventSystem/eventListener.hpp"
#include "simpleEventSystem/eventLoop.hpp"

namespace simpleEventSystem {
    EventListener::~EventListener() noexcept {
        FUNCTRACE()
        EventLoop::getInstance().removeListenerFromGroups(this, mListenerGroups);
        mListenerGroups = {};
        return;
    }

    void EventListener::onEvent(Event*) {
        EVENT_LOG("Base class call of EventListener::onEvent, probably not valid");
    }

    void EventListener::subscribeToListenerGroup(const std::string& listenerGroupName, int listenerPriority) {
        FUNCTRACE()
        EventLoop::getInstance().addListenerToGroup(this, listenerGroupName, listenerPriority);
        mListenerGroups.emplace(listenerGroupName);
        return;
    }

    void EventListener::subscribeToListenerGroups(const std::unordered_set<std::string>& groups, int listenerPriority) {
        FUNCTRACE()
        EventLoop::getInstance().addListenerToGroups(this, groups, listenerPriority);
        for (auto& group : groups) {
            mListenerGroups.emplace(group);
        }

        return;
    }

    void EventListener::unsubscribeFromListenerGroup(const std::string& listenerGroupName) {
        FUNCTRACE()
        EventLoop::getInstance().removeListenerFromGroup(this, listenerGroupName);
        mListenerGroups.erase(listenerGroupName);
        return;
    }

    void EventListener::unsubscribeFromListenerGroups(const std::unordered_set<std::string>& groups) {
        FUNCTRACE()
        EventLoop::getInstance().removeListenerFromGroups(this, groups);
        for (auto& group : groups) {
            mListenerGroups.erase(group);
        }

        return;
    }

    bool EventListener::isInListenerGroup(const std::string& listenerGroupName) {
        FUNCTRACE()
        return mListenerGroups.find(listenerGroupName) != mListenerGroups.end();
    }

    std::unordered_set<std::string> EventListener::getListenerGroups() const {
        FUNCTRACE()
        return mListenerGroups;
    }
} // namespace simpleEventSystem
