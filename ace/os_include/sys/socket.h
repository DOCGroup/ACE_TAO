// -*- C++ -*-

//=============================================================================
/**
 *  @file    socket.h
 *
 *  main sockets header
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_SOCKET_H
#define ACE_OS_INCLUDE_SYS_SOCKET_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/uio.h"

#if !defined (ACE_LACKS_SYS_SOCKET_H)
# include /**/ <sys/socket.h>
#endif /* !ACE_LACKS_SYS_SOCKET_H */

# if !defined (ACE_HAS_MSG) && !defined (SCO)
struct msghdr {};
# endif /* ACE_HAS_MSG */

# if defined (ACE_HAS_BROKEN_SENDMSG)
typedef struct msghdr ACE_SENDMSG_TYPE;
# else
typedef const struct msghdr ACE_SENDMSG_TYPE;
# endif /* ACE_HAS_BROKEN_SENDMSG */

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

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_SOCKET_H */
