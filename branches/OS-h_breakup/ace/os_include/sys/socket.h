/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    socket.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_SOCKET_H
# define ACE_OS_INCLUDE_SYS_SOCKET_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# include "ace/os_include/stdio.h"
# include "ace/os_include/sys/types.h"

# if !defined (ACE_LACKS_SYS_SOCKET_H) && !defined (ACE_LACKS_UNIX_SYS_HEADERS)
#   if defined (ACE_HAS_AIX_BROKEN_SOCKET_HEADER)
#     undef __cplusplus
#     include /**/ <sys/socket.h>
#     define __cplusplus
#   else
#     include /**/ <sys/socket.h>
#   endif /* ACE_HAS_AIX_BROKEN_SOCKET_HEADER */
# endif /* !ACE_LACKS_SYS_SOCKET_H && !ACE_LACKS_UNIX_SYS_HEADERS */

# if defined (ACE_HAS_SOCKIO_H)
#   include /**/ <sys/sockio.h>
# endif /* ACE_HAS_SOCKIO_ */

extern "C"
{
# if defined (VXWORKS)
  struct  hostent {
    char    *h_name;        /* official name of host */
    char    **h_aliases;    /* aliases:  not used on VxWorks */
    int     h_addrtype;     /* host address type */
    int     h_length;       /* address length */
    char    **h_addr_list;  /* (first, only) address from name server */
#   define h_addr h_addr_list[0]   /* the first address */
  };
# elif defined (ACE_HAS_CYGWIN32_SOCKET_H)
#   include /**/ <cygwin32/socket.h>
# elif !defined (ACE_WIN32)
#   if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#     define queue _Queue_
#   endif /* ACE_HAS_STL_QUEUE_CONFLICT */
#   include /**/ <netdb.h>
#   if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#     undef queue
#   endif /* ACE_HAS_STL_QUEUE_CONFLICT */
# endif /* VXWORKS */
}

extern "C"
{
# if defined (VXWORKS)
      // Work around a lack of ANSI prototypes for these functions on VxWorks.
      unsigned long  inet_addr (const char *);
      char           *inet_ntoa (const struct in_addr);
      struct in_addr inet_makeaddr (const int, const int);
      unsigned long  inet_network (const char *);
# elif !defined (ACE_WIN32)
#   include /**/ <arpa/inet.h>
# endif /* ! VXWORKS */
}

// not sure where this should go...
#if !defined (ACE_WIN32) && !defined (ACE_PSOS)
// This part if to avoid STL name conflict with the map structure
// in net/if.h.
#   if defined (ACE_HAS_STL_MAP_CONFLICT)
#     define map _Resource_Allocation_Map_
#   endif /* ACE_HAS_STL_MAP_CONFLICT */
#   include /**/ <net/if.h>
#   if defined (ACE_HAS_STL_MAP_CONFLICT)
#     undef map
#   endif /* ACE_HAS_STL_MAP_CONFLICT */

#   if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#     define queue _Queue_
#   endif /* ACE_HAS_STL_QUEUE_CONFLICT */
//#   include /**/ <netinet/in.h>
#     include "ace/os_include/netinet/in.h"
#   if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#     undef queue
#   endif /* ACE_HAS_STL_QUEUE_CONFLICT */

#   if !defined (ACE_LACKS_TCP_H)
#     if defined(ACE_HAS_CONFLICTING_XTI_MACROS)
#       if defined(TCP_NODELAY)
#         undef TCP_NODELAY
#       endif
#       if defined(TCP_MAXSEG)
#         undef TCP_MAXSEG
#       endif
#     endif
#     include /**/ <netinet/tcp.h>
#   endif /* ACE_LACKS_TCP_H */
#endif /* !ACE_WIN32 && !ACE_PSOS */

# if defined (SD_RECEIVE)
#   define ACE_SHUTDOWN_READ SD_RECEIVE
# elif defined (SHUT_RD)
#   define ACE_SHUTDOWN_READ SHUT_RD
# else
#   define ACE_SHUTDOWN_READ 0
# endif /* SD_RECEIVE */

# if defined (SD_SEND)
#   define ACE_SHUTDOWN_WRITE SD_SEND
# elif defined (SHUT_WR)
#   define ACE_SHUTDOWN_WRITE SHUT_WR
# else
#   define ACE_SHUTDOWN_WRITE 1
# endif /* SD_RECEIVE */

