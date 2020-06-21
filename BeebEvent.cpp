//
//  BeebEvent.cpp
//  BeebEm4
//
//  Created by Ian Rutt on 20/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#include <iostream>
#include "BeebEvent.hpp"

BeebEvent::BeebEvent(BeebEventType eType, char keyNumber) {
    myEventType = eType;
    myKeyNumber = keyNumber;
}

BeebEventType BeebEvent::eventType() {
    return myEventType;
}

char BeebEvent::keyNumber() {
    return myKeyNumber;
}
