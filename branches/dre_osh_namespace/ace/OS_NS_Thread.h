// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_Thread.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_THREAD_H
# define ACE_OS_NS_THREAD_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# include "ace/Global_Macros.h"
# include "ace/Default_Constants.h"
# include "ace/os_include/os_pthread.h"
# include "ace/Base_Thread_Adapter.h"
# include "ace/os_include/sys/os_sem.h"
# include "ace/OS_Memory.h"

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

# if defined (ACE_PSOS)
// Use pSOS semaphores, wrapped . . .
typedef struct
{
  /// Semaphore handle.  This is allocated by pSOS.
  u_long sema_;

  /// Name of the semaphore: really a 32 bit number to pSOS
  char name_[4];
} ACE_sema_t;
# endif /* ACE_PSOS */

# if defined (ACE_PSOS)

// Wrapper for NT events on pSOS.
class ACE_OS_Export ACE_event_t
{
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

# endif /* ACE_PSOS */

# if defined (ACE_WIN32)
typedef DWORD ACE_thread_t;
typedef HANDLE ACE_hthread_t;
#   if defined (ACE_HAS_TSS_EMULATION)
      typedef DWORD ACE_OS_thread_key_t;
      typedef u_int ACE_thread_key_t;
#   else  /* ! ACE_HAS_TSS_EMULATION */
      typedef DWORD ACE_thread_key_t;
#   endif /* ! ACE_HAS_TSS_EMULATION */
# endif /* ACE_WIN32 */

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

#   if defined (ACE_HAS_PTHREADS)
// moved to pthread.h
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
#     include /**/ <sysLib.h> // for sysClkRateGet()
#     include /**/ <taskLib.h>
#     include /**/ <taskHookLib.h>

// make sure these are included for VXWORKS.
// @todo move these to a common place, perhaps the top of the file.
#     include "ace/os_include/os_fcntl.h"
#     include "ace/os_include/os_netdb.h"
#     include "ace/os_include/os_semaphore.h"
#     include "ace/os_include/os_signal.h"
#     include "ace/os_include/os_stdio.h"
#     include "ace/os_include/os_stdlib.h"
#     include "ace/os_include/os_stropts.h"
#     include "ace/os_include/os_unistd.h"
#     include "ace/os_include/arpa/os_inet.h"
#     include "ace/os_include/sys/os_select.h"
#     include "ace/os_include/sys/os_socket.h"

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

typedef SEM_ID ACE_mutex_t;
// Implement ACE_thread_mutex_t with ACE_mutex_t because there's just
// one process . . .
typedef ACE_mutex_t ACE_thread_mutex_t;
#     if !defined (ACE_HAS_POSIX_SEM)
// Use VxWorks semaphores, wrapped ...
typedef struct
{
  /// Semaphore handle.  This is allocated by VxWorks.
  SEM_ID sema_;

  /// Name of the semaphore:  always NULL with VxWorks.
  char *name_;
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

#   elif defined (ACE_HAS_WTHREADS)

typedef CRITICAL_SECTION ACE_thread_mutex_t;

typedef struct
{
  /// Either USYNC_THREAD or USYNC_PROCESS
  int type_;
  union
  {
    HANDLE proc_mutex_;
    CRITICAL_SECTION thr_mutex_;
  };
} ACE_mutex_t;

// Wrapper for NT Events.
typedef HANDLE ACE_event_t;

#     if defined (ACE_WIN32)
//@@ ACE_USES_WINCE_SEMA_SIMULATION is used to debug
//   semaphore simulation on WinNT.  It should be
//   changed to ACE_USES_HAS_WINCE at some later point.
#       if !defined (ACE_USES_WINCE_SEMA_SIMULATION)
typedef HANDLE ACE_sema_t;
#       else
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

#       endif /* ACE_USES_WINCE_SEMA_SIMULATION */
#     endif /* defined (ACE_WIN32) */

// These need to be different values, neither of which can be 0...
#     define USYNC_THREAD 1
#     define USYNC_PROCESS 2

#     define THR_CANCEL_DISABLE      0
#     define THR_CANCEL_ENABLE       0
#     define THR_CANCEL_DEFERRED     0
#     define THR_CANCEL_ASYNCHRONOUS 0
#     define THR_DETACHED            0x02000000 /* ignore in most places */
#     define THR_BOUND               0          /* ignore in most places */
#     define THR_NEW_LWP             0          /* ignore in most places */
#     define THR_DAEMON              0          /* ignore in most places */
#     define THR_JOINABLE            0          /* ignore in most places */
#     define THR_SUSPENDED   CREATE_SUSPENDED
#     define THR_USE_AFX             0x01000000
#     define THR_SCHED_FIFO          0
#     define THR_SCHED_RR            0
#     define THR_SCHED_DEFAULT       0
#     define THR_SCOPE_PROCESS       0
#     define THR_SCOPE_SYSTEM        0
#   endif /* ACE_HAS_PTHREADS / STHREADS / PSOS / VXWORKS / WTHREADS **********/

// If we're using PACE then we don't want this class (since PACE
// takes care of it) unless we're on Windows. Win32 mutexes, semaphores,
// and condition variables are not yet supported in PACE.
#   if defined (ACE_LACKS_COND_T)
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

  /// Returns the number of waiters.
  long waiters (void) const;

protected:
  /// Number of waiting threads.
  long waiters_;

  /// Serialize access to the waiters count.
  ACE_thread_mutex_t waiters_lock_;

  /// Queue up threads waiting for the condition to become signaled.
  ACE_sema_t sema_;

#     if defined (VXWORKS) || defined (ACE_PSOS)
  /**
   * A semaphore used by the broadcast/signal thread to wait for all
   * the waiting thread(s) to wake up and be released from the
   * semaphore.
   */
  ACE_sema_t waiters_done_;
#     elif defined (ACE_WIN32)
  /**
   * An auto reset event used by the broadcast/signal thread to wait
   * for the waiting thread(s) to wake up and get a chance at the
   * semaphore.
   */
  HANDLE waiters_done_;
#     else
#       error "Please implement this feature or check your config.h file!"
#     endif /* VXWORKS || ACE_PSOS */

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
#   endif /* ACE_LACKS_COND_T */

#   if defined (ACE_LACKS_RWLOCK_T) && !defined (ACE_HAS_PTHREADS_UNIX98_EXT)

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
//#   elif defined (ACE_HAS_PTHREADS_UNIX98_EXT)
//typedef pthread_rwlock_t ACE_rwlock_t;
#   elif defined (ACE_HAS_STHREADS)
#     include /**/ <synch.h>
typedef rwlock_t ACE_rwlock_t;
#   endif /* ACE_LACKS_RWLOCK_T */

// Define some default thread priorities on all threaded platforms, if
// not defined above or in the individual platform config file.
// ACE_THR_PRI_FIFO_DEF should be used by applications for default
// real-time thread priority.  ACE_THR_PRI_OTHER_DEF should be used
// for non-real-time priority.
#   if !defined(ACE_THR_PRI_FIFO_DEF)
#     if defined (ACE_WTHREADS)
      // It would be more in spirit to use THREAD_PRIORITY_NORMAL.  But,
      // using THREAD_PRIORITY_ABOVE_NORMAL should give preference to the
      // threads in this process, even if the process is not in the
      // REALTIME_PRIORITY_CLASS.
#       define ACE_THR_PRI_FIFO_DEF THREAD_PRIORITY_ABOVE_NORMAL
#     else  /* ! ACE_WTHREADS */
#       define ACE_THR_PRI_FIFO_DEF 0
#     endif /* ! ACE_WTHREADS */
#   endif /* ! ACE_THR_PRI_FIFO_DEF */

#   if !defined(ACE_THR_PRI_OTHER_DEF)
#     if defined (ACE_WTHREADS)
      // It would be more in spirit to use THREAD_PRIORITY_NORMAL.  But,
      // using THREAD_PRIORITY_ABOVE_NORMAL should give preference to the
      // threads in this process, even if the process is not in the
      // REALTIME_PRIORITY_CLASS.
#       define ACE_THR_PRI_OTHER_DEF THREAD_PRIORITY_NORMAL
#     else  /* ! ACE_WTHREADS */
#       define ACE_THR_PRI_OTHER_DEF 0
#     endif /* ! ACE_WTHREADS */
#   endif /* ! ACE_THR_PRI_OTHER_DEF */

// Recursive mutex support.
//
// There are two parts to this:
// 1. The mutex type itself. This is based on whether or not the
//    platform supports recursive mutexes natively or they're emulated.
// 2. Support for using the recursive mutex with a condition variable.
//    When a thread waits on a condition variable, it has to relinquish
//    the lock and wait atomically, then reacquire it after the condition
//    variable is signaled. In non-recursive mutexes, the platform
//    handles this automatically. But in recursive mutexes, especially
//    when emulated, the recursion count needs to be maintained across
//    the wait. Since another thread needs to be able to acquire the
//    lock, it needs to appear free, even if the waiting thread had done
//    multiple acquires. Thus, there's another structure to hold this
//    information, and is used with the recursive_mutex_cond_unlock()
//    and recursive_mutex_cond_relock() methods to maintain the expected
//    state when the wait finishes.
#   if defined (ACE_HAS_RECURSIVE_MUTEXES)
typedef ACE_thread_mutex_t ACE_recursive_thread_mutex_t;
#     if defined (ACE_WIN32)
// Windows has recursive mutexes, but doesn't have condition variables,
// so there's no built-in support for this. Thus, the condition-related
// unlock/relock is augmented in ACE.
struct ACE_recursive_mutex_state
{
  // On Windows the augmented processing is simply unlocking/relocking
  // the recursive locks - the condition handles a single lock ok.
  LONG relock_count_;
};
#     else
// No need for special handling; just need a type for method signatures.
typedef int ACE_recursive_mutex_state;
#     endif /* ACE_WIN32 */
#   else
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

// Since recursive mutex is emulated, the state saving needs to be handled
// in ACE as well. These members save those from ACE_recursive_thread_mutex_t.
struct ACE_recursive_mutex_state
{
  int nesting_level_;
  ACE_thread_t owner_id_;
};
#   endif /* ACE_HAS_RECURSIVE_MUTEXES */

# else /* !ACE_HAS_THREADS, i.e., the OS/platform doesn't support threading. */

// Give these things some reasonable value...
#   define ACE_SCOPE_PROCESS 0
#   define ACE_SCOPE_LWP 1
#   define ACE_SCOPE_THREAD 2
#   define ACE_SCHED_OTHER 0
#   define ACE_SCHED_FIFO 1
#   define ACE_SCHED_RR 2
#   if !defined (THR_CANCEL_DISABLE)
#     define THR_CANCEL_DISABLE      0
#   endif /* ! THR_CANCEL_DISABLE */
#   if !defined (THR_CANCEL_ENABLE)
#     define THR_CANCEL_ENABLE       0
#   endif /* ! THR_CANCEL_ENABLE */
#   if !defined (THR_CANCEL_DEFERRED)
#     define THR_CANCEL_DEFERRED     0
#   endif /* ! THR_CANCEL_DEFERRED */
#   if !defined (THR_CANCEL_ASYNCHRONOUS)
#     define THR_CANCEL_ASYNCHRONOUS 0
#   endif /* ! THR_CANCEL_ASYNCHRONOUS */
#   if !defined (THR_JOINABLE)
#     define THR_JOINABLE    0     /* ignore in most places */
#   endif /* ! THR_JOINABLE */
#   if !defined (THR_DETACHED)
#     define THR_DETACHED    0     /* ignore in most places */
#   endif /* ! THR_DETACHED */
#   if !defined (THR_DAEMON)
#     define THR_DAEMON      0     /* ignore in most places */
#   endif /* ! THR_DAEMON */
#   if !defined (THR_BOUND)
#     define THR_BOUND       0     /* ignore in most places */
#   endif /* ! THR_BOUND */
#   if !defined (THR_NEW_LWP)
#     define THR_NEW_LWP     0     /* ignore in most places */
#   endif /* ! THR_NEW_LWP */
#   if !defined (THR_SUSPENDED)
#     define THR_SUSPENDED   0     /* ignore in most places */
#   endif /* ! THR_SUSPENDED */
#   if !defined (THR_SCHED_FIFO)
#     define THR_SCHED_FIFO 0
#   endif /* ! THR_SCHED_FIFO */
#   if !defined (THR_SCHED_RR)
#     define THR_SCHED_RR 0
#   endif /* ! THR_SCHED_RR */
#   if !defined (THR_SCHED_DEFAULT)
#     define THR_SCHED_DEFAULT 0
#   endif /* ! THR_SCHED_DEFAULT */
#   if !defined (USYNC_THREAD)
#     define USYNC_THREAD 0
#   endif /* ! USYNC_THREAD */
#   if !defined (USYNC_PROCESS)
#     define USYNC_PROCESS 0
#   endif /* ! USYNC_PROCESS */
#   if !defined (THR_SCOPE_PROCESS)
#     define THR_SCOPE_PROCESS 0
#   endif /* ! THR_SCOPE_PROCESS */
#   if !defined (THR_SCOPE_SYSTEM)
#     define THR_SCOPE_SYSTEM 0
#   endif /* ! THR_SCOPE_SYSTEM */

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
typedef int ACE_recursive_mutex_state;
#   if !defined (ACE_HAS_POSIX_SEM) && !defined (ACE_PSOS)
typedef int ACE_sema_t;
#   endif /* !ACE_HAS_POSIX_SEM && !ACE_PSOS */
typedef int ACE_rwlock_t;
typedef int ACE_thread_t;
typedef int ACE_hthread_t;
typedef unsigned int ACE_thread_key_t;

// Ensure that ACE_THR_PRI_FIFO_DEF and ACE_THR_PRI_OTHER_DEF are
// defined on non-threaded platforms, to support application source
// code compatibility.  ACE_THR_PRI_FIFO_DEF should be used by
// applications for default real-time thread priority.
// ACE_THR_PRI_OTHER_DEF should be used for non-real-time priority.
#   if !defined(ACE_THR_PRI_FIFO_DEF)
#     define ACE_THR_PRI_FIFO_DEF 0
#   endif /* ! ACE_THR_PRI_FIFO_DEF */
#   if !defined(ACE_THR_PRI_OTHER_DEF)
#     define ACE_THR_PRI_OTHER_DEF 0
#   endif /* ! ACE_THR_PRI_OTHER_DEF */

# endif /* ACE_HAS_THREADS ***********************************************/

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

  void to_string (char*);


  // != Comparison operator.
  int operator== (const ACE_Thread_ID &) const;
  int operator!= (const ACE_Thread_ID &) const;

private:
  /// Identify the thread.
  ACE_thread_t thread_id_;

  /// Handle to the thread (typically used to "wait" on Win32).
  ACE_hthread_t thread_handle_;
};

// = The ACE_Sched_Priority type should be used for platform-
//   independent thread and process priorities, by convention.
//   int should be used for OS-specific priorities.
typedef int ACE_Sched_Priority;

# if !defined (ACE_DEFAULT_SYNCH_TYPE)
#   if defined (VXWORKS)
      // Types include these options: SEM_Q_PRIORITY, SEM_Q_FIFO,
      // SEM_DELETE_SAFE, and SEM_INVERSION_SAFE.  SEM_Q_FIFO is
      // used as the default because that is VxWorks' default.
#     define ACE_DEFAULT_SYNCH_TYPE SEM_Q_FIFO
#   else
#     define ACE_DEFAULT_SYNCH_TYPE USYNC_THREAD
#   endif /* VXWORKS */
#endif /* ! ACE_DEFAULT_SYNCH_TYPE */

// forward declaration
class ACE_Sched_Params;
class ACE_Time_Value;

# if defined (ACE_PSOS)
typedef u_long ACE_idtype_t;
typedef u_long ACE_id_t;
typedef u_long ACE_pri_t;
#   define ACE_SELF (0)
# elif defined (ACE_WIN32)
typedef int ACE_idtype_t;
typedef DWORD ACE_id_t;
typedef int ACE_pri_t;
#   define ACE_SELF (0)
# else /* !defined (ACE_WIN32) && !defined (ACE_PSOS) */
#   if defined (ACE_HAS_IDTYPE_T)
  typedef idtype_t ACE_idtype_t;
#   else
  typedef int ACE_idtype_t;
#   endif /* ACE_HAS_IDTYPE_T */
#   if defined (ACE_HAS_STHREADS) || defined (DIGITAL_UNIX)
#     if defined (ACE_LACKS_PRI_T)
    typedef int pri_t;
#     endif /* ACE_LACKS_PRI_T */
  typedef id_t ACE_id_t;
#     define ACE_SELF P_MYID
  typedef pri_t ACE_pri_t;
#   else  /* ! ACE_HAS_STHREADS && ! DIGITAL_UNIX */
  typedef long ACE_id_t;
#     define ACE_SELF (-1)
  typedef short ACE_pri_t;
#   endif /* ! ACE_HAS_STHREADS && ! DIGITAL_UNIX */
# endif /* !defined (ACE_WIN32) && !defined (ACE_PSOS) */

# if defined (ACE_HAS_TSS_EMULATION)
    // Allow config.h to set the default number of thread keys.
#   if !defined (ACE_DEFAULT_THREAD_KEYS)
#     define ACE_DEFAULT_THREAD_KEYS 64
#   endif /* ! ACE_DEFAULT_THREAD_KEYS */

// forward declaration
class ACE_TSS_Keys;

/**
 * @class ACE_TSS_Emulation
 *
 * @brief Thread-specific storage emulation.
 *
 * This provides a thread-specific storage implementation.
 * It is intended for use on platforms that don't have a
 * native TSS, or have a TSS with limitations such as the
 * number of keys or lack of support for removing keys.
 */
class ACE_OS_Export ACE_TSS_Emulation
{
public:
  typedef void (*ACE_TSS_DESTRUCTOR)(void *value) /* throw () */;

