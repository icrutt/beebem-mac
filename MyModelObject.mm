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
    tc.startMainLoop();
    return self;
}

@end
