//
//  BEXViewController.mm
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BEXViewController.h"
#import "BeebEmXModel.h"
#import "BeebEmVideoFrame.hpp"
#import "BeebEmMenuEvent.hpp"

//BEXViewController.m

@implementation BEXViewController
 
- (id) init {
    self = [super init];
    NSLog(@"%s","In constructor");
    self.myObj = [[BeebEmXModel alloc] init];
    self.ViewOutlet.delegate = self;
    [self performSelectorInBackground:@selector(mainLoop) withObject:self];
    return self;
}

- (void) mainLoop {
    while (1) {
        BeebEmXFrame* nextFrame = [_myObj getNextFrame];
        if (self.ViewOutlet) self.ViewOutlet.delegate = self;
        if (nextFrame !=NULL) {
            [_ViewOutlet performSelectorOnMainThread:@selector(updateFrame:) withObject:nextFrame waitUntilDone:YES];
            [_ViewOutlet performSelectorOnMainThread:@selector(setNeedsDisplay:) withObject:[NSNumber numberWithBool:YES] waitUntilDone:YES];
        }
        BeebEmXMenuStatus* nextStatus = [_myObj getNextMenuStatus];
        if (nextStatus != NULL) {
            BeebEmMenuItem t = [nextStatus eventType];
            long state = ([nextStatus isSet]==YES) ? NSOnState : NSOffState;
            switch (t) {
                case bbcb:
                    [bbcbItem setState:state];
                    break;
                case bbci:
                    [bbciItem setState:state];
                    break;
                case bbcp:
                    [bbcpItem setState:state];
                    break;
                case bbcm:
                    [bbcmItem setState:state];
                    break;
                case mbcn:
                    [mbcnItem setState:state];
                    break;
                case mbca:
                    [mbcaItem setState:state];
                    break;
                case mbco:
                    [mbcoItem setState:state];
                    break;
                case mbcw:
                    [mbcwItem setState:state];
                    break;
                default:
                    break;
            }
        }
    }
}

- (void) sendNewEvent:(BEXView *) sender theEvent:(NSEvent *)theEvent {
    [_myObj sendNewEvent:theEvent];
}
 
-(void)newKeyboardEvent:(BEXView *) sender theEvent:(NSEvent *)theEvent{
    [_myObj sendNewEvent:theEvent];
}

//====================================================
//                                                  //
// Menu Items  - File Menu                          //
//                                                  //
//====================================================

const char* getFileToOpen() {
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg setCanChooseFiles:YES];
    [openDlg setAllowsMultipleSelection:NO];
    if ( [openDlg runModalForDirectory:nil file:nil] == NSOKButton )
        {
            const char* tmpString = [[[openDlg URLs] objectAtIndex:0] fileSystemRepresentation];
            char* retString = new char[1024];
            strcpy(retString, tmpString);
            return retString;
        } else {
            return nullptr;
        }
    }

const char* getFileToSave() {
    NSSavePanel* saveDlg = [NSSavePanel savePanel];
    [saveDlg setCanCreateDirectories:YES];
    [saveDlg setCanSelectHiddenExtension:YES];
    if ( [saveDlg runModalForDirectory:nil file:nil] == NSOKButton )
    {
        const char* tmpString = [[saveDlg URL] fileSystemRepresentation];
        char* retString = new char[1024];
        strcpy(retString, tmpString);
        return retString;
    } else {
        return nullptr;
    }
}

- (IBAction)runDisc:(id)sender
{
    const char* fPath = getFileToOpen();
    if (fPath!=nullptr) [_myObj sendMenuEvent:rund theURL:fPath];
}

- (IBAction)openDisc0:(id)sender
{
    const char* fPath = getFileToOpen();
    if (fPath!=nullptr) [_myObj sendMenuEvent:opn0 theURL:fPath];
}

- (IBAction)openDisc1:(id)sender
{
    const char* fPath = getFileToOpen();
    if (fPath!=nullptr) [_myObj sendMenuEvent:opn1 theURL:fPath];
}

- (IBAction)newDisc0:(id)sender
{
    const char* fPath = getFileToSave();
    if (fPath!=nullptr) [_myObj sendMenuEvent:new0 theURL:fPath];
}

- (IBAction)newDisc1:(id)sender {
    const char* fPath = getFileToSave();
    if (fPath!=nullptr) [_myObj sendMenuEvent:new1 theURL:fPath];
}

- (IBAction)BeebReset:(id)sender {
    const char* fPath = "";
    [_myObj sendMenuEvent:rest theURL:fPath];
}

//====================================================
//                                                  //
// Menu Items  - Hardware Menu                      //
//                                                  //
//====================================================

- (IBAction)BBC_B:(id)sender {
    const char* fPath = "";
    [_myObj sendMenuEvent:bbcb theURL:fPath];
}

- (IBAction)BBC_Integra:(id)sender {
    const char* fPath = "";
    [_myObj sendMenuEvent:bbci theURL:fPath];
}

- (IBAction)BBC_Bplus:(id)sender {
    const char* fPath = "";
    [_myObj sendMenuEvent:bbcp theURL:fPath];
}

- (IBAction)BBC_Master:(id)sender {
    const char* fPath = "";
    [_myObj sendMenuEvent:bbcm theURL:fPath];
}

- (IBAction)disc8271:(id)sender {
    const char* fPath = "";
    [_myObj sendMenuEvent:mbcn theURL:fPath];
}

- (IBAction)discAc1770:(id)sender {
    const char* fPath = "";
    [_myObj sendMenuEvent:mbca theURL:fPath];
}

- (IBAction)discOp1770:(id)sender {
    const char* fPath = "";
    [_myObj sendMenuEvent:mbco theURL:fPath];
}

- (IBAction)discWa1770:(id)sender {
    const char* fPath = "";
    [_myObj sendMenuEvent:mbcw theURL:fPath];
}


@end
