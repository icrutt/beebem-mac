//
//  BEXViewController.m
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BEXViewController.h"
#import "BeebEmXModel.h"
#import "BeebEmVideoFrame.hpp"

//BEXViewController.m

@implementation BEXViewController
 
- (id) init {
    self = [super init];
    NSLog(@"%s","In constructor");
    self.myObj = [[BeebEmXModel alloc] init];
    self.ViewOutlet.delegate = self;
    [self performSelectorInBackground:@selector(mainLoop) withObject:self];
    return self;
}

- (void) mainLoop {
    while (1) {
        BeebEmXFrame* nextFrame = [_myObj getNextFrame];
        if (self.ViewOutlet) self.ViewOutlet.delegate = self;
        if (nextFrame !=NULL) {
            [_ViewOutlet performSelectorOnMainThread:@selector(updateFrame:) withObject:nextFrame waitUntilDone:YES];
            [_ViewOutlet performSelectorOnMainThread:@selector(setNeedsDisplay:) withObject:[NSNumber numberWithBool:YES] waitUntilDone:YES];
        }
    }
}

- (void) sendNewEvent:(BEXView *) sender theEvent:(NSEvent *)theEvent {
    [_myObj sendNewEvent:theEvent];
}
 
-(void)newKeyboardEvent:(BEXView *) sender theEvent:(NSEvent *)theEvent{
    [_myObj sendNewEvent:theEvent];
}

@end