  /// Maximum number of TSS keys allowed over the life of the program.
  enum { ACE_TSS_THREAD_KEYS_MAX = ACE_DEFAULT_THREAD_KEYS };

  /// Returns the total number of keys allocated so far.
  static u_int total_keys ();

  /// Sets the argument to the next available key.  Returns 0 on success,
  /// -1 if no keys are available.
  static int next_key (ACE_thread_key_t &key);

  /// Release a key that was used. This way the key can be given out in a
  /// new request. Returns 0 on success, 1 if the key was not reserved.
  static int release_key (ACE_thread_key_t key);

  /// Returns the exit hook associated with the key.  Does _not_ check
  /// for a valid key.
  static ACE_TSS_DESTRUCTOR tss_destructor (const ACE_thread_key_t key);

  /// Associates the TSS destructor with the key.  Does _not_ check
  /// for a valid key.
  static void tss_destructor (const ACE_thread_key_t key,
                              ACE_TSS_DESTRUCTOR destructor);

  /// Accesses the object referenced by key in the current thread's TSS array.
  /// Does _not_ check for a valid key.
  static void *&ts_object (const ACE_thread_key_t key);

  /**
   * Setup an array to be used for local TSS.  Returns the array
   * address on success.  Returns 0 if local TSS had already been
   * setup for this thread.  There is no corresponding tss_close ()
   * because it is not needed.
   * NOTE: tss_open () is called by ACE for threads that it spawns.
   * If your application spawns threads without using ACE, and it uses
   * ACE's TSS emulation, each of those threads should call tss_open
   * ().  See the ace_thread_adapter () implementation for an example.
   */
  static void *tss_open (void *ts_storage[ACE_TSS_THREAD_KEYS_MAX]);

