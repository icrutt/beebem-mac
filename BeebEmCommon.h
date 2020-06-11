//
//  BeebEmCommon.h
//  BeebEm4
//
//  Created by Ian Rutt on 10/06/2020.
//  Copyright © 2020 Jon Welch. All rights reserved.
//

#ifndef BeebEmCommon_h
#define BeebEmCommon_h

/* Used to keep a count of total number of cycles executed */
typedef int CycleCountT;
#define CycleCountTMax INT_MAX
#define CycleCountWrap (INT_MAX / 2)

#define NO_TIMER_INT_DUE    -1000000

class BeebEmCommon {
public:
    static int trace;
    static int trace_186;
    static int IgnoreIllegalInstructions;
    static int DumpAfterEach;
    static CycleCountT TotalCycles;
    static int ProgramCounter;
    static unsigned char intStatus;
    static unsigned char NMIStatus;
    static unsigned int Cycles;
    static int PrePC;
    static unsigned int NMILock;
    static int DisplayCycles;
    static int CyclesToInt;
    static int SwitchOnCycles; // Reset delay
    static int OpCodes;
    static int BHardware;
};

#endif /* BeebEmCommon_h */
