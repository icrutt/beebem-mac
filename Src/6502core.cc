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

/* Mike Wyatt 7/6/97 - Added undocumented instructions */

#include <stdio.h>
#include <stdlib.h>

#include "6502core.h"
#include "beebmem.h"
#include "beebsound.h"
#include "disc8271.h"
#include "sysvia.h"
#include "uservia.h"
#include "video.h"
#include "atodconv.h"
#include "BeebEm-main.h"
#include "disc1770.h"
#include "serial.h"
#include "tube.h"
#include "debug.h"
#include "uefstate.h"
#include "z80mem.h"
#include "z80.h"
#include "econet.h"
#include "Arm.h"
#include "BeebEmLog.hpp"
#include "via.h"
#include "BeebEvent.hpp"

// Needs to be passed to the CPU code another way (by registering with the CPU object)


/* Note how GETCFLAG is special since being bit 0 we don't need to test it to get a clean 0/1 */
#define GETCFLAG ((PSR & FlagC))
#define GETZFLAG ((PSR & FlagZ)>0)
#define GETIFLAG ((PSR & FlagI)>0)
#define GETDFLAG ((PSR & FlagD)>0)
#define GETBFLAG ((PSR & FlagB)>0)
#define GETVFLAG ((PSR & FlagV)>0)
#define GETNFLAG ((PSR & FlagN)>0)

/* Types for internal function arrays */
typedef void (*InstrHandlerFuncType)(int16 Operand);
typedef int16 (*AddrModeHandlerFuncType)(int WantsAddr);

static int CyclesTable[]={
/*0 1 2 3 4 5 6 7 8 9 a b c d e f */
  7,6,0,0,0,3,5,5,3,2,2,0,0,4,6,0, /* 0 */
  2,5,0,0,0,4,6,0,2,4,0,0,0,4,7,0, /* 1 */
  6,6,0,0,3,3,5,0,4,2,2,0,4,4,6,0, /* 2 */
  2,5,0,0,0,4,6,0,2,4,0,0,0,4,7,0, /* 3 */
  6,6,0,0,0,3,5,0,3,2,2,2,3,4,6,0, /* 4 */
  2,5,0,0,0,4,6,0,2,4,0,0,0,4,7,0, /* 5 */
  6,6,0,0,0,3,5,0,4,2,2,0,5,4,6,0, /* 6 */
  2,5,0,0,0,4,6,0,2,4,0,0,0,4,7,0, /* 7 */
  2,6,0,0,3,3,3,3,2,0,2,0,4,4,4,0, /* 8 */
  2,6,0,0,4,4,4,0,2,5,2,0,0,5,0,0, /* 9 */
  2,6,2,0,3,3,3,0,2,2,2,0,4,4,4,0, /* a */
  2,5,0,0,4,4,4,0,2,4,2,0,4,4,4,0, /* b */
  2,6,0,0,3,3,5,0,2,2,2,0,4,4,6,0, /* c */
  2,5,0,0,0,4,6,0,2,4,0,0,4,4,7,0, /* d */
  2,6,0,0,3,3,5,0,2,2,2,0,4,4,6,0, /* e */
  2,5,0,0,0,4,6,0,2,4,0,0,0,4,7,0  /* f */
}; /* CyclesTable */

// Number of cycles to start of memory read cycle
static int CyclesToMemRead[]={
	/*0 1 2 3 4 5 6 7 8 9 a b c d e f */
	0,5,0,0,0,2,2,0,0,0,0,0,0,3,3,0, /* 0 */
	0,4,0,0,0,3,3,0,0,3,0,0,0,3,4,0, /* 1 */
	0,5,0,0,0,2,2,0,0,0,0,0,3,3,3,0, /* 2 */
	0,4,0,0,0,3,3,0,0,3,0,0,0,3,4,0, /* 3 */
	0,5,0,0,0,2,2,0,0,0,0,0,0,3,3,0, /* 4 */
	0,4,0,0,0,3,3,0,0,3,0,0,0,3,4,0, /* 5 */
	0,5,0,0,0,2,2,0,0,0,0,0,0,3,3,0, /* 6 */
	0,4,0,0,0,3,3,0,0,3,0,0,0,3,4,0, /* 7 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 8 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 9 */
	0,5,0,0,2,2,2,0,0,0,0,0,3,3,3,0, /* a */
	0,4,0,0,3,3,3,0,0,3,0,0,3,3,4,0, /* b */
	0,5,0,0,2,2,2,0,0,0,0,0,3,3,3,0, /* c */
	0,4,0,0,0,3,3,0,0,3,0,0,0,3,4,0, /* d */
	0,5,0,0,2,2,2,0,0,0,0,0,3,3,3,0, /* e */
	0,4,0,0,0,3,3,0,0,3,0,0,0,3,4,0  /* f */
};

// Number of cycles to start of memory write cycle
static int CyclesToMemWrite[]={
	/*0 1 2 3 4 5 6 7 8 9 a b c d e f */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* 0 */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* 1 */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* 2 */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* 3 */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* 4 */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* 5 */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* 6 */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* 7 */
	0,5,0,0,2,2,2,0,0,0,0,0,3,3,3,0, /* 8 */
	0,5,0,0,3,3,3,0,0,4,0,0,0,0,0,0, /* 9 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* a */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* b */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* c */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* d */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0, /* e */
	0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0  /* f */
};

// 1 if first cycle happened

/* Get a two byte address from the program counter, and then post inc the program counter */
#define GETTWOBYTEFROMPC(var) \
  var=ReadPaged(BeebEmCommon::ProgramCounter); \
  var|=(ReadPaged(BeebEmCommon::ProgramCounter+1)<<8); \
  BeebEmCommon::ProgramCounter+=2;

#define WritePaged(addr,val) BeebWriteMem(addr,val)
#define ReadPaged(Address) BeebReadMem(Address)

void PollVIAs(unsigned int nCycles);
void PollHardware(unsigned int Cycles);

/*----------------------------------------------------------------------------*/
inline void CPU6502::Carried() {
	// Correct cycle count for indirection across page boundary
	if (((CurrentInstruction & 0xf)==0x1 ||
		 (CurrentInstruction & 0xf)==0x9 ||
		 (CurrentInstruction & 0xf)==0xd) &&
		(CurrentInstruction & 0xf0)!=0x90)
	{
		BeebEmCommon::Cycles++;
	}
	else if (CurrentInstruction==0xBC ||
			 CurrentInstruction==0xBE)
	{
		BeebEmCommon::Cycles++;
	}
}

/*----------------------------------------------------------------------------*/
void CPU6502::DoIntCheck()
{
	if (!IntDue)
	{
		IntDue = (BeebEmCommon::intStatus != 0);
		if (!IntDue)
		{
			BeebEmCommon::CyclesToInt = NO_TIMER_INT_DUE;
		}
		else if (BeebEmCommon::CyclesToInt == NO_TIMER_INT_DUE)
		{
			// Non-timer interrupt has occurred
			BeebEmCommon::CyclesToInt = 0;
		}
	}
}

/*----------------------------------------------------------------------------*/
// IO read + write take extra cycle & require sync with 1MHz clock (taken 
// from Model-b - have not seen this documented anywhere)
void CPU6502::SyncIO()
{
	if ((BeebEmCommon::TotalCycles+BeebEmCommon::Cycles) & 1)
	{
		BeebEmCommon::Cycles++;
		IOCycles = 1;
		PollVIAs(1);
	}
	else
	{
		IOCycles = 0;
	}
}

void CPU6502::AdjustForIORead()
{
	BeebEmCommon::Cycles++;
	IOCycles += 1;
	PollVIAs(1);
}
void CPU6502::AdjustForIOWrite()
{
	BeebEmCommon::Cycles++;
	IOCycles += 1;
	PollVIAs(1);
	DoIntCheck();
}
/*----------------------------------------------------------------------------*/
void CPU6502::AdvanceCyclesForMemRead()
{
	// Advance VIAs to point where mem read happens
	BeebEmCommon::Cycles += CyclesToMemRead[CurrentInstruction];
	PollVIAs(CyclesToMemRead[CurrentInstruction]);
	
	// Check if interrupt should be taken if instruction does
	// a read but not a write (write instructions checked below).
	if (CyclesToMemRead[CurrentInstruction] != 0 &&
		CyclesToMemWrite[CurrentInstruction] == 0)
	{
		DoIntCheck();
	}
}
void CPU6502::AdvanceCyclesForMemWrite()
{
	// Advance VIAs to point where mem write happens
	BeebEmCommon::Cycles += CyclesToMemWrite[CurrentInstruction];
	PollVIAs(CyclesToMemWrite[CurrentInstruction]);
	
	DoIntCheck();
}

/*----------------------------------------------------------------------------*/

inline int CPU6502::SignExtendByte(signed char in) {
  /*if (in & 0x80) return(in | 0xffffff00); else return(in); */
  /* I think this should sign extend by virtue of the casts - gcc does anyway - the code
  above will definitly do the trick */
  return((int)in);
} /* SignExtendByte */

/*----------------------------------------------------------------------------*/
/* Set the Z flag if 'in' is 0, and N if bit 7 is set - leave all other bits  */
/* untouched.                                                                 */
inline void CPU6502::SetPSRZN(const unsigned char in) {
  PSR&=~(FlagZ | FlagN);
  PSR|=((in==0)<<1) | (in & 128);
}; /* SetPSRZN */

/*----------------------------------------------------------------------------*/
/* Note: n is 128 for true - not 1                                            */
inline void CPU6502::SetPSR(int mask,int c,int z,int i,int d,int b, int v, int n) {
  PSR&=~mask;
  PSR|=c | (z<<1) | (i<<2) | (d<<3) | (b<<4) | (v<<6) | n;
} /* SetPSR */

/*----------------------------------------------------------------------------*/
/* NOTE!!!!! n is 128 or 0 - not 1 or 0                                       */
inline void CPU6502::SetPSRCZN(int c,int z, int n) {
  PSR&=~(FlagC | FlagZ | FlagN);
  PSR|=c | (z<<1) | n;
} /* SetPSRCZN */

/*----------------------------------------------------------------------------*/
void CPU6502::DumpRegs() {
  static char FlagNames[]="CZIDB-VNczidb-vn";
  int FlagNum;

  fprintf(stderr,"  PC=0x%x A=0x%x X=0x%x Y=0x%x S=0x%x PSR=0x%x=",
    BeebEmCommon::ProgramCounter,Accumulator,XReg,YReg,StackReg,PSR);
  for(FlagNum=0;FlagNum<8;FlagNum++)
    fputc(FlagNames[FlagNum+8*((PSR & (1<<FlagNum))==0)],stderr);
  fputc('\n',stderr);
} /* DumpRegs */

/*----------------------------------------------------------------------------*/
inline void CPU6502::Push(unsigned char ToPush) {
  BEEBWRITEMEM_DIRECT(0x100+StackReg,ToPush);
  StackReg--;
} /* Push */

