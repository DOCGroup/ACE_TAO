// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_in.h
 *
 *  Internet address family
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_NETINET_OS_IN_H
#define ACE_OS_INCLUDE_NETINET_OS_IN_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_inttypes.h"
#include "ace/os_include/sys/os_socket.h"

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
#  include /**/ <ws2tcpip.h>
#endif /* ACE_HAS_WINSOCK2 */

#if !defined (ACE_LACKS_NETINET_IN_H)
#  if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#    define queue _Queue_
#  endif /* ACE_HAS_STL_QUEUE_CONFLICT */
   extern "C" {
#  include /**/ <netinet/in.h>
   }
#  if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#    undef queue
#  endif /* ACE_HAS_STL_QUEUE_CONFLICT */
#endif /* !ACE_LACKS_NETINET_IN_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if !defined (IPPORT_RESERVED)
#  define IPPORT_RESERVED       1024
#endif /* !IPPORT_RESERVED */

#if !defined (IPPORT_USERRESERVED)
#  define IPPORT_USERRESERVED       5000
#endif /* !IPPORT_USERRESERVED */

// Define INET loopback address constant if it hasn't been defined
// Dotted Decimal 127.0.0.1 == Hexidecimal 0x7f000001
#if !defined (INADDR_LOOPBACK)
#  define INADDR_LOOPBACK ((ACE_UINT32) 0x7f000001)
#endif /* INADDR_LOOPBACK */

// The INADDR_NONE address is generally 255.255.255.255.
#if !defined (INADDR_NONE)
#  define INADDR_NONE ((ACE_UINT32) 0xffffffff)
#endif /* INADDR_NONE */

// Define INET string length constants if they haven't been defined
//
// for IPv4 dotted-decimal
#if !defined (INET_ADDRSTRLEN)
#  define INET_ADDRSTRLEN 16
#endif /* INET_ADDRSTRLEN */
//
// for IPv6 hex string
#if !defined (INET6_ADDRSTRLEN)
#  define INET6_ADDRSTRLEN 46
#endif /* INET6_ADDRSTRLEN */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_NETINET_OS_IN_H */
