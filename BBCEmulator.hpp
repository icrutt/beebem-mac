//
//  BBCEmulator.hpp
//  TestGUI
//
//  Created by Ian Rutt on 07/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef BBCEmulator_hpp
#define BBCEmulator_hpp

#include <stdio.h>
#include "GlueInterface.h"

class BBCEmulator {
private:
    int testVal;
    int delay;
    GlueInterface* glue;
public:
    BBCEmulator();
    void testMethod(int val);
    void mainLoop();
    void registerGlue(GlueInterface*);
    int getValue();
    BeebEmVideoFrame* getNextFrame();
    BeebEmMenuEvent* getMenuStatus();
};

#endif /* BBCEmulator_hpp */
