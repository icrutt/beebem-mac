/****************************************************************
 BeebEm - BBC Micro and Master 128 Emulator
 Copyright (C) 1994  David Alan Gilbert
 Copyright (C) 1997  Mike Wyatt
 Copyright (C) 2001  Richard Gellman
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public 
 License along with this program; if not, write to the Free 
 Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA  02110-1301, USA.
 ****************************************************************/

/* 6502Core - header - David Alan Gilbert 16/10/94 */

#ifndef CORE6502_HEADER
#define CORE6502_HEADER

#include "port.h"
#include "stdio.h"
#include "BeebEmCommon.h"

typedef enum IRQ_Nums {
  sysVia,
  userVia,
  serial,
  tube,
  teletext,
  hdc,
} IRQ_Nums;

typedef enum NMI_Nums {
	nmi_floppy,
	nmi_econet,
} NMI_Nums;

typedef enum PSR_Flags
{
	FlagC=1,
	FlagZ=2,
	FlagI=4,
	FlagD=8,
	FlagB=16,
	FlagV=64,
	FlagN=128
} PSR_Flags;

typedef int int16;

#define SetTrigger(after,var) var=BeebEmCommon::TotalCycles+after;
#define IncTrigger(after,var) var+=(after);
#define ClearTrigger(var) var=CycleCountTMax;
#define AdjustTrigger(var) if (var!=CycleCountTMax) var-=CycleCountWrap;

class CPU6502 {
private:
    // Members
    FILE *InstrLog;
    FILE *osclilog; //=fopen("/oscli.log","wt");
    unsigned int InstrCount;
    int CurrentInstruction;
    int CPUDebug=0;
    int Accumulator,XReg,YReg;
    unsigned char StackReg,PSR;
    unsigned char IRQCycles;
    typedef int int16;
    unsigned int ViaCycles;     // Number of cycles VIAs advanced for mem read and writes
    int IOCycles=0;             // Number of additional cycles for IO read / writes
    bool IntDue=false;          // Flag indicating if an interrupt is due
    unsigned char Branched;     // Branched - 1 if the instruction branched
    
    // Member Functions - General
    void DoNMI();
    void core_dumpstate();
    void DoInterrupt();
    void Carried();
    void DoIntCheck();
    void AdvanceCyclesForMemRead();
    void AdvanceCyclesForMemWrite();
    int SignExtendByte(signed char in);
    void SetPSRZN(const unsigned char in);
    void SetPSR(int mask,int c,int z,int i,int d,int b, int v, int n);
    void SetPSRCZN(int c,int z, int n);
    void Push(unsigned char ToPush);
    unsigned char Pop();
    void PushWord(int16 topush);
    int16 PopWord();
    int16 RelAddrModeHandler_Data();
    void Dis6502(char *str);
    void MemoryDump6502(int addr, int count);
    void PollVIAs(unsigned int nCycles);
    void PollHardware(unsigned int nCycles);
    
    // Member Functions - Instruction handlers
    void ADCInstrHandler(int16 operand);
    void ANDInstrHandler(int16 operand);
    void ASLInstrHandler(int16 address);
    void TRBInstrHandler(int16 address);
    void TSBInstrHandler(int16 address);
    void ASLInstrHandler_Acc();
    void BCCInstrHandler();
    void BCSInstrHandler();
    void BEQInstrHandler();
    void BITInstrHandler(int16 operand);
    void BITImmedInstrHandler(int16 operand);
    void BMIInstrHandler();
    void BNEInstrHandler();
    void BPLInstrHandler();
    void BRKInstrHandler();
    void BVCInstrHandler();
    void BVSInstrHandler();
    void BRAInstrHandler();
    void CMPInstrHandler(int16 operand);
    void CPXInstrHandler(int16 operand);
    void CPYInstrHandler(int16 operand);
    void DECInstrHandler(int16 address);
    void DEXInstrHandler();
    void DEAInstrHandler();
    void EORInstrHandler(int16 operand);
    void INCInstrHandler(int16 address);
    void INXInstrHandler();
    void INAInstrHandler();
    void JSRInstrHandler(int16 address);
    void LDAInstrHandler(int16 operand);
    void LDXInstrHandler(int16 operand);
    void LDYInstrHandler(int16 operand);
    void LSRInstrHandler(int16 address);
    void LSRInstrHandler_Acc();
    void ORAInstrHandler(int16 operand);
    void ROLInstrHandler(int16 address);
    void ROLInstrHandler_Acc();
    void RORInstrHandler(int16 address);
    void RORInstrHandler_Acc();
    void SBCInstrHandler(int16 operand);
    void STXInstrHandler(int16 address);
    void STYInstrHandler(int16 address);
    void BadInstrHandler(int opcode);
    
    // Member Functions - Addressing Modes
    int16 AbsAddrModeHandler_Data();
    int16 AbsAddrModeHandler_Address();
    int16 ZeroPgAddrModeHandler_Address();
    int16 IndXAddrModeHandler_Data();
    int16 IndXAddrModeHandler_Address();
    int16 IndYAddrModeHandler_Data();
    int16 IndYAddrModeHandler_Address();
    int16 ZeroPgXAddrModeHandler_Data();
    int16 ZeroPgXAddrModeHandler_Address();
    int16 AbsXAddrModeHandler_Data();
    int16 AbsXAddrModeHandler_Address();
    int16 AbsYAddrModeHandler_Data();
    int16 AbsYAddrModeHandler_Address();
    int16 IndAddrModeHandler_Address();
    int16 ZPIndAddrModeHandler_Address();
    int16 ZPIndAddrModeHandler_Data();
    int16 IndAddrXModeHandler_Address();
    int16 ZeroPgYAddrModeHandler_Data();
    int16 ZeroPgYAddrModeHandler_Address();
public:
    void Init6502core();           // Initialise 6502core
    void Exec6502Instruction();    // Execute one 6502 instruction, move program counter on
    void Save6502UEF(FILE *SUEF);
    void Load6502UEF(FILE *SUEF);
    void SyncIO();
    void AdjustForIORead();
    void AdjustForIOWrite();
    int i186_execute(int num_cycles);
    void DumpRegs();
};

#endif
