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
    std::mutex menuEventMutex;
    std::mutex menuStatusMutex;
    int val;
    bool newVal;
    std::queue<int> valQueue;
    std::queue<BeebEmVideoFrame *> frameQueue;
    std::queue<BeebEvent* > eventQueue;
    std::queue<BeebEmMenuEvent* > menuEventQueue;  // Passes menu clicks from GUI to emulator
    std::queue<BeebEmMenuEvent* > menuStatusQueue; // Passes menu item status from Emulator to GUI
public:
    TestGlue();
    void sendVideoFrame(BeebEmVideoFrame* frame);
    BeebEmVideoFrame* getVideoFrame();
    void sendEvent(BeebEvent*);
    BeebEvent* getEvent();
    void sendMenuEvent(BeebEmMenuEvent*);
    BeebEmMenuEvent* getMenuEvent();
    void sendMenuStatus(BeebEmMenuEvent*);
    BeebEmMenuEvent* getMenuStatus();
};

#endif /* TestGlue_hpp */
