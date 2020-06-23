//
//  BeebEmXModel.m
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BeebEmXModel.h"
#import "BeebEmXFrame.h"
#import "BeebEmMenuEvent.hpp"

@implementation BeebEmXModel

-(void) toLog {
    NSLog(@"%i",count);
}

-(id) init {
    self = [super init];
    shiftState = 0;
    glue = new TestGlue();
    tc.registerGlue(glue);
    [self performSelectorInBackground:@selector(backgroundTask) withObject:self];
    NSLog(@"%s","Launched background task");
    return self;
}

-(void) backgroundTask {
    tc.mainLoop();
}

-(BeebEmXFrame* ) getNextFrame {
    BeebEmVideoFrame* tmpFrame = tc.getNextFrame();
    if (tmpFrame != NULL) {
        BeebEmXFrame* nextFrame = [[BeebEmXFrame alloc] initWithBeebFrame:tmpFrame];
        return nextFrame;
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


//-(void) flagsChanged:(NSEvent *)event
//{
//   // NSLog(@"Got modifier flags: %lu",(unsigned long)[event modifierFlags]);
//    int newFlags = [event modifierFlags];
//    if ((NSEventModifierFlagShift & oldFlags) ^ (NSEventModifierFlagShift & newFlags))
//    {
//        if (NSEventModifierFlagShift & newFlags) {
//            NSLog(@"Got shift down");
//        } else {
//            NSLog(@"Got shift up");
//        };
//    }
//    oldFlags = newFlags;
//    //if (NSEventModifierFlagShift & [event modifierFlags]) NSLog(@"Got shift %i",NSEventModifierFlagShift);
//}


@end
