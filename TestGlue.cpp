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
void TestGlue::sendValue(int inVal) {
    intMutex.lock();
    val = inVal;
    valQueue.push(inVal);
    //  std::cout << "Queue contains " << valQueue.size() << " elements" << std::endl;
    newVal = true;
    intMutex.unlock();
}
bool TestGlue::isValueWaiting() {
    return newVal;
}
int TestGlue::getValue() {
    intMutex.lock();
    newVal = false;
    if (valQueue.empty()) {
        intMutex.unlock();
        return -1;
    } else {
        int tmpVal = valQueue.front();
        valQueue.pop();
        intMutex.unlock();
        return tmpVal;
    }
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