  /// Shutdown TSS emulation.  For use only by ACE_OS::cleanup_tss ().
  static void tss_close ();

private:
  // Global TSS structures.
  /// Contains the possible value of the next key to be allocated. Which key
  /// is actually allocated is based on the tss_keys_used
  static u_int total_keys_;

  /// Array of thread exit hooks (TSS destructors) that are called for each
  /// key (that has one) when the thread exits.
  static ACE_TSS_DESTRUCTOR tss_destructor_ [ACE_TSS_THREAD_KEYS_MAX];

  /// TSS_Keys instance to administrate whether a specific key is in used
  /// or not
  static ACE_TSS_Keys tss_keys_used_;

#   if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  /// Location of current thread's TSS array.
  static void **tss_base (void* ts_storage[] = 0, u_int *ts_created = 0);
#   else  /* ! ACE_HAS_THREAD_SPECIFIC_STORAGE */
  /// Location of current thread's TSS array.
  static void **&tss_base ();
#   endif /* ! ACE_HAS_THREAD_SPECIFIC_STORAGE */

#   if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  // Rely on native thread specific storage for the implementation,
  // but just use one key.
  static ACE_OS_thread_key_t native_tss_key_;

  // Used to indicate if native tss key has been allocated
  static int key_created_;
#   endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE */
};

# else   /* ! ACE_HAS_TSS_EMULATION */
#   if defined (TLS_MINIMUM_AVAILABLE)
    // WIN32 platforms define TLS_MINIMUM_AVAILABLE natively.
#     define ACE_DEFAULT_THREAD_KEYS TLS_MINIMUM_AVAILABLE
#   endif /* TSL_MINIMUM_AVAILABLE */

# endif /* ACE_HAS_TSS_EMULATION */

// moved ACE_TSS_Ref, ACE_TSS_Info, and ACE_TSS_Keys class
// declarations from OS.cpp so they are visible to the single
// file of template instantiations.
# if defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) || (defined (ACE_PSOS) && defined (ACE_PSOS_HAS_TSS))
/**
 * @class ACE_TSS_Ref
 *
 * @brief "Reference count" for thread-specific storage keys.
 *
 * Since the <ACE_Unbounded_Stack> doesn't allow duplicates, the
 * "reference count" is the identify of the thread_id.
 */
class ACE_TSS_Ref
{
public:
  /// Constructor
  ACE_TSS_Ref (ACE_thread_t id);

