/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    msg.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_MSG_H
# define ACE_OS_INCLUDE_SYS_MSG_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if defined (ACE_HAS_BROKEN_MSG_H)
#   define _KERNEL
# endif /* ACE_HAS_BROKEN_MSG_H */

# if !defined (ACE_LACKS_SYSV_MSG_H)
#   include /**/ <sys/msg.h>
# endif /* ACE_LACKS_SYSV_MSG_H */

# if defined (ACE_HAS_BROKEN_MSG_H)
#   undef _KERNEL
# endif /* ACE_HAS_BROKEN_MSG_H */


# if defined (ACE_LACKS_MSGBUF_T)
struct msgbuf {};
# endif /* ACE_LACKS_MSGBUF_T */

# if defined (ACE_LACKS_SYSV_MSQ_PROTOS)
extern "C"
{
  int msgget (key_t, int);
  int msgrcv (int, void *, size_t, long, int);
  int msgsnd (int, const void *, size_t, int);
  int msgctl (int, int, struct msqid_ds *);
}
# endif /* ACE_LACKS_SYSV_MSQ_PROTOS */

#endif /* ACE_OS_INCLUDE_SYS_MSG_H */
