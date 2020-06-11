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
int BeebEmCommon::ProgramCounter;
unsigned char BeebEmCommon::intStatus {0}; /* bit set (nums in IRQ_Nums) if interrupt being caused */
unsigned char BeebEmCommon::NMIStatus {0}; /* bit set (nums in NMI_Nums) if NMI being caused */
unsigned int BeebEmCommon::Cycles;
// The number of cycles to be used by the current instruction - exported to allow fernangling by memory subsystem
int BeebEmCommon::PrePC;
unsigned int BeebEmCommon::NMILock {0};
// Well I think NMI's are maskable - to stop repeated NMI's - the lock is released when an RTI is done
int BeebEmCommon::DisplayCycles {0};   // When a timer interrupt is due this is the number of cycles to it (usually -ve)
int BeebEmCommon::CyclesToInt {NO_TIMER_INT_DUE};
int BeebEmCommon::SwitchOnCycles {2000000}; // Reset delay
int BeebEmCommon::OpCodes {2}; // 1 = documented only, 2 = commonly used undocumenteds, 3 = full set
int BeebEmCommon::BHardware=0; // 0 = all hardware, 1 = basic hardware only
