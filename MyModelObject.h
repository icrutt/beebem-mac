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
#import "TestClassWrapper.hpp"

@interface MyModelObject : NSObject

{
    @private
    int count;
    TestClassWrapper tc;
    
}
@property (nonatomic,weak) id <MyDelegate> delegate;

-(void) getCount;
-(void) toLog;
-(id) init;
-(void) backgroundTask;

@end

#endif /* MyModelObject_h */
