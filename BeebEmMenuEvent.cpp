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

BeebEmMenuItem BeebEmMenuEvent::item() {
    return theItem;
}

const char* BeebEmMenuEvent::URL() {
    return theURL;
}

