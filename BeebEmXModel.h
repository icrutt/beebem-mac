//
//  BeebEmXModel.h
//  TestGUI2
//
//  Created by Ian Rutt on 08/06/2020.
//  Copyright © 2020 Ian Rutt. All rights reserved.
//

// ---------------------------------------
// The 'model' of the VMC architecture.
// Has an instance of the main Emulator class
// and a pointer to the glue object

#ifndef BeebEmXModel_h
#define BeebEmXModel_h

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "BBCEmulator.hpp"
#import "GlueInterface.h"
#import "BeebEmXFrame.h"
#import "BeebEmXMenuStatus.h"

@interface BeebEmXModel : NSObject

{
@private
    BBCEmulator beebEm;
    GlueInterface* glue;
    int shiftState;
}

- (id) init;
- (void) backgroundTask;
- (BeebEmXFrame*) getNextFrame;
- (BeebEmXMenuStatus*) getNextMenuStatus;
- (void) sendNewEvent:(NSEvent*)theEvent;
- (void) sendMenuEvent:(BeebEmMenuItem)item theURL:(const char *)theURL;

@end

#endif /* BeebEmXModel_h */
