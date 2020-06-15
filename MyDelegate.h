//
//  MyDelegate.h
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef MyDelegate_h
#define MyDelegate_h

@class MyModelObject;

@protocol MyDelegate <NSObject>
-(void) refreshImage:(MyModelObject*) sender theImage:(NSImage*)theImage;
@end

#endif /* MyDelegate_h */
