// $Id$

/*
 ****************************************************************
 *
 * Component =   pccTimer.C - mvme167
 *
 * Synopsis  =
 *
 * Copyright (c) 1995, 1996, 1997 Chorus systemes
 * (R) Chorus is a registered trademark of Chorus systemes
 *
 ****************************************************************
 *
 *
 ****************************************************************
 */

/*  Everything is relative from 
/vobs/chorus_work/merge1/mvme177/appl/include/chorus
*/

#include <stdio.h>
#include <chorus.h>
#include <exec/chContext.h>
//#include <exec/c_cpuContext.H>
#include <util/chKnTimeVal.h>
//#include <util/macro.h>
#include <tick/chTick.h>
//#include <kbim/p_chIntr.h>
//#include <stdconsio.h>
#include <bsp/pccchip2.h>
#include <bsp/nvram.h>
#include <bsp/board.h>
#include <exec/chPanic.h>
#include <bsp/intrLevel.h>
#include <timer/chProf.h>
#include <exec/f_chTrap.h>
#include <ims/ims_f.h>
//#include <sched/schedIntf.H>
//#include <p_boardInfo.H>
#include "pccTimer.h"

ACE_RCSID(MT_Cubit, pccTimer, "$Id$")


static unsigned int quartz;
static unsigned int timer_reload_value;
int timerHz             = 100;

volatile struct
pccchip2 *pccchip2 = (volatile struct pccchip2 *) PCCchip2_BASE;

volatile struct
nvram* nvram = ((volatile struct nvram*)NVRAM_BASE);


int
pccTimer(int option, int* uTime)
{
	KnTimeVal 	time;
	
	int res = K_EOK;
    	//option &= ~PCC_USERDATA;
    	static int benchEnable1 = 0;
    	static int benchEnable2 = 0;
    	static unsigned int benchLastTime = 0;

  	switch(option) 
	{

	case PCC2_TIMER1_START:
        	if (benchEnable1) return K_EINVAL;
        	benchEnable1 = 1;
        	pccTimer1Start();
        	benchLastTime = pccTimer1Reset();
        	break;
 
    	case PCC2_TIMER1_STOP:
        	benchLastTime = pccTime1Get();
        	if (!benchEnable1) return K_EINVAL;
        	benchEnable1 = 0;
        	pccTimer1Stop();
        	break;
 
    	case PCC2_TIMER1_BPOINT:
        	benchLastTime = pccTime1Get();
        	if (!benchEnable1) return K_EINVAL;
        	break;

	case PCC2_TIMER2_START:
        	if (benchEnable2) return K_EINVAL;
        	benchEnable2 = 1;
        	pccTimer2Start();
        	benchLastTime = pccTimer2Reset();
        	break;
 
    	case PCC2_TIMER2_STOP:
        	benchLastTime = pccTime2Get();
        	if (!benchEnable2) return K_EINVAL;
        	benchEnable2 = 0;
        	pccTimer2Stop();
        	break;
 
    	case PCC2_TIMER2_BPOINT:
        	benchLastTime = pccTime2Get();
        	if (!benchEnable2) return K_EINVAL;
        	break;
 
    	default:
        	return K_EINVAL;
	}
	
        // Here only is last time must be returned
    if (uTime) 
	{
        	*uTime = benchLastTime;
    	}

    	return res;
}


    //
    // Return the value of the timer counter, in microseconds.
    //
    unsigned int
pccTime1Get()
{
    return pccchip2->timer1cnt;
}

    unsigned int
pccTimer1Reset()
{
    pccchip2->timer1cnt = 0;
    return pccchip2->timer1cnt;
}

    void
pccTimer1Start()
{
	unsigned int temp;

	// Clear the counter
    pccchip2->timer1cnt = 0;
	// Start the counter running
	// Do not reset the counter when it matches the compare register (~PCC2_COC)
	// Clear the overflow counter
    temp = pccchip2->timer1ctl;
    pccchip2->timer1ctl = temp | PCC2_COVF | ~PCC2_COC | PCC2_CEN;

}

   void
