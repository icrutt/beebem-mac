//
//  BeebEmMenuEvent.hpp
//  BeebEm4
//
//  Created by Ian Rutt on 23/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#ifndef BeebEmMenuEvent_hpp
#define BeebEmMenuEvent_hpp

#include <stdio.h>
#include "BeebEmMenuItem.h"

class BeebEmMenuEvent {
private:
    BeebEmMenuItem theItem;
    const char* theURL;
    bool itemSet;
public:
    BeebEmMenuEvent(BeebEmMenuItem item, const char* URL);
    BeebEmMenuEvent(BeebEmMenuItem item);
    BeebEmMenuEvent(BeebEmMenuItem item, bool isSet);
    BeebEmMenuItem item();
    bool isSet();
    const char* URL();
};

#endif /* BeebEmMenuEvent_hpp */
