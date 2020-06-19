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
#import "ViewDelegate.h"
#import "BeebFrame.h"

@interface MyView : NSView{
    BeebFrame* currentData;
    int imCount;
}

// holds it's own subviews
@property (strong, nonatomic) NSView *anotherView;
@property (strong, nonatomic) NSImageView *myImageView;
@property (nonatomic,weak) id <MyViewDelegate> delegate;

-(void) updateFrame:(BeebFrame*) newFrame;

@end
#endif /* MyView_h */
