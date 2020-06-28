//
//  BEXViewController.h
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#ifndef BEXViewController_h
#define BEXViewController_h

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import "BEXView.h"
#import "BeebEmXModel.h" 
#import "BEXViewDelegate.h"
#import "BeebEmMenuItem.h"

@interface BEXViewController: NSViewController <BEXViewDelegate>
{
    IBOutlet NSMenu* theMenu;
    IBOutlet NSMenuItem* bbcbItem;
    IBOutlet NSMenuItem* bbciItem;
    IBOutlet NSMenuItem* bbcpItem;
    IBOutlet NSMenuItem* bbcmItem;
    IBOutlet NSMenuItem* mbcnItem;
    IBOutlet NSMenuItem* mbcaItem;
    IBOutlet NSMenuItem* mbcoItem;
    IBOutlet NSMenuItem* mbcwItem;
}
@property (strong, nonatomic) BeebEmXModel *myObj;
@property (strong,nonatomic) IBOutlet BEXView *ViewOutlet;

- (id) init;
// File Menu
- (IBAction)runDisc:(id)sender;
- (IBAction)openDisc0:(id)sender;
- (IBAction)openDisc1:(id)sender;
- (IBAction)newDisc0:(id)sender;
- (IBAction)newDisc1:(id)sender;
- (IBAction)BeebReset:(id)sender;

// Hardware Menu
- (IBAction)BBC_B:(id)sender;
- (IBAction)BBC_Integra:(id)sender;
- (IBAction)BBC_Bplus:(id)sender;
- (IBAction)BBC_Master:(id)sender;
- (IBAction)disc8271:(id)sender;
- (IBAction)discAc1770:(id)sender;
- (IBAction)discOp1770:(id)sender;
- (IBAction)discWa1770:(id)sender;

@end


#endif /* BEXViewController_h */
