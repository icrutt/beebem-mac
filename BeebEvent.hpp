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

//=============================================

enum BeebEventType {
    keyDown,
    keyUp,
};

//=============================================

class BeebEvent {
private:
    BeebEventType myEventType;
    char myKeyNumber;
public:
    BeebEvent(BeebEventType, char);
    BeebEventType eventType();
    char keyNumber();
};

#endif /* BeebEvent_hpp */
