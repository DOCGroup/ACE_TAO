/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_threads.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_THREADS_H
# define ACE_ACE_THREADS_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# include "ace/ace_sys_types.h"
# include "ace/OS_Export.h"

# if !defined (ACE_DEFAULT_SYNCH_TYPE)
#   define ACE_DEFAULT_SYNCH_TYPE USYNC_THREAD
# endif /* ! ACE_DEFAULT_SYNCH_TYPE */

// This is for C++ static methods.
# if defined (VXWORKS)
typedef int ACE_THR_FUNC_INTERNAL_RETURN_TYPE;
typedef FUNCPTR ACE_THR_FUNC_INTERNAL;  // where typedef int (*FUNCPTR) (...)
# elif defined (ACE_PSOS)
typedef void (*ACE_THR_FUNC_INTERNAL)(void *);
# else
typedef ACE_THR_FUNC ACE_THR_FUNC_INTERNAL;
# endif /* VXWORKS */

extern "C" {
typedef void (*ACE_THR_C_DEST)(void *);
}
typedef void (*ACE_THR_DEST)(void *);


// Since Linux doesn't define this, I'm not sure how it should be handled.
// Need to look into it...
# if defined (ACE_HAS_PRIOCNTL)
#   include /**/ <sys/priocntl.h>
# endif /* ACE_HAS_PRIOCNTL */

# if defined (ACE_HAS_PRIOCNTL)
  // Need to #include thread.h before #defining THR_BOUND, etc.,
  // when building without threads on SunOS 5.x.
#   if defined (sun)
#     include /**/ <thread.h>
#   endif /* sun */

  // Need to #include these before #defining USYNC_PROCESS on SunOS 5.x.
#   include /**/ <sys/rtpriocntl.h>
#   include /**/ <sys/tspriocntl.h>
# endif /* ACE_HAS_PRIOCNTL */

// This needs to go here *first* to avoid problems with AIX.
# if defined (ACE_HAS_PTHREADS)
extern "C" {
#   define ACE_DONT_INCLUDE_ACE_SIGNAL_H
#   include "ace/ace_signal.h"
#   undef ACE_DONT_INCLUDE_ACE_SIGNAL_H
#   include /**/ <pthread.h>
#   if defined (DIGITAL_UNIX)
#     define pthread_self __pthread_self
extern "C" pthread_t pthread_self (void);
#   endif /* DIGITAL_UNIX */
}
#   if defined (HPUX_10)
//    HP-UX 10 needs to see cma_sigwait, and since _CMA_NOWRAPPERS_ is defined,
//    this header does not get included from pthreads.h.
#     include /**/ <dce/cma_sigwait.h>
#   endif /* HPUX_10 */
# endif /* ACE_HAS_PTHREADS */

// There are a lot of threads-related macro definitions in the config files.
// They came in at different times and from different places and platform
// requirements as threads evolved.  They are probably not all needed - some
// overlap or are otherwise confused.  This is an attempt to start
// straightening them out.
# if defined (ACE_HAS_PTHREADS_STD)    /* POSIX.1c threads (pthreads) */
  // ... and 2-parameter asctime_r and ctime_r
#   if !defined (ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R) && \
       !defined (ACE_HAS_STHREADS)
#     define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R
#   endif
# endif /* ACE_HAS_PTHREADS_STD */


# if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION))
#   define ACE_TSS_TYPE(T) ACE_TSS< T >
#   if defined (ACE_HAS_BROKEN_CONVERSIONS)
#     define ACE_TSS_GET(I, T) (*(I))
#   else
#     define ACE_TSS_GET(I, T) ((I)->operator T * ())
#   endif /* ACE_HAS_BROKEN_CONVERSIONS */
# else
#   define ACE_TSS_TYPE(T) T
#   define ACE_TSS_GET(I, T) (I)
# endif /* ACE_HAS_THREADS && (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATIOND) */

# if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#   define ACE_MT(X) X
#   if !defined (_REENTRANT)
#     define _REENTRANT
#   endif /* _REENTRANT */
# else
#   define ACE_MT(X)
# endif /* ACE_MT_SAFE */

# if !defined (ACE_DEFAULT_THREAD_PRIORITY)
#   define ACE_DEFAULT_THREAD_PRIORITY (-0x7fffffffL - 1L)
# endif /* ACE_DEFAULT_THREAD_PRIORITY */


# if defined (ACE_HAS_POSIX_SEM)
#   include /**/ <semaphore.h>
#   if !defined (SEM_FAILED) && !defined (ACE_LACKS_NAMED_POSIX_SEM)
#     define SEM_FAILED ((sem_t *) -1)
#   endif  /* !SEM_FAILED */

// look familiar?
// looks like this is only used with posix sem
#   if !defined (SEM_FAILED)
#     define SEM_FAILED ((sem_t *) -1)
#   endif  /* !SEM_FAILED */


typedef struct
{
  sem_t *sema_;
  // Pointer to semaphore handle.  This is allocated by ACE if we are
  // working with an unnamed POSIX semaphore or by the OS if we are
  // working with a named POSIX semaphore.

  char *name_;
  // Name of the semaphore (if this is non-NULL then this is a named
  // POSIX semaphore, else its an unnamed POSIX semaphore).

#   if defined (ACE_LACKS_NAMED_POSIX_SEM)
  int new_sema_;
  // this->sema_ doesn't always get created dynamically if a platform
  // doesn't support named posix semaphores.  We use this flag to
  // remember if we need to delete <sema_> or not.
#   endif /* ACE_LACKS_NAMED_POSIX_SEM */
} ACE_sema_t;
# endif /* ACE_HAS_POSIX_SEM */

# if defined (ACE_HAS_THREADS)

#   if defined (ACE_HAS_STHREADS)
#     include /**/ <synch.h>
#     include /**/ <thread.h>
#     define ACE_SCOPE_PROCESS P_PID
#     define ACE_SCOPE_LWP P_LWPID
#     define ACE_SCOPE_THREAD (ACE_SCOPE_LWP + 1)
#   else
#     define ACE_SCOPE_PROCESS 0
#     define ACE_SCOPE_LWP 1
#     define ACE_SCOPE_THREAD 2
#   endif /* ACE_HAS_STHREADS */

