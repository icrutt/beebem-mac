//
//  MyViewDelegate.h
//  BeebEm4
//
//  Created by Ian Rutt on 21/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#ifndef MyViewDelegate_h
#define MyViewDelegate_h

@class MyView;
 
@protocol MyViewDelegate <NSObject>
 
- (void) newKeyboardEvent:(MyView*) sender theEvent:(NSEvent *)theEvent;
 
@end

#endif /* MyViewDelegate_h */
