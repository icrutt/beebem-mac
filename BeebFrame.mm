//
//  BeebFrame.m
//  BeebEm4
//
//  Created by Ian Rutt on 19/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BeebFrame.h"

@implementation BeebFrame

- (id)initWithPointer:(char* )theData {
    self = [super init];
    if (self) {
        frameData = theData;
    }
    return self;
}

- (char*)getPointer {
    return frameData;
}

- (char)getVal:(int)i {
    return frameData[i];
}

@end
