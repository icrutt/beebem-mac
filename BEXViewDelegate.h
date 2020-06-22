//
//  BEXViewDelegate.h
//  BeebEm4
//
//  Created by Ian Rutt on 21/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#ifndef BEXViewDelegate_h
#define BEXViewDelegate_h

@class BEXView;
 
@protocol BEXViewDelegate <NSObject>
 
- (void) newKeyboardEvent:(BEXView*) sender theEvent:(NSEvent *)theEvent;
 
@end

#endif /* BEXViewDelegate_h */
