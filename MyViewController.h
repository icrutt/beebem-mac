//
//  MyViewController.h
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef MyViewController_h
#define MyViewController_h
//MyViewController.h
#import <Foundation/Foundation.h>

#import "MyView.h"  // your custom view
#import "MyModelObject.h" // your custom model
#import "MyDelegate.h"

@interface MyViewController: NSViewController <MyDelegate>

@property (strong, nonatomic) MyView *myView;
@property (strong, nonatomic) MyModelObject *myObj;

- (id) init;

@end


#endif /* MyViewController_h */