pccTimer1Stop()
{
	unsigned int temp;

	// Stop the counter running
    temp = pccchip2->timer1ctl;
    pccchip2->timer1ctl = temp  | ~PCC2_CEN;

}


    //
    // Return the value of the timer counter, in microseconds.
    //
    unsigned int
pccTime2Get()
{
    return pccchip2->timer2cnt;
}

    unsigned int
pccTimer2Reset()
{
    pccchip2->timer2cnt = 0;
    return pccchip2->timer2cnt;
}

    void
pccTimer2Start()
{
	unsigned int temp;

	// Clear the counter
    pccchip2->timer2cnt = 0;
	// Start the counter running
	// Do not reset the counter when it matches the compare register (~PCC2_COC)
	// Clear the overflow counter
    temp = pccchip2->timer2ctl;
    pccchip2->timer2ctl = temp | PCC2_COVF | ~PCC2_COC | PCC2_CEN;

}

   void
pccTimer2Stop()
{
	unsigned int temp;

	// Stop the counter running
    temp = pccchip2->timer2ctl;
    pccchip2->timer2ctl = temp  | ~PCC2_CEN;

}

    //
    // Init the PCC2 timer
    //
    void
pccTimerInit()
{
    volatile unsigned int temp;
    char c;

        /* compute board quartz frequency from nvram information */
    c = nvram->config_rom.speed[0];
    quartz = ((unsigned int) c - (unsigned int) '0') * 10;
    c = nvram->config_rom.speed[1];
    quartz += ((unsigned int) c - (unsigned int) '0');

        /* Step 1. Initialize prescaler control register         */
        /* On a mvme177, the VME2Chip runs at half the MPU speed */
    temp = pccchip2->prescaleadjust & 0xFFFFFF00;

    switch(quartz) {
    case 60 :   pccchip2->prescaleadjust = temp | 0x000000E2;  /* 256-30 */
               // ASSERT(((BoardInfo*)chorusContext->ctxBoardInfo)->boardId
               //        == BOARD_MVME177);
                break;
    default :   {
            char tab [80];
            printf(tab,"%s : %d", "Unknown clock frequency", quartz);
            svSysPanic(KP_DEBUG, 0, 0, tab, __FILE__, __LINE__);
            }
    }

    timer_reload_value = 1000000 / timerHz;


        /* Step 2. Set pcc timer 1 and timer 2 compare register */
//    pccchip2->timer1cmp   = timer_reload_value;
    pccchip2->timer1cmp   = ~0;
    pccchip2->timer2cmp   = ~0;


        /* Step 3. Clear timer 1 and timer 2 counter register */
    pccchip2->timer1cnt = 0;
    pccchip2->timer2cnt = 0;


        /* Step 4. Enable pcc timer 1 and timer 2*/
    temp = pccchip2->timer1ctl;
    pccchip2->timer1ctl = temp | PCC2_COVF | PCC2_COC | PCC2_CEN;

    temp = pccchip2->timer2ctl;
    pccchip2->timer2ctl = temp | PCC2_COVF | PCC2_COC | PCC2_CEN;


        /* Step 5. VBR is already set */
// No need to change the VBR if we are not Compare Feature of the PCC2 Timer

        /* Step 6. set the pcc timer 1 interrupt level */
//    temp = pccchip2->irqlvl1;
//    temp = temp & 0xfffffff8;
//    pccchip2->irqlvl1 = temp | TIMER_INT_LEVEL;


 //   KnIntrEntry clockConf;

    //clockConf.name = VME_CHIP2_TIMER1;
    //clockConf.hdl  = getHdlFromCf();
    //clockConf.on   = clockStart_p;
    //clockConf.off  = clockStop_p;
    //clockConf.down = clockDown_p;
    //clockConf.ison = clockTest_p;

    //svIntrConnect (&clockConf);
}
