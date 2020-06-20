//
//  BeebEvent.cpp
//  BeebEm4
//
//  Created by Ian Rutt on 20/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#include "BeebEvent.hpp"

BeebEventType BeebEvent::eventType() {
    return BeebEventType(nullEvent);
}

char BeebEvent::keyNumber() {
    return 0;
}

BeebEventType BeebEventKeyDown::eventType() {
    return myEventType;
}

BeebEventType BeebEventKeyUp::eventType() {
    return myEventType;
}

BeebEventKeyDown::BeebEventKeyDown(char keyNumber) {
    myKeyNumber = keyNumber;
}

BeebEventKeyUp::BeebEventKeyUp(char keyNumber) {
    myKeyNumber = keyNumber;
}

BeebEventNull::BeebEventNull() {
}

char BeebEventKeyDown::keyNumber() {
    return myKeyNumber;
}

char BeebEventKeyUp::keyNumber() {
    return myKeyNumber;
}

BeebEventType BeebEventNull::eventType() {
    return myEventType;
}

char BeebEventNull::keyNumber() {
    return 0;
}
