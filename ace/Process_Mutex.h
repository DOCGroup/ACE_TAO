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

#if !(defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS))
#include "ace/SV_Semaphore_Complex.h"
#else
#include "ace/Synch.h"
#endif /* !(ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS) */

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

#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS)
  /// Return the underlying mutex.
  const ACE_mutex_t &lock (void) const;
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS */

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS)
  ACE_Mutex lock_;
#else
  /// If the user does not provide a name we generate a unique name in
  /// this buffer.
  ACE_TCHAR name_[ACE_UNIQUE_NAME_LEN];

  /// Create and return the unique name.
  const ACE_TCHAR *unique_name (void);

  /// We need this to get the right semantics...
  ACE_SV_Semaphore_Complex lock_;
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS */
};

#if defined (__ACE_INLINE__)
#include "ace/Process_Mutex.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_PROCESS_MUTEX_H */
