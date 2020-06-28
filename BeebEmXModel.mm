//
//  BeebEmXModel.mm
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BeebEmXModel.h"
#import "BeebEmXFrame.h"
#import "BeebEmMenuEvent.hpp"
#import "BeebEmXMenuStatus.h"
#import "BeebEmGlue.hpp"

@implementation BeebEmXModel

-(id) init {
    self = [super init];
    shiftState = 0;
    glue = new BeebEmGlue();
    beebEm.registerGlue(glue);
    [self performSelectorInBackground:@selector(backgroundTask) withObject:self];
    NSLog(@"%s","Launched background task");
    return self;
}

-(void) backgroundTask {
    beebEm.mainLoop();
}

-(BeebEmXFrame* ) getNextFrame {
    BeebEmVideoFrame* tmpFrame = beebEm.getNextFrame();
    if (tmpFrame != NULL) {
        BeebEmXFrame* nextFrame = [[BeebEmXFrame alloc] initWithBeebFrame:tmpFrame];
        return nextFrame;
    } else {
        return NULL;
    }
}

-(BeebEmXMenuStatus* ) getNextMenuStatus {
    BeebEmMenuEvent* tmpEvent = beebEm.getMenuStatus();
    if (tmpEvent != NULL) {
        BeebEmXMenuStatus* nextStatus = [[BeebEmXMenuStatus alloc] initWithMenuEvent:tmpEvent];
        return nextStatus;
    } else {
        return NULL;
    }
}


-(void) sendNewEvent:(NSEvent *)theEvent {
    switch (theEvent.type) {
        case 10:
            glue->sendEvent(new BeebEvent(keyDown, [theEvent keyCode]));
            break;
        case 11:
            glue->sendEvent(new BeebEvent(keyUp, [theEvent keyCode]));
            break;
        case 12:
            if ([theEvent keyCode]==60){
                if (shiftState==0) {
                    glue->sendEvent(new BeebEvent(keyDown, 200));
                    NSLog(@"Shift down");
                    shiftState=1;
                } else {
                    glue->sendEvent(new BeebEvent(keyUp, 200));
                    NSLog(@"Shift up");
                    shiftState=0;
                }
            }
        default:
            NSLog(@"Unknown type of event %i",[theEvent keyCode]);
    }
}

-(void) sendMenuEvent:(BeebEmMenuItem)item theURL:(const char *)theURL
{
    BeebEmMenuEvent* menuEvent = new BeebEmMenuEvent(item,theURL);
    glue->sendMenuEvent(menuEvent);
}

@end