  /// Default constructor
  ACE_TSS_Ref (void);

  /// Check for equality.
  int operator== (const ACE_TSS_Ref &) const;

  /// Check for inequality.
  int operator!= (const ACE_TSS_Ref &) const;

// private:

  /// ID of thread using a specific key.
  ACE_thread_t tid_;
};

/**
 * @class ACE_TSS_Info
 *
 * @brief Thread Specific Key management.
 *
 * This class maps a key to a "destructor."
 */
class ACE_TSS_Info
{
public:
  /// Constructor
  ACE_TSS_Info (ACE_thread_key_t key,
                void (*dest)(void *) = 0,
                void *tss_inst = 0);

  /// Default constructor
  ACE_TSS_Info (void);

  /// Returns 1 if the key is in use, 0 if not.
  int key_in_use (void) const { return thread_count_ != -1; }

  /// Mark the key as being in use if the flag is non-zero, or
  /// not in use if the flag is 0.
  void key_in_use (int flag) { thread_count_ = flag == 0  ?  -1  :  1; }

  /// Check for equality.
  int operator== (const ACE_TSS_Info &) const;

  /// Check for inequality.
  int operator!= (const ACE_TSS_Info &) const;

  /// Dump the state.
  void dump (void);

private:
  /// Key to the thread-specific storage item.
  ACE_thread_key_t key_;

  /// "Destructor" that gets called when the item is finally released.
  void (*destructor_)(void *);

  /// Pointer to ACE_TSS<xxx> instance that has/will allocate the key.
  void *tss_obj_;

  /// Count of threads that are using this key.  Contains -1 when the
  /// key is not in use.
  int thread_count_;

  friend class ACE_TSS_Cleanup;
};

/**
 * @class ACE_TSS_Keys
 *
 * @brief Collection of in-use flags for a thread's TSS keys.
 * For internal use only by ACE_TSS_Cleanup; it is public because
 * some compilers can't use nested classes for template instantiation
 * parameters.
 *
 * Wrapper around array of whether each key is in use.  A simple
 * typedef doesn't work with Sun C++ 4.2.
 */
class ACE_TSS_Keys
{
public:
  /// Default constructor, to initialize all bits to zero (unused).
  ACE_TSS_Keys (void);

  /// Mark the specified key as being in use, if it was not already so marked.
  /// Returns 1 if the had already been marked, 0 if not.
  int test_and_set (const ACE_thread_key_t key);

  /// Mark the specified key as not being in use, if it was not already so
  /// cleared.  Returns 1 if the key had already been cleared, 0 if not.
  int test_and_clear (const ACE_thread_key_t key);

  /// Return whether the specific key is marked as in use.
  /// Returns 1 if the key is been marked, 0 if not.
  int is_set (const ACE_thread_key_t key) const;

private:
  /// For a given key, find the word and bit number that represent it.
  static void find (const u_int key, u_int &word, u_int &bit);

  enum
    {
#   if ACE_SIZEOF_LONG == 8
      ACE_BITS_PER_WORD = 64,
#   elif ACE_SIZEOF_LONG == 4
      ACE_BITS_PER_WORD = 32,
#   else
#     error ACE_TSS_Keys only supports 32 or 64 bit longs.
#   endif /* ACE_SIZEOF_LONG == 8 */
      ACE_WORDS = (ACE_DEFAULT_THREAD_KEYS - 1) / ACE_BITS_PER_WORD + 1
    };

