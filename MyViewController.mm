//
//  MyViewController.m
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MyViewController.h"
#import "MyModelObject.h"
#import "BeebFrame.h"

//MyViewController.m

@implementation MyViewController

- (id) init {
    self = [super init];
    NSLog(@"%s","In constructor");
    self.myObj = [[MyModelObject alloc] init];
    [self performSelectorInBackground:@selector(mainLoop) withObject:self];
    return self;
}

- (void) mainLoop {
    while (1) {
        char* nextFrame = [_myObj getNextFrame];
        if (nextFrame) {
            BeebFrame* theNextFrame = [[BeebFrame alloc] initWithPointer:nextFrame];
            [_ViewOutlet performSelectorOnMainThread:@selector(updateFrame:) withObject:theNextFrame waitUntilDone:YES];
            [_ViewOutlet performSelectorOnMainThread:@selector(setNeedsDisplay:) withObject:[NSNumber numberWithBool:YES] waitUntilDone:YES];
        }
    }
}

- (void) sendNewEvent:(MyView *) sender theEvent:(NSEvent *)theEvent {
    NSLog(@"Sending event...");
    [_myObj sendNewEvent:theEvent];
}
  
@end
