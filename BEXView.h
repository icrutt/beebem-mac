//
//  BEXView.h
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef BEXView_h
#define BEXView_h

//BEXView.h
#import <Cocoa/Cocoa.h>
#import "BeebEmXFrame.h"
#import "BEXViewDelegate.h"

@interface BEXView : NSView {
    BeebEmXFrame* currentData;
    int oldFlags;
}
@property (nonatomic, weak) id <BEXViewDelegate> delegate;

- (void)updateFrame: (BeebEmXFrame* ) frame;

@end
#endif /* BEXView_h */
