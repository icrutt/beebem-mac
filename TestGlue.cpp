//
//  TestGlue.cpp
//  TestGUI2
//
//  Created by Ian Rutt on 09/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#include "TestGlue.hpp"
#include <iostream>

TestGlue::TestGlue(){
  newVal = false;
}

void TestGlue::sendVideoFrame(BeebEmVideoFrame* frame) {
    frameMutex.lock();
    //std::cout << "Pushed video frame" << std::endl;
    frameQueue.push(frame);
    frameMutex.unlock();
}

BeebEmVideoFrame* TestGlue::getVideoFrame() {
    frameMutex.lock();
    if (frameQueue.empty()) {
        frameMutex.unlock();
        return nullptr;
    } else {
        BeebEmVideoFrame* tmpPointer = frameQueue.front();
        frameQueue.pop();
        frameMutex.unlock();
        return tmpPointer;
    }
}

void TestGlue::sendEvent(BeebEvent* event) {
    eventMutex.lock();
    eventQueue.push(event);
    eventMutex.unlock();
}

BeebEvent* TestGlue::getEvent() {
    eventMutex.lock();
    if (eventQueue.empty()) {
        eventMutex.unlock();
        return nullptr;
    } else {
        BeebEvent* tmp = eventQueue.front();
        eventQueue.pop();
        eventMutex.unlock();
        return tmp;
    }
}

void TestGlue::sendMenuEvent(BeebEmMenuEvent* event) {
    menuEventMutex.lock();
    menuEventQueue.push(event);
    menuEventMutex.unlock();
}

BeebEmMenuEvent* TestGlue::getMenuEvent() {
    menuEventMutex.lock();
    if (menuEventQueue.empty()) {
        menuEventMutex.unlock();
        return nullptr;
    } else {
        BeebEmMenuEvent* tmp = menuEventQueue.front();
        menuEventQueue.pop();
        menuEventMutex.unlock();
        return tmp;
    }

}
