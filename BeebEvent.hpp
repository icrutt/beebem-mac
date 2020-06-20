//
//  BeebEvent.hpp
//  BeebEm4
//
//  Created by Ian Rutt on 20/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#ifndef BeebEvent_hpp
#define BeebEvent_hpp

#include <stdio.h>

enum BeebEventType {
  keyDown,
    keyUp,
    nullEvent
};

class BeebEvent {
public:
    virtual BeebEventType eventType();
    virtual char keyNumber();
};

class BeebEventKeyDown: public BeebEvent{
private:
    const BeebEventType myEventType = keyDown;
    char myKeyNumber;
public:
    BeebEventKeyDown(char keyNumber);
    BeebEventType eventType();
    char keyNumber();
};

class BeebEventKeyUp: public BeebEvent{
private:
    const BeebEventType myEventType = keyUp;
    char myKeyNumber;
public:
    BeebEventKeyUp(char keyNumber);
    BeebEventType eventType();
    char keyNumber();
};

class BeebEventNull: public BeebEvent {
    private:
    const BeebEventType myEventType = nullEvent;
public:
    BeebEventNull();
    BeebEventType eventType();
    char keyNumber();
};

#endif /* BeebEvent_hpp */
