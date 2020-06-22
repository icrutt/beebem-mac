//
//  BeebEmXModel.h
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef BeebEmXModel_h
#define BeebEmXModel_h

//BeebEmXModel.h
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "TestClass.hpp"
#import "TestGlue.hpp"
#import "BeebEmXFrame.h"

@interface BeebEmXModel : NSObject

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
-(BeebEmXFrame*) getNextFrame;
-(void) sendNewEvent:(NSEvent*)theEvent;

@end

#endif /* BeebEmXModel_h */
