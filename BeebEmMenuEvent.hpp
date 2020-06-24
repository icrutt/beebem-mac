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

enum BeebEmMenuItem {
    rund,  // Run disc
    opn0,  // Open Disc 0
    opn1,  // Open Disc 1
    new0,  // New Disc 0
    new1,  // New Disc 1
    rest,  // Reset
    bbcb,  // BBC B
    bbci,  // BBC B integra
    bbcp,  // BBC B+
    bbcm   // BBC Master
};

class BeebEmMenuEvent {
private:
    BeebEmMenuItem theItem;
    const char* theURL;
public:
    BeebEmMenuEvent(BeebEmMenuItem item, const char* URL);
    BeebEmMenuEvent(BeebEmMenuItem item);
    BeebEmMenuItem item();
    const char* URL();
};

#endif /* BeebEmMenuEvent_hpp */