  /// Bit flag collection.  A bit value of 1 indicates that the key is in
  /// use by this thread.
  u_long key_bit_words_[ACE_WORDS];
};

# endif /* defined (ACE_WIN32) || defined (ACE_HAS_TSS_EMULATION) */

// Support non-scalar thread keys, such as with some POSIX
// implementations, e.g., MVS.
# if defined (ACE_HAS_NONSCALAR_THREAD_KEY_T)
#   define ACE_KEY_INDEX(OBJ,KEY) \
  u_int OBJ; \
  ACE_OS::memcpy (&OBJ, &KEY, sizeof (u_int))
# else
#   define ACE_KEY_INDEX(OBJ,KEY) u_int OBJ = KEY
# endif /* ACE_HAS_NONSCALAR_THREAD_KEY_T */

# if defined (ACE_HAS_THR_C_FUNC)
// This is necessary to work around nasty problems with MVS C++.
extern "C" ACE_OS_Export void ace_mutex_lock_cleanup_adapter (void *args);
#   define ACE_PTHREAD_CLEANUP_PUSH(A) pthread_cleanup_push (ace_mutex_lock_cleanup_adapter, (void *) A);
#   define ACE_PTHREAD_CLEANUP_POP(A) pthread_cleanup_pop(A)
# elif defined (ACE_HAS_PTHREADS) && !defined (ACE_LACKS_PTHREAD_CLEANUP)
// Though we are defining a extern "C" function to match the prototype of
// pthread_cleanup_push, it is undone by the Solaris header file
// /usr/include/pthread.h. So this macro generates a warning under Solaris
// with SunCC. This is a bug in the Solaris header file.
extern "C" ACE_OS_Export void ace_mutex_lock_cleanup_adapter (void *args);
#   define ACE_PTHREAD_CLEANUP_PUSH(A) pthread_cleanup_push (ace_mutex_lock_cleanup_adapter, (void *) A);
#   define ACE_PTHREAD_CLEANUP_POP(A) pthread_cleanup_pop(A)
# else
#   define ACE_PTHREAD_CLEANUP_PUSH(A)
#   define ACE_PTHREAD_CLEANUP_POP(A)
# endif /* ACE_HAS_THR_C_FUNC */

// forward decl's
class ACE_event_t;

namespace ACE_OS {

# if 0
  //@{ @name A set of wrappers for threads (these are portable since they use the ACE_Thread_ID).
  int thr_continue (const ACE_Thread_ID &thread);
  int thr_create (ACE_THR_FUNC,
                  void *args,
                  long flags,
                  ACE_Thread_ID *,
                  long priority = ACE_DEFAULT_THREAD_PRIORITY,
                  void *stack = 0,
                  size_t stacksize = 0);
  int thr_getprio (ACE_Thread_ID thr_id,
                   int &prio,
                   int *policy = 0);
  int thr_join (ACE_Thread_ID waiter_id,
                ACE_THR_FUNC_RETURN *status);
  int thr_kill (ACE_Thread_ID thr_id,
                int signum);
  ACE_Thread_ID thr_self (void);
  int thr_setprio (ACE_Thread_ID thr_id,
                   int prio);
  int thr_setprio (const ACE_Sched_Priority prio);
  int thr_suspend (ACE_Thread_ID target_thread);
  int thr_cancel (ACE_Thread_ID t_id);
  //@}
# endif /* 0 */


  //@{ @name A set of wrappers for threads

  /// This is necessary to deal with POSIX pthreads and their use of
  /// structures for thread ids.
  extern ACE_thread_t NULL_thread;

  /// This is necessary to deal with POSIX pthreads and their use of
  /// structures for thread handles.
  extern ACE_hthread_t NULL_hthread;

  /// This is necessary to deal with POSIX pthreads and their use of
  /// structures for TSS keys.
  extern ACE_thread_key_t NULL_key;

# if defined (CHORUS)
  /// This is used to map an actor's id into a KnCap for killing and
  /// waiting actors.
  KnCap actorcaps_[ACE_CHORUS_MAX_ACTORS];
# endif /* CHORUS */
  //@}

  /**
   * Call TSS destructors for the current thread.  If the current
   * thread is the main thread, then the argument must be 1.
   * For private use of ACE_Object_Manager and ACE_Thread_Adapter only.
   */
  void cleanup_tss (const u_int main_thread);

  //@{ @name A set of wrappers for condition variables.
  int condattr_init (ACE_condattr_t &attributes,
                     int type = ACE_DEFAULT_SYNCH_TYPE);
  int condattr_destroy (ACE_condattr_t &attributes);
  int cond_broadcast (ACE_cond_t *cv);
  int cond_destroy (ACE_cond_t *cv);
  int cond_init (ACE_cond_t *cv,
                 short type = ACE_DEFAULT_SYNCH_TYPE,
                 const char *name = 0,
                 void *arg = 0);
  int cond_init (ACE_cond_t *cv,
                 ACE_condattr_t &attributes,
                 const char *name = 0,
                 void *arg = 0);
# if defined (ACE_HAS_WCHAR)
  int cond_init (ACE_cond_t *cv,
                 short type,
                 const wchar_t *name,
                 void *arg = 0);
  int cond_init (ACE_cond_t *cv,
                 ACE_condattr_t &attributes,
                 const wchar_t *name,
                 void *arg = 0);
# endif /* ACE_HAS_WCHAR */
  int cond_signal (ACE_cond_t *cv);
  int cond_timedwait (ACE_cond_t *cv,
                      ACE_mutex_t *m,
                      ACE_Time_Value *);
  int cond_wait (ACE_cond_t *cv,
                 ACE_mutex_t *m);
# if defined (ACE_WIN32) && defined (ACE_HAS_WTHREADS)
  int cond_timedwait (ACE_cond_t *cv,
                      ACE_thread_mutex_t *m,
                      ACE_Time_Value *);
  int cond_wait (ACE_cond_t *cv,
                 ACE_thread_mutex_t *m);
# endif /* ACE_WIN32 && ACE_HAS_WTHREADS */

  //@{ @name A set of wrappers for auto-reset and manual events.

  int event_destroy (ACE_event_t *event);

  int event_init (ACE_event_t *event,
                  int manual_reset = 0,
                  int initial_state = 0,
                  int type = ACE_DEFAULT_SYNCH_TYPE,
                  const char *name = 0,
                  void *arg = 0,
                  LPSECURITY_ATTRIBUTES sa = 0);

# if defined (ACE_HAS_WCHAR)
  int event_init (ACE_event_t *event,
                  int manual_reset,
                  int initial_state,
                  int type,
                  const wchar_t *name,
                  void *arg = 0,
                  LPSECURITY_ATTRIBUTES sa = 0);
# endif /* ACE_HAS_WCHAR */

  int event_pulse (ACE_event_t *event);

  int event_reset (ACE_event_t *event);

  int event_signal (ACE_event_t *event);

