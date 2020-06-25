//
//  BeebEmXMenuStatus.m
//  BeebEm4
//
//  Created by Ian Rutt on 25/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "BeebEmMenuEvent.hpp"
#import "BeebEmXMenuStatus.h"

@interface BeebEmXMenuStatus() {
    BeebEmMenuEvent* event;
}
@end

@implementation BeebEmXMenuStatus

- (id)initWithMenuEvent:(BeebEmMenuEvent* )theEvent {
    self = [super init];
    if (self) {
        event = theEvent;
    }
    return self;
}

-(BeebEmMenuItem) eventType {
    return event->item();
}

-(BOOL) isSet {
    if (event->isSet()) {
        return YES;
    } else {
        return NO;
    }
}


@end
