//
//  BeebEmGlue.hpp
//  TestGUI2
//
//  Created by Ian Rutt on 09/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef BeebEmGlue_hpp
#define BeebEmGlue_hpp

#include <stdio.h>
#include <queue>
#include <mutex>
#include "GlueInterface.h"
#include "BeebEvent.hpp"
#include <MacTypes.h>

class BeebEmGlue : public GlueInterface {
private:
    std::mutex frameMutex;
    std::mutex eventMutex;
    std::mutex menuEventMutex;
    std::mutex menuStatusMutex;
    std::queue<BeebEmVideoFrame *> frameQueue;     // Passes video frames from Emulator to GUI
    std::queue<BeebEvent* > eventQueue;            // Passes keyboard events from GUI to emulator
    std::queue<BeebEmMenuEvent* > menuEventQueue;  // Passes menu clicks from GUI to emulator
    std::queue<BeebEmMenuEvent* > menuStatusQueue; // Passes menu item status from Emulator to GUI
public:
    // Video frames, Emulator -> GUI
    void sendVideoFrame(BeebEmVideoFrame* frame);
    BeebEmVideoFrame* getVideoFrame();
    
    // Keyboard events, GUI -> Emulator
    void sendEvent(BeebEvent*);
    BeebEvent* getEvent();
    
    // Menu clicks, GUI -> Emulator
    void sendMenuEvent(BeebEmMenuEvent*);
    BeebEmMenuEvent* getMenuEvent();
    
    // Menu status updates, Emulator -> GUI
    void sendMenuStatus(BeebEmMenuEvent*);
    BeebEmMenuEvent* getMenuStatus();
};

#endif /* BeebEmGlue_hpp */
