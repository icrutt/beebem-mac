//
//  BeebEmXFrame.h
//  BeebEm4
//
//  Created by Ian Rutt on 19/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

// ------------------------------------------------
// Objective-C wrapper for video frame.
// Wraps a pointer to a BeebEmVideoFrame instance.

#ifndef BeebEmXFrame_h
#define BeebEmXFrame_h

@interface BeebEmXFrame : NSObject

- (char*)getPointer;
- (char)getVal:(int)i;
- (int)getWidth;
- (int)getHeight;
- (int)getVOffset;

@end

#endif /* BeebEmXFrame_h */
