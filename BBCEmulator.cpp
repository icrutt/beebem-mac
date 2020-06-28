//
//  BBCEmulator.cpp
//  TestGUI
//
//  Created by Ian Rutt on 07/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#include <chrono>
#include <iostream>
#include <thread>
#include "BBCEmulator.hpp"
#include "BeebEm-main.h"
#include "BeebEmCommon.h"

BBCEmulator::BBCEmulator()
{
    testVal = 0;
    delay = 1;
}

void BBCEmulator::testMethod(int val)
{
    testVal = val;
    
}

void BBCEmulator::mainLoop()
{
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    using namespace std::this_thread; // sleep_for, sleep_until
    
    BeebEmMain();
    
}

void BBCEmulator::registerGlue(GlueInterface* gl){
  glue = gl;
    BeebEmCommon::beebGlue = gl;
}

BeebEmVideoFrame* BBCEmulator::getNextFrame() {
    BeebEmVideoFrame* nextFrame = glue->getVideoFrame();
    return nextFrame;
}

BeebEmMenuEvent* BBCEmulator::getMenuStatus() {
    BeebEmMenuEvent* nextEvent = glue->getMenuStatus();
    return nextEvent;
}
