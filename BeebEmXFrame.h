//
//  BeebEmXFrame.h
//  BeebEm4
//
//  Created by Ian Rutt on 19/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#ifndef BeebEmXFrame_h
#define BeebEmXFrame_h

@interface BeebEmXFrame : NSObject{
    char * frameData;
}

- (id)initWithPointer:(char* )theData;
- (char*)getPointer;
- (char)getVal:(int)i;

@end

#endif /* BeebEmXFrame_h */
