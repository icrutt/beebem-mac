//
//  BEXViewController.h
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef BEXViewController_h
#define BEXViewController_h
//BEXViewController.h
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import "BEXView.h"  // your custom view
#import "BeebEmXModel.h" // your custom model
#import "BEXViewDelegate.h"

@interface BEXViewController: NSViewController <BEXViewDelegate>

@property (strong, nonatomic) BeebEmXModel *myObj;
@property (strong,nonatomic) IBOutlet BEXView *ViewOutlet;

- (id) init;
- (IBAction)runDisc:(id)sender;
- (IBAction)openDisc0:(id)sender;
- (IBAction)openDisc1:(id)sender;
- (IBAction)newDisc0:(id)sender;
- (IBAction)newDisc1:(id)sender;

@end


#endif /* BEXViewController_h */