  int event_timedwait (ACE_event_t *event,
                       ACE_Time_Value *timeout,
                       int use_absolute_time = 1);

  int event_wait (ACE_event_t *event);

  //@}

  int lwp_getparams (ACE_Sched_Params &);

  int lwp_setparams (const ACE_Sched_Params &);


  //@{ @name A set of wrappers for mutex locks.

  int mutex_destroy (ACE_mutex_t *m);

  int mutex_init (ACE_mutex_t *m,
                  int type = ACE_DEFAULT_SYNCH_TYPE,
                  const char *name = 0,
                  ACE_mutexattr_t *arg = 0,
                  LPSECURITY_ATTRIBUTES sa = 0);

#if defined (ACE_HAS_WCHAR)
  int mutex_init (ACE_mutex_t *m,
                  int type,
                  const wchar_t *name,
                  ACE_mutexattr_t *arg = 0,
                  LPSECURITY_ATTRIBUTES sa = 0);
#endif /* ACE_HAS_WCHAR */

  /// Win32 note: Abandoned mutexes are not treated differently. 0 is
  /// returned since the calling thread does get the ownership.
  int mutex_lock (ACE_mutex_t *m);

  /// This method is only implemented for Win32.  For abandoned
  /// mutexes, <abandoned> is set to 1 and 0 is returned.
  int mutex_lock (ACE_mutex_t *m,
                  int &abandoned);

  /**
   * This method attempts to acquire a lock, but gives up if the lock
   * has not been acquired by the given time.  If the lock is not
   * acquired within the given amount of time, then this method
   * returns -1 with an <ETIME> errno on platforms that actually
   * support timed mutexes.  The timeout should be an absolute time.
   * Note that the mutex should not be a recursive one, i.e., it
   * should only be a standard mutex or an error checking mutex.
   */
  int mutex_lock (ACE_mutex_t *m,
                  const ACE_Time_Value &timeout);

  /**
   * If <timeout> == 0, calls <ACE_OS::mutex_lock(m)>.  Otherwise,
   * this method attempts to acquire a lock, but gives up if the lock
   * has not been acquired by the given time, in which case it returns
   * -1 with an <ETIME> errno on platforms that actually support timed
   * mutexes.  The timeout should be an absolute time.  Note that the
   * mutex should not be a recursive one, i.e., it should only be a
   * standard mutex or an error checking mutex.
   */
  int mutex_lock (ACE_mutex_t *m,
                  const ACE_Time_Value *timeout);

  /// Handle asynchronous thread cancellation cleanup.
  void mutex_lock_cleanup (void *mutex);

  /// Win32 note: Abandoned mutexes are not treated differently. 0 is
  /// returned since the calling thread does get the ownership.
  int mutex_trylock (ACE_mutex_t *m);

  /// This method is only implemented for Win32.  For abandoned
  /// mutexes, <abandoned> is set to 1 and 0 is returned.
  int mutex_trylock (ACE_mutex_t *m,
                     int &abandoned);

  int mutex_unlock (ACE_mutex_t *m);

  //@}

  /// Low-level interface to <priocntl>(2).
  /**
   * Can't call the following priocntl, because that's a macro on
   * Solaris.
   */
  int priority_control (ACE_idtype_t, ACE_id_t, int, void *);

  //@{ @name A set of wrappers for recursive mutex locks.

  // These two methods are primarily in support of
  // ACE_Condition<ACE_Recursive_Thread_Mutex> and should probably not
  // be called outside that context.
  int recursive_mutex_cond_unlock (ACE_recursive_thread_mutex_t *m,
                                   ACE_recursive_mutex_state &state);

  void recursive_mutex_cond_relock (ACE_recursive_thread_mutex_t *m,
                                    ACE_recursive_mutex_state &state);

  int recursive_mutex_destroy (ACE_recursive_thread_mutex_t *m);

  int recursive_mutex_init (ACE_recursive_thread_mutex_t *m,
                            const ACE_TCHAR *name = 0,
                            ACE_mutexattr_t *arg = 0,
                            LPSECURITY_ATTRIBUTES sa = 0);

  int recursive_mutex_lock (ACE_recursive_thread_mutex_t *m);

  int recursive_mutex_trylock (ACE_recursive_thread_mutex_t *m);

  int recursive_mutex_unlock (ACE_recursive_thread_mutex_t *m);

  //@}


  //@{ @name A set of wrappers for readers/writer locks.

  int rw_rdlock (ACE_rwlock_t *rw);

  int rw_tryrdlock (ACE_rwlock_t *rw);

  int rw_trywrlock (ACE_rwlock_t *rw);

  int rw_trywrlock_upgrade (ACE_rwlock_t *rw);

  int rw_unlock (ACE_rwlock_t *rw);

  int rw_wrlock (ACE_rwlock_t *rw);

  int rwlock_destroy (ACE_rwlock_t *rw);

  int rwlock_init (ACE_rwlock_t *rw,
                   int type = ACE_DEFAULT_SYNCH_TYPE,
                   const ACE_TCHAR *name = 0,
                   void *arg = 0);

  //@}

  //@{ @name Thread scheduler interface.
  /// Set scheduling parameters.  An id of ACE_SELF indicates, e.g.,
  /// set the parameters on the calling thread.
  int sched_params (const ACE_Sched_Params &, ACE_id_t id = ACE_SELF);
  //@}

  /// Find the schedling class ID that corresponds to the class name.
  int scheduling_class (const char *class_name, ACE_id_t &);

  //@{ @name A set of wrappers for semaphores.

  int sema_destroy (ACE_sema_t *s);

  int sema_init (ACE_sema_t *s,
                 u_int count,
                 int type = ACE_DEFAULT_SYNCH_TYPE,
                 const char *name = 0,
                 void *arg = 0,
                 int max = 0x7fffffff,
                 LPSECURITY_ATTRIBUTES sa = 0);

# if defined (ACE_HAS_WCHAR)
  int sema_init (ACE_sema_t *s,
                 u_int count,
                 int type,
                 const wchar_t *name,
                 void *arg = 0,
                 int max = 0x7fffffff,
                 LPSECURITY_ATTRIBUTES sa = 0);
# endif /* ACE_HAS_WCHAR */

  int sema_post (ACE_sema_t *s);

  int sema_post (ACE_sema_t *s,
                 u_int release_count);

  int sema_trywait (ACE_sema_t *s);

  int sema_wait (ACE_sema_t *s);

  int sema_wait (ACE_sema_t *s,
                 ACE_Time_Value &tv);

  int sema_wait (ACE_sema_t *s,
                 ACE_Time_Value *tv);

