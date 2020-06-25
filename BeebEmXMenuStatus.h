//
//  BeebEmXMenuStatus.h
//  BeebEm4
//
//  Created by Ian Rutt on 25/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#ifndef BeebEmXMenuStatus_h
#define BeebEmXMenuStatus_h

#import "BeebEmMenuItem.h"

@interface BeebEmXMenuStatus : NSObject
-(BeebEmMenuItem) eventType;
-(BOOL) isSet;
@end


#endif /* BeebEmXMenuStatus_h */
