// -*- C++ -*-

//=============================================================================
/**
 *  @file    msg.h
 *
 *  XSI message queue structures
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_MSG_H
#define ACE_OS_INCLUDE_SYS_MSG_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/ipc.h"

#if !defined (ACE_LACKS_SYS_MSG_H)
// OSF1 has problems with sys/msg.h and C++...
#  if defined (ACE_HAS_BROKEN_MSG_H)
#    define _KERNEL
#  endif /* ACE_HAS_BROKEN_MSG_H */
#  include /**/ <sys/msg.h>
#  if defined (ACE_HAS_BROKEN_MSG_H)
#    undef _KERNEL
#  endif /* ACE_HAS_BROKEN_MSG_H */
#endif /* !ACE_LACKS_SYS_MSG_H */

#if defined (ACE_LACKS_MSGBUF_T)
struct msgbuf {};
#endif /* ACE_LACKS_MSGBUF_T */

#if defined (ACE_LACKS_SYSV_MSQ_PROTOS)
extern "C"
{
  int msgget (key_t, int);
  int msgrcv (int, void *, size_t, long, int);
  int msgsnd (int, const void *, size_t, int);
  int msgctl (int, int, struct msqid_ds *);
}
#endif /* ACE_LACKS_SYSV_MSQ_PROTOS */

#if defined (ACE_WIN32)
struct msqid_ds {};
#endif /* ACE_WIN32 */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_MSG_H */