/*----------------------------------------------------------------------------*/
inline unsigned char CPU6502::Pop(void) {
  StackReg++;
  return(WholeRam[0x100+StackReg]);
} /* Pop */

/*----------------------------------------------------------------------------*/
inline void CPU6502::PushWord(int16 topush) {
  Push((topush>>8) & 255);
  Push(topush & 255);
} /* PushWord */

/*----------------------------------------------------------------------------*/
inline int16 CPU6502::PopWord() {
  int16 RetValue;

  RetValue=Pop();
  RetValue|=(Pop()<<8);
  return(RetValue);
} /* PopWord */

/*-------------------------------------------------------------------------*/
/* Relative addressing mode handler                                        */
inline int16 CPU6502::RelAddrModeHandler_Data() {
  int EffectiveAddress;

  /* For branches - is this correct - i.e. is the program counter incremented
     at the correct time? */
  EffectiveAddress=SignExtendByte((signed char)ReadPaged(BeebEmCommon::ProgramCounter++));
  EffectiveAddress+=BeebEmCommon::ProgramCounter;

  return(EffectiveAddress);
} /* RelAddrModeHandler */

/*----------------------------------------------------------------------------*/
inline void CPU6502::ADCInstrHandler(int16 operand) {
  /* NOTE! Not sure about C and V flags */
  int TmpResultV,TmpResultC;
  if (!GETDFLAG) {
    TmpResultC=Accumulator+operand+GETCFLAG;
    TmpResultV=(signed char)Accumulator+(signed char)operand+GETCFLAG;
    Accumulator=TmpResultC & 255;
    SetPSR(FlagC | FlagZ | FlagV | FlagN, (TmpResultC & 256)>0,Accumulator==0,0,0,0,((Accumulator & 128)>0) ^ (TmpResultV<0),(Accumulator & 128));
  } else {
    int ZFlag=0,NFlag=0,CFlag=0,VFlag=0;
    int TmpResult,TmpCarry=0;
    int ln,hn;

    /* Z flag determined from 2's compl result, not BCD result! */
    TmpResult=Accumulator+operand+GETCFLAG;
    ZFlag=((TmpResult & 0xff)==0);

    ln=(Accumulator & 0xf)+(operand & 0xf)+GETCFLAG;
    if (ln>9) {
      ln += 6;
      ln &= 0xf;
      TmpCarry=0x10;
    }
    hn=(Accumulator & 0xf0)+(operand & 0xf0)+TmpCarry;
    /* N and V flags are determined before high nibble is adjusted.
       NOTE: V is not always correct */
    NFlag=hn & 128;
    VFlag=(hn ^ Accumulator) & 128 && !((Accumulator ^ operand) & 128);
    if (hn>0x90) {
      hn += 0x60;
      hn &= 0xf0;
      CFlag=1;
    }
    Accumulator=hn|ln;
	ZFlag=(Accumulator==0);
	NFlag=(Accumulator&128);
    SetPSR(FlagC | FlagZ | FlagV | FlagN,CFlag,ZFlag,0,0,0,VFlag,NFlag);
  }
} /* ADCInstrHandler */

/*----------------------------------------------------------------------------*/
inline void CPU6502::ANDInstrHandler(int16 operand) {
  Accumulator=Accumulator & operand;
  PSR&=~(FlagZ | FlagN);
  PSR|=((Accumulator==0)<<1) | (Accumulator & 128);
} /* ANDInstrHandler */

inline void CPU6502::ASLInstrHandler(int16 address) {
	unsigned char oldVal,newVal;
	oldVal=ReadPaged(address);
	BeebEmCommon::Cycles+=1;
	PollVIAs(1);
	WritePaged(address,oldVal);
	newVal=(((unsigned int)oldVal)<<1) & 254;
	BeebEmCommon::Cycles+=CyclesToMemWrite[CurrentInstruction] - 1;
	PollVIAs(CyclesToMemWrite[CurrentInstruction] - 1);
	WritePaged(address,newVal);
	SetPSRCZN((oldVal & 128)>0, newVal==0,newVal & 128);
} /* ASLInstrHandler */

inline void CPU6502::TRBInstrHandler(int16 address) {
	unsigned char oldVal,newVal;
	oldVal=ReadPaged(address);
	newVal=(Accumulator ^ 255) & oldVal;
    WritePaged(address,newVal);
    PSR&=253;
	PSR|=((Accumulator & oldVal)==0) ? 2 : 0;
} // TRBInstrHandler

inline void CPU6502::TSBInstrHandler(int16 address) {
	unsigned char oldVal,newVal;
	oldVal=ReadPaged(address);
	newVal=Accumulator | oldVal;
    WritePaged(address,newVal);
    PSR&=253;
	PSR|=((Accumulator & oldVal)==0) ? 2 : 0;
} // TSBInstrHandler

inline void CPU6502::ASLInstrHandler_Acc(void) {
  unsigned char oldVal,newVal;
  /* Accumulator */
  oldVal=Accumulator;
  Accumulator=newVal=(((unsigned int)Accumulator)<<1) & 254;
  SetPSRCZN((oldVal & 128)>0, newVal==0,newVal & 128);
} /* ASLInstrHandler_Acc */

inline void CPU6502::BCCInstrHandler(void) {
  if (!GETCFLAG) {
    BeebEmCommon::ProgramCounter=RelAddrModeHandler_Data();
    Branched=1;
  } else BeebEmCommon::ProgramCounter++;
} /* BCCInstrHandler */

inline void CPU6502::BCSInstrHandler(void) {
  if (GETCFLAG) {
    BeebEmCommon::ProgramCounter=RelAddrModeHandler_Data();
    Branched=1;
  } else BeebEmCommon::ProgramCounter++;
} /* BCSInstrHandler */

inline void CPU6502::BEQInstrHandler(void) {
  if (GETZFLAG) {
    BeebEmCommon::ProgramCounter=RelAddrModeHandler_Data();
    Branched=1;
  } else BeebEmCommon::ProgramCounter++;
} /* BEQInstrHandler */

inline void CPU6502::BITInstrHandler(int16 operand) {
  PSR&=~(FlagZ | FlagN | FlagV);
  /* z if result 0, and NV to top bits of operand */
  PSR|=(((Accumulator & operand)==0)<<1) | (operand & 192);
} /* BITInstrHandler */

inline void CPU6502::BITImmedInstrHandler(int16 operand) {
	PSR&=~FlagZ;
	/* z if result 0, and NV to top bits of operand */
	PSR|=(((Accumulator & operand)==0)<<1);
} /* BITImmedInstrHandler */

inline void CPU6502::BMIInstrHandler(void) {
  if (GETNFLAG) {
    BeebEmCommon::ProgramCounter=RelAddrModeHandler_Data();
    Branched=1;
  } else BeebEmCommon::ProgramCounter++;
} /* BMIInstrHandler */

inline void CPU6502::BNEInstrHandler(void) {
  if (!GETZFLAG) {
    BeebEmCommon::ProgramCounter=RelAddrModeHandler_Data();
    Branched=1;
  } else BeebEmCommon::ProgramCounter++;
} /* BNEInstrHandler */

inline void CPU6502::BPLInstrHandler(void) {
  if (!GETNFLAG) {
    BeebEmCommon::ProgramCounter=RelAddrModeHandler_Data();
    Branched=1;
  } else BeebEmCommon::ProgramCounter++;
}; /* BPLInstrHandler */

inline void CPU6502::BRKInstrHandler(void) {
  if (CPUDebug) {
  fprintf(stderr,"BRK Instruction at 0x%04x after %i instructions. ACCON: 0x%02x ROMSEL: 0x%02x",BeebEmCommon::ProgramCounter,InstrCount,ACCCON,PagedRomReg);
  //fclose(InstrLog);
  exit(1); 
  }
  PushWord(BeebEmCommon::ProgramCounter+1);
  SetPSR(FlagB,0,0,0,0,1,0,0); /* Set B before pushing */
  Push(PSR);
  SetPSR(FlagI,0,0,1,0,0,0,0); /* Set I after pushing - see Birnbaum */
  BeebEmCommon::ProgramCounter=BeebReadMem(0xfffe) | (BeebReadMem(0xffff)<<8);
} /* BRKInstrHandler */

inline void CPU6502::BVCInstrHandler(void) {
  if (!GETVFLAG) {
    BeebEmCommon::ProgramCounter=RelAddrModeHandler_Data();
    Branched=1;
  } else BeebEmCommon::ProgramCounter++;
} /* BVCInstrHandler */

inline void CPU6502::BVSInstrHandler(void) {
  if (GETVFLAG) {
    BeebEmCommon::ProgramCounter=RelAddrModeHandler_Data();
    Branched=1;
  } else BeebEmCommon::ProgramCounter++;
} /* BVSInstrHandler */

inline void CPU6502::BRAInstrHandler(void) {
    BeebEmCommon::ProgramCounter=RelAddrModeHandler_Data();
    Branched=1;
} /* BRAnstrHandler */

inline void CPU6502::CMPInstrHandler(int16 operand) {
  /* NOTE! Should we consult D flag ? */
  unsigned char result=Accumulator-operand;
  unsigned char CFlag;
  CFlag=0; if (Accumulator>=operand) CFlag=FlagC;
  SetPSRCZN(CFlag,Accumulator==operand,result & 128);
} /* CMPInstrHandler */

inline void CPU6502::CPXInstrHandler(int16 operand) {
  unsigned char result=(XReg-operand);
  SetPSRCZN(XReg>=operand,XReg==operand,result & 128);
} /* CPXInstrHandler */

inline void CPU6502::CPYInstrHandler(int16 operand) {
  unsigned char result=(YReg-operand);
  SetPSRCZN(YReg>=operand,YReg==operand,result & 128);
} /* CPYInstrHandler */

inline void CPU6502::DECInstrHandler(int16 address) {
	unsigned char val;
	val=ReadPaged(address);
	BeebEmCommon::Cycles+=1;
	PollVIAs(1);
	WritePaged(address,val);
	val=(val-1);
	BeebEmCommon::Cycles+=CyclesToMemWrite[CurrentInstruction] - 1;
	PollVIAs(CyclesToMemWrite[CurrentInstruction] - 1);
	WritePaged(address,val);
	SetPSRZN(val);
} /* DECInstrHandler */

inline void CPU6502::DEXInstrHandler(void) {
  XReg=(XReg-1) & 255;
  SetPSRZN(XReg);
} /* DEXInstrHandler */

inline void CPU6502::DEAInstrHandler(void) {
  Accumulator=(Accumulator-1) & 255;
  SetPSRZN(Accumulator);
} /* DEAInstrHandler */

inline void CPU6502::EORInstrHandler(int16 operand) {
  Accumulator^=operand;
  SetPSRZN(Accumulator);
} /* EORInstrHandler */

