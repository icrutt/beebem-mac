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
}

-(void) toLog {
    NSLog(@"%i",count);
}

-(id) init {
    self = [super init];
    [self performSelectorInBackground:@selector(backgroundTask) withObject:self];
    NSLog(@"%s","Launched background task");
    return self;
}

-(void) backgroundTask {
    tc.startMainLoop();
}

@end
