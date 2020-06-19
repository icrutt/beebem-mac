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

class TestGlue : public GlueInterface {
private:
    std::mutex intMutex;
    std::mutex frameMutex;
    int val;
    bool newVal;
    std::queue<int> valQueue;
    std::queue<char *> frameQueue;
public:
    TestGlue();
    void sendValue(int);
    bool isValueWaiting();
    int getValue();
    void sendVideoFrame(char* frame);
    char* getVideoFrame();
};

#endif /* TestGlue_hpp */