inline void CPU6502::INCInstrHandler(int16 address) {
	unsigned char val;
	val=ReadPaged(address);
	BeebEmCommon::Cycles+=1;
	PollVIAs(1);
	WritePaged(address,val);
	val=(val+1) & 255;
	BeebEmCommon::Cycles+=CyclesToMemWrite[CurrentInstruction] - 1;
	PollVIAs(CyclesToMemWrite[CurrentInstruction] - 1);
	WritePaged(address,val);
	SetPSRZN(val);
} /* INCInstrHandler */

inline void CPU6502::INXInstrHandler(void) {
  XReg+=1;
  XReg&=255;
  SetPSRZN(XReg);
} /* INXInstrHandler */

inline void CPU6502::INAInstrHandler(void) {
  Accumulator+=1;
  Accumulator&=255;
  SetPSRZN(Accumulator);
} /* INAInstrHandler */

inline void CPU6502::JSRInstrHandler(int16 address) {
  PushWord(BeebEmCommon::ProgramCounter-1);
  BeebEmCommon::ProgramCounter=address;
/*  if (BeebEmCommon::ProgramCounter==0xffdd) {
	  // OSCLI logging for elite debugging
	  unsigned char *bptr;
	  char pcbuf[256]; char *pcptr=pcbuf;
	  int blk=((YReg*256)+XReg);
	  bptr=WholeRam+((WholeRam[blk+1]*256)+WholeRam[blk]);
  	  while((*bptr != 13) && ((pcptr-pcbuf)<254)) {
		  *pcptr=*bptr; pcptr++;bptr++; 
	  } 
	  *pcptr=0;
	  fprintf(osclilog,"%s\n",pcbuf);
  }
  if (BeebEmCommon::ProgramCounter==0xffdd) {
	  char errstr[250];
	  sprintf(errstr,"OSFILE called\n");
	  MessageBox(GETHWND,errstr,"BBC Emulator",MB_OKCANCEL|MB_ICONERROR);
  }*/

} /* JSRInstrHandler */

inline void CPU6502::LDAInstrHandler(int16 operand) {
  Accumulator=operand;
  SetPSRZN(Accumulator);
} /* LDAInstrHandler */

inline void CPU6502::LDXInstrHandler(int16 operand) {
  XReg=operand;
  SetPSRZN(XReg);
} /* LDXInstrHandler */

inline void CPU6502::LDYInstrHandler(int16 operand) {
  YReg=operand;
  SetPSRZN(YReg);
} /* LDYInstrHandler */

inline void CPU6502::LSRInstrHandler(int16 address) {
	unsigned char oldVal,newVal;
	oldVal=ReadPaged(address);
	BeebEmCommon::Cycles+=1;
	PollVIAs(1);
	WritePaged(address,oldVal);
	newVal=(((unsigned int)oldVal)>>1) & 127;
	BeebEmCommon::Cycles+=CyclesToMemWrite[CurrentInstruction] - 1;
	PollVIAs(CyclesToMemWrite[CurrentInstruction] - 1);
	WritePaged(address,newVal);
	SetPSRCZN((oldVal & 1)>0, newVal==0,0);
} /* LSRInstrHandler */

inline void CPU6502::LSRInstrHandler_Acc(void) {
  unsigned char oldVal,newVal;
  /* Accumulator */
  oldVal=Accumulator;
  Accumulator=newVal=(((unsigned int)Accumulator)>>1) & 127;
  SetPSRCZN((oldVal & 1)>0, newVal==0,0);
} /* LSRInstrHandler_Acc */

inline void CPU6502::ORAInstrHandler(int16 operand) {
  Accumulator=Accumulator | operand;
  SetPSRZN(Accumulator);
} /* ORAInstrHandler */

inline void CPU6502::ROLInstrHandler(int16 address) {
	unsigned char oldVal,newVal;
	oldVal=ReadPaged(address);
	BeebEmCommon::Cycles+=1;
	PollVIAs(1);
	WritePaged(address,oldVal);
	newVal=((unsigned int)oldVal<<1) & 254;
	newVal+=GETCFLAG;
	BeebEmCommon::Cycles+=CyclesToMemWrite[CurrentInstruction] - 1;
	PollVIAs(CyclesToMemWrite[CurrentInstruction] - 1);
	WritePaged(address,newVal);
	SetPSRCZN((oldVal & 128)>0,newVal==0,newVal & 128);
} /* ROLInstrHandler */

inline void CPU6502::ROLInstrHandler_Acc(void) {
	unsigned char oldVal,newVal;
	
	oldVal=Accumulator;
	newVal=((unsigned int)oldVal<<1) & 254;
	newVal+=GETCFLAG;
	Accumulator=newVal;
	SetPSRCZN((oldVal & 128)>0,newVal==0,newVal & 128);
} /* ROLInstrHandler_Acc */

inline void CPU6502::RORInstrHandler(int16 address) {
	unsigned char oldVal,newVal;
	oldVal=ReadPaged(address);
	BeebEmCommon::Cycles+=1;
	PollVIAs(1);
	WritePaged(address,oldVal);
	newVal=((unsigned int)oldVal>>1) & 127;
	newVal+=GETCFLAG*128;
	BeebEmCommon::Cycles+=CyclesToMemWrite[CurrentInstruction] - 1;
	PollVIAs(CyclesToMemWrite[CurrentInstruction] - 1);
	WritePaged(address,newVal);
	SetPSRCZN(oldVal & 1,newVal==0,newVal & 128);
} /* RORInstrHandler */

inline void CPU6502::RORInstrHandler_Acc(void) {
  unsigned char oldVal,newVal;

  oldVal=Accumulator;
  newVal=((unsigned int)oldVal>>1) & 127;
  newVal+=GETCFLAG*128;
  Accumulator=newVal;
  SetPSRCZN(oldVal & 1,newVal==0,newVal & 128);
} /* RORInstrHandler_Acc */

inline void CPU6502::SBCInstrHandler(int16 operand) {
  /* NOTE! Not sure about C and V flags */
  int TmpResultV,TmpResultC;
  unsigned char nhn,nln;
  if (!GETDFLAG) {
    TmpResultV=(signed char)Accumulator-(signed char)operand-(1-GETCFLAG);
    TmpResultC=Accumulator-operand-(1-GETCFLAG);
    Accumulator=TmpResultC & 255;
    SetPSR(FlagC | FlagZ | FlagV | FlagN, TmpResultC>=0,Accumulator==0,0,0,0,
      ((Accumulator & 128)>0) ^ ((TmpResultV & 256)!=0),(Accumulator & 128));
  } else {
    int ZFlag=0,NFlag=0,CFlag=1,VFlag=0;
    int TmpResult,TmpCarry=0;
    int ln,hn,oln,ohn;
	nhn=(Accumulator>>4)&15; nln=Accumulator & 15;

    /* Z flag determined from 2's compl result, not BCD result! */
    TmpResult=Accumulator-operand-(1-GETCFLAG);
    ZFlag=((TmpResult & 0xff)==0);

	ohn=operand & 0xf0; oln = operand & 0xf;
	if ((oln>9) && ((Accumulator&15)<10)) { oln-=10; ohn+=0x10; } 
	// promote the lower nibble to the next ten, and increase the higher nibble
    ln=(Accumulator & 0xf)-oln-(1-GETCFLAG);
    if (ln<0) {
	  if ((Accumulator & 15)<10) ln-=6;
      ln&=0xf;
      TmpCarry=0x10;
    }
    hn=(Accumulator & 0xf0)-ohn-TmpCarry;
    /* N and V flags are determined before high nibble is adjusted.
       NOTE: V is not always correct */
    NFlag=hn & 128;
	TmpResultV=(signed char)Accumulator-(signed char)operand-(1-GETCFLAG);
	if ((TmpResultV<-128)||(TmpResultV>127)) VFlag=1; else VFlag=0;
    if (hn<0) {
      hn-=0x60;
      hn&=0xf0;
      CFlag=0;
    }
    Accumulator=hn|ln;
	if (Accumulator==0) ZFlag=1;
	NFlag=(hn &128);
	CFlag=(TmpResult&256)==0;
    SetPSR(FlagC | FlagZ | FlagV | FlagN,CFlag,ZFlag,0,0,0,VFlag,NFlag);
  }
} /* SBCInstrHandler */

inline void CPU6502::STXInstrHandler(int16 address) {
  WritePaged(address,XReg);
} /* STXInstrHandler */

inline void CPU6502::STYInstrHandler(int16 address) {
  WritePaged(address,YReg);
} /* STYInstrHandler */

inline void CPU6502::BadInstrHandler(int opcode) {
	if (!BeebEmCommon::IgnoreIllegalInstructions)
	{
#ifdef WIN32
		char errstr[250];
		sprintf(errstr,"Unsupported 6502 instruction 0x%02X at 0x%04X\n"
			"  OK - instruction will be skipped\n"
			"  Cancel - dump memory and exit",opcode,BeebEmCommon::ProgramCounter-1);
		if (MessageBox(GETHWND,errstr,"BBC Emulator",MB_OKCANCEL|MB_ICONERROR) == IDCANCEL)
		{
			beebmem_dumpstate();
			exit(0);
		}
#else
		fprintf(stderr,"Bad instruction handler called:\n");
		DumpRegs();
		fprintf(stderr,"Dumping main memory\n");
		beebmem_dumpstate();
		// abort();
#endif
	}

	/* Do not know what the instruction does but can guess if it is 1,2 or 3 bytes */
	switch (opcode & 0xf)
	{
	/* One byte instructions */
	case 0xa:
		break;

	/* Two byte instructions */
	case 0x0:
	case 0x2:  /* Inst 0xf2 causes the 6502 to hang! Try it on your BBC Micro */
	case 0x3:
	case 0x4:
	case 0x7:
	case 0x9:
	case 0xb:
		BeebEmCommon::ProgramCounter++;
		break;

	/* Three byte instructions */
	case 0xc:
	case 0xe:
	case 0xf:
		BeebEmCommon::ProgramCounter+=2;
		break;
	}
} /* BadInstrHandler */

/*-------------------------------------------------------------------------*/
/* Absolute  addressing mode handler                                       */
inline int16 CPU6502::AbsAddrModeHandler_Data(void) {
  int FullAddress;

  /* Get the address from after the instruction */
  
  GETTWOBYTEFROMPC(FullAddress)

  /* And then read it */
  return(ReadPaged(FullAddress));
} /* AbsAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Absolute  addressing mode handler                                       */
inline int16 CPU6502::AbsAddrModeHandler_Address(void) {
  int FullAddress;

  /* Get the address from after the instruction */
  GETTWOBYTEFROMPC(FullAddress)

  /* And then read it */
  return(FullAddress);
} /* AbsAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Zero page addressing mode handler                                       */
inline int16 CPU6502::ZeroPgAddrModeHandler_Address(void) {
  return(ReadPaged(BeebEmCommon::ProgramCounter++));
} /* ZeroPgAddrModeHandler_Address */