#   if !defined (ACE_HAS_PTHREADS)
#     define ACE_SCHED_OTHER 0
#     define ACE_SCHED_FIFO 1
#     define ACE_SCHED_RR 2
#   endif /* ! ACE_HAS_PTHREADS */



//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
#   if defined (ACE_HAS_PTHREADS)
#     define ACE_SCHED_OTHER SCHED_OTHER
#     define ACE_SCHED_FIFO SCHED_FIFO
#     define ACE_SCHED_RR SCHED_RR


#     if defined(__GLIBC__)
#       if !defined (_XOPEN_SOURCE) \
           || (defined (_XOPEN_SOURCE) && (_XOPEN_SOURCE - 0) < 600)
// pthread_mutex_timedlock() prototype is not visible if _XOPEN_SOURCE
// is not >= 600 (i.e. for XPG6).
extern "C" int pthread_mutex_timedlock (pthread_mutex_t *mutex,
                                        const struct timespec * abstime);
#       endif  /* _XOPEN_SOURCE && _XOPEN_SOURCE < 600 */
#     endif /* __GLIBC__ */


// Definitions for mapping POSIX pthreads draft 6 into 1003.1c names

#     if defined (ACE_HAS_PTHREADS_DRAFT6)
#       define PTHREAD_SCOPE_PROCESS           PTHREAD_SCOPE_LOCAL
#       define PTHREAD_SCOPE_SYSTEM            PTHREAD_SCOPE_GLOBAL
#       define PTHREAD_CREATE_UNDETACHED       0
#       define PTHREAD_CREATE_DETACHED         1
#       define PTHREAD_CREATE_JOINABLE         0
#       define PTHREAD_EXPLICIT_SCHED          0
#       define PTHREAD_MIN_PRIORITY            0
#       define PTHREAD_MAX_PRIORITY            126
#     endif /* ACE_HAS_PTHREADS_DRAFT6 */

// Definitions for THREAD- and PROCESS-LEVEL priorities...some
// implementations define these while others don't.  In order to
// further complicate matters, we don't redefine the default (*_DEF)
// values if they've already been defined, which allows individual
// programs to have their own ACE-wide "default".

// PROCESS-level values
#     if defined (_POSIX_PRIORITY_SCHEDULING) && \
         !defined(_UNICOS) && !defined(UNIXWARE_7_1)
#       define ACE_PROC_PRI_FIFO_MIN  (sched_get_priority_min(SCHED_FIFO))
#       define ACE_PROC_PRI_RR_MIN    (sched_get_priority_min(SCHED_RR))
#       if defined (HPUX)
          // HP-UX's other is the SCHED_HPUX class, which uses historical
          // values that have reverse semantics from POSIX (low value is
          // more important priority). To use these in pthreads calls,
          // the values need to be converted. The other scheduling classes
          // don't need this special treatment.
#         define ACE_PROC_PRI_OTHER_MIN \
                      (sched_get_priority_min(SCHED_OTHER))
#       else
#         define ACE_PROC_PRI_OTHER_MIN (sched_get_priority_min(SCHED_OTHER))
#       endif /* HPUX */
#     else /* UNICOS is missing a sched_get_priority_min() implementation,
              SCO too */
#       define ACE_PROC_PRI_FIFO_MIN  0
#       define ACE_PROC_PRI_RR_MIN    0
#       define ACE_PROC_PRI_OTHER_MIN 0
#     endif

#     if defined (_POSIX_PRIORITY_SCHEDULING) && !defined(UNIXWARE_7_1)
#       define ACE_PROC_PRI_FIFO_MAX  (sched_get_priority_max(SCHED_FIFO))
#       define ACE_PROC_PRI_RR_MAX    (sched_get_priority_max(SCHED_RR))
#       if defined (HPUX)
#         define ACE_PROC_PRI_OTHER_MAX \
                      (sched_get_priority_max(SCHED_OTHER))
#       else
#         define ACE_PROC_PRI_OTHER_MAX (sched_get_priority_max(SCHED_OTHER))
#       endif /* HPUX */
#     else /* SCO missing sched_get_priority_max() implementation */
#       define ACE_PROC_PRI_FIFO_MAX  59
#       define ACE_PROC_PRI_RR_MAX    59
#       define ACE_PROC_PRI_OTHER_MAX 59
#     endif

#     if !defined(ACE_PROC_PRI_FIFO_DEF)
#       define ACE_PROC_PRI_FIFO_DEF (ACE_PROC_PRI_FIFO_MIN + (ACE_PROC_PRI_FIFO_MAX - ACE_PROC_PRI_FIFO_MIN)/2)
#     endif
#     if !defined(ACE_PROC_PRI_RR_DEF)
#       define ACE_PROC_PRI_RR_DEF (ACE_PROC_PRI_RR_MIN + (ACE_PROC_PRI_RR_MAX - ACE_PROC_PRI_RR_MIN)/2)
#     endif
#     if !defined(ACE_PROC_PRI_OTHER_DEF)
#       define ACE_PROC_PRI_OTHER_DEF (ACE_PROC_PRI_OTHER_MIN + (ACE_PROC_PRI_OTHER_MAX - ACE_PROC_PRI_OTHER_MIN)/2)
#     endif

