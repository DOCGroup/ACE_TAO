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

// @todo move the ACE_HTONL, etc macros here
// Byte-order (endian-ness) determination.
# if defined (BYTE_ORDER)
#   if (BYTE_ORDER == LITTLE_ENDIAN)
#     define ACE_LITTLE_ENDIAN 0x0123
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   elif (BYTE_ORDER == BIG_ENDIAN)
#     define ACE_BIG_ENDIAN 0x3210
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   else
#     error: unknown BYTE_ORDER!
#   endif /* BYTE_ORDER */
# elif defined (_BYTE_ORDER)
#   if (_BYTE_ORDER == _LITTLE_ENDIAN)
#     define ACE_LITTLE_ENDIAN 0x0123
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   elif (_BYTE_ORDER == _BIG_ENDIAN)
#     define ACE_BIG_ENDIAN 0x3210
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   else
#     error: unknown _BYTE_ORDER!
#   endif /* _BYTE_ORDER */
# elif defined (__BYTE_ORDER)
#   if (__BYTE_ORDER == __LITTLE_ENDIAN)
#     define ACE_LITTLE_ENDIAN 0x0123
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   elif (__BYTE_ORDER == __BIG_ENDIAN)
#     define ACE_BIG_ENDIAN 0x3210
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   else
#     error: unknown __BYTE_ORDER!
#   endif /* __BYTE_ORDER */
# else /* ! BYTE_ORDER && ! __BYTE_ORDER */
  // We weren't explicitly told, so we have to figure it out . . .
#   if defined (i386) || defined (__i386__) || defined (_M_IX86) || \
     defined (vax) || defined (__alpha) || defined (__LITTLE_ENDIAN__) ||\
     defined (ARM) || defined (_M_IA64)
    // We know these are little endian.
#     define ACE_LITTLE_ENDIAN 0x0123
#     define ACE_BYTE_ORDER ACE_LITTLE_ENDIAN
#   else
    // Otherwise, we assume big endian.
#     define ACE_BIG_ENDIAN 0x3210
#     define ACE_BYTE_ORDER ACE_BIG_ENDIAN
#   endif
# endif /* ! BYTE_ORDER && ! __BYTE_ORDER */

// Byte swapping macros to deal with differences between little endian
// and big endian machines.  Note that "long" here refers to 32 bit
// quantities.
# define ACE_SWAP_LONG(L) ((ACE_SWAP_WORD ((L) & 0xFFFF) << 16) \
            | ACE_SWAP_WORD(((L) >> 16) & 0xFFFF))
# define ACE_SWAP_WORD(L) ((((L) & 0x00FF) << 8) | (((L) & 0xFF00) >> 8))

# if defined (ACE_LITTLE_ENDIAN)
#   define ACE_HTONL(X) ACE_SWAP_LONG (X)
#   define ACE_NTOHL(X) ACE_SWAP_LONG (X)
#   define ACE_IDL_NCTOHL(X) (X)
#   define ACE_IDL_NSTOHL(X) (X)
# else
#   define ACE_HTONL(X) X
#   define ACE_NTOHL(X) X
#   define ACE_IDL_NCTOHL(X) (X << 24)
#   define ACE_IDL_NSTOHL(X) ((X) << 16)
# endif /* ACE_LITTLE_ENDIAN */

# if defined (ACE_LITTLE_ENDIAN)
#   define ACE_HTONS(x) ACE_SWAP_WORD(x)
#   define ACE_NTOHS(x) ACE_SWAP_WORD(x)
# else
#   define ACE_HTONS(x) x
#   define ACE_NTOHS(x) x
# endif /* ACE_LITTLE_ENDIAN */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_ARPA_OS_INET_H */