/*-------------------------------------------------------------------------*/
/* Indexed with X preinc addressing mode handler                           */
inline int16 CPU6502::IndXAddrModeHandler_Data(void) {
  unsigned char ZeroPageAddress;
  int EffectiveAddress;

  ZeroPageAddress=(ReadPaged(BeebEmCommon::ProgramCounter++)+XReg) & 255;

  EffectiveAddress=WholeRam[ZeroPageAddress] | (WholeRam[ZeroPageAddress+1]<<8);
  return(ReadPaged(EffectiveAddress));
} /* IndXAddrModeHandler_Data */

/*-------------------------------------------------------------------------*/
/* Indexed with X preinc addressing mode handler                           */
inline int16 CPU6502::IndXAddrModeHandler_Address(void) {
  unsigned char ZeroPageAddress;
  int EffectiveAddress;

  ZeroPageAddress=(ReadPaged(BeebEmCommon::ProgramCounter++)+XReg) & 255;

  EffectiveAddress=WholeRam[ZeroPageAddress] | (WholeRam[ZeroPageAddress+1]<<8);
  return(EffectiveAddress);
} /* IndXAddrModeHandler_Address */

/*-------------------------------------------------------------------------*/
/* Indexed with Y postinc addressing mode handler                          */
inline int16 CPU6502::IndYAddrModeHandler_Data(void) {
  int EffectiveAddress;
  unsigned char ZPAddr=ReadPaged(BeebEmCommon::ProgramCounter++);
  EffectiveAddress=WholeRam[ZPAddr]+YReg;
  if (EffectiveAddress>0xff) Carried();
  EffectiveAddress+=(WholeRam[ZPAddr+1]<<8);

  return(ReadPaged(EffectiveAddress));
} /* IndYAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Indexed with Y postinc addressing mode handler                          */
inline int16 CPU6502::IndYAddrModeHandler_Address(void) {
  int EffectiveAddress;
  unsigned char ZPAddr=ReadPaged(BeebEmCommon::ProgramCounter++);
  EffectiveAddress=WholeRam[ZPAddr]+YReg;
  if (EffectiveAddress>0xff) Carried();
  EffectiveAddress+=(WholeRam[ZPAddr+1]<<8);

  return(EffectiveAddress);
} /* IndYAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Zero page wih X offset addressing mode handler                          */
inline int16 CPU6502::ZeroPgXAddrModeHandler_Data(void) {
  int EffectiveAddress;
  EffectiveAddress=(ReadPaged(BeebEmCommon::ProgramCounter++)+XReg) & 255;
  return(WholeRam[EffectiveAddress]);
} /* ZeroPgXAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Zero page wih X offset addressing mode handler                          */
inline int16 CPU6502::ZeroPgXAddrModeHandler_Address(void) {
  int EffectiveAddress;
  EffectiveAddress=(ReadPaged(BeebEmCommon::ProgramCounter++)+XReg) & 255;
  return(EffectiveAddress);
} /* ZeroPgXAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Absolute with X offset addressing mode handler                          */
inline int16 CPU6502::AbsXAddrModeHandler_Data(void) {
  int EffectiveAddress;
  GETTWOBYTEFROMPC(EffectiveAddress);
  if ((EffectiveAddress & 0xff00)!=((EffectiveAddress+XReg) & 0xff00)) Carried();
  EffectiveAddress+=XReg;
  EffectiveAddress&=0xffff;

  return(ReadPaged(EffectiveAddress));
} /* AbsXAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Absolute with X offset addressing mode handler                          */
inline int16 CPU6502::AbsXAddrModeHandler_Address(void) {
  int EffectiveAddress;
  GETTWOBYTEFROMPC(EffectiveAddress)
  if ((EffectiveAddress & 0xff00)!=((EffectiveAddress+XReg) & 0xff00)) Carried();
  EffectiveAddress+=XReg;
  EffectiveAddress&=0xffff;

  return(EffectiveAddress);
} /* AbsXAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Absolute with Y offset addressing mode handler                          */
inline int16 CPU6502::AbsYAddrModeHandler_Data(void) {
  int EffectiveAddress;
  GETTWOBYTEFROMPC(EffectiveAddress);
  if ((EffectiveAddress & 0xff00)!=((EffectiveAddress+YReg) & 0xff00)) Carried();
  EffectiveAddress+=YReg;
  EffectiveAddress&=0xffff;

  return(ReadPaged(EffectiveAddress));
} /* AbsYAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Absolute with Y offset addressing mode handler                          */
inline int16 CPU6502::AbsYAddrModeHandler_Address(void) {
  int EffectiveAddress;
  GETTWOBYTEFROMPC(EffectiveAddress)
  if ((EffectiveAddress & 0xff00)!=((EffectiveAddress+YReg) & 0xff00)) Carried();
  EffectiveAddress+=YReg;
  EffectiveAddress&=0xffff;

  return(EffectiveAddress);
} /* AbsYAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Indirect addressing mode handler                                        */
inline int16 CPU6502::IndAddrModeHandler_Address(void) {
  /* For jump indirect only */
  int VectorLocation;
  int EffectiveAddress;

  GETTWOBYTEFROMPC(VectorLocation)

  /* Ok kiddies, deliberate bug time.
  According to my BBC Master Reference Manual Part 2
  the 6502 has a bug concerning this addressing mode and VectorLocation==xxFF
  so, we're going to emulate that bug -- Richard Gellman */
  if ((VectorLocation & 0xff)!=0xff || MachineType==3) {
   EffectiveAddress=ReadPaged(VectorLocation);
   EffectiveAddress|=ReadPaged(VectorLocation+1) << 8; }
  else {
   EffectiveAddress=ReadPaged(VectorLocation);
   EffectiveAddress|=ReadPaged(VectorLocation-255) << 8;
  }
  return(EffectiveAddress);
} /* IndAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Zero page Indirect addressing mode handler                                        */
inline int16 CPU6502::ZPIndAddrModeHandler_Address(void) {
  int VectorLocation;
  int EffectiveAddress;

  VectorLocation=ReadPaged(BeebEmCommon::ProgramCounter++);
  EffectiveAddress=ReadPaged(VectorLocation)+(ReadPaged(VectorLocation+1)<<8);

   // EffectiveAddress|=ReadPaged(VectorLocation+1) << 8; }
  return(EffectiveAddress);
} /* ZPIndAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Zero page Indirect addressing mode handler                                        */
inline int16 CPU6502::ZPIndAddrModeHandler_Data(void) {
  int VectorLocation;
  int EffectiveAddress;

  VectorLocation=ReadPaged(BeebEmCommon::ProgramCounter++);
  EffectiveAddress=ReadPaged(VectorLocation)+(ReadPaged(VectorLocation+1)<<8);

   // EffectiveAddress|=ReadPaged(VectorLocation+1) << 8; }
  return(ReadPaged(EffectiveAddress));
} /* ZPIndAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Pre-indexed absolute Indirect addressing mode handler                                        */
inline int16 CPU6502::IndAddrXModeHandler_Address(void) {
  /* For jump indirect only */
  int VectorLocation;
  int EffectiveAddress;

  GETTWOBYTEFROMPC(VectorLocation)

  EffectiveAddress=ReadPaged(VectorLocation+XReg);
  EffectiveAddress|=ReadPaged(VectorLocation+1+XReg) << 8; 
  EffectiveAddress&=0xffff;
   // EffectiveAddress|=ReadPaged(VectorLocation+1) << 8; }
  return(EffectiveAddress);
} /* ZPIndAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Zero page with Y offset addressing mode handler                         */
inline int16 CPU6502::ZeroPgYAddrModeHandler_Data(void) {
  int EffectiveAddress;
  EffectiveAddress=(ReadPaged(BeebEmCommon::ProgramCounter++)+YReg) & 255;
  return(WholeRam[EffectiveAddress]);
} /* ZeroPgYAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Zero page with Y offset addressing mode handler                         */
inline int16 CPU6502::ZeroPgYAddrModeHandler_Address(void) {
  int EffectiveAddress;
  EffectiveAddress=(ReadPaged(BeebEmCommon::ProgramCounter++)+YReg) & 255;
  return(EffectiveAddress);
} /* ZeroPgYAddrModeHandler */

/*-------------------------------------------------------------------------*/
/* Initialise 6502core                                                     */
void CPU6502::Init6502core(void) {
  BeebEmCommon::ProgramCounter=BeebReadMem(0xfffc) | (BeebReadMem(0xfffd)<<8);
  Accumulator=XReg=YReg=0; /* For consistancy of execution */
  StackReg=0xff; /* Initial value ? */
  PSR=FlagI; /* Interrupts off for starters */

  BeebEmCommon::intStatus=0;
  BeebEmCommon::NMIStatus=0;
  BeebEmCommon::NMILock=0;
} /* Init6502core */

/*-------------------------------------------------------------------------*/
void CPU6502::DoInterrupt(void) {
  PushWord(BeebEmCommon::ProgramCounter);
  Push(PSR & ~FlagB);
  BeebEmCommon::ProgramCounter=BeebReadMem(0xfffe) | (BeebReadMem(0xffff)<<8);
  SetPSR(FlagI,0,0,1,0,0,0,0);
  IRQCycles=7;
} /* DoInterrupt */

/*-------------------------------------------------------------------------*/
void CPU6502::DoNMI(void) {
  BeebEmCommon::NMILock=1;
  PushWord(BeebEmCommon::ProgramCounter);
  Push(PSR);
  BeebEmCommon::ProgramCounter=BeebReadMem(0xfffa) | (BeebReadMem(0xfffb)<<8);
  SetPSR(FlagI,0,0,1,0,0,0,0); /* Normal interrupts should be disabled during NMI ? */
  IRQCycles=7;
} /* DoNMI */

int i186_execute(int num_cycles);
int DebugDisassembleInstruction(int addr, bool host, char *opstr);

void CPU6502::Dis6502(char *str)
{
    DebugDisassembleInstruction(BeebEmCommon::ProgramCounter, true, str);
	
	sprintf(str + strlen(str), "%02X %02X %02X ", Accumulator, XReg, YReg);
	
    sprintf(str + strlen(str), (PSR & FlagC) ? "C" : ".");
	sprintf(str + strlen(str), (PSR & FlagZ) ? "Z" : ".");
	sprintf(str + strlen(str), (PSR & FlagI) ? "I" : ".");
	sprintf(str + strlen(str), (PSR & FlagD) ? "D" : ".");
	sprintf(str + strlen(str), (PSR & FlagB) ? "B" : ".");
	sprintf(str + strlen(str), (PSR & FlagV) ? "V" : ".");
	sprintf(str + strlen(str), (PSR & FlagN) ? "N" : ".");
}

