//
//  ViewDelegate.h
//  BeebEm4
//
//  Created by Ian Rutt on 18/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#ifndef ViewDelegate_h
#define ViewDelegate_h

@class MyView;

@protocol MyViewDelegate <NSObject>
-(void) sendNewFrame:(MyView*) sender theFrame:(char *)theFrame;
@end


#endif /* ViewDelegate_h */