// THREAD-level values
#     if defined(PRI_FIFO_MIN) && defined(PRI_FIFO_MAX) && defined(PRI_RR_MIN) && defined(PRI_RR_MAX) && defined(PRI_OTHER_MIN) && defined(PRI_OTHER_MAX)
#       if !defined (ACE_THR_PRI_FIFO_MIN)
#         define ACE_THR_PRI_FIFO_MIN  (long) PRI_FIFO_MIN
#       endif /* !ACE_THR_PRI_FIFO_MIN */
#       if !defined (ACE_THR_PRI_FIFO_MAX)
#         define ACE_THR_PRI_FIFO_MAX  (long) PRI_FIFO_MAX
#       endif /* !ACE_THR_PRI_FIFO_MAX */
#       if !defined (ACE_THR_PRI_RR_MIN)
#         define ACE_THR_PRI_RR_MIN    (long) PRI_RR_MIN
#       endif /* !ACE_THR_PRI_RR_MIN */
#       if !defined (ACE_THR_PRI_RR_MAX)
#         define ACE_THR_PRI_RR_MAX    (long) PRI_RR_MAX
#       endif /* !ACE_THR_PRI_RR_MAX */
#       if !defined (ACE_THR_PRI_OTHER_MIN)
#         define ACE_THR_PRI_OTHER_MIN (long) PRI_OTHER_MIN
#       endif /* !ACE_THR_PRI_OTHER_MIN */
#       if !defined (ACE_THR_PRI_OTHER_MAX)
#         define ACE_THR_PRI_OTHER_MAX (long) PRI_OTHER_MAX
#       endif /* !ACE_THR_PRI_OTHER_MAX */
#     elif defined (AIX)
        // AIX's priority range is 1 (low) to 127 (high). There aren't
        // any preprocessor macros I can find. PRIORITY_MIN is for
        // process priorities, as far as I can see, and does not apply
        // to thread priority. The 1 to 127 range is from the
        // pthread_attr_setschedparam man page (Steve Huston, 18-May-2001).
#       if !defined (ACE_THR_PRI_FIFO_MIN)
#         define ACE_THR_PRI_FIFO_MIN  (long) 1
#       endif /* !ACE_THR_PRI_FIFO_MIN */
#       if !defined (ACE_THR_PRI_FIFO_MAX)
#         define ACE_THR_PRI_FIFO_MAX  (long) 127
#       endif /* !ACE_THR_PRI_FIFO_MAX */
#       if !defined (ACE_THR_PRI_RR_MIN)
#         define ACE_THR_PRI_RR_MIN    (long) 1
#       endif /* !ACE_THR_PRI_RR_MIN */
#       if !defined (ACE_THR_PRI_RR_MAX)
#         define ACE_THR_PRI_RR_MAX    (long) 127
#       endif /* !ACE_THR_PRI_RR_MAX */
#       if !defined (ACE_THR_PRI_OTHER_MIN)
#         define ACE_THR_PRI_OTHER_MIN (long) 1
#       endif /* !ACE_THR_PRI_OTHER_MIN */
#       if !defined (ACE_THR_PRI_OTHER_MAX)
#         define ACE_THR_PRI_OTHER_MAX (long) 127
#       endif /* !ACE_THR_PRI_OTHER_MAX */
#     elif defined (sun)
#       if !defined (ACE_THR_PRI_FIFO_MIN)
#         define ACE_THR_PRI_FIFO_MIN  (long) 0
#       endif /* !ACE_THR_PRI_FIFO_MIN */
#       if !defined (ACE_THR_PRI_FIFO_MAX)
#         define ACE_THR_PRI_FIFO_MAX  (long) 59
#       endif /* !ACE_THR_PRI_FIFO_MAX */
#       if !defined (ACE_THR_PRI_RR_MIN)
#         define ACE_THR_PRI_RR_MIN    (long) 0
#       endif /* !ACE_THR_PRI_RR_MIN */
#       if !defined (ACE_THR_PRI_RR_MAX)
#         define ACE_THR_PRI_RR_MAX    (long) 59
#       endif /* !ACE_THR_PRI_RR_MAX */
#       if !defined (ACE_THR_PRI_OTHER_MIN)
#         define ACE_THR_PRI_OTHER_MIN (long) 0
#       endif /* !ACE_THR_PRI_OTHER_MIN */
#       if !defined (ACE_THR_PRI_OTHER_MAX)
#         define ACE_THR_PRI_OTHER_MAX (long) 127
#       endif /* !ACE_THR_PRI_OTHER_MAX */
#     else
#       if !defined (ACE_THR_PRI_FIFO_MIN)
#         define ACE_THR_PRI_FIFO_MIN  (long) ACE_PROC_PRI_FIFO_MIN
#       endif /* !ACE_THR_PRI_FIFO_MIN */
#       if !defined (ACE_THR_PRI_FIFO_MAX)
#         define ACE_THR_PRI_FIFO_MAX  (long) ACE_PROC_PRI_FIFO_MAX
#       endif /* !ACE_THR_PRI_FIFO_MAX */
#       if !defined (ACE_THR_PRI_RR_MIN)
#         define ACE_THR_PRI_RR_MIN    (long) ACE_PROC_PRI_RR_MIN
#       endif /* !ACE_THR_PRI_RR_MIN */
#       if !defined (ACE_THR_PRI_RR_MAX)
#         define ACE_THR_PRI_RR_MAX    (long) ACE_PROC_PRI_RR_MAX
#       endif /* !ACE_THR_PRI_RR_MAX */
#       if !defined (ACE_THR_PRI_OTHER_MIN)
#         define ACE_THR_PRI_OTHER_MIN (long) ACE_PROC_PRI_OTHER_MIN
#       endif /* !ACE_THR_PRI_OTHER_MIN */
#       if !defined (ACE_THR_PRI_OTHER_MAX)
#         define ACE_THR_PRI_OTHER_MAX (long) ACE_PROC_PRI_OTHER_MAX
#       endif /* !ACE_THR_PRI_OTHER_MAX */
#     endif
#     if !defined(ACE_THR_PRI_FIFO_DEF)
#       define ACE_THR_PRI_FIFO_DEF  ((ACE_THR_PRI_FIFO_MIN + ACE_THR_PRI_FIFO_MAX)/2)
#     endif
#     if !defined(ACE_THR_PRI_RR_DEF)
#       define ACE_THR_PRI_RR_DEF ((ACE_THR_PRI_RR_MIN + ACE_THR_PRI_RR_MAX)/2)
#     endif
#     if !defined(ACE_THR_PRI_OTHER_DEF)
#       define ACE_THR_PRI_OTHER_DEF ((ACE_THR_PRI_OTHER_MIN + ACE_THR_PRI_OTHER_MAX)/2)
#     endif

// Typedefs to help compatibility with Windows NT and Pthreads.
typedef pthread_t ACE_hthread_t;
typedef pthread_t ACE_thread_t;

