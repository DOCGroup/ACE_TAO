/*
 * ACE reactor demonstration
 *
 * $Id$
 * Date: 26-Jan-2006
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <ace/Time_Value.h>

/**
 * The port number used by client and server.
 */
static const int PORT = 4711;

/**
 * The base size. 0x100000 = 1 MiB
 */
static const int BASE = 0x100000;

/**
 * The timeout value for connections. (30 seconds)
 */
static const ACE_Time_Value connTimeout(30);

#endif /* __COMMON_H__ */

