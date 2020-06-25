//
//  BeebEmMenuEvent.cpp
//  BeebEm4
//
//  Created by Ian Rutt on 23/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#include "BeebEmMenuEvent.hpp"

BeebEmMenuEvent::BeebEmMenuEvent(BeebEmMenuItem item, const char* URL) {
    theItem = item;
    theURL = URL;
}

BeebEmMenuEvent::BeebEmMenuEvent(BeebEmMenuItem item) {
    theItem = item;
}

BeebEmMenuEvent::BeebEmMenuEvent(BeebEmMenuItem item, bool isSet) {
    theItem = item;
    itemSet = isSet;
}

BeebEmMenuItem BeebEmMenuEvent::item() {
    return theItem;
}

const char* BeebEmMenuEvent::URL() {
    return theURL;
}

bool BeebEmMenuEvent::isSet() {
    return itemSet;
}