#     if defined (ACE_HAS_TSS_EMULATION)
        typedef pthread_key_t ACE_OS_thread_key_t;
        typedef u_long ACE_thread_key_t;
#     else  /* ! ACE_HAS_TSS_EMULATION */
        typedef pthread_key_t ACE_thread_key_t;
#     endif /* ! ACE_HAS_TSS_EMULATION */

#     if !defined (ACE_LACKS_COND_T)
typedef pthread_mutex_t ACE_mutex_t;
typedef pthread_cond_t ACE_cond_t;
typedef pthread_condattr_t ACE_condattr_t;
typedef pthread_mutexattr_t ACE_mutexattr_t;
#     endif /* ! ACE_LACKS_COND_T */
typedef pthread_mutex_t ACE_thread_mutex_t;

#     if !defined (PTHREAD_CANCEL_DISABLE)
#       define PTHREAD_CANCEL_DISABLE      0
#     endif /* PTHREAD_CANCEL_DISABLE */

#     if !defined (PTHREAD_CANCEL_ENABLE)
#       define PTHREAD_CANCEL_ENABLE       0
#     endif /* PTHREAD_CANCEL_ENABLE */

#     if !defined (PTHREAD_CANCEL_DEFERRED)
#       define PTHREAD_CANCEL_DEFERRED     0
#     endif /* PTHREAD_CANCEL_DEFERRED */

#     if !defined (PTHREAD_CANCEL_ASYNCHRONOUS)
#       define PTHREAD_CANCEL_ASYNCHRONOUS 0
#     endif /* PTHREAD_CANCEL_ASYNCHRONOUS */

#     define THR_CANCEL_DISABLE      PTHREAD_CANCEL_DISABLE
#     define THR_CANCEL_ENABLE       PTHREAD_CANCEL_ENABLE
#     define THR_CANCEL_DEFERRED     PTHREAD_CANCEL_DEFERRED
#     define THR_CANCEL_ASYNCHRONOUS PTHREAD_CANCEL_ASYNCHRONOUS

#     if !defined (PTHREAD_CREATE_JOINABLE)
#       if defined (PTHREAD_CREATE_UNDETACHED)
#         define PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_UNDETACHED
#       else
#         define PTHREAD_CREATE_JOINABLE 0
#       endif /* PTHREAD_CREATE_UNDETACHED */
#     endif /* PTHREAD_CREATE_JOINABLE */

#     if !defined (PTHREAD_CREATE_DETACHED)
#       define PTHREAD_CREATE_DETACHED 1
#     endif /* PTHREAD_CREATE_DETACHED */

#     if !defined (PTHREAD_PROCESS_PRIVATE) && !defined (ACE_HAS_PTHREAD_PROCESS_ENUM)
#       if defined (PTHREAD_MUTEXTYPE_FAST)
#         define PTHREAD_PROCESS_PRIVATE PTHREAD_MUTEXTYPE_FAST
#       else
#         define PTHREAD_PROCESS_PRIVATE 0
#       endif /* PTHREAD_MUTEXTYPE_FAST */
#     endif /* PTHREAD_PROCESS_PRIVATE */

#     if !defined (PTHREAD_PROCESS_SHARED) && !defined (ACE_HAS_PTHREAD_PROCESS_ENUM)
#       if defined (PTHREAD_MUTEXTYPE_FAST)
#         define PTHREAD_PROCESS_SHARED PTHREAD_MUTEXTYPE_FAST
#       else
#         define PTHREAD_PROCESS_SHARED 1
#       endif /* PTHREAD_MUTEXTYPE_FAST */
#     endif /* PTHREAD_PROCESS_SHARED */

#     if defined (ACE_HAS_PTHREADS_DRAFT4)
#       if defined (PTHREAD_PROCESS_PRIVATE)
#         if !defined (USYNC_THREAD)
#           define USYNC_THREAD    PTHREAD_PROCESS_PRIVATE
#         endif /* ! USYNC_THREAD */
#       else
#         if !defined (USYNC_THREAD)
#           define USYNC_THREAD    MUTEX_NONRECURSIVE_NP
#         endif /* ! USYNC_THREAD */
#       endif /* PTHREAD_PROCESS_PRIVATE */

#       if defined (PTHREAD_PROCESS_SHARED)
#         if !defined (USYNC_PROCESS)
#           define USYNC_PROCESS   PTHREAD_PROCESS_SHARED
#         endif /* ! USYNC_PROCESS */
#       else
#         if !defined (USYNC_PROCESS)
#           define USYNC_PROCESS   MUTEX_NONRECURSIVE_NP
#         endif /* ! USYNC_PROCESS */
#       endif /* PTHREAD_PROCESS_SHARED */
#     elif !defined (ACE_HAS_STHREADS)
#       if !defined (USYNC_THREAD)
#         define USYNC_THREAD PTHREAD_PROCESS_PRIVATE
#       endif /* ! USYNC_THREAD */
#       if !defined (USYNC_PROCESS)
#         define USYNC_PROCESS PTHREAD_PROCESS_SHARED
#       endif /* ! USYNC_PROCESS */
#     endif /* ACE_HAS_PTHREADS_DRAFT4 */

/* MM-Graz:  prevent warnings */
#     undef THR_BOUND
#     undef THR_NEW_LWP
#     undef THR_DETACHED
#     undef THR_SUSPENDED
#     undef THR_DAEMON

#     define THR_BOUND               0x00000001
#     if defined (CHORUS)
#       define THR_NEW_LWP             0x00000000
#     else
#       define THR_NEW_LWP             0x00000002
#     endif /* CHORUS */
#     define THR_DETACHED            0x00000040
#     define THR_SUSPENDED           0x00000080
#     define THR_DAEMON              0x00000100
#     define THR_JOINABLE            0x00010000
#     define THR_SCHED_FIFO          0x00020000
#     define THR_SCHED_RR            0x00040000
#     define THR_SCHED_DEFAULT       0x00080000

#     if defined (ACE_HAS_IRIX62_THREADS)
#       define THR_SCOPE_SYSTEM        0x00100000
#     else
#       define THR_SCOPE_SYSTEM        THR_BOUND
#     endif /*ACE_HAS_IRIX62_THREADS*/

