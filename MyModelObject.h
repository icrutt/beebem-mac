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
#import "MyDelegate.h"
#import "TestClass.hpp"
#import "TestGlue.hpp"

@interface MyModelObject : NSObject

{
    @private
    int count;
    TestClass tc;
    GlueInterface* glue;
}
@property (nonatomic,weak) id <MyDelegate> delegate;

-(void) getCount;
-(void) toLog;
-(id) init;
-(void) backgroundTask;
-(int) getNextVal;
-(char*) getNextFrame;

@end

#endif /* MyModelObject_h */