# if defined (SD_BOTH)
#   define ACE_SHUTDOWN_BOTH SD_BOTH
# elif defined (SHUT_RDWR)
#   define ACE_SHUTDOWN_BOTH SHUT_RDWR
# else
#   define ACE_SHUTDOWN_BOTH 2
# endif /* SD_RECEIVE */

# if defined (ACE_HAS_PHARLAP_RT)
#   define ACE_IPPROTO_TCP SOL_SOCKET
# else
#   define ACE_IPPROTO_TCP IPPROTO_TCP
# endif /* ACE_HAS_PHARLAP_RT */

# if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
typedef WSAPROTOCOL_INFO ACE_Protocol_Info;

// Callback function that's used by the QoS-enabled <ACE_OS::ioctl>
// method.
typedef LPWSAOVERLAPPED_COMPLETION_ROUTINE ACE_OVERLAPPED_COMPLETION_FUNC;
typedef GROUP ACE_SOCK_GROUP;
# else  /*  (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0) */

struct ACE_OVERLAPPED
{
  u_long Internal;
  u_long InternalHigh;
  u_long Offset;
  u_long OffsetHigh;
  ACE_HANDLE hEvent;
};

struct ACE_Protocol_Info
{
  u_long dwServiceFlags1;
  int iAddressFamily;
  int iProtocol;
  char szProtocol[255+1];
};

// Callback function that's used by the QoS-enabled <ACE_OS::ioctl>
// method.
typedef void (*ACE_OVERLAPPED_COMPLETION_FUNC) (u_long error,
                                                u_long bytes_transferred,
                                                ACE_OVERLAPPED *overlapped,
                                                u_long flags);
typedef u_long ACE_SOCK_GROUP;

# endif /* (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0) */


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


# if defined (ACE_HAS_BROKEN_SENDMSG)
typedef struct msghdr ACE_SENDMSG_TYPE;
# else
typedef const struct msghdr ACE_SENDMSG_TYPE;
# endif /* ACE_HAS_BROKEN_SENDMSG */

# if !defined (ACE_HAS_MSG) && !defined (SCO)
struct msghdr {};
# endif /* ACE_HAS_MSG */

# if defined (ACE_HAS_MSG) && defined (ACE_LACKS_MSG_ACCRIGHTS)
#   if !defined (msg_accrights)
#     undef msg_control
#     define msg_accrights msg_control
#   endif /* ! msg_accrights */

#   if !defined (msg_accrightslen)
#     undef msg_controllen
#     define msg_accrightslen msg_controllen
#   endif /* ! msg_accrightslen */
# endif /* ACE_HAS_MSG && ACE_LACKS_MSG_ACCRIGHTS */

# if defined (ACE_WIN32)
#   include "ace/os_include/sys/uio.h"  // needed for iovec
struct msghdr
{
  sockaddr * msg_name;
  // optional address

  int msg_namelen;
  // size of address

  iovec *msg_iov;
  /* scatter/gather array */

  int msg_iovlen;
  // # elements in msg_iov

  caddr_t msg_accrights;
  // access rights sent/received

  int msg_accrightslen;
};
# endif /* ACE_WIN32 */

// this stuff is normally in <netdb.h> 
# if defined (ACE_HAS_STRUCT_NETDB_DATA)
typedef char ACE_HOSTENT_DATA[sizeof(struct hostent_data)];
typedef char ACE_SERVENT_DATA[sizeof(struct servent_data)];
typedef char ACE_PROTOENT_DATA[sizeof(struct protoent_data)];
# else
#   if !defined ACE_HOSTENT_DATA_SIZE
#     define ACE_HOSTENT_DATA_SIZE (4*1024)
#   endif /*ACE_HOSTENT_DATA_SIZE */
#   if !defined ACE_SERVENT_DATA_SIZE
#     define ACE_SERVENT_DATA_SIZE (4*1024)
#   endif /*ACE_SERVENT_DATA_SIZE */
#   if !defined ACE_PROTOENT_DATA_SIZE
#     define ACE_PROTOENT_DATA_SIZE (2*1024)
#   endif /*ACE_PROTOENT_DATA_SIZE */
typedef char ACE_HOSTENT_DATA[ACE_HOSTENT_DATA_SIZE];
typedef char ACE_SERVENT_DATA[ACE_SERVENT_DATA_SIZE];
typedef char ACE_PROTOENT_DATA[ACE_PROTOENT_DATA_SIZE];
# endif /* ACE_HAS_STRUCT_NETDB_DATA */

#endif /* ACE_OS_INCLUDE_SYS_SOCKET_H */