#     define THR_SCOPE_PROCESS       0x00200000
#     define THR_INHERIT_SCHED       0x00400000
#     define THR_EXPLICIT_SCHED      0x00800000
#     define THR_SCHED_IO            0x01000000

// still in pthreads
////////////////////////////////////////////////
#     if !defined (ACE_HAS_STHREADS)
#       if !defined (ACE_HAS_POSIX_SEM)
/**
 * @class ACE_sema_t
 *
 * @brief This is used to implement semaphores for platforms that support
 * POSIX pthreads, but do *not* support POSIX semaphores, i.e.,
 * it's a different type than the POSIX <sem_t>.
 */

class ACE_OS;

// this is used by linux
class ACE_OS_Export ACE_sema_t
{
friend class ACE_OS;
protected:
  /// Serialize access to internal state.
  ACE_mutex_t lock_;

  /// Block until there are no waiters.
  ACE_cond_t count_nonzero_;

  /// Count of the semaphore.
  u_long count_;

  /// Number of threads that have called <ACE_OS::sema_wait>.
  u_long waiters_;
};
#       endif /* !ACE_HAS_POSIX_SEM */

#       if defined (ACE_LACKS_PTHREAD_YIELD) && defined (ACE_HAS_THR_YIELD)
// looks like sun with pthreads and sthreads

        // If we are on Solaris we can just reuse the existing
        // implementations of these synchronization types.
#         if !defined (ACE_LACKS_RWLOCK_T)
#           include /**/ <synch.h>
typedef rwlock_t ACE_rwlock_t;
#         endif /* !ACE_LACKS_RWLOCK_T */
#         include /**/ <thread.h>
#       endif /* (ACE_LACKS_PTHREAD_YIELD) && defined (ACE_HAS_THR_YIELD) */
#     else /* !ACE_HAS_STHREADS */
//sthreads w/ posix threads
#       if !defined (ACE_HAS_POSIX_SEM)
// must be the sun sema_t
typedef sema_t ACE_sema_t;
#       endif /* !ACE_HAS_POSIX_SEM */
#     endif /* !ACE_HAS_STHREADS */



//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
#   elif defined (ACE_HAS_STHREADS)
// Solaris threads, without PTHREADS.
// Typedefs to help compatibility with Windows NT and Pthreads.
typedef thread_t ACE_thread_t;
typedef thread_key_t ACE_thread_key_t;
typedef mutex_t ACE_mutex_t;
#     if !defined (ACE_LACKS_RWLOCK_T)
typedef rwlock_t ACE_rwlock_t;
#     endif /* !ACE_LACKS_RWLOCK_T */
#     if !defined (ACE_HAS_POSIX_SEM)
typedef sema_t ACE_sema_t;
#     endif /* !ACE_HAS_POSIX_SEM */

typedef cond_t ACE_cond_t;
struct ACE_OS_Export ACE_condattr_t
{
  int type;
};
struct ACE_OS_Export ACE_mutexattr_t
{
  int type;
};
typedef ACE_thread_t ACE_hthread_t;
typedef ACE_mutex_t ACE_thread_mutex_t;

#     define THR_CANCEL_DISABLE      0
#     define THR_CANCEL_ENABLE       0
#     define THR_CANCEL_DEFERRED     0
#     define THR_CANCEL_ASYNCHRONOUS 0
#     define THR_JOINABLE            0
#     define THR_SCHED_FIFO          0
#     define THR_SCHED_RR            0
#     define THR_SCHED_DEFAULT       0

#   elif defined (ACE_PSOS)

// Some versions of pSOS provide native mutex support.  For others,
// implement ACE_thread_mutex_t and ACE_mutex_t using pSOS semaphores.
// Either way, the types are all u_longs.
typedef u_long ACE_mutex_t;
typedef u_long ACE_thread_mutex_t;
typedef u_long ACE_thread_t;
typedef u_long ACE_hthread_t;

#     if defined (ACE_PSOS_HAS_COND_T)
typedef u_long ACE_cond_t;
typedef u_long ACE_condattr_t;
struct ACE_OS_Export ACE_mutexattr_t
{
  int type;
};
#     endif /* ACE_PSOS_HAS_COND_T */


// TCB registers 0-7 are for application use
#     define PSOS_TASK_REG_TSS 0
#     define PSOS_TASK_REG_MAX 7

#     define PSOS_TASK_MIN_PRIORITY   1
#     define PSOS_TASK_MAX_PRIORITY 239

// Key type: the ACE TSS emulation requires the key type be unsigned,
// for efficiency.  Current POSIX and Solaris TSS implementations also
// use unsigned int, so the ACE TSS emulation is compatible with them.
// Native pSOS TSD, where available, uses unsigned long as the key type.
#     if defined (ACE_PSOS_HAS_TSS)
typedef u_long ACE_thread_key_t;
#     else
typedef u_int ACE_thread_key_t;
#     endif /* ACE_PSOS_HAS_TSS */

#     define THR_CANCEL_DISABLE      0  /* thread can never be cancelled */
#     define THR_CANCEL_ENABLE       0      /* thread can be cancelled */
#     define THR_CANCEL_DEFERRED     0      /* cancellation deferred to cancellation point */
#     define THR_CANCEL_ASYNCHRONOUS 0      /* cancellation occurs immediately */

#     define THR_BOUND               0
#     define THR_NEW_LWP             0
#     define THR_DETACHED            0
#     define THR_SUSPENDED           0
#     define THR_DAEMON              0
#     define THR_JOINABLE            0

#     define THR_SCHED_FIFO          0
#     define THR_SCHED_RR            0
#     define THR_SCHED_DEFAULT       0
#     define USYNC_THREAD            T_LOCAL
#     define USYNC_PROCESS           T_GLOBAL

/* from psos.h */
/* #define T_NOPREEMPT     0x00000001   Not preemptible bit */
/* #define T_PREEMPT       0x00000000   Preemptible */
/* #define T_TSLICE        0x00000002   Time-slicing enabled bit */
/* #define T_NOTSLICE      0x00000000   No Time-slicing */
/* #define T_NOASR         0x00000004   ASRs disabled bit */
/* #define T_ASR           0x00000000   ASRs enabled */

