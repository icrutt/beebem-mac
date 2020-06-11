//
//  BeebCommon.cpp
//  BeebEm4
//
//  Created by Ian Rutt on 11/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#include <stdio.h>
#include "BeebEmCommon.h"

// Static members need to be declared outside the initial class declaration, and can be initialised
int BeebEmCommon::trace {0};
int BeebEmCommon::trace_186 {0};
int BeebEmCommon::IgnoreIllegalInstructions {1};
int BeebEmCommon::DumpAfterEach {0};
CycleCountT BeebEmCommon::TotalCycles{0};
