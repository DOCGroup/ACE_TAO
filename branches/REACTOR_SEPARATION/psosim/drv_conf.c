/* $Id$ */
/***********************************************************************/
/*                                                                     */
/*   MODULE:  drv_conf.c                                               */
/*   DATE:    94/01/06                                                 */
/*   PURPOSE: pSOSystem driver configuration and initialization        */
/*                                                                     */
/*            There is one function in this file:                      */
/*                                                                     */
/*            SetUpDrivers to setup drivers except for Network drivers */
/*                                                                     */
/*                                                                     */
/*---------------------------------------------------------------------*/
/*                                                                     */
/*           Copyright 1991 - 1993, Integrated Systems, Inc.           */
/*                      ALL RIGHTS RESERVED                            */
/*                                                                     */
/*   Permission is hereby granted to licensees of Integrated Systems,  */
/*   Inc. products to use or abstract this computer program for the    */
/*   sole purpose of implementing a product based on Integrated        */
/*   Systems, Inc. products.   No other rights to reproduce, use,      */
/*   or disseminate this computer program, whether in part or in       */
/*   whole, are granted.                                               */
/*                                                                     */
/*   Integrated Systems, Inc. makes no representation or warranties    */
/*   with respect to the performance of this computer program, and     */
/*   specifically disclaims any responsibility for any damages,        */
/*   special or consequential, connected with the use of this program. */
/*                                                                     */
/***********************************************************************/
#include "sys_conf.h"
#include "bsp.h"
#include <bspfuncs.h>
#include <configs.h>

struct ioparms;
#define NULLF ((void (*)(struct ioparms *)) 0)
UCHAR *SetUpDrivers(UCHAR *FreeMemPtr)
{

/*---------------------------------------------------------------------*/
/* Install pSOSystem serial device driver                              */
/*---------------------------------------------------------------------*/
#if (SC_DEV_SERIAL && BSP_SERIAL)
    InstallDriver(SC_DEV_SERIAL, CnslInit, NULLF, NULLF,CnslRead,
                  CnslWrite, CnslCntrl, 0, 0);
    FreeMemPtr = CnslSetup(FreeMemPtr, SD_DEF_BAUD, SC_APP_CONSOLE);
#endif

/*---------------------------------------------------------------------*/
/* Install pSOSystem tick timer                                        */
/*---------------------------------------------------------------------*/
#if SC_DEV_TIMER
    InstallDriver(SC_DEV_TIMER, RtcInit, NULLF, NULLF, NULLF, NULLF,
                  NULLF, 0, 0);
#endif

/*---------------------------------------------------------------------*/
/* Install pSOSystem ramdisk driver                                    */
/*---------------------------------------------------------------------*/
#if SC_DEV_RAMDISK
    InstallDriver(SC_DEV_RAMDISK, RdskInit, NULLF, NULLF, RdskRead,
                  RdskWrite, NULLF, 0, 0);
#endif

/*---------------------------------------------------------------------*/
/* Install pSOSystem TFTP driver                                       */
/*---------------------------------------------------------------------*/
#if (SC_DEV_TFTP)
    InstallDriver(SC_DEV_TFTP, TftpInit, TftpOpen, TftpClose, TftpRead,
                  NULLF, TftpCntl, 0, 0);
#endif

/*---------------------------------------------------------------------*/
/* Add additional drivers to install here.                             */
/*---------------------------------------------------------------------*/

return(FreeMemPtr);
}

