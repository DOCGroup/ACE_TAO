// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_inet.h
 *
 *  definitions for internet operations
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_ARPA_OS_INET_H
#define ACE_OS_INCLUDE_ARPA_OS_INET_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/netinet/os_in.h"

#if !defined (ACE_LACKS_ARPA_INET_H)
   extern "C" {
#  include /**/ <arpa/inet.h>
   }
#endif /* !ACE_LACKS_ARPA_INET_H */

#if defined (VXWORKS)
#  include /**/ <inetLib.h>
#endif /* VXWORKS */

/**
 * In some environments it is useful to swap the bytes on write, for
 * instance: a fast server can be feeding a lot of slow clients that
 * happen to have the wrong byte order.
 * Because this is a rarely used feature we disable it by default to
 * minimize footprint.
 * This macro enables the functionality, but we still need a way to
 * activate it on a per-connection basis.
 */
// #define ACE_ENABLE_SWAP_ON_WRITE

/**
 * In some environements we never need to swap bytes when reading, for
 * instance embebbed systems (such as avionics) or homogenous
 * networks.
 * Setting this macro disables the capabilities to demarshall streams
 * in the wrong byte order.
 */
// #define ACE_DISABLE_SWAP_ON_READ

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined (ACE_VXWORKS) && ACE_VXWORKS <= 0x540
   // Work around a lack of ANSI prototypes for these functions on VxWorks.
   unsigned long  inet_addr (const char *);
   char           *inet_ntoa (const struct in_addr);
   struct in_addr inet_makeaddr (const int, const int);
   unsigned long  inet_network (const char *);
#endif /* ! (ACE_VXWORKS) && ACE_VXWORKS <= 0x540 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_ARPA_OS_INET_H */
