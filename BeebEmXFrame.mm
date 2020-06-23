//
//  BeebEmXFrame.m
//  BeebEm4
//
//  Created by Ian Rutt on 19/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BeebEmXFrame.h"
#import "BeebEmVideoFrame.hpp"

@interface BeebEmXFrame() {
    BeebEmVideoFrame* frame;
}
@end

@implementation BeebEmXFrame

- (id)initWithBeebFrame:(BeebEmVideoFrame* )theFrame {
    self = [super init];
    if (self) {
        frame = theFrame;
    }
    return self;
}

- (char*)getPointer {
    return frame->getPointer();
}

- (char)getVal:(int)i {
    return frame->getVal(i);
}

- (int)getWidth {
    return frame->getWidth();
}

- (int)getHeight {
    return frame->getHeight();
}

- (int)getVOffset {
    return frame->getVOffset();
}

@end
