// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_socket.h
 *
 *  main sockets header
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_OS_SOCKET_H
#define ACE_OS_INCLUDE_SYS_OS_SOCKET_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/uio.h"

#if defined (ACE_HAS_CYGWIN32_SOCKET_H)
   extern "C" {
#  include /**/ <cygwin32/socket.h>
   }
#endif /* ACE_HAS_CYGWIN32_SOCKET_H */

#if !defined (ACE_LACKS_SYS_SOCKET_H)
#  if defined (ACE_HAS_AIX_BROKEN_SOCKET_HEADER)
#    undef __cplusplus
#  endif /* ACE_HAS_AIX_BROKEN_SOCKET_HEADER */
#  include /**/ <sys/socket.h>
#  if defined (ACE_HAS_AIX_BROKEN_SOCKET_HEADER)
#    define __cplusplus
#  endif /* ACE_HAS_AIX_BROKEN_SOCKET_HEADER */
#endif /* !ACE_LACKS_SYS_SOCKET_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if !defined (ACE_HAS_MSG) && !defined (SCO)
   struct msghdr {};
#endif /* ACE_HAS_MSG */

#if defined (ACE_HAS_BROKEN_SENDMSG)
   typedef struct msghdr ACE_SENDMSG_TYPE;
#else
   typedef const struct msghdr ACE_SENDMSG_TYPE;
#endif /* ACE_HAS_BROKEN_SENDMSG */

#if defined (ACE_HAS_MSG) && defined (ACE_LACKS_MSG_ACCRIGHTS)
#  if !defined (msg_accrights)
#    undef msg_control
#    define msg_accrights msg_control
#  endif /* ! msg_accrights */

#  if !defined (msg_accrightslen)
#    undef msg_controllen
#    define msg_accrightslen msg_controllen
#  endif /* ! msg_accrightslen */
#endif /* ACE_HAS_MSG && ACE_LACKS_MSG_ACCRIGHTS */

#if defined (ACE_WIN32)
   struct msghdr
   {
     /// Optional address
     sockaddr * msg_name;

     /// Size of address
     int msg_namelen;

     /// Scatter/gather array
     iovec *msg_iov;

     /// # elements in msg_iov
     int msg_iovlen;

     /// Access rights sent/received
     caddr_t msg_accrights;

     int msg_accrightslen;
   };
#endif /* ACE_WIN32 */

#if defined (ACE_HAS_4_4BSD_SENDMSG_RECVMSG)
   // Control message size to pass a file descriptor.
#  define ACE_BSD_CONTROL_MSG_LEN sizeof (struct cmsghdr) + sizeof (ACE_HANDLE)
#  if defined (ACE_LACKS_CMSG_DATA_MACRO)
#    if defined (ACE_LACKS_CMSG_DATA_MEMBER)
#      define CMSG_DATA(cmsg) ((unsigned char *) ((struct cmsghdr *) (cmsg) + 1))
#    else
#      define CMSG_DATA(cmsg) ((cmsg)->cmsg_data)
#    endif /* ACE_LACKS_CMSG_DATA_MEMBER */
#  endif /* ACE_LACKS_CMSG_DATA_MACRO */
#endif /* ACE_HAS_4_4BSD_SENDMSG_RECVMSG */

#if defined (ACE_PSOS)
   // Some versions have missing preprocessor definitions
#  if !defined (AF_UNIX)
#    define AF_UNIX 0x1
#  endif /* AF_UNIX */
#  define PF_UNIX AF_UNIX
#  define PF_INET AF_INET
#  if !defined (AF_MAX)
#    define AF_MAX AF_INET
#  endif /* AF_MAX */
#endif /* ACE_PSOS */

// Increase the range of "address families".  Please note that this
// must appear _after_ the include of sys/socket.h, for the AF_FILE
// definition on Linux/glibc2.
#if !defined (AF_ANY)
#  define AF_ANY (-1)
#endif /* AF_ANY */

#define AF_SPIPE (AF_MAX + 1)
#if !defined (AF_FILE)
#  define AF_FILE (AF_MAX + 2)
#endif /* ! AF_FILE */
#define AF_DEV (AF_MAX + 3)
#define AF_UPIPE (AF_SPIPE)

#if defined (ACE_HAS_IPV6)
#  if defined (ACE_USES_IPV4_IPV6_MIGRATION)
#    define ACE_ADDRESS_FAMILY_INET  AF_UNSPEC
#    define ACE_PROTOCOL_FAMILY_INET PF_UNSPEC
#  else
#    define ACE_ADDRESS_FAMILY_INET AF_INET6
#    define ACE_PROTOCOL_FAMILY_INET PF_INET6
#  endif /* ACE_USES_IPV4_IPV6_MIGRATION */
#else /* !ACE_HAS_IPV6 */
#  define ACE_ADDRESS_FAMILY_INET AF_INET
#  define ACE_PROTOCOL_FAMILY_INET PF_INET
#endif /* ACE_HAS_IPV6 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_OS_SOCKET_H */