/* #define SM_GLOBAL       0x00000001  1 = Global */
/* #define SM_LOCAL        0x00000000  0 = Local */
/* #define SM_PRIOR        0x00000002  Queue by priority */
/* #define SM_FIFO         0x00000000  Queue by FIFO order */

/* #define T_NOFPU         0x00000000   Not using FPU */
/* #define T_FPU           0x00000002   Using FPU bit */

#   elif defined (VXWORKS)
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

//#     define MAXPATHLEN  1024
//#     define MAXNAMLEN   255
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
#   endif /* !ACE_HAS_POSIX_SEM */

typedef char * ACE_thread_t;
typedef int ACE_hthread_t;
// Key type: the ACE TSS emulation requires the key type be unsigned,
// for efficiency.  (Current POSIX and Solaris TSS implementations also
// use u_int, so the ACE TSS emulation is compatible with them.)
typedef u_int ACE_thread_key_t;

      // Marker for ACE_Thread_Manager to indicate that it allocated
      // an ACE_thread_t.  It is placed at the beginning of the ID.
#   define ACE_THR_ID_ALLOCATED '\022'


////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
# elif defined (ACE_HAS_WTHREADS)

typedef DWORD ACE_thread_t;
typedef HANDLE ACE_hthread_t;

//#   define ACE_INVALID_PID ((pid_t) -1)
#   if defined (ACE_HAS_TSS_EMULATION)
      typedef DWORD ACE_OS_thread_key_t;
      typedef u_int ACE_thread_key_t;
#   else  /* ! ACE_HAS_TSS_EMULATION */
      typedef DWORD ACE_thread_key_t;
#   endif /* ! ACE_HAS_TSS_EMULATION */

typedef CRITICAL_SECTION ACE_thread_mutex_t;

typedef struct
{
  int type_; // Either USYNC_THREAD or USYNC_PROCESS
  union
  {
    HANDLE proc_mutex_;
    CRITICAL_SECTION thr_mutex_;
  };
} ACE_mutex_t;

// Wrapper for NT Events.
typedef HANDLE ACE_event_t;
#define ACE_EVENT_T_DEFINED

//@@ ACE_USES_WINCE_SEMA_SIMULATION is used to debug
//   semaphore simulation on WinNT.  It should be
//   changed to ACE_USES_HAS_WINCE at some later point.
#   if !defined (ACE_USES_WINCE_SEMA_SIMULATION)
typedef HANDLE ACE_sema_t;
#   else
/**
 * @class ACE_sema_t
 *
 * @brief Semaphore simulation for Windows CE.
 */
class ACE_OS_Export ACE_sema_t
{
public:
  /// Serializes access to <count_>.
  ACE_thread_mutex_t lock_;

  /// This event is signaled whenever the count becomes non-zero.
  ACE_event_t count_nonzero_;

  /// Current count of the semaphore.
  u_int count_;
};

#   endif /* ACE_USES_WINCE_SEMA_SIMULATION */

// These need to be different values, neither of which can be 0...
#   define USYNC_THREAD 1
#   define USYNC_PROCESS 2

#   define THR_CANCEL_DISABLE      0
#   define THR_CANCEL_ENABLE       0
#   define THR_CANCEL_DEFERRED     0
#   define THR_CANCEL_ASYNCHRONOUS 0
#   define THR_DETACHED            0x02000000 /* ignore in most places */
#   define THR_BOUND               0          /* ignore in most places */
#   define THR_NEW_LWP             0          /* ignore in most places */
#   define THR_DAEMON              0          /* ignore in most places */
#   define THR_JOINABLE            0          /* ignore in most places */
#   define THR_SUSPENDED   CREATE_SUSPENDED
#   define THR_USE_AFX             0x01000000
#   define THR_SCHED_FIFO          0
#   define THR_SCHED_RR            0
#   define THR_SCHED_DEFAULT       0
#   define THR_SCOPE_PROCESS       0
#   define THR_SCOPE_SYSTEM        0
# endif /* ACE_HAS_PTHREADS / STHREADS / PSOS / VXWORKS / WTHREADS */

# if defined (ACE_LACKS_COND_T) && defined (ACE_WIN32)
/**
 * @class ACE_cond_t
 *
 * @brief This structure is used to implement condition variables on
 * platforms that lack it natively, such as VxWorks, pSoS, and
 * Win32.
 *
 * At the current time, this stuff only works for threads
 * within the same process.
 */
class ACE_OS_Export ACE_cond_t
{
public:
  friend class ACE_OS;

  /// Returns the number of waiters.
  long waiters (void) const;

protected:
  /// Number of waiting threads.
  long waiters_;

  /// Serialize access to the waiters count.
  ACE_thread_mutex_t waiters_lock_;

  /// Queue up threads waiting for the condition to become signaled.
  ACE_sema_t sema_;

#   if defined (VXWORKS) || defined (ACE_PSOS)
  /**
   * A semaphore used by the broadcast/signal thread to wait for all
   * the waiting thread(s) to wake up and be released from the
   * semaphore.
   */
  ACE_sema_t waiters_done_;
#   elif defined (ACE_WIN32)
  /**
   * An auto reset event used by the broadcast/signal thread to wait
   * for the waiting thread(s) to wake up and get a chance at the
   * semaphore.
   */
  HANDLE waiters_done_;
#   else
#     error "Please implement this feature or check your config.h file!"
#   endif /* VXWORKS || ACE_PSOS */

  /// Keeps track of whether we were broadcasting or just signaling.
  size_t was_broadcast_;
};

struct ACE_OS_Export ACE_condattr_t
{
  int type;
};

struct ACE_OS_Export ACE_mutexattr_t
{
  int type;
};

# endif /* ACE_LACKS_COND_T */

# if defined (ACE_LACKS_RWLOCK_T) && !defined (ACE_HAS_PTHREADS_UNIX98_EXT)

/**
 * @class ACE_rwlock_t
 *
 * @brief This is used to implement readers/writer locks on NT,
 *     VxWorks, and POSIX pthreads.
 *
 *     At the current time, this stuff only works for threads
 *     within the same process.
 */
