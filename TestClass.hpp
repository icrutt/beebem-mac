//
//  TestClass.hpp
//  TestGUI
//
//  Created by Ian Rutt on 07/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef TestClass_hpp
#define TestClass_hpp

#include <stdio.h>
#include "GlueInterface.h"

class TestClass {
private:
    int testVal;
    int delay;
    GlueInterface* glue;
public:
    TestClass();
    void testMethod(int val);
    void mainLoop();
    void registerGlue(GlueInterface*);
    int getValue();
    BeebEmVideoFrame* getNextFrame();
    BeebEmMenuEvent* getMenuStatus();
};

#endif /* TestClass_hpp */
