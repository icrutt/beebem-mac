//
//  TestClassWrapper.cpp
//  TestGUI
//
//  Created by Ian Rutt on 07/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#include "TestClassWrapper.hpp"
//#include <thread>

TestClassWrapper::TestClassWrapper() {
    tc = TestClass();
    glue = new TestGlue();
    tc.registerGlue(glue);
}

void TestClassWrapper::startMainLoop(){
    th = std::thread(&TestClass::mainLoop,&tc);
}

int TestClassWrapper::getValue() {
    return glue->getValue();
}