struct ACE_OS_Export ACE_rwlock_t
{
protected:
  friend class ACE_OS;

  ACE_mutex_t lock_;
  // Serialize access to internal state.

  ACE_cond_t waiting_readers_;
  // Reader threads waiting to acquire the lock.

  int num_waiting_readers_;
  // Number of waiting readers.

  ACE_cond_t waiting_writers_;
  // Writer threads waiting to acquire the lock.

  int num_waiting_writers_;
  // Number of waiting writers.

  int ref_count_;
  // Value is -1 if writer has the lock, else this keeps track of the
  // number of readers holding the lock.

  int important_writer_;
  // indicate that a reader is trying to upgrade

  ACE_cond_t waiting_important_writer_;
  // condition for the upgrading reader
};
# elif defined (ACE_HAS_PTHREADS_UNIX98_EXT)
typedef pthread_rwlock_t ACE_rwlock_t;
# elif defined (ACE_HAS_STHREADS)
#   include /**/ <synch.h>
typedef rwlock_t ACE_rwlock_t;
# endif /* ACE_LACKS_RWLOCK_T */

// Define some default thread priorities on all threaded platforms, if
// not defined above or in the individual platform config file.
// ACE_THR_PRI_FIFO_DEF should be used by applications for default
// real-time thread priority.  ACE_THR_PRI_OTHER_DEF should be used
// for non-real-time priority.
# if !defined(ACE_THR_PRI_FIFO_DEF)
#   if defined (ACE_WTHREADS)
      // It would be more in spirit to use THREAD_PRIORITY_NORMAL.  But,
      // using THREAD_PRIORITY_ABOVE_NORMAL should give preference to the
      // threads in this process, even if the process is not in the
      // REALTIME_PRIORITY_CLASS.
#     define ACE_THR_PRI_FIFO_DEF THREAD_PRIORITY_ABOVE_NORMAL
#   else  /* ! ACE_WTHREADS */
#     define ACE_THR_PRI_FIFO_DEF 0
#   endif /* ! ACE_WTHREADS */
# endif /* ! ACE_THR_PRI_FIFO_DEF */

# if !defined(ACE_THR_PRI_OTHER_DEF)
#   if defined (ACE_WTHREADS)
      // It would be more in spirit to use THREAD_PRIORITY_NORMAL.  But,
      // using THREAD_PRIORITY_ABOVE_NORMAL should give preference to the
      // threads in this process, even if the process is not in the
      // REALTIME_PRIORITY_CLASS.
#     define ACE_THR_PRI_OTHER_DEF THREAD_PRIORITY_NORMAL
#   else  /* ! ACE_WTHREADS */
#     define ACE_THR_PRI_OTHER_DEF 0
#   endif /* ! ACE_WTHREADS */
# endif /* ! ACE_THR_PRI_OTHER_DEF */

# if defined (ACE_HAS_RECURSIVE_MUTEXES)
typedef ACE_thread_mutex_t ACE_recursive_thread_mutex_t;
# else
/**
 * @class ACE_recursive_thread_mutex_t
 *
 * @brief Implement a thin C++ wrapper that allows nested acquisition
 * and release of a mutex that occurs in the same thread.
 *
 * This implementation is based on an algorithm sketched by Dave
 * Butenhof <butenhof@zko.dec.com>.  Naturally, I take the
 * credit for any mistakes ;-)
 */
class ACE_recursive_thread_mutex_t
{
public:
  /// Guards the state of the nesting level and thread id.
  ACE_thread_mutex_t nesting_mutex_;

  /// This condition variable suspends other waiting threads until the
  /// mutex is available.
  ACE_cond_t lock_available_;

  /// Current nesting level of the recursion.
  int nesting_level_;

  /// Current owner of the lock.
  ACE_thread_t owner_id_;
};
# endif /* ACE_WIN32 */

#else /* !ACE_HAS_THREADS, i.e., the OS/platform doesn't support threading. */

// Give these things some reasonable value...
# define ACE_SCOPE_PROCESS 0
# define ACE_SCOPE_LWP 1
# define ACE_SCOPE_THREAD 2
# define ACE_SCHED_OTHER 0
# define ACE_SCHED_FIFO 1
# define ACE_SCHED_RR 2
# if !defined (THR_CANCEL_DISABLE)
#   define THR_CANCEL_DISABLE      0
# endif /* ! THR_CANCEL_DISABLE */
# if !defined (THR_CANCEL_ENABLE)
#   define THR_CANCEL_ENABLE       0
# endif /* ! THR_CANCEL_ENABLE */
# if !defined (THR_CANCEL_DEFERRED)
#   define THR_CANCEL_DEFERRED     0
# endif /* ! THR_CANCEL_DEFERRED */
# if !defined (THR_CANCEL_ASYNCHRONOUS)
#   define THR_CANCEL_ASYNCHRONOUS 0
# endif /* ! THR_CANCEL_ASYNCHRONOUS */
# if !defined (THR_JOINABLE)
#   define THR_JOINABLE    0     /* ignore in most places */
# endif /* ! THR_JOINABLE */
# if !defined (THR_DETACHED)
#   define THR_DETACHED    0     /* ignore in most places */
# endif /* ! THR_DETACHED */
# if !defined (THR_DAEMON)
#   define THR_DAEMON      0     /* ignore in most places */
# endif /* ! THR_DAEMON */
# if !defined (THR_BOUND)
#   define THR_BOUND       0     /* ignore in most places */
# endif /* ! THR_BOUND */
# if !defined (THR_NEW_LWP)
#   define THR_NEW_LWP     0     /* ignore in most places */
# endif /* ! THR_NEW_LWP */
# if !defined (THR_SUSPENDED)
#   define THR_SUSPENDED   0     /* ignore in most places */
# endif /* ! THR_SUSPENDED */
# if !defined (THR_SCHED_FIFO)
#   define THR_SCHED_FIFO 0
# endif /* ! THR_SCHED_FIFO */
# if !defined (THR_SCHED_RR)
#   define THR_SCHED_RR 0
# endif /* ! THR_SCHED_RR */
# if !defined (THR_SCHED_DEFAULT)
#   define THR_SCHED_DEFAULT 0
# endif /* ! THR_SCHED_DEFAULT */
# if !defined (USYNC_THREAD)
#   define USYNC_THREAD 0
# endif /* ! USYNC_THREAD */
# if !defined (USYNC_PROCESS)
#   define USYNC_PROCESS 0
# endif /* ! USYNC_PROCESS */
# if !defined (THR_SCOPE_PROCESS)
#   define THR_SCOPE_PROCESS 0
# endif /* ! THR_SCOPE_PROCESS */
# if !defined (THR_SCOPE_SYSTEM)
#   define THR_SCOPE_SYSTEM 0
# endif /* ! THR_SCOPE_SYSTEM */

