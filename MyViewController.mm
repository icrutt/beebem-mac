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

//MyViewController.m

@implementation MyViewController

@synthesize myView;

- (IBAction)sayHello:(id)sender {
    //[_label setStringValue:@"Hello world!"];
    [_myObj getCount];
    NSLog(@"%s","In sayHello");
    [_myObj toLog];
}

- (id) init {
    self = [super init];
    NSLog(@"%s","In constructor");
    self.myObj = [[MyModelObject alloc] init];
    self.myObj.delegate = self;
    return self;
}

- (void) sendNewValue:(MyModelObject*) sender theVal:(int)theVal {
    NSLog(@"%s","In delegate");
    [_label setStringValue:[NSString stringWithFormat:@"%i",theVal]];
}

@end
