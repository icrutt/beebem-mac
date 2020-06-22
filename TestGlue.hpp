//
//  TestGlue.hpp
//  TestGUI2
//
//  Created by Ian Rutt on 09/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef TestGlue_hpp
#define TestGlue_hpp

#include <stdio.h>
#include <queue>
#include <mutex>
#include "GlueInterface.h"
#include "BeebEvent.hpp"
#include <MacTypes.h>

class TestGlue : public GlueInterface {
private:
    std::mutex intMutex;
    std::mutex frameMutex;
    std::mutex eventMutex;
    int val;
    bool newVal;
    std::queue<int> valQueue;
    std::queue<char *> frameQueue;
    std::queue<BeebEvent* > eventQueue;
public:
    TestGlue();
    void sendVideoFrame(char* frame);
    char* getVideoFrame();
    void sendEvent(BeebEvent*);
    BeebEvent* getEvent();
};

#endif /* TestGlue_hpp */