  //@}

  //@{ @name A set of wrappers for System V semaphores.
  int semctl (int int_id,
              int semnum,
              int cmd,
              semun);
  int semget (key_t key,
              int nsems,
              int flags);
  int semop (int int_id,
             struct sembuf *sops,
             size_t nsops);
  //@}

  /// Friendly interface to <priocntl>(2).
  int set_scheduling_params (const ACE_Sched_Params &,
                             ACE_id_t id = ACE_SELF);

  int sigtimedwait (const sigset_t *set,
                    siginfo_t *info,
                    const ACE_Time_Value *timeout);

  int sigwait (sigset_t *set,
               int *sig = 0);

  int sigwaitinfo (const sigset_t *set,
                   siginfo_t *info);

  int thr_cancel (ACE_thread_t t_id);

  int thr_cmp (ACE_hthread_t t1,
               ACE_hthread_t t2);
  // These are non-portable since they use ACE_thread_t and
  // ACE_hthread_t and will go away in a future release.
  int thr_continue (ACE_hthread_t target_thread);

  /*
   * Creates a new thread having <flags> attributes and running <func>
   * with <args> (if <thread_adapter> is non-0 then <func> and <args>
   * are ignored and are obtained from <thread_adapter>).  <thr_id>
   * and <t_handle> are set to the thread's ID and handle (?),
   * respectively.  The thread runs at <priority> priority (see
   * below).
   *
   * The <flags> are a bitwise-OR of the following:
   * = BEGIN<INDENT>
   * THR_CANCEL_DISABLE, THR_CANCEL_ENABLE, THR_CANCEL_DEFERRED,
   * THR_CANCEL_ASYNCHRONOUS, THR_BOUND, THR_NEW_LWP, THR_DETACHED,
   * THR_SUSPENDED, THR_DAEMON, THR_JOINABLE, THR_SCHED_FIFO,
   * THR_SCHED_RR, THR_SCHED_DEFAULT, THR_EXPLICIT_SCHED,
   * THR_SCOPE_SYSTEM, THR_SCOPE_PROCESS
   * = END<INDENT>
   *
   * By default, or if <priority> is set to
   * ACE_DEFAULT_THREAD_PRIORITY, an "appropriate" priority value for
   * the given scheduling policy (specified in <flags}>, e.g.,
   * <THR_SCHED_DEFAULT>) is used.  This value is calculated
   * dynamically, and is the median value between the minimum and
   * maximum priority values for the given policy.  If an explicit
   * value is given, it is used.  Note that actual priority values are
   * EXTREMEMLY implementation-dependent, and are probably best
   * avoided.
   *
   * Note that <thread_adapter> is always deleted by <thr_create>,
   * therefore it must be allocated with global operator new.
   */
  int thr_create (ACE_THR_FUNC func,
                  void *args,
                  long flags,
                  ACE_thread_t *thr_id,
                  ACE_hthread_t *t_handle = 0,
                  long priority = ACE_DEFAULT_THREAD_PRIORITY,
                  void *stack = 0,
                  size_t stacksize = 0,
                  ACE_Base_Thread_Adapter *thread_adapter = 0);

  int thr_equal (ACE_thread_t t1,
                 ACE_thread_t t2);

  void thr_exit (ACE_THR_FUNC_RETURN status = 0);

  int thr_getconcurrency (void);

  int thr_getprio (ACE_hthread_t id,
                   int &priority);

  int thr_getprio (ACE_hthread_t id,
                   int &priority,
                   int &policy);

# if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  int thr_getspecific (ACE_OS_thread_key_t key,
                       void **data);
# endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */

  int thr_getspecific (ACE_thread_key_t key,
                       void **data);

  int thr_join (ACE_hthread_t waiter_id,
                ACE_THR_FUNC_RETURN *status);

  int thr_join (ACE_thread_t waiter_id,
                ACE_thread_t *thr_id,
                ACE_THR_FUNC_RETURN *status);

  int thr_key_detach (void *inst);

  int thr_key_used (ACE_thread_key_t key);

# if defined (ACE_HAS_THR_C_DEST)
#   if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  int thr_keycreate (ACE_OS_thread_key_t *key,
                     ACE_THR_C_DEST,
                     void *inst = 0);
#   endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
  int thr_keycreate (ACE_thread_key_t *key,
                     ACE_THR_C_DEST,
                     void *inst = 0);
# else
#   if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  int thr_keycreate (ACE_OS_thread_key_t *key,
                     ACE_THR_DEST,
                     void *inst = 0);
#   endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */
  int thr_keycreate (ACE_thread_key_t *key,
                     ACE_THR_DEST,
                     void *inst = 0);
# endif /* ACE_HAS_THR_C_DEST */

  int thr_keyfree (ACE_thread_key_t key);

  int thr_kill (ACE_thread_t thr_id,
                int signum);

  size_t thr_min_stack (void);

  ACE_thread_t thr_self (void);

  void thr_self (ACE_hthread_t &);

  int thr_setcancelstate (int new_state,
                          int *old_state);

  int thr_setcanceltype (int new_type,
                         int *old_type);

  int thr_setconcurrency (int hint);

  int thr_setprio (ACE_hthread_t id,
                   int priority,
                   int policy = -1);

  int thr_setprio (const ACE_Sched_Priority prio);

# if defined (ACE_HAS_TSS_EMULATION) && defined (ACE_HAS_THREAD_SPECIFIC_STORAGE)
  int thr_setspecific (ACE_OS_thread_key_t key,
                       void *data);
# endif /* ACE_HAS_TSS_EMULATION && ACE_HAS_THREAD_SPECIFIC_STORAGE */

  int thr_setspecific (ACE_thread_key_t key,
                       void *data);

  int thr_sigsetmask (int how,
                      const sigset_t *nsm,
                      sigset_t *osm);

  int thr_suspend (ACE_hthread_t target_thread);

  void thr_testcancel (void);

  void thr_yield (void);

  //@{ @name A set of wrappers for mutex locks that only work within a single process.

  int thread_mutex_destroy (ACE_thread_mutex_t *m);

  int thread_mutex_init (ACE_thread_mutex_t *m,
                         int type = ACE_DEFAULT_SYNCH_TYPE,
                         const char *name = 0,
                         ACE_mutexattr_t *arg = 0);

#if defined (ACE_HAS_WCHAR)
  int thread_mutex_init (ACE_thread_mutex_t *m,
                         int type,
                         const wchar_t *name,
                         ACE_mutexattr_t *arg = 0);