// These are dummies needed for class OS.h
typedef int ACE_cond_t;
struct ACE_OS_Export ACE_condattr_t
{
  int type;
};
struct ACE_OS_Export ACE_mutexattr_t
{
  int type;
};
typedef int ACE_mutex_t;
typedef int ACE_thread_mutex_t;
typedef int ACE_recursive_thread_mutex_t;
# if !defined (ACE_HAS_POSIX_SEM) && !defined (ACE_PSOS)
typedef int ACE_sema_t;
# endif /* !ACE_HAS_POSIX_SEM && !ACE_PSOS */
typedef int ACE_rwlock_t;
typedef int ACE_thread_t;
typedef int ACE_hthread_t;
typedef u_int ACE_thread_key_t;

// Ensure that ACE_THR_PRI_FIFO_DEF and ACE_THR_PRI_OTHER_DEF are
// defined on non-threaded platforms, to support application source
// code compatibility.  ACE_THR_PRI_FIFO_DEF should be used by
// applications for default real-time thread priority.
// ACE_THR_PRI_OTHER_DEF should be used for non-real-time priority.
# if !defined(ACE_THR_PRI_FIFO_DEF)
#   define ACE_THR_PRI_FIFO_DEF 0
# endif /* ! ACE_THR_PRI_FIFO_DEF */
# if !defined(ACE_THR_PRI_OTHER_DEF)
#   define ACE_THR_PRI_OTHER_DEF 0
# endif /* ! ACE_THR_PRI_OTHER_DEF */

#endif /* ACE_HAS_THREADS */




#if !defined (ACE_EVENT_T_DEFINED)
// Wrapper for NT events on UNIX.
class ACE_OS_Export ACE_event_t
{
  friend class ACE_OS;
protected:
  /// Protect critical section.
  ACE_mutex_t lock_;

  /// Keeps track of waiters.
  ACE_cond_t condition_;

  /// Specifies if this is an auto- or manual-reset event.
  int manual_reset_;

  /// "True" if signaled.
  int is_signaled_;

  /// Number of waiting threads.
  u_long waiting_threads_;
};
#define ACE_EVENT_T_DEFINED
#endif /* ACE_EVENT_T_DEFINED */


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
#   include /**/ <netinet/in.h>
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


/**
 * @class ACE_Thread_ID
 *
 * @brief Defines a platform-independent thread ID.
 */
class ACE_OS_Export ACE_Thread_ID
{
public:
  // = Initialization method.
  ACE_Thread_ID (ACE_thread_t, ACE_hthread_t);
  ACE_Thread_ID (const ACE_Thread_ID &id);

  // = Set/Get the Thread ID.
  ACE_thread_t id (void);
  void id (ACE_thread_t);

  // = Set/Get the Thread handle.
  ACE_hthread_t handle (void);
  void handle (ACE_hthread_t);

  // != Comparison operator.
  int operator== (const ACE_Thread_ID &) const;
  int operator!= (const ACE_Thread_ID &) const;

private:
  /// Identify the thread.
  ACE_thread_t thread_id_;

  /// Handle to the thread (typically used to "wait" on Win32).
  ACE_hthread_t thread_handle_;
};





// mit pthread stuff
#if defined (ACE_HAS_RECV_TIMEDWAIT) && defined (ACE_LACKS_TIMEDWAIT_PROTOTYPES)
extern "C" ssize_t recv_timedwait (ACE_HANDLE handle,
                                   char *buf,
                                   int len,
                                   int flags,
                                   struct timespec *timeout);
extern "C" ssize_t read_timedwait (ACE_HANDLE handle,
                                   char *buf,
                                   size_t n,
                                   struct timespec *timeout);
extern "C" ssize_t recvmsg_timedwait (ACE_HANDLE handle,
                                      struct msghdr *msg,
                                      int flags,
                                      struct timespec *timeout);
extern "C" ssize_t recvfrom_timedwait (ACE_HANDLE handle,
                                       char *buf,
                                       int len,
                                       int flags,
                                       struct sockaddr *addr,
                                       int
                                       *addrlen,
                                       struct timespec *timeout);
extern "C" ssize_t readv_timedwait (ACE_HANDLE handle,
                                    iovec *iov,
                                    int iovcnt,
                                    struct timespec* timeout);
extern "C" ssize_t send_timedwait (ACE_HANDLE handle,
                                   const char *buf,
                                   int len,
                                   int flags,
                                   struct timespec *timeout);
extern "C" ssize_t write_timedwait (ACE_HANDLE handle,
                                    const void *buf,
                                    size_t n,
                                    struct timespec *timeout);
extern "C" ssize_t sendmsg_timedwait (ACE_HANDLE handle,
                                      ACE_SENDMSG_TYPE *msg,
                                      int flags,
                                      struct timespec *timeout);
extern "C" ssize_t sendto_timedwait (ACE_HANDLE handle,
                                     const char *buf,
                                     int len,
                                     int flags,
                                     const struct sockaddr *addr,
                                     int addrlen,
                                     struct timespec *timeout);
extern "C" ssize_t writev_timedwait (ACE_HANDLE handle,
                                     ACE_WRITEV_TYPE *iov,
                                     int iovcnt,
                                     struct timespec *timeout);
#endif /* ACE_LACKS_TIMEDWAIT_PROTOTYPES */


#endif /* ACE_ACE_THREADS_H */
