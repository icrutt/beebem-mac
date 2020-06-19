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

-(void) getCount {
    NSLog(@"%s","In getCount");
    count = tc.getValue();
    [self.delegate sendNewValue:self theVal:count];
}

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

-(int) getNextVal {
    return tc.getValue();
}

-(char* ) getNextFrame {
    char* nextFrame =  tc.getNextFrame();
    return nextFrame;
}

@end
