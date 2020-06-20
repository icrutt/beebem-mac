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

void TestGlue::sendVideoFrame(char* frame) {
    frameMutex.lock();
    //std::cout << "Pushed video frame" << std::endl;
    frameQueue.push(frame);
    frameMutex.unlock();
}

char* TestGlue::getVideoFrame() {
    frameMutex.lock();
    if (frameQueue.empty()) {
        frameMutex.unlock();
        return nullptr;
    } else {
        char* tmpPointer = frameQueue.front();
        frameQueue.pop();
        frameMutex.unlock();
        return tmpPointer;
    }
}

void TestGlue::sendEvent(BeebEvent event) {
    eventMutex.lock();
    std::cout << "Adding event to queue" << std::endl;
    eventQueue.push(event);
    eventMutex.unlock();
}

BeebEvent TestGlue::getEvent() {
    eventMutex.lock();
    if (eventQueue.empty()) {
        //std::cout << "Queue is empty" << std::endl;
        eventMutex.unlock();
        return BeebEventNull();
    } else {
        BeebEvent tmp = eventQueue.front();
        BeebEventType tmpEvT = tmp.eventType();
        std::cout << "Got event " << tmpEvT << std::endl;
        eventQueue.pop();
        eventMutex.unlock();
        return tmp;
    }
}
