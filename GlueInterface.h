//
//  GlueInterface.h
//  TestGUI2
//
//  Created by Ian Rutt on 09/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef GlueInterface_h
#define GlueInterface_h

#include "BeebEvent.hpp"
#include "BeebEmVideoFrame.hpp"
#include "BeebEmMenuEvent.hpp"

class GlueInterface {
public:
    virtual void sendVideoFrame(BeebEmVideoFrame*) = 0;
    virtual BeebEmVideoFrame* getVideoFrame() = 0;
    virtual void sendEvent(BeebEvent*) = 0;
    virtual BeebEvent* getEvent() = 0;
    virtual void sendMenuEvent(BeebEmMenuEvent*) = 0;
    virtual BeebEmMenuEvent* getMenuEvent() = 0;
    virtual void sendMenuStatus(BeebEmMenuEvent*) = 0;
    virtual BeebEmMenuEvent* getMenuStatus() = 0;
};

#endif /* GlueInterface_h */
