//
//  MyView.h
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef MyView_h
#define MyView_h

//MyView.h
#import <Cocoa/Cocoa.h>
#import "BeebFrame.h"

@interface MyView : NSView {
    BeebFrame* currentData;
}

-(void) updateFrame:(BeebFrame*) newFrame;

@end
#endif /* MyView_h */