#endif /* ACE_HAS_WCHAR */

  int thread_mutex_lock (ACE_thread_mutex_t *m);

  int thread_mutex_lock (ACE_thread_mutex_t *m,
                         const ACE_Time_Value &timeout);

  int thread_mutex_lock (ACE_thread_mutex_t *m,
                         const ACE_Time_Value *timeout);

  int thread_mutex_trylock (ACE_thread_mutex_t *m);

  int thread_mutex_unlock (ACE_thread_mutex_t *m);

  //@}

  /**
   * This method uses process id and object pointer to come up with a
   * machine wide unique name.  The process ID will provide uniqueness
   * between processes on the same machine. The "this" pointer of the
   * <object> will provide uniqueness between other "live" objects in
   * the same process. The uniqueness of this name is therefore only
   * valid for the life of <object>.
   */
  void unique_name (const void *object,
                    ACE_TCHAR *name,
                    size_t length);

  //@}

} /* namespace ACE_OS */

/**
 * @class ACE_event_t
 *
 * @brief Wrapper for NT events on UNIX.
 */
class ACE_OS_Export ACE_event_t
{
  friend int ACE_OS::event_init(ACE_event_t*, int, int, int, const char*, void*,int);
  friend int ACE_OS::event_destroy(ACE_event_t*);
  friend int ACE_OS::event_wait(ACE_event_t*);
  friend int ACE_OS::event_timedwait(ACE_event_t*, ACE_Time_Value*, int);
  friend int ACE_OS::event_signal(ACE_event_t*);
  friend int ACE_OS::event_pulse(ACE_event_t*);
  friend int ACE_OS::event_reset(ACE_event_t*);
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
  unsigned long waiting_threads_;
};

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
# if defined (ACE_WIN32)
OSVERSIONINFO ACE_OS::win32_versioninfo_;
// Cached win32 version information.

HINSTANCE ACE_OS::win32_resource_module_;

#   if defined (ACE_OS_HAS_DLL) && (ACE_OS_HAS_DLL == 1) && !defined (ACE_HAS_WINCE)
// This function is called by the OS when the ACE DLL is loaded. We
// use it to determine the default module containing ACE's resources.
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID)
{
  if (reason == DLL_PROCESS_ATTACH)
    {
#     if defined (ACE_DISABLES_THREAD_LIBRARY_CALLS) && (ACE_DISABLES_THREAD_LIBRARY_CALLS == 1)
      ::DisableThreadLibraryCalls (instance);
#     endif /* ACE_DISABLES_THREAD_LIBRARY_CALLS */
      ACE_OS::set_win32_resource_module(instance);
    }
  return TRUE;
}
#   endif /* ACE_OS_HAS_DLL && ACE_OS_HAS_DLL == 1 */
# endif /* ACE_WIN32 */

/**
 * @class ACE_OS_Thread_Mutex_Guard
 *
 * This data structure is meant to be used within an ACE_OS
 * function.  It performs automatic aquisition and release of
 * an ACE_thread_mutex_t.
 *
 * For internal use only by ACE_OS.
 */
class ACE_OS_Thread_Mutex_Guard
{
public:
  /// Implicitly and automatically acquire the lock.
  ACE_OS_Thread_Mutex_Guard (ACE_thread_mutex_t &m);

  /// Implicitly release the lock.
  ~ACE_OS_Thread_Mutex_Guard (void);

  /// Explicitly acquire the lock.
  int acquire (void);

  /// Explicitly release the lock.
  int release (void);

protected:
  /// Reference to the mutex.
  ACE_thread_mutex_t &lock_;

  /// Keeps track of whether we acquired the lock or failed.
  int owner_;

  // = Prevent assignment and initialization.
  ACE_OS_Thread_Mutex_Guard &operator= (const ACE_OS_Thread_Mutex_Guard &);
  ACE_OS_Thread_Mutex_Guard (const ACE_OS_Thread_Mutex_Guard &);
};

/**
 * @class ACE_OS_Recursive_Thread_Mutex_Guard
 *
 * @brief For internal use only by ACE_OS.
 *
 * This data structure is meant to be used within an ACE_OS
 * function.  It performs automatic aquisition and release of
 * an ACE_recursive_thread_mutex_t.
 */
class ACE_OS_Recursive_Thread_Mutex_Guard
{
public:
  /// Implicitly and automatically acquire the lock.
  ACE_OS_Recursive_Thread_Mutex_Guard (ACE_recursive_thread_mutex_t &m);

  /// Implicitly release the lock.
  ~ACE_OS_Recursive_Thread_Mutex_Guard (void);

  /// Explicitly acquire the lock.
  int acquire (void);

  /// Explicitly release the lock.
  int release (void);

protected:
  /// Reference to the mutex.
  ACE_recursive_thread_mutex_t &lock_;

  /// Keeps track of whether we acquired the lock or failed.
  int owner_;

  // = Prevent assignment and initialization.
  ACE_OS_Recursive_Thread_Mutex_Guard &operator= (
    const ACE_OS_Recursive_Thread_Mutex_Guard &);
  ACE_OS_Recursive_Thread_Mutex_Guard (
    const ACE_OS_Recursive_Thread_Mutex_Guard &);
};

// used in time and unistd
# define ACE_OS_GUARD \
  ACE_OS_Thread_Mutex_Guard ace_os_guard__ (*(ACE_thread_mutex_t *) \
    ACE_OS_Object_Manager::preallocated_object[ \
      ACE_OS_Object_Manager::ACE_OS_MONITOR_LOCK]);

// used in Thread
# define ACE_TSS_CLEANUP_GUARD \
  ACE_OS_Recursive_Thread_Mutex_Guard ace_tss_cleanup_guard__ (*(ACE_recursive_thread_mutex_t *) \
    ACE_OS_Object_Manager::preallocated_object[ \
      ACE_OS_Object_Manager::ACE_TSS_CLEANUP_LOCK]);

// used in Thread
# define ACE_TSS_BASE_GUARD \
  ACE_OS_Recursive_Thread_Mutex_Guard ace_tss_base_guard__ (*(ACE_recursive_thread_mutex_t *) \
    ACE_OS_Object_Manager::preallocated_object[ \
      ACE_OS_Object_Manager::ACE_TSS_BASE_LOCK]);

#else  /* ! ACE_MT_SAFE */
# define ACE_OS_GUARD
# define ACE_TSS_CLEANUP_GUARD
# define ACE_TSS_BASE_GUARD
#endif /* ! ACE_MT_SAFE */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_Thread.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_THREAD_H */