void CPU6502::MemoryDump6502(int addr, int count)
{
	int a, b;
	int s, e;
	int v;
	char info[80];
	
	s = addr & 0xffff0;
	e = (addr + count - 1) | 0xf;
	if (e > 0xfffff)
		e = 0xfffff;
	for (a = s; a < e; a += 16)
	{
		sprintf(info, "%04X  ", a);
		
		for (b = 0; b < 16; ++b)
		{
			sprintf(info+strlen(info), "%02X ", DebugReadMem(a+b, true));
		}
		
		for (b = 0; b < 16; ++b)
		{
			v = DebugReadMem(a+b, true);
			if (v < 32 || v > 127)
				v = '.';
			sprintf(info+strlen(info), "%c", v);
		}
		
		BeebEmLog::writeLog("%s\n", info);
	}
	
}

/*-------------------------------------------------------------------------*/
/* Execute one 6502 instruction, move program counter on                   */
void CPU6502::Exec6502Instruction(CArm* arm) {
  static int OldNMIStatus;
  int BadCount=0;
  int OldPC;
  int loop,loopc;
  bool iFlagJustCleared;
  bool iFlagJustSet;
   loopc=(DebugEnabled ? 1 : 1024); // Makes debug window more responsive
 
    for(loop=0;loop<loopc;loop++) {

	if (DebugEnabled && !DebugDisassembler(BeebEmCommon::ProgramCounter,Accumulator,XReg,YReg,PSR,true))
		continue;
	  
	  z80_execute();

	  if (Enable_Arm)
	  {
		  arm->exec(4);
	  }
	  if (Tube186Enabled)
		  i186_execute(12 * 4);
	  
// 	  if (BeebEmCommon::ProgramCounter == 0x09c0) 
//	  {
//		  BeebEmCommon::trace = 100;
//	  }

	  if (BeebEmCommon::trace)
	  {
		  char str[128];
		  Dis6502(str);
		  BeebEmLog::writeLog("%s\n", str);
		  BeebEmCommon::trace--;
	  }
	  
  
	  
//	  if (BeebEmCommon::trace && (BeebEmCommon::ProgramCounter >= 0x8000) && (BeebEmCommon::ProgramCounter <= 0xa000) && ((PagedRomReg & 0x0f) == 8))
//	  {
//		  char str[128];
//		  Dis6502(str);
//		  BeebEmLog::writeLog("%s\n", str);
//		  BeebEmCommon::trace--;
//	  }
	  
 
// if (BeebEmCommon::ProgramCounter == 0x80c2) 
//  {
//	  MemoryDump6502(0x280, 0x20);
//	  BeebEmCommon::trace = 1;
//  }
 
 if (trace_186)
  {
      char str[128];
      Dis6502(str);
      BeebEmLog::writeLog("%s\n", str);
  }
	  
 Branched=0;
 iFlagJustCleared=false;
 iFlagJustSet=false;
 BeebEmCommon::Cycles=0;
 IOCycles = 0;
 BadCount=0;
 IntDue = false;
  
 /* Read an instruction and post inc program couter */
  OldPC=BeebEmCommon::ProgramCounter;
  BeebEmCommon::PrePC=BeebEmCommon::ProgramCounter;
  CurrentInstruction=ReadPaged(BeebEmCommon::ProgramCounter++);
  
  // cout << "Fetch at " << hex << (BeebEmCommon::ProgramCounter-1) << " giving 0x" << CurrentInstruction << dec << "\n"; 

  // Advance VIAs to point where mem read happens
  ViaCycles=0;
  AdvanceCyclesForMemRead();
  
  if (BeebEmCommon::OpCodes>=1) { // Documented opcodes
  switch (CurrentInstruction) {
    case 0x00:
 	  BRKInstrHandler();
      break;
    case 0x01:
      ORAInstrHandler(IndXAddrModeHandler_Data());
      break;
	case 0x04:
	  if (MachineType==3) TSBInstrHandler(ZeroPgAddrModeHandler_Address()); else BeebEmCommon::ProgramCounter+=1;
	  break;
    case 0x05:
      ORAInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0x06:
      ASLInstrHandler(ZeroPgAddrModeHandler_Address());
      break;
    case 0x08:
      Push(PSR|48); /* PHP */
      break;
    case 0x09:
      ORAInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0x0a:
      ASLInstrHandler_Acc();
      break;
	case 0x0c:
	  if (MachineType==3) TSBInstrHandler(AbsAddrModeHandler_Address()); else BeebEmCommon::ProgramCounter+=2;
	  break;
    case 0x0d:
      ORAInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0x0e:
      ASLInstrHandler(AbsAddrModeHandler_Address());
      break;
    case 0x10:
      BPLInstrHandler();
      break;
    case 0x30:
      BMIInstrHandler();
      break;
    case 0x50:
      BVCInstrHandler();
      break;
    case 0x70:
      BVSInstrHandler();
      break;
    case 0x80:
      BRAInstrHandler();
      break;
    case 0x90:
      BCCInstrHandler();
      break;
    case 0xb0:
      BCSInstrHandler();
      break;
    case 0xd0:
      BNEInstrHandler();
      break;
    case 0xf0:
      BEQInstrHandler();
      break;
    case 0x11:
      ORAInstrHandler(IndYAddrModeHandler_Data());
      break;
    case 0x12:
      if (MachineType==3) ORAInstrHandler(ZPIndAddrModeHandler_Data());
      break;
	case 0x14:
	  if (MachineType==3) TRBInstrHandler(ZeroPgAddrModeHandler_Address()); else BeebEmCommon::ProgramCounter+=1;
	  break;
    case 0x15:
      ORAInstrHandler(ZeroPgXAddrModeHandler_Data());
      break;
    case 0x16:
      ASLInstrHandler(ZeroPgXAddrModeHandler_Address());
      break;
    case 0x18:
      PSR&=255-FlagC; /* CLC */
      break;
    case 0x19:
      ORAInstrHandler(AbsYAddrModeHandler_Data());
      break;
    case 0x1a:
      if (MachineType==3) INAInstrHandler();
      break;
	case 0x1c:
	  if (MachineType==3) TRBInstrHandler(AbsAddrModeHandler_Address()); else BeebEmCommon::ProgramCounter+=2;
	  break;
    case 0x1d:
      ORAInstrHandler(AbsXAddrModeHandler_Data());
      break;
    case 0x1e:
      ASLInstrHandler(AbsXAddrModeHandler_Address());
      break;
    case 0x20:
      JSRInstrHandler(AbsAddrModeHandler_Address());
      break;
    case 0x21:
      ANDInstrHandler(IndXAddrModeHandler_Data());
      break;
    case 0x24:
      BITInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0x25:
      ANDInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0x26:
      ROLInstrHandler(ZeroPgAddrModeHandler_Address());
      break;
	case 0x28:
		{
			unsigned char oldPSR=PSR;
			PSR=Pop(); /* PLP */
			if ((oldPSR ^ PSR) & FlagI)
			{
				if (PSR & FlagI)
					iFlagJustSet=true;
				else
					iFlagJustCleared=true;
			}
		}
		break;
    case 0x29:
      ANDInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0x2a:
      ROLInstrHandler_Acc();
      break;
    case 0x2c:
      BITInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0x2d:
      ANDInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0x2e:
      ROLInstrHandler(AbsAddrModeHandler_Address());
      break;
    case 0x31:
      ANDInstrHandler(IndYAddrModeHandler_Data());
      break;
    case 0x32:
      if (MachineType==3) ANDInstrHandler(ZPIndAddrModeHandler_Data());
      break;
    case 0x34: /* BIT Absolute,X */
      if (MachineType==3) BITInstrHandler(ZeroPgXAddrModeHandler_Data()); else BeebEmCommon::ProgramCounter+=1;
      break;
    case 0x35:
      ANDInstrHandler(ZeroPgXAddrModeHandler_Data());
      break;
    case 0x36:
      ROLInstrHandler(ZeroPgXAddrModeHandler_Address());
      break;
    case 0x38:
      PSR|=FlagC; /* SEC */
      break;
    case 0x39:
      ANDInstrHandler(AbsYAddrModeHandler_Data());
      break;
    case 0x3a:
      if (MachineType==3) DEAInstrHandler();
      break;
    case 0x3c: /* BIT Absolute,X */
      if (MachineType==3) BITInstrHandler(AbsXAddrModeHandler_Data()); else BeebEmCommon::ProgramCounter+=2;
      break;
    case 0x3d:
      ANDInstrHandler(AbsXAddrModeHandler_Data());
      break;
    case 0x3e:
      ROLInstrHandler(AbsXAddrModeHandler_Address());
      break;
    case 0x40:
      PSR=Pop(); /* RTI */
      BeebEmCommon::ProgramCounter=PopWord();
      BeebEmCommon::NMILock=0;
      break;
    case 0x41:
      EORInstrHandler(IndXAddrModeHandler_Data());
      break;
    case 0x45:
      EORInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0x46:
      LSRInstrHandler(ZeroPgAddrModeHandler_Address());
      break;
    case 0x48:
      Push(Accumulator); /* PHA */
      break;
    case 0x49:
      EORInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0x4a:
      LSRInstrHandler_Acc();
      break;
    case 0x4c:
      BeebEmCommon::ProgramCounter=AbsAddrModeHandler_Address(); /* JMP */
/*    if (BeebEmCommon::ProgramCounter==0xffdd) {
	  // OSCLI logging for elite debugging
	  unsigned char *bptr;
	  char pcbuf[256]; char *pcptr=pcbuf;
	  int blk=((YReg*256)+XReg);
	  bptr=WholeRam+((WholeRam[blk+1]*256)+WholeRam[blk]);
  	  while((*bptr != 13) && ((pcptr-pcbuf)<254)) {
		  *pcptr=*bptr; pcptr++;bptr++; 
	  } 
	  *pcptr=0;
	  fprintf(osclilog,"%s\n",pcbuf);
  }
  if (BeebEmCommon::ProgramCounter==0xffdd) {
	  char errstr[250];
	  sprintf(errstr,"OSFILE called\n");
	  MessageBox(GETHWND,errstr,"BBC Emulator",MB_OKCANCEL|MB_ICONERROR);
  }*/

      break;
    case 0x4d:
      EORInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0x4e:
      LSRInstrHandler(AbsAddrModeHandler_Address());
      break;
    case 0x51:
      EORInstrHandler(IndYAddrModeHandler_Data());
      break;
    case 0x52:
      if (MachineType==3) EORInstrHandler(ZPIndAddrModeHandler_Data());
      break;
    case 0x55:
      EORInstrHandler(ZeroPgXAddrModeHandler_Data());
      break;
    case 0x56:
      LSRInstrHandler(ZeroPgXAddrModeHandler_Address());
      break;
    case 0x58:
	  if (PSR & FlagI)
		iFlagJustCleared=true;
	  PSR&=255-FlagI; /* CLI */
	  break;
    case 0x59:
      EORInstrHandler(AbsYAddrModeHandler_Data());
      break;
    case 0x5a:
      if (MachineType==3) Push(YReg); /* PHY */
      break;
    case 0x5d:
      EORInstrHandler(AbsXAddrModeHandler_Data());
      break;
    case 0x5e:
      LSRInstrHandler(AbsXAddrModeHandler_Address());
      break;
    case 0x60:
      BeebEmCommon::ProgramCounter=PopWord()+1; /* RTS */
      break;
    case 0x61:
      ADCInstrHandler(IndXAddrModeHandler_Data());
      break;
    case 0x64:
      if (MachineType==3) BEEBWRITEMEM_DIRECT(ZeroPgAddrModeHandler_Address(),0); /* STZ Zero Page */
      break;
    case 0x65:
      ADCInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0x66:
      RORInstrHandler(ZeroPgAddrModeHandler_Address());
      break;
    case 0x68:
      Accumulator=Pop(); /* PLA */
      PSR&=~(FlagZ | FlagN);
      PSR|=((Accumulator==0)<<1) | (Accumulator & 128);
      break;
    case 0x69:
      ADCInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0x6a:
      RORInstrHandler_Acc();
      break;
    case 0x6c:
      BeebEmCommon::ProgramCounter=IndAddrModeHandler_Address(); /* JMP */
      break;
    case 0x6d:
      ADCInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0x6e:
      RORInstrHandler(AbsAddrModeHandler_Address());
      break;
    case 0x71:
      ADCInstrHandler(IndYAddrModeHandler_Data());
      break;
    case 0x72:
      if (MachineType==3) ADCInstrHandler(ZPIndAddrModeHandler_Data());
      break;
    case 0x74:
	  if (MachineType==3) { BEEBWRITEMEM_DIRECT(ZeroPgXAddrModeHandler_Address(),0); } else BeebEmCommon::ProgramCounter+=1; /* STZ Zpg,X */
      break;
    case 0x75:
      ADCInstrHandler(ZeroPgXAddrModeHandler_Data());
      break;
    case 0x76:
      RORInstrHandler(ZeroPgXAddrModeHandler_Address());
      break;
    case 0x78:
	  if (!(PSR & FlagI))
		iFlagJustSet = true;
      PSR|=FlagI; /* SEI */
      break;
    case 0x79:
      ADCInstrHandler(AbsYAddrModeHandler_Data());
      break;
    case 0x7a:
		if (MachineType==3) {
			YReg=Pop(); /* PLY */
			PSR&=~(FlagZ | FlagN);
			PSR|=((YReg==0)<<1) | (YReg & 128);
		}
	  break;
    case 0x7c:
      if (MachineType==3) BeebEmCommon::ProgramCounter=IndAddrXModeHandler_Address(); /* JMP abs,X*/ else BeebEmCommon::ProgramCounter+=2;
      break;
    case 0x7d:
      ADCInstrHandler(AbsXAddrModeHandler_Data());
      break;
    case 0x7e:
      RORInstrHandler(AbsXAddrModeHandler_Address());
      break;
    case 0x81:
 	  AdvanceCyclesForMemWrite();
      WritePaged(IndXAddrModeHandler_Address(),Accumulator); /* STA */
      break;
    case 0x84:
	  AdvanceCyclesForMemWrite();
      BEEBWRITEMEM_DIRECT(ZeroPgAddrModeHandler_Address(),YReg);
      break;
    case 0x85:
	  AdvanceCyclesForMemWrite();
      BEEBWRITEMEM_DIRECT(ZeroPgAddrModeHandler_Address(),Accumulator); /* STA */
      break;
    case 0x86:
	  AdvanceCyclesForMemWrite();
      BEEBWRITEMEM_DIRECT(ZeroPgAddrModeHandler_Address(),XReg);
      break;
    case 0x88:
      YReg=(YReg-1) & 255; /* DEY */
      PSR&=~(FlagZ | FlagN);
      PSR|=((YReg==0)<<1) | (YReg & 128);
      break;
    case 0x89: /* BIT Immediate */
      if (MachineType==3) BITImmedInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++));
      break;
    case 0x8a:
      Accumulator=XReg; /* TXA */
      PSR&=~(FlagZ | FlagN);
      PSR|=((Accumulator==0)<<1) | (Accumulator & 128);
      break;
    case 0x8c:
      AdvanceCyclesForMemWrite();
      STYInstrHandler(AbsAddrModeHandler_Address());
      break;
    case 0x8d:
	  AdvanceCyclesForMemWrite();
	  WritePaged(AbsAddrModeHandler_Address(),Accumulator); /* STA */
      break;
    case 0x8e:
	  AdvanceCyclesForMemWrite();
	  STXInstrHandler(AbsAddrModeHandler_Address());
      break;
    case 0x91:
	  AdvanceCyclesForMemWrite();
      WritePaged(IndYAddrModeHandler_Address(),Accumulator); /* STA */
      break;
    case 0x92:
	  AdvanceCyclesForMemWrite();
      if (MachineType==3) WritePaged(ZPIndAddrModeHandler_Address(),Accumulator); /* STA */
      break;
    case 0x94:
	  AdvanceCyclesForMemWrite();
      STYInstrHandler(ZeroPgXAddrModeHandler_Address());
      break;
    case 0x95:
	  AdvanceCyclesForMemWrite();
      WritePaged(ZeroPgXAddrModeHandler_Address(),Accumulator); /* STA */
      break;
    case 0x96:
	  AdvanceCyclesForMemWrite();
      STXInstrHandler(ZeroPgYAddrModeHandler_Address());
      break;
    case 0x98:
      Accumulator=YReg; /* TYA */
      PSR&=~(FlagZ | FlagN);
      PSR|=((Accumulator==0)<<1) | (Accumulator & 128);
      break;
    case 0x99:
	  AdvanceCyclesForMemWrite();
      WritePaged(AbsYAddrModeHandler_Address(),Accumulator); /* STA */
      break;
    case 0x9a:
      StackReg=XReg; /* TXS */
      break;
    case 0x9c:
      WritePaged(AbsAddrModeHandler_Address(),0); /* STZ Absolute */
	  /* here's a curiosity, STZ Absolute IS on the 6502 UNOFFICIALLY
	  and on the 65C12 OFFICIALLY. Something we should know? - Richard Gellman */
      break;
    case 0x9d:
	  AdvanceCyclesForMemWrite();
      WritePaged(AbsXAddrModeHandler_Address(),Accumulator); /* STA */
      break;
    case 0x9e:
		if (MachineType==3) { WritePaged(AbsXAddrModeHandler_Address(),0); } /* STZ Abs,X */ 
		else WritePaged(AbsXAddrModeHandler_Address(),Accumulator & XReg);
      break;
    case 0xa0:
      LDYInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0xa1:
      LDAInstrHandler(IndXAddrModeHandler_Data());
      break;
    case 0xa2:
      LDXInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0xa4:
      LDYInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0xa5:
      LDAInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0xa6:
      LDXInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0xa8:
      YReg=Accumulator; /* TAY */
      PSR&=~(FlagZ | FlagN);
      PSR|=((Accumulator==0)<<1) | (Accumulator & 128);
      break;
    case 0xa9:
      LDAInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0xaa:
      XReg=Accumulator; /* TXA */
      PSR&=~(FlagZ | FlagN);
      PSR|=((Accumulator==0)<<1) | (Accumulator & 128);
      break;
    case 0xac:
      LDYInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0xad:
      LDAInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0xae:
      LDXInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0xb1:
      LDAInstrHandler(IndYAddrModeHandler_Data());
      break;
    case 0xb2:
      if (MachineType==3) LDAInstrHandler(ZPIndAddrModeHandler_Data());
      break;
    case 0xb4:
      LDYInstrHandler(ZeroPgXAddrModeHandler_Data());
      break;
    case 0xb5:
      LDAInstrHandler(ZeroPgXAddrModeHandler_Data());
      break;
    case 0xb6:
      LDXInstrHandler(ZeroPgYAddrModeHandler_Data());
      break;
    case 0xb8:
      PSR&=255-FlagV; /* CLV */
      break;
    case 0xb9:
      LDAInstrHandler(AbsYAddrModeHandler_Data());
      break;
    case 0xba:
      XReg=StackReg; /* TSX */
      PSR&=~(FlagZ | FlagN);
      PSR|=((XReg==0)<<1) | (XReg & 128);
      break;
    case 0xbc:
      LDYInstrHandler(AbsXAddrModeHandler_Data());
      break;
    case 0xbd:
      LDAInstrHandler(AbsXAddrModeHandler_Data());
      break;
    case 0xbe:
      LDXInstrHandler(AbsYAddrModeHandler_Data());
      break;
    case 0xc0:
      CPYInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0xc1:
      CMPInstrHandler(IndXAddrModeHandler_Data());
      break;
    case 0xc4:
      CPYInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0xc5:
      CMPInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0xc6:
      DECInstrHandler(ZeroPgAddrModeHandler_Address());
      break;
    case 0xc8:
      YReg+=1; /* INY */
      YReg&=255;
      PSR&=~(FlagZ | FlagN);
      PSR|=((YReg==0)<<1) | (YReg & 128);
      break;
    case 0xc9:
      CMPInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0xca:
      DEXInstrHandler();
      break;
    case 0xcc:
      CPYInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0xcd:
      CMPInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0xce:
      DECInstrHandler(AbsAddrModeHandler_Address());
      break;
    case 0xd1:
      CMPInstrHandler(IndYAddrModeHandler_Data());
      break;
    case 0xd2:
      if (MachineType==3) CMPInstrHandler(ZPIndAddrModeHandler_Data());
      break;
    case 0xd5:
      CMPInstrHandler(ZeroPgXAddrModeHandler_Data());
      break;
    case 0xd6:
      DECInstrHandler(ZeroPgXAddrModeHandler_Address());
      break;
    case 0xd8:
      PSR&=255-FlagD; /* CLD */
      break;
    case 0xd9:
      CMPInstrHandler(AbsYAddrModeHandler_Data());
      break;
    case 0xda:
      if (MachineType==3) Push(XReg); /* PHX */
      break;
    case 0xdd:
      CMPInstrHandler(AbsXAddrModeHandler_Data());
      break;
    case 0xde:
      DECInstrHandler(AbsXAddrModeHandler_Address());
      break;
    case 0xe0:
      CPXInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0xe1:
      SBCInstrHandler(IndXAddrModeHandler_Data());
      break;
    case 0xe4:
      CPXInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0xe5:
      SBCInstrHandler(WholeRam[ReadPaged(BeebEmCommon::ProgramCounter++)]/*zp */);
      break;
    case 0xe6:
      INCInstrHandler(ZeroPgAddrModeHandler_Address());
      break;
    case 0xe8:
      INXInstrHandler();
      break;
    case 0xe9:
      SBCInstrHandler(ReadPaged(BeebEmCommon::ProgramCounter++)); /* immediate */
      break;
    case 0xea:
      /* NOP */
      break;
    case 0xec:
      CPXInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0xed:
      SBCInstrHandler(AbsAddrModeHandler_Data());
      break;
    case 0xee:
      INCInstrHandler(AbsAddrModeHandler_Address());
      break;
    case 0xf1:
      SBCInstrHandler(IndYAddrModeHandler_Data());
      break;
    case 0xf2:
      if (MachineType==3) SBCInstrHandler(ZPIndAddrModeHandler_Data());
      break;
    case 0xf5:
      SBCInstrHandler(ZeroPgXAddrModeHandler_Data());
      break;
    case 0xf6:
      INCInstrHandler(ZeroPgXAddrModeHandler_Address());
      break;
    case 0xf8:
      PSR|=FlagD; /* SED */
      break;
    case 0xf9:
      SBCInstrHandler(AbsYAddrModeHandler_Data());
      break;
    case 0xfa:
		if (MachineType==3) {
	  XReg=Pop(); /* PLX */
      PSR&=~(FlagZ | FlagN);
      PSR|=((XReg==0)<<1) | (XReg & 128);
		}
		break;
    case 0xfd:
      SBCInstrHandler(AbsXAddrModeHandler_Data());
      break;
    case 0xfe:
      INCInstrHandler(AbsXAddrModeHandler_Address());
      break;
	default:
		BadCount++;
	  }
	  }
 if (BeebEmCommon::OpCodes==3) {
  switch (CurrentInstruction) {
    case 0x07: /* Undocumented Instruction: ASL zp and ORA zp */
      {
        int16 zpaddr = ZeroPgAddrModeHandler_Address();
        ASLInstrHandler(zpaddr);
        ORAInstrHandler(WholeRam[zpaddr]);
      }
      break;
    case 0x03: /* Undocumented Instruction: ASL-ORA (zp,X) */
      {
        int16 zpaddr = IndXAddrModeHandler_Address();
        ASLInstrHandler(zpaddr);
        ORAInstrHandler(WholeRam[zpaddr]);
      }
      break;
    case 0x13: /* Undocumented Instruction: ASL-ORA (zp),Y */
      {
        int16 zpaddr = IndYAddrModeHandler_Address();
        ASLInstrHandler(zpaddr);
        ORAInstrHandler(WholeRam[zpaddr]);
      }
      break;
    case 0x0f: /* Undocumented Instruction: ASL-ORA abs */
      {
        int16 zpaddr = AbsAddrModeHandler_Address();
        ASLInstrHandler(zpaddr);
        ORAInstrHandler(WholeRam[zpaddr]);
      }
      break;
    case 0x17: /* Undocumented Instruction: ASL-ORA zp,X */
      {
        int16 zpaddr = ZeroPgXAddrModeHandler_Address();
        ASLInstrHandler(zpaddr);
        ORAInstrHandler(WholeRam[zpaddr]);
      }
      break;
    case 0x1b: /* Undocumented Instruction: ASL-ORA abs,Y */
      {
        int16 zpaddr = AbsYAddrModeHandler_Address();
        ASLInstrHandler(zpaddr);
        ORAInstrHandler(WholeRam[zpaddr]);
      }
      break;
    case 0x1f: /* Undocumented Instruction: ASL-ORA abs,X */
      {
        int16 zpaddr = AbsXAddrModeHandler_Address();
        ASLInstrHandler(zpaddr);
        ORAInstrHandler(WholeRam[zpaddr]);
      }
      break;
	case 0x23: /* Undocumented Instruction: ROL-AND (zp,X) */
		{
		int16 zpaddr=IndXAddrModeHandler_Address();
		ROLInstrHandler(zpaddr);
		ANDInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x27: /* Undocumented Instruction: ROL-AND zp */
		{
		int16 zpaddr=ZeroPgAddrModeHandler_Address();
		ROLInstrHandler(zpaddr);
		ANDInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x2f: /* Undocumented Instruction: ROL-AND abs */
		{
		int16 zpaddr=AbsAddrModeHandler_Address();
		ROLInstrHandler(zpaddr);
		ANDInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x33: /* Undocumented Instruction: ROL-AND (zp),Y */
		{
		int16 zpaddr=IndYAddrModeHandler_Address();
		ROLInstrHandler(zpaddr);
		ANDInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x37: /* Undocumented Instruction: ROL-AND zp,X */
		{
		int16 zpaddr=ZeroPgXAddrModeHandler_Address();
		ROLInstrHandler(zpaddr);
		ANDInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x3b: /* Undocumented Instruction: ROL-AND abs.Y */
		{
		int16 zpaddr=AbsYAddrModeHandler_Address();
		ROLInstrHandler(zpaddr);
		ANDInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x3f: /* Undocumented Instruction: ROL-AND abs.X */
		{
		int16 zpaddr=AbsXAddrModeHandler_Address();
		ROLInstrHandler(zpaddr);
		ANDInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x43: /* Undocumented Instruction: LSR-EOR (zp,X) */
		{
		int16 zpaddr=IndXAddrModeHandler_Address();
		LSRInstrHandler(zpaddr);
		EORInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x47: /* Undocumented Instruction: LSR-EOR zp */
		{
		int16 zpaddr=ZeroPgAddrModeHandler_Address();
		LSRInstrHandler(zpaddr);
		EORInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x4f: /* Undocumented Instruction: LSR-EOR abs */
		{
		int16 zpaddr=AbsAddrModeHandler_Address();
		LSRInstrHandler(zpaddr);
		EORInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x53: /* Undocumented Instruction: LSR-EOR (zp),Y */
		{
		int16 zpaddr=IndYAddrModeHandler_Address();
		LSRInstrHandler(zpaddr);
		EORInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x57: /* Undocumented Instruction: LSR-EOR zp,X */
		{
		int16 zpaddr=ZeroPgXAddrModeHandler_Address();
		LSRInstrHandler(zpaddr);
		EORInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x5b: /* Undocumented Instruction: LSR-EOR abs,Y */
		{
		int16 zpaddr=AbsYAddrModeHandler_Address();
		LSRInstrHandler(zpaddr);
		EORInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x5f: /* Undocumented Instruction: LSR-EOR abs,X */
		{
		int16 zpaddr=AbsXAddrModeHandler_Address();
		LSRInstrHandler(zpaddr);
		EORInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x44:
	case 0x54:
		BeebEmCommon::ProgramCounter+=1;
		break;
	case 0x5c:
		BeebEmCommon::ProgramCounter+=2;
		break;
	case 0x63: /* Undocumented Instruction: ROR-ADC (zp,X) */
		{
		int16 zpaddr=IndXAddrModeHandler_Address();
		RORInstrHandler(zpaddr);
		ADCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x67: /* Undocumented Instruction: ROR-ADC zp */
		{
		int16 zpaddr=ZeroPgAddrModeHandler_Address();
		RORInstrHandler(zpaddr);
		ADCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x6f: /* Undocumented Instruction: ROR-ADC abs */
		{
		int16 zpaddr=AbsAddrModeHandler_Address();
		RORInstrHandler(zpaddr);
		ADCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x73: /* Undocumented Instruction: ROR-ADC (zp),Y */
		{
		int16 zpaddr=IndYAddrModeHandler_Address();
		RORInstrHandler(zpaddr);
		ADCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x77: /* Undocumented Instruction: ROR-ADC zp,X */
		{
		int16 zpaddr=ZeroPgXAddrModeHandler_Address();
		RORInstrHandler(zpaddr);
		ADCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x7b: /* Undocumented Instruction: ROR-ADC abs,Y */
		{
		int16 zpaddr=AbsYAddrModeHandler_Address();
		RORInstrHandler(zpaddr);
		ADCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0x7f: /* Undocumented Instruction: ROR-ADC abs,X */
		{
		int16 zpaddr=AbsXAddrModeHandler_Address();
		RORInstrHandler(zpaddr);
		ADCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	// Undocumented DEC-CMP and INC-SBC Instructions
	case 0xc3: // DEC-CMP (zp,X)
		{
		int16 zpaddr=IndXAddrModeHandler_Address();
		DECInstrHandler(zpaddr);
		CMPInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xc7: // DEC-CMP zp
		{
		int16 zpaddr=ZeroPgAddrModeHandler_Address();
		DECInstrHandler(zpaddr);
		CMPInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xcf: // DEC-CMP abs
		{
		int16 zpaddr=AbsAddrModeHandler_Address();
		DECInstrHandler(zpaddr);
		CMPInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xd3: // DEC-CMP (zp),Y
		{
		int16 zpaddr=IndYAddrModeHandler_Address();
		DECInstrHandler(zpaddr);
		CMPInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xd7: // DEC-CMP zp,X
		{
		int16 zpaddr=ZeroPgXAddrModeHandler_Address();
		DECInstrHandler(zpaddr);
		CMPInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xdb: // DEC-CMP abs,Y
		{
		int16 zpaddr=AbsYAddrModeHandler_Address();
		DECInstrHandler(zpaddr);
		CMPInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xdf: // DEC-CMP abs,X
		{
		int16 zpaddr=AbsXAddrModeHandler_Address();
		DECInstrHandler(zpaddr);
		CMPInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xd4:
	case 0xf4:
		BeebEmCommon::ProgramCounter+=1;
		break;
	case 0xdc:
	case 0xfc:
		BeebEmCommon::ProgramCounter+=2;
		break;
	case 0xe3: // INC-SBC (zp,X)
		{
		int16 zpaddr=IndXAddrModeHandler_Address();
		INCInstrHandler(zpaddr);
		SBCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xe7: // INC-SBC zp
		{
		int16 zpaddr=ZeroPgAddrModeHandler_Address();
		INCInstrHandler(zpaddr);
		SBCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xef: // INC-SBC abs
		{
		int16 zpaddr=AbsAddrModeHandler_Address();
		INCInstrHandler(zpaddr);
		SBCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xf3: // INC-SBC (zp).Y
		{
		int16 zpaddr=IndYAddrModeHandler_Address();
		INCInstrHandler(zpaddr);
		SBCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xf7: // INC-SBC zp,X
		{
		int16 zpaddr=ZeroPgXAddrModeHandler_Address();
		INCInstrHandler(zpaddr);
		SBCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xfb: // INC-SBC abs,Y
		{
		int16 zpaddr=AbsYAddrModeHandler_Address();
		INCInstrHandler(zpaddr);
		SBCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	case 0xff: // INC-SBC abs,X
		{
		int16 zpaddr=AbsXAddrModeHandler_Address();
		INCInstrHandler(zpaddr);
		SBCInstrHandler(WholeRam[zpaddr]);
		}
		break;
	// REALLY Undocumented instructions 6B, 8B and CB
    case 0x6b:
		ANDInstrHandler(WholeRam[BeebEmCommon::ProgramCounter++]);
		RORInstrHandler_Acc();
		break;
	case 0x8b:
		Accumulator=XReg; /* TXA */
		PSR&=~(FlagZ | FlagN);
		PSR|=((Accumulator==0)<<1) | (Accumulator & 128);
		ANDInstrHandler(WholeRam[BeebEmCommon::ProgramCounter++]);
		break;
	case 0xcb:
		// SBX #n - I dont know if this uses the carry or not, i'm assuming its
		// Subtract #n from X with carry.
		{
			unsigned char TmpAcc=Accumulator;
			Accumulator=XReg;
			SBCInstrHandler(WholeRam[BeebEmCommon::ProgramCounter++]);
			XReg=Accumulator;
			Accumulator=TmpAcc; // Fudge so that I dont have to do the whole SBC code again
		}
		break;
    default:
      BadCount++;
      break;
  }
 }
 if (BeebEmCommon::OpCodes>=2) {
  switch (CurrentInstruction) {
	      case 0x0b:
	case 0x2b:
      ANDInstrHandler(WholeRam[BeebEmCommon::ProgramCounter++]); /* AND-MVC #n,b7 */
	  PSR|=((Accumulator & 128)>>7);
      break;
    case 0x4b: /* Undocumented Instruction: AND imm and LSR A */
      ANDInstrHandler(WholeRam[BeebEmCommon::ProgramCounter++]);
      LSRInstrHandler_Acc();
      break;
    case 0x87: /* Undocumented Instruction: SAX zp (i.e. (zp) = A & X) */
      /* This one does not seem to change the processor flags */
      WholeRam[ZeroPgAddrModeHandler_Address()] = Accumulator & XReg;
      break;
    case 0x83: /* Undocumented Instruction: SAX (zp,X) */
      WholeRam[IndXAddrModeHandler_Address()] = Accumulator & XReg;
      break;
    case 0x8f: /* Undocumented Instruction: SAX abs */
      WholeRam[AbsAddrModeHandler_Address()] = Accumulator & XReg;
      break;
    case 0x93: /* Undocumented Instruction: SAX (zp),Y */
      WholeRam[IndYAddrModeHandler_Address()] = Accumulator & XReg;
      break;
    case 0x97: /* Undocumented Instruction: SAX zp,Y */
      WholeRam[ZeroPgYAddrModeHandler_Address()] = Accumulator & XReg;
      break;
    case 0x9b: /* Undocumented Instruction: SAX abs,Y */
      WholeRam[AbsYAddrModeHandler_Address()] = Accumulator & XReg;
      break;
    case 0x9f: /* Undocumented Instruction: SAX abs,X */
      WholeRam[AbsXAddrModeHandler_Address()] = Accumulator & XReg;
      break;
    case 0xab: /* Undocumented Instruction: LAX #n */
      LDAInstrHandler(WholeRam[BeebEmCommon::ProgramCounter++]);
      XReg = Accumulator;
      break;
    case 0xa3: /* Undocumented Instruction: LAX (zp,X) */
      LDAInstrHandler(IndXAddrModeHandler_Data());
      XReg = Accumulator;
      break;
    case 0xa7: /* Undocumented Instruction: LAX zp */
      LDAInstrHandler(WholeRam[WholeRam[BeebEmCommon::ProgramCounter++]]);
      XReg = Accumulator;
      break;
    case 0xaf: /* Undocumented Instruction: LAX abs */
      LDAInstrHandler(AbsAddrModeHandler_Data());
      XReg = Accumulator;
      break;
    case 0xb3: /* Undocumented Instruction: LAX (zp),Y */
      LDAInstrHandler(IndYAddrModeHandler_Data());
      XReg = Accumulator;
      break;
    case 0xb7: /* Undocumented Instruction: LAX zp,Y */
      LDXInstrHandler(ZeroPgYAddrModeHandler_Data());
      Accumulator = XReg;
      break;
	case 0xbb:
    case 0xbf: /* Undocumented Instruction: LAX abs,Y */
      LDAInstrHandler(AbsYAddrModeHandler_Data());
      XReg = Accumulator;
      break;
	default:
		BadCount++;
  }
 }
 if (BadCount==BeebEmCommon::OpCodes) 
	 BadInstrHandler(CurrentInstruction);
	  
	// This block corrects the cycle count for the branch instructions
	if ((CurrentInstruction==0x10) ||
		(CurrentInstruction==0x30) ||
		(CurrentInstruction==0x50) ||
		(CurrentInstruction==0x70) ||
		(CurrentInstruction==0x80) ||
		(CurrentInstruction==0x90) ||
		(CurrentInstruction==0xb0) ||
		(CurrentInstruction==0xd0) ||
		(CurrentInstruction==0xf0)) {

		if (Branched)
		{
			BeebEmCommon::Cycles++;
			if ((BeebEmCommon::ProgramCounter & 0xff00) != ((OldPC+2) & 0xff00))
				BeebEmCommon::Cycles+=1;
		}
	}
	
	BeebEmCommon::Cycles += CyclesTable[CurrentInstruction] -
		CyclesToMemRead[CurrentInstruction] - CyclesToMemWrite[CurrentInstruction];
	
        PollVIAs(BeebEmCommon::Cycles - ViaCycles);
        PollHardware(BeebEmCommon::Cycles);

	DoKbdIntCheck();
	
	// Check for IRQ
	DoIntCheck();
	if (IntDue && (!GETIFLAG || iFlagJustSet) &&
		(BeebEmCommon::CyclesToInt <= (-2-IOCycles) && !iFlagJustCleared))
	{
		// Int noticed 2 cycles before end of instruction - interrupt now
		BeebEmCommon::CyclesToInt = NO_TIMER_INT_DUE;
		DoInterrupt();
		PollHardware(IRQCycles);
		PollVIAs(IRQCycles);
		IRQCycles=0;
	}
	
	// Check for NMI
	if ((BeebEmCommon::NMIStatus && !OldNMIStatus) || (BeebEmCommon::NMIStatus & 1<<nmi_econet))
	{
		BeebEmCommon::NMIStatus &= ~(1<<nmi_econet);
		DoNMI();
		PollHardware(IRQCycles);
		PollVIAs(IRQCycles);
		IRQCycles=0;
	}
	OldNMIStatus=BeebEmCommon::NMIStatus;
	
	if ( (EnableTube) && (TubeEnabled) )
		SyncTubeProcessor();
  }
} /* Exec6502Instruction */


void CPU6502::PollVIAs(unsigned int nCycles)
{
	if (nCycles != 0)
	{
		if (BeebEmCommon::CyclesToInt != NO_TIMER_INT_DUE)
			BeebEmCommon::CyclesToInt -= nCycles;
		
		SysVIA_poll(nCycles);
		UserVIA_poll(nCycles);
		
		ViaCycles += nCycles;
	}
}

void CPU6502::PollHardware(unsigned int nCycles)
{
	BeebEmCommon::TotalCycles+=nCycles;
	
	if (BeebEmCommon::TotalCycles > CycleCountWrap)
	{
		BeebEmCommon::TotalCycles -= CycleCountWrap;
		//SoundCycles -= CycleCountWrap;
		AdjustTrigger(AtoDTrigger);
		AdjustTrigger(SoundTrigger);
		AdjustTrigger(Disc8271Trigger);
		AdjustTrigger(AMXTrigger);
		AdjustTrigger(PrinterTrigger);
		AdjustTrigger(VideoTriggerCount);
		AdjustTrigger(TapeTrigger);
		AdjustTrigger(EconetTrigger);
		AdjustTrigger(EconetFlagFillTimeoutTrigger);
		if ( (EnableTube) && (TubeEnabled) )
			WrapTubeCycles();
	}

	VideoPoll(nCycles);
	if (!BeebEmCommon::BHardware) {
		AtoD_poll(nCycles);
		Serial_Poll();
	}
	Disc8271_poll(nCycles);
	Sound_Trigger(nCycles);
	if (BeebEmCommon::DisplayCycles>0) BeebEmCommon::DisplayCycles-=nCycles; // Countdown time till end of display of info.
	if ((MachineType==3) || (!NativeFDC)) Poll1770(nCycles); // Do 1770 Background stuff
	
	if (EconetEnabled && EconetPoll()) {
		if (EconetNMIenabled ) { 
			BeebEmCommon::NMIStatus|=1<<nmi_econet;
//			if (DebugEnabled)
				//DebugDisplayTrace(DEBUG_ECONET, true, "Econet: NMI asserted");
		}
		else {
//			if (DebugEnabled)
				//DebugDisplayTrace(DEBUG_ECONET, true, "Econet: NMI requested but supressed");
		}
	}
}

/*-------------------------------------------------------------------------*/
/* Dump state                                                              */
void CPU6502::core_dumpstate(void) {
  fprintf(stderr, "core:\n");
  DumpRegs();
}; /* core_dumpstate */

/*-------------------------------------------------------------------------*/
void CPU6502::Save6502UEF(FILE *SUEF) {
	fput16(0x0460,SUEF);
	fput32(16,SUEF);
	fput16(BeebEmCommon::ProgramCounter,SUEF);
	fputc(Accumulator,SUEF);
	fputc(XReg,SUEF);
	fputc(YReg,SUEF);
	fputc(StackReg,SUEF);
	fputc(PSR,SUEF);
	fput32(BeebEmCommon::TotalCycles,SUEF);
	fputc(BeebEmCommon::intStatus,SUEF);
	fputc(BeebEmCommon::NMIStatus,SUEF);
	fputc(BeebEmCommon::NMILock,SUEF);
	fput16(0,SUEF);
}

void CPU6502::Load6502UEF(FILE *SUEF) {
	int Dlong;
	BeebEmCommon::ProgramCounter=fget16(SUEF);
	Accumulator=fgetc(SUEF);
	XReg=fgetc(SUEF);
	YReg=fgetc(SUEF);
	StackReg=fgetc(SUEF);
	PSR=fgetc(SUEF);
	//BeebEmCommon::TotalCycles=fget32(SUEF);
	Dlong=fget32(SUEF);
	BeebEmCommon::intStatus=fgetc(SUEF);
	BeebEmCommon::NMIStatus=fgetc(SUEF);
	BeebEmCommon::NMILock=fgetc(SUEF);
	//AtoDTrigger=Disc8271Trigger=AMXTrigger=PrinterTrigger=VideoTriggerCount=BeebEmCommon::TotalCycles+100;
	//if (UseHostClock) SoundTrigger=BeebEmCommon::TotalCycles+100;
	// Make sure emulator doesn't lock up waiting for triggers.

}
