/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_vxworks.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_VXWORKS_H
#define ACE_ACE_VXWORKS_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// this one is still broken! ;-(



#   if defined (VXWORKS)
// For mutex implementation using mutual-exclusion semaphores (which
// can be taken recursively).
#     include /**/ <semLib.h>

#     include /**/ <envLib.h>
#     include /**/ <hostLib.h>
#     include /**/ <ioLib.h>
#     include /**/ <remLib.h>
#     include /**/ <selectLib.h>
#     include /**/ <sigLib.h>
#     include /**/ <sockLib.h>
#     include /**/ <sysLib.h>
#     include /**/ <taskLib.h>
#     include /**/ <taskHookLib.h>

extern "C"
struct sockaddr_un {
  short sun_family;    // AF_UNIX.
  char  sun_path[108]; // path name.
};

#     define MAXPATHLEN  1024
#     define MAXNAMLEN   255
#     define NSIG (_NSIGS + 1)

// task options:  the other options are either obsolete, internal, or for
// Fortran or Ada support
#     define VX_UNBREAKABLE        0x0002  /* breakpoints ignored */
#     define VX_FP_TASK            0x0008  /* floating point coprocessor */
#     define VX_PRIVATE_ENV        0x0080  /* private environment support */
#     define VX_NO_STACK_FILL      0x0100  /* do not stack fill for
                                              checkstack () */

#     define THR_CANCEL_DISABLE      0
#     define THR_CANCEL_ENABLE       0
#     define THR_CANCEL_DEFERRED     0
#     define THR_CANCEL_ASYNCHRONOUS 0
#     define THR_BOUND               0
#     define THR_NEW_LWP             0
#     define THR_DETACHED            0
#     define THR_SUSPENDED           0
#     define THR_DAEMON              0
#     define THR_JOINABLE            0
#     define THR_SCHED_FIFO          0
#     define THR_SCHED_RR            0
#     define THR_SCHED_DEFAULT       0
#     define THR_INHERIT_SCHED       0
#     define THR_EXPLICIT_SCHED      0
#     define THR_SCHED_IO            0
#     define THR_SCOPE_SYSTEM        0
#     define THR_SCOPE_PROCESS       0
#     define USYNC_THREAD            0
#     define USYNC_PROCESS           1 /* It's all global on VxWorks
                                          (without MMU option). */
#     if !defined (ACE_DEFAULT_SYNCH_TYPE)
 // Types include these options: SEM_Q_PRIORITY, SEM_Q_FIFO,
 // SEM_DELETE_SAFE, and SEM_INVERSION_SAFE.  SEM_Q_FIFO is
 // used as the default because that is VxWorks' default.
#       define ACE_DEFAULT_SYNCH_TYPE SEM_Q_FIFO
#     endif /* ! ACE_DEFAULT_SYNCH_TYPE */

typedef SEM_ID ACE_mutex_t;
// Implement ACE_thread_mutex_t with ACE_mutex_t because there's just
// one process . . .
typedef ACE_mutex_t ACE_thread_mutex_t;
#     if !defined (ACE_HAS_POSIX_SEM)
// Use VxWorks semaphores, wrapped ...
typedef struct
{
  SEM_ID sema_;
  // Semaphore handle.  This is allocated by VxWorks.

  char *name_;
  // Name of the semaphore:  always NULL with VxWorks.
} ACE_sema_t;
#     endif /* !ACE_HAS_POSIX_SEM */
typedef char * ACE_thread_t;
typedef int ACE_hthread_t;
// Key type: the ACE TSS emulation requires the key type be unsigned,
// for efficiency.  (Current POSIX and Solaris TSS implementations also
// use u_int, so the ACE TSS emulation is compatible with them.)
typedef u_int ACE_thread_key_t;

      // Marker for ACE_Thread_Manager to indicate that it allocated
      // an ACE_thread_t.  It is placed at the beginning of the ID.
#     define ACE_THR_ID_ALLOCATED '\022'


#endif /* ACE_ACE_VXWORKS_H */
