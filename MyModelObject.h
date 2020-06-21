//
//  MyModelObject.h
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef MyModelObject_h
#define MyModelObject_h

//MyModelObject.h
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "TestClass.hpp"
#import "TestGlue.hpp"

@interface MyModelObject : NSObject

{
    @private
    int count;
    TestClass tc;
    GlueInterface* glue;
    int shiftState;
}

-(void) toLog;
-(id) init;
-(void) backgroundTask;
-(char*) getNextFrame;
-(void) sendNewEvent:(NSEvent*)theEvent;

@end

#endif /* MyModelObject_h */
