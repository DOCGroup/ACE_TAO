/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Process_Mutex.h
 *
 *  $Id$
 *
 *   A wrapper for mutexes that can be used across processes on
 *   the same host machine, as well as within a process, of
 *   course.
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_PROCESS_MUTEX_H
#define ACE_PROCESS_MUTEX_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// The set of platforms encompassed by the set of 'defined' directives
// below have cross-process mutex capability. Those that don't need to
// use System V Semaphores. The following private macro sets this
// capability difference and is used in Process_Mutex.* only.
#if defined (_ACE_HAS_INTER_PROC_MUTEX)
#  undef _ACE_HAS_INTER_PROC_MUTEX
#endif /* _ACE_HAS_INTER_PROC_MUTEX */

// This may need more qualification on pthreads version.
#if (defined (ACE_WIN32) || defined (ACE_PSOS) || \
     defined (ACE_HAS_PTHREADS) || defined (ACE_HAS_STHREADS))
#  define _ACE_HAS_INTER_PROC_MUTEX
#endif

#if defined (_ACE_HAS_INTER_PROC_MUTEX)
#include "ace/Synch.h"
#else
#include "ace/SV_Semaphore_Complex.h"
#endif /* _ACE_HAS_INTER_PROC_MUTEX */

/**
 * @class ACE_Process_Mutex
 *
 * @brief A wrapper for mutexes that can be used across processes on
 * the same host machine, as well as within a process, of
 * course.
 */
class ACE_Export ACE_Process_Mutex
{
public:
  /// Create a Process_Mutex, passing in the optional <name>.
  ACE_Process_Mutex (const char *name = 0,
                     void *arg = 0);

#if defined (ACE_HAS_WCHAR)
  /// Create a Process_Mutex, passing in the optional <name>. (wchar_t version)
  ACE_Process_Mutex (const wchar_t *name,
                     void *arg = 0);
#endif /* ACE_HAS_WCHAR */

  ~ACE_Process_Mutex (void);

  /**
   * Explicitly destroy the mutex.  Note that only one thread should
   * call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /// Acquire lock ownership (wait on queue if necessary).
  int acquire (void);

  /// Acquire lock ownership, but timeout if lock if hasn't been
  /// acquired by given time.
  int acquire (ACE_Time_Value &tv);

  /**
   * Conditionally acquire lock (i.e., don't wait on queue).  Returns
   * -1 on failure.  If we "failed" because someone else already had
   * the lock, <errno> is set to <EBUSY>.
   */
  int tryacquire (void);

  /// Release lock and unblock a thread at head of queue.
  int release (void);

  /// Acquire lock ownership (wait on queue if necessary).
  int acquire_read (void);

  /// Acquire lock ownership (wait on queue if necessary).
  int acquire_write (void);

  /**
   * Conditionally acquire a lock (i.e., won't block).  Returns -1 on
   * failure.  If we "failed" because someone else already had the
   * lock, <errno> is set to <EBUSY>.
   */
  int tryacquire_read (void);

  /**
   * Conditionally acquire a lock (i.e., won't block).  Returns -1 on
   * failure.  If we "failed" because someone else already had the
   * lock, <errno> is set to <EBUSY>.
   */
  int tryacquire_write (void);

  /**
   * This is only here for consistency with the other synchronization
   * APIs and usability with Lock adapters. Assumes the caller already has
   * acquired the mutex and returns 0 in all cases.
   */
  int tryacquire_write_upgrade (void);

#if defined (_ACE_HAS_INTER_PROC_MUTEX)
  /// Return the underlying mutex.
  const ACE_mutex_t &lock (void) const;
#endif /* _ACE_HAS_INTER_PROC_MUTEX */

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
#if defined (_ACE_HAS_INTER_PROC_MUTEX)
  ACE_Mutex lock_;
#else
  /// If the user does not provide a name we generate a unique name in
  /// this buffer.
  ACE_TCHAR name_[ACE_UNIQUE_NAME_LEN];

  /// Create and return the unique name.
  const ACE_TCHAR *unique_name (void);

  /// We need this to get the right semantics...
  ACE_SV_Semaphore_Complex lock_;
#endif /* _ACE_HAS_INTER_PROC_MUTEX */
};

#if defined (__ACE_INLINE__)
#include "ace/Process_Mutex.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_PROCESS_MUTEX_H */
