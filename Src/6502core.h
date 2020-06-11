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

#define SetTrigger(after,var) var=BeebEmCommon::TotalCycles+after;
#define IncTrigger(after,var) var+=(after);
#define ClearTrigger(var) var=CycleCountTMax;
#define AdjustTrigger(var) if (var!=CycleCountTMax) var-=CycleCountWrap;

/*-------------------------------------------------------------------------*/
/* Initialise 6502core                                                     */
void Init6502core(void);

/*-------------------------------------------------------------------------*/
/* Execute one 6502 instruction, move program counter on                   */
void Exec6502Instruction(void);

void DoNMI(void);
void core_dumpstate(void);
void DoInterrupt(void);
void Save6502UEF(FILE *SUEF);
void Load6502UEF(FILE *SUEF);
void SyncIO(void);
void AdjustForIORead(void);
void AdjustForIOWrite(void);
int i186_execute(int num_cycles);
void DumpRegs(void);
#endif
