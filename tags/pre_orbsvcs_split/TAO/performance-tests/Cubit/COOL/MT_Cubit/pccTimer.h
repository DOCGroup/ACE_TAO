// $Id$
/*
 ****************************************************************
 *
 * Component =   PCC2 Timer Header File
 *
 * Synopsis  =
 *
 *
 ****************************************************************
 *
 *
 ****************************************************************
 */



#ifndef _PCCTIMER_H
#define _PCCTIMER_H

int
pccTimer(int option, int* uTime);
unsigned int
pccTime1Get();
unsigned int
pccTimer1Reset();
void
pccTimer1Start();
void
pccTimer1Stop();
unsigned int
pccTime2Get();
unsigned int
pccTimer2Reset();
void
pccTimer2Start();
void
pccTimer2Stop();
void
pccTimerInit();


#define PCC2_TIMER1_START                0x0
#define PCC2_TIMER1_STOP                 0x1
#define PCC2_TIMER1_BPOINT                0x2
#define PCC2_TIMER2_START                0x10
#define PCC2_TIMER2_STOP                 0x20
#define PCC2_TIMER2_BPOINT                0x40
#define PCC2_USERDATA                   0x100

#include <c_defs.h>
#include <mem/chAddr_f.h>

//extern_C int sysBench   __((int, int*));

#endif  /* _PCCTIMER_H */
