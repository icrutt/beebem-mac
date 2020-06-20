//
//  MyModelObject.m
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MyModelObject.h"

@implementation MyModelObject

-(void) toLog {
    NSLog(@"%i",count);
}

-(id) init {
    self = [super init];
    glue = new TestGlue();
    tc.registerGlue(glue);
    [self performSelectorInBackground:@selector(backgroundTask) withObject:self];
    NSLog(@"%s","Launched background task");
    return self;
}

-(void) backgroundTask {
    tc.mainLoop();
}

-(char* ) getNextFrame {
    char* nextFrame =  tc.getNextFrame();
    return nextFrame;
}

-(void) sendNewEvent:(NSEvent *)theEvent {
    NSLog(@"Sending new event");
    switch (theEvent.type) {
        case 10:
            glue->sendEvent(BeebEventKeyDown([theEvent keyCode]));
            break;
        case 11:
            glue->sendEvent(BeebEventKeyUp([theEvent keyCode]));
            break;
        default:
            NSLog(@"Unknown type of event %i",[theEvent keyCode]);
    }
}

@end
