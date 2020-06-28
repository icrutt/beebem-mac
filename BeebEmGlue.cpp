//
//  BeebEmGlue.cpp
//  TestGUI2
//
//  Created by Ian Rutt on 09/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#include "BeebEmGlue.hpp"
#include <iostream>

void BeebEmGlue::sendVideoFrame(BeebEmVideoFrame* frame) {
    frameMutex.lock();
    //std::cout << "Pushed video frame" << std::endl;
    frameQueue.push(frame);
    frameMutex.unlock();
}

BeebEmVideoFrame* BeebEmGlue::getVideoFrame() {
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

void BeebEmGlue::sendEvent(BeebEvent* event) {
    eventMutex.lock();
    eventQueue.push(event);
    eventMutex.unlock();
}

BeebEvent* BeebEmGlue::getEvent() {
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

void BeebEmGlue::sendMenuEvent(BeebEmMenuEvent* event) {
    menuEventMutex.lock();
    menuEventQueue.push(event);
    menuEventMutex.unlock();
}

BeebEmMenuEvent* BeebEmGlue::getMenuEvent() {
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

void BeebEmGlue::sendMenuStatus(BeebEmMenuEvent* event) {
    menuStatusMutex.lock();
    menuStatusQueue.push(event);
    menuStatusMutex.unlock();
}

BeebEmMenuEvent* BeebEmGlue::getMenuStatus() {
    menuStatusMutex.lock();
    if (menuStatusQueue.empty()) {
        menuStatusMutex.unlock();
        return nullptr;
    } else {
        BeebEmMenuEvent* tmp = menuStatusQueue.front();
        menuStatusQueue.pop();
        menuStatusMutex.unlock();
        return tmp;
    }
    
}
