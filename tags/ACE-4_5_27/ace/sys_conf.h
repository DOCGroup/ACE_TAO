/* ============================================================================ */
/*                                                                              */
/* -*- C++ -*-                                                                  */
/* $Id$                       */
/*                                                                              */
/* ============================================================================ */
/*                                                                              */
/* = LIBRARY                                                                    */
/*    ace                                                                       */
/*                                                                              */
/* = FILENAM                                                                    */
/*    sys_conf.h  (definitions for pSOSim and pSOSystem configuration)          */
/*                                                                              */
/* = AUTHOR                                                                     */
/*    Chris Gill                                                                */
/*                                                                              */
/* ============================================================================ */

#ifndef _SYS_CONF_H
#define _SYS_CONF_H

#if defined(__cplusplus)
extern "C" {
#endif

/***********************************************************************/
/*                                                                     */
/*  These symbols are used to define other symbols in this file.       */
/*  They should never be changed.                                      */
/*                                                                     */
/***********************************************************************/
#define YES     1
#define NO      0

#define USE_RARP 0

#define DBG_SA    1  /* Standalone pROBE debugging */
#define DBG_XS    2  /* XRAY+ via RS-232 */
#define DBG_XN    3  /* XRAY+ via network */
#define DBG_AP    4  /* Run application (w/ pROBE in standalone mode) */

#define STORAGE   5  /* Get SD_'s from storage area */
#define SYS_CONF  6  /* Get SD_'s from #defines set in this file */


/***********************************************************************/
/*                                                                     */
/*                 B A S I C   P A R A M E T E R S                     */
/*                                                                     */
/*  The parameters in this section determine the fashion in which      */
/*  many of the other parameters in this file will be used.            */
/*                                                                     */
/*  Many of the parameters in this file have names beginning with      */
/*  either "SC_" or "SD_".  Those beginning with "SC_" are ALWAYS      */
/*  controlled by the values you set in this file.  The values of      */
/*  the "SD_" parameters can be determined either by the definitions   */
/*  given in this file, or by the data found in the target board's     */
/*  parameter storage area.  SC_SD_PARAMETERS determines this.  If     */
/*  SC_SD_PARAMETERS is "SYS_CONF", then the values given in this      */
/*  file will be used for the SD_ parameters.  If SC_SD_PARAMETERS     */
/*  is "STORAGE", then pSOSystem will attempt to use the values found  */
/*  in the board's parameter storage area for the SD_ variables.  If   */
/*  the parameter storage area has not been initialized or has been    */
/*  corrupted, then the values given in this file will be used.  They  */
/*  will also be written to the storage area.                          */
/*                                                                     */
/*  If SC_SD_PARAMETERS is STORAGE, you may enable a "startup dialog"  */
/*  which will allow you to view, and optionally change, the           */
/*  parameter values found in the storage area.  The dialog is enabled */
/*  by setting SC_STARTUP_DIALOG to YES.  In this case,                */
/*  SD_STARTUP_DELAY specifies the number of seconds that the dialog   */
/*  will wait for input before proceeding to boot the system.          */
/*                                                                     */
/*  SE_DEBUG_MODE determines how the system will operate, as follows:  */
/*                                                                     */
/*     DBG_SA: Boot pROBE+ in standalone mode.                         */
/*                                                                     */
/*     DBG_XS: Boot into pROBE+ and wait for the host debugger (XRAY+) */
/*             via a serial connection                                 */
/*                                                                     */
/*     DBG_XN: Boot into pROBE+ and wait for the host debugger (XRAY+) */
/*             via a network connection                                */
/*                                                                     */
/*     STORAGE: Use the mode (DBG_SA, DBG_XS, or DBG_XN) found in the  */
/*              parameter storage area.  If a valid mode is not found, */
/*              then use DBG_SA.                                       */
/*                                                                     */
/***********************************************************************/
#define SC_SD_PARAMETERS    STORAGE
#define SC_STARTUP_DIALOG   NO
#define SD_STARTUP_DELAY    60
#define SE_DEBUG_MODE       STORAGE

/***********************************************************************/
/*                                                                     */
/*        O P E R A T I N G   S Y S T E M   C O M P O N E N T S        */
/*                                                                     */
/*  Setting the definition line to YES will cause that component to    */
/*  be built into the system.  NO means the component will NOT be      */
/*  put into the system.  It is an error to specify both SC_PSOS       */
/*  and SC_PSOSM as present!                                           */
/*                                                                     */
/***********************************************************************/
#define SC_PSOS             YES  /* pSOS+ real-time kernel */
#define SC_PSOSM            NO   /* pSOS+ real-time multiprocessing kernel */
#define SC_PROBE            YES   /* pROBE+ system-level debugger */
#define SC_PHILE            NO   /* pHILE+ file system manager */
#define SC_PREPC            YES  /* pREPC+ C run-time library */
#define SC_PNA              YES   /* pNA+ TCP/IP networking manager */
#define SC_PRPC             NO   /* pRPC+ RPC component */
#define SC_PX               NO   /* pX+ X-windows client */
#define SC_PSE              NO   /* pSE+ STREAMS component */
#define SC_PTLI             NO   /* pTLI TLI component */


/***********************************************************************/
/*                                                                     */
/*      S E R I A L   C H A N N E L   C O N F I G U R A T I O N        */
/*                                                                     */
/*  * SC_DEF_BAUD specifies the default baud rate to be used for       */
/*    the serial channels                                              */
/*  * SC_APP_CONSOLE specifies the serial channel number (starting     */
/*    from 1) which should be used for the application's "console".    */
/*  * SC_PROBE_CONSOLE specifies the serial channel number (starting   */
/*    from 1) which pROBE+ should use to display output and receive    */
/*    commands, or to communicate with XRAY+ over.                     */
/*  * SC_PROBE_HOST specifies the serial channel number (starting      */
/*    from 1) which should be used for the pROBE+ "host" channel.      */
/*    This is not normally required, so it can be disabled by          */
/*    using a value of 0.  The host channel is explained in the        */
/*    pROBE+ User's Manual.                                            */
/*                                                                     */
/***********************************************************************/
#define SD_DEF_BAUD           9600
#define SC_APP_CONSOLE        1
#define SC_PROBE_CONSOLE      1
#define SC_PROBE_HOST         0


/***********************************************************************/
/*                                                                     */
/*              L A N   C O N F I G U R A T I O N                      */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
#define SD_LAN1               YES
#define SD_LAN1_IP            0x80FCA58C
#define SD_LAN1_SUBNET_MASK   0
#define SC_LAN1_BUFFS         40

/***********************************************************************/
/*                                                                     */
/*      S H A R E D   M E M O R Y   C O N F I G U R A T I O N          */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
/*---------------------------------------------------------------------*/
/* If you want to build a Shared Memory Interface for the network or   */
/* multi-processing pSOS+m, set SD_SM_NODE (a non-zero value) to the   */
/* node number of THIS node.                                           */
/*---------------------------------------------------------------------*/
#define SD_SM_NODE            0

/*---------------------------------------------------------------------*/
/* Shared memory network interface                                     */
/*---------------------------------------------------------------------*/
#define SD_NISM               NO
#define SD_NISM_IP            0x00000000
#define SD_NISM_DIRADDR       0x300000
#define SC_NISM_BUFFS         30
#define SC_NISM_LEVEL         2
#define SD_NISM_SUBNET_MASK   0


/*---------------------------------------------------------------------*/
/* If you want to build a multi-node pSOS+m system, set SD_KISM        */
/* to the number of nodes in the system.                               */
/*---------------------------------------------------------------------*/
#define SD_KISM               NO
#define SD_KISM_DIRADDR       0x1000400


/***********************************************************************/
/*                                                                     */
/*         M I S C E L L A N E O U S   N E T W O R K I N G             */
/*                       P A R A M E T E R S                           */
/*                                                                     */
/***********************************************************************/
#define SD_DEF_GTWY_IP        0


/***********************************************************************/
/*                                                                     */
/*                B U S   C O N F I G U R A T I O N                    */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
#define SD_VME_BASE_ADDR      0x01000000


/***********************************************************************/
/* SC_RAM_SIZE                                                         */
/*                                                                     */
/* Normally, pSOSystem uses ALL of the memory remaining on a board     */
/* for dynamic allocation ("region 0").  You may override this by      */
/* setting SC_RAM_SIZE to a non-zero value.  If you do, pSOSystem will */
/* not touch any memory after address (BSP_RAM_BASE + SC_RAM_SIZE - 1).*/
/*                                                                     */
/* NOTE: This is usefull when the building a BOOT ROM you and want to  */
/* make sure it does not "over flow" into the space you are putting    */
/* the download code.                                                  */
/***********************************************************************/
#define SC_RAM_SIZE           0x800000

/***********************************************************************/
/*                                                                     */
/*                     I / O   D E V I C E S                           */
/*                                                                     */
/*  Each device may be included in the system by specifying a major    */
/*  number for it, which determines its slot in the pSOS+ I/O switch   */
/*  table.  To leave a device driver out of the system, use NO for     */
/*  the major number.                                                  */
/*                                                                     */
/*  Note the following:                                                */
/*    * Major device 0 is reserved and cannot be used for any of these */
/*      devices.  Setting a device number to 0 here is the same as NO. */
/*    * No device number may be higher than SC_DEVMAX (SC_DEVMAX may   */
/*      be increased, if desired)                                      */
/*    * The lines defining the symbols DEV_SERIAL, DEV_TIMER, etc,     */
/*      should not be changed.  These are for use by application       */
/*      programs as the "device number" parameter on de_* calls.       */
/*                                                                     */
/***********************************************************************/
#define SC_DEV_SERIAL      1            /* Serial driver */
#define SC_DEV_TIMER       2            /* Periodic tick timer */
#define SC_DEV_RAMDISK     NO           /* RAM disk */
#define SC_DEV_TFTP        NO           /* TFTP pseudo driver */
#define SC_DEV_SCSI        NO           /* SCSI bus */
#define SC_DEV_DLPI        NO
#define SC_DEVMAX          6            /* Maximum device number */

#define DEV_SERIAL    (SC_DEV_SERIAL << 16)
#define DEV_TIMER     (SC_DEV_TIMER << 16)
#define DEV_RAMDISK   (SC_DEV_RAMDISK << 16)
#define DEV_SCSI      (SC_DEV_SCSI << 16)
#define DEV_TFTP      (SC_DEV_TFTP << 16)
#define DEV_DLPI      (SC_DEV_DLPI << 16)

/***********************************************************************/
/*                                                                     */
/* C O M P O N E N T   C O N F I G U R A T I O N   P A R A M E T E R S */
/*                                                                     */
/*  These parameters should work as is for this application.  You      */
/*  may want to change some of them if you start customizing this      */
/*  application.                                                       */
/*                                                                     */
/***********************************************************************/

/*---------------------------------------------------------------------*/
/* pSOS+ configuration parameters                                      */
/*---------------------------------------------------------------------*/
#define KC_RN0USIZE    0x100    /* region 0 unit size */
#define KC_NTASK       12       /* max number of tasks */
#define KC_NQUEUE      10       /* max number of message queues */
#define KC_NSEMA4      30       /* max number of semaphores */
#define KC_NMSGBUF     100      /* max number of message buffers */
#define KC_NTIMER      10       /* max number of timers */
#define KC_NLOCOBJ     50       /* max number of local objects */

/* note: because pSOSim *simulates* a realtime clock, this value is set     */
/*       very low to avoid crazy skew between the simulated and real worlds */
#define KC_TICKS2SEC   1000000  /* clock tick interrupt frequency */

#define KC_TICKS2SLICE 10       /* time slice quantum, in ticks */
#define KC_SYSSTK      0xFFFF   /* pSOS+ system stack size (bytes) */
#define KC_ROOTSSTK    0xFFFF   /* ROOT supervisor stack size */
#define KC_ROOTUSTK    0        /* ROOT user stack size */
#define KC_ROOTMODE    0x2000   /* ROOT initial mode */
#define KC_STARTCO     0        /* callout at task activation */
#define KC_DELETECO    0        /* callout at task deletion */
#define KC_SWITCHCO    0        /* callout at task switch */
#define KC_FATAL       0        /* fatal error handler address */


/*---------------------------------------------------------------------*/
/* pSOS+m configuration parameters                                     */
/*---------------------------------------------------------------------*/

#define MC_NGLBOBJ      8       /* sizeof global obj table in each node*/
#define MC_NAGENT      10       /* number of RPC agents in this node */
#define MC_FLAGS        0       /* operating mode flags */
#define MC_ROSTER       0       /* addr of user roster change callout */
#define MC_KIMAXBUF   100       /* maximum KI packet buffer length */
#define MC_ASYNCERR     0       /* asynchronous calls error callout */

/*---------------------------------------------------------------------*/
/* pROBE+ configuration parameters                                     */
/*---------------------------------------------------------------------*/
#define RC_BRKOPC      0x4E40   /* Instruction break opcode */
#define RC_ILEVEL      MAX_ILEV /* pROBE+ interrupt mask */
#define RC_SMODE       0        /* Start mode */
#define RC_FLAGS       0x0C     /* Initial flag settings */

/*---------------------------------------------------------------------*/
/* pHILE+ configuration parameters                                     */
/*---------------------------------------------------------------------*/
#define FC_LOGBSIZE    9        /* Block size (base-2 exponent) */
#define FC_NBUF        6        /* Number of cache buffers */
#define FC_NMOUNT      3        /* Max # of mounted volumes */
#define FC_NFCB        10       /* Max # of opened files per system */
#define FC_NCFILE      2        /* Max # of opened files per task */
#define FC_MSDOS       1        /* MS-DOS volume mount flag */

/*---------------------------------------------------------------------*/
/* pREPC+ configuration parameters                                     */
/*---------------------------------------------------------------------*/
#define LC_BUFSIZ      128      /* I/O buffer size */
#define LC_NUMFILES    50       /* Max number of open files per task */
#define LC_WAITOPT     0        /* Wait option for memory allocation */
#define LC_TIMEOPT     0        /* Timeout option for mem allocation */
#define LC_SSIZE       512      /* Size of print buffer */

/*---------------------------------------------------------------------*/
/* pNA+ configuration parameters                                       */
/*---------------------------------------------------------------------*/
#define NC_NNI         5        /* size of pNA NI Table */
#define NC_NROUTE      10       /* size of pNA Routing Table */
#define NC_NARP        20       /* size of pNA ARP table */
#define NC_DEFUID      0        /* Default User ID of a task */
#define NC_DEFGID      0        /* Default Group ID of a task */
#define NC_HOSTNAME    "scg"    /* Hostname of the node */
#define NC_NHENTRY     8        /* Number of Host table entries */

/*---------------------------------------------------------------------*/
/* pSE+ configuration parameters                                       */
/*---------------------------------------------------------------------*/
#define NBUFS_0     64          /* number of 0 length buffers */
#define NBUFS_32    128         /* number of 32 byte buffers */
#define NBUFS_64    128         /* number of 64 byte buffers */
#define NBUFS_128   256         /* number of 128 byte buffers */
#define NBUFS_256   32          /* number of 256 byte buffers */
#define NBUFS_512   32          /* number of 512 byte buffers */
#define NBUFS_1024  16          /* number of 1k byte buffers */
#define NBUFS_2048  16          /* number of 2k byte buffers */
#define NBUFS_4096  8           /* number of 4k byte buffers */
#define SE_MAX_PSE_STRBUFS  9   /* max number of stream buffer types */

#define SE_MAX_PSE_MODULES 2    /* max number of streams modules */
#define SC_SHARE_NI   NO        /* does pna share NI with streams? */

#define SE_DATA_SIZE 0x0C00     /* size of pSE data area */
                                /* (must be at least 3K) */
#define SE_TASK_PRIO  250       /* priority for pSE task */
#define SE_STACK_SIZE 0x2000    /* stack size for pSE task */
#define SE_DEF_UID    0         /* default user id */
#define SE_DEF_GID    0         /* default group id */
#define SE_N_FDS      32        /* max # of system-wide */
                                /*  stream descriptors */
#define SE_N_TASKFDS  8         /* max # of per-task stream */
                                /*  descriptors */
#define SE_N_LINKS    32        /* max # of multiplexing links */
#define SE_N_TIMEOUTS 32        /* max # of timeout requests */
#define SE_N_BUFCALLS 32        /* max # of bufcall requests */
#define SE_N_QUEUES   256       /* # of queues */
#define SE_N_MBLKS    512       /* reserved for future use, must be 0 */

/***********************************************************************/
/*                                                                     */
/*  This defines the IP address of the host system where the           */
/*  "server" program should be running.                                */
/*                                                                     */
/***********************************************************************/
#define HOST_IP       0x80FCA58C 

#if defined(__cplusplus)
}
#endif

#endif /* _SYS_CONF_H */

