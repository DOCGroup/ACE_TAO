/*
 * RTEMS Network configuration/initialization
 *
 *  This file is a merger of the netdemo/init.c and networkconfig.h
 *  with some modifications to support loopback only.  This file
 *  is OK for a starting point for a real networked application.
 *     --joel sherrill 16 Mar 2001
 * 
 * This program may be distributed and used for any purpose.
 * I ask only that you:
 *	1. Leave this author information intact.
 *	2. Document any changes you make.
 *
 * W. Eric Norum
 * Saskatchewan Accelerator Laboratory
 * University of Saskatchewan
 * Saskatoon, Saskatchewan, CANADA
 * eric@skatter.usask.ca
 *
 *  $Id$
 */

#include <bsp.h>

#define CONFIGURE_TEST_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_TEST_NEEDS_CLOCK_DRIVER
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXECUTIVE_RAM_SIZE	(512*1024)
#define CONFIGURE_MAXIMUM_SEMAPHORES	20
#define CONFIGURE_MAXIMUM_TASKS		20

#define CONFIGURE_MICROSECONDS_PER_TICK	10000

#define CONFIGURE_INIT_TASK_STACK_SIZE	(10*1024)
#define CONFIGURE_INIT_TASK_PRIORITY	120
#define CONFIGURE_INIT_TASK_INITIAL_MODES (RTEMS_PREEMPT | \
                                           RTEMS_NO_TIMESLICE | \
                                           RTEMS_NO_ASR | \
                                           RTEMS_INTERRUPT_LEVEL(0))

#define CONFIGURE_INIT
rtems_task Init (rtems_task_argument argument);

#include <confdefs.h>

#include <stdio.h>
#include <rtems/rtems_bsdnet.h>
/* start of #include "../networkconfig.h" */

/*
 * Network configuration
 * 
 ************************************************************
 * EDIT THIS FILE TO REFLECT YOUR NETWORK CONFIGURATION     *
 * BEFORE RUNNING ANY RTEMS PROGRAMS WHICH USE THE NETWORK! * 
 ************************************************************
 *
 *  $Id$
 */

#ifndef _RTEMS_NETWORKCONFIG_H_
#define _RTEMS_NETWORKCONFIG_H_

#ifndef RTEMS_BSP_NETWORK_DRIVER_NAME
#warning "RTEMS_BSP_NETWORK_DRIVER_NAME is not defined"
#define RTEMS_BSP_NETWORK_DRIVER_NAME "no_network1"
#endif

#ifndef RTEMS_BSP_NETWORK_DRIVER_ATTACH
#warning "RTEMS_BSP_NETWORK_DRIVER_ATTACH is not defined"
#define RTEMS_BSP_NETWORK_DRIVER_ATTACH 0
#endif

/* #define RTEMS_USE_BOOTP */

#include <bsp.h>

/*
 * Define RTEMS_SET_ETHERNET_ADDRESS if you want to specify the
 * Ethernet address here.  If RTEMS_SET_ETHERNET_ADDRESS is not
 * defined the driver will choose an address.
 */
#define RTEMS_SET_ETHERNET_ADDRESS
#if (defined (RTEMS_SET_ETHERNET_ADDRESS))
/* static char ethernet_address[6] = { 0x08, 0x00, 0x3e, 0x12, 0x28, 0xb1 }; */
static char ethernet_address[6] = { 0x00, 0x80, 0x7F, 0x22, 0x61, 0x77 };

#endif

#define RTEMS_USE_LOOPBACK 
#define RTEMS_USE_LOOPBACK_ONLY
#ifdef RTEMS_USE_LOOPBACK 
/*
 * Loopback interface
 */
int rtems_bsdnet_loopattach(struct rtems_bsdnet_ifconfig *, int);

#ifdef RTEMS_USE_LOOPBACK_ONLY
static struct rtems_bsdnet_ifconfig netdriver_config = {
#else
static struct rtems_bsdnet_ifconfig loopback_config = {
#endif
	"lo0",				/* name */
	rtems_bsdnet_loopattach,	/* attach function */

	NULL,				/* link to next interface */

	"127.0.0.1",			/* IP address */
	"255.0.0.0",			/* IP net mask */
};
#endif

/*
 * Default network interface
 */
#ifndef RTEMS_USE_LOOPBACK_ONLY
static struct rtems_bsdnet_ifconfig netdriver_config = {
	RTEMS_BSP_NETWORK_DRIVER_NAME,		/* name */
	RTEMS_BSP_NETWORK_DRIVER_ATTACH,	/* attach function */

#ifdef RTEMS_USE_LOOPBACK 
	&loopback_config,		/* link to next interface */
#else
	NULL,				/* No more interfaces */
#endif

#if (defined (RTEMS_USE_BOOTP))
	NULL,				/* BOOTP supplies IP address */
	NULL,				/* BOOTP supplies IP net mask */
#else
	"XXX.YYY.ZZZ.XYZ",		/* IP address */
	"255.255.255.0",		/* IP net mask */
#endif /* !RTEMS_USE_BOOTP */

#if (defined (RTEMS_SET_ETHERNET_ADDRESS))
	ethernet_address,               /* Ethernet hardware address */
#else
	NULL,                           /* Driver supplies hardware address */
#endif
	0				/* Use default driver parameters */
};
#endif

/*
 * Network configuration
 */
struct rtems_bsdnet_config rtems_bsdnet_config = {
	&netdriver_config,

#if (defined (RTEMS_USE_BOOTP))
	rtems_bsdnet_do_bootp,
#else
	NULL,
#endif

	0,			/* Default network task priority */
	0,			/* Default mbuf capacity */
	0,			/* Default mbuf cluster capacity */

#if (!defined (RTEMS_USE_BOOTP))
	"rtems_host",		/* Host name */
	"nodomain.com",		/* Domain name */
	"XXX.YYY.ZZZ.1",	/* Gateway */
	"XXX.YYY.ZZZ.1",	/* Log host */
	{"XXX.YYY.ZZZ.1" },	/* Name server(s) */
	{"XXX.YYY.ZZZ.1" },	/* NTP server(s) */

	/*
	 *  A real example -- DO NOT USE THIS YOURSELF!!!
	 */

#if 0
	"dy4",			/* Host name */
	"NOT_oarcorp.com",	/* Domain name */
	"192.168.1.2",   	/* Gateway */
	"192.168.1.2", 		/* Log host */
	{"192.168.1.2" },	/* Name server(s) */
	{"192.168.1.2" },	/* NTP server(s) */
#endif
#endif /* !RTEMS_USE_BOOTP */

};

/*
 * For TFTP test application
 */
#if (defined (RTEMS_USE_BOOTP))
#define RTEMS_TFTP_TEST_HOST_NAME "BOOTP_HOST"
#define RTEMS_TFTP_TEST_FILE_NAME "BOOTP_FILE"
#else
#define RTEMS_TFTP_TEST_HOST_NAME "XXX.YYY.ZZZ.XYZ"
#define RTEMS_TFTP_TEST_FILE_NAME "tftptest"
#endif

#endif /* _RTEMS_NETWORKCONFIG_H_ */
/* end of #include "../networkconfig.h" */

/*
 * RTEMS Startup Task
 */
rtems_task
Init (rtems_task_argument ignored)
{
  int doSocket(void);

  rtems_bsdnet_initialize_network ();
  rtems_bsdnet_show_inet_routes ();
  exit (0);
}
