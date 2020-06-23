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
    rund,
    opn0,
    opn1,
    new0,
    new1
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
