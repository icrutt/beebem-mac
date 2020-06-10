//
//  TestClassWrapper.hpp
//  TestGUI
//
//  Created by Ian Rutt on 07/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef TestClassWrapper_hpp
#define TestClassWrapper_hpp

#include <stdio.h>
#include <thread>
#include "TestClass.hpp"
#include "TestGlue.hpp"

class TestClassWrapper
{
private:
    TestClass tc;
    std::thread th;
    GlueInterface* glue;
public:
    TestClassWrapper();
    void startMainLoop();
    int getValue();
};

#endif /* TestClassWrapper_hpp */
