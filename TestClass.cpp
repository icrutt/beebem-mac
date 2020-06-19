//
//  TestClass.cpp
//  TestGUI
//
//  Created by Ian Rutt on 07/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#include <chrono>
#include <iostream>
#include <thread>
#include "TestClass.hpp"
#include "BeebEm-main.h"
#include "BeebEmCommon.h"

TestClass::TestClass()
{
    testVal = 0;
    delay = 1;
}

void TestClass::testMethod(int val)
{
    testVal = val;
    
}

void TestClass::mainLoop()
{
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    using namespace std::this_thread; // sleep_for, sleep_until
    
    BeebEmMain();
    
//    while(1){
//        sleep_for(seconds(delay));
//        testVal += 1;
//        glue->sendValue(testVal);
//        std::cout << "Testval " << testVal <<std::endl;
//    }
}

void TestClass::registerGlue(GlueInterface* gl){
  glue = gl;
    BeebEmCommon::beebGlue = gl;
}

int TestClass::getValue() {
    return glue->getValue();
}

char* TestClass::getNextFrame() {
    char* nextFrame = glue->getVideoFrame();
    return nextFrame;
}
