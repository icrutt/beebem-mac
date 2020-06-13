#ifndef UEFSTATE_HEADER
#define UEFSTATE_HEADER
#include "6502core.h"
void fput32(unsigned int word32,FILE *fileptr);
void fput16(unsigned int word16,FILE *fileptr);
unsigned int fget32(FILE *fileptr);
unsigned int fget16(FILE *fileptr);
void SaveUEFState(CPU6502* cpu,char *StateName);
void LoadUEFState(CPU6502* cpu,char *StateName);
#endif
