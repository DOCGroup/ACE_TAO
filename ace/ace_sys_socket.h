/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_socket.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_SOCKET_H
#define ACE_ACE_SYS_SOCKET_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ace_stdio.h"

#include /**/ <sys/socket.h>

typedef u_long ACE_SOCK_GROUP;

// For Win32 compatibility...
# if !defined (ACE_WSOCK_VERSION)
#   define ACE_WSOCK_VERSION 0, 0
# endif /* ACE_WSOCK_VERSION */


// Increase the range of "address families".  Please note that this
// must appear _after_ the include of sys/socket.h, for the AF_FILE
// definition on Linux/glibc2.
# if !defined (AF_ANY)
#   define AF_ANY (-1)
# endif /* AF_ANY */

# define AF_SPIPE (AF_MAX + 1)
# if !defined (AF_FILE)
#   define AF_FILE (AF_MAX + 2)
# endif /* ! AF_FILE */
# define AF_DEV (AF_MAX + 3)
# define AF_UPIPE (AF_SPIPE)

# if !defined(MAXHOSTNAMELEN)
#   define MAXHOSTNAMELEN  256
# endif /* MAXHOSTNAMELEN */

// Define INET loopback address constant if it hasn't been defined
// Dotted Decimal 127.0.0.1 == Hexidecimal 0x7f000001
# if !defined (INADDR_LOOPBACK)
#   define INADDR_LOOPBACK ((ACE_UINT32) 0x7f000001)
# endif /* INADDR_LOOPBACK */

// The INADDR_NONE address is generally 255.255.255.255.
# if !defined (INADDR_NONE)
#   define INADDR_NONE ((ACE_UINT32) 0xffffffff)
# endif /* INADDR_NONE */

// Define INET string length constants if they haven't been defined
//
// for IPv4 dotted-decimal
# if !defined (INET_ADDRSTRLEN)
#   define INET_ADDRSTRLEN 16
# endif /* INET_ADDRSTRLEN */
//
// for IPv6 hex string
# if !defined (INET6_ADDRSTRLEN)
#   define INET6_ADDRSTRLEN 46
# endif /* INET6_ADDRSTRLEN */

# if defined (ACE_HAS_IPV6)

#   if defined (ACE_USES_IPV4_IPV6_MIGRATION)
#     define ACE_ADDRESS_FAMILY_INET  AF_UNSPEC
#     define ACE_PROTOCOL_FAMILY_INET PF_UNSPEC
#   else
#     define ACE_ADDRESS_FAMILY_INET AF_INET6
#     define ACE_PROTOCOL_FAMILY_INET PF_INET6
#   endif /* ACE_USES_IPV4_IPV6_MIGRATION */

# else
#   define ACE_ADDRESS_FAMILY_INET AF_INET
#   define ACE_PROTOCOL_FAMILY_INET PF_INET
# endif

#endif /* ACE_ACE_SYS_SOCKET_H */
