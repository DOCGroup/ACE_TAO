/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Process_Semaphore.h
 *
 *  $Id$
 *
 *   Wrapper for Dijkstra style general semaphores that work
 *   across processes.
 *
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_PROCESS_SEMAPHORE_H
#define ACE_PROCESS_SEMAPHORE_H
#include "ace/pre.h"

#include "ace/Threads/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !(defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS))
#include "ace/IPC/SV_Semaphore_Complex.h"
#endif /* !(ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS) */

/**
 * @class ACE_Process_Semaphore
 *
 * @brief Wrapper for Dijkstra style general semaphores that work
 * across processes.
 */
class ACE_Export ACE_Process_Semaphore
{
public:
  /// Initialize the semaphore, with an initial value of <count> and a
  /// maximum value of <max>.
  ACE_Process_Semaphore (u_int count = 1, // By default make this unlocked.
                         const ACE_TCHAR *name = 0,
                         void * = 0,
                         int max = 0x7FFFFFFF);

  /**
   * This method is a no-op, i.e., it doesn't remove the semaphore.
   * If you want to remove the semaphore, you must call the <remove>
   * method explicitly.
   */
  ~ACE_Process_Semaphore (void);

  /**
   * Explicitly destroy the semaphore.  Note that only one thread
   * should call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /// Block the thread until the semaphore count becomes greater than
  /// 0, then decrement it.
  int acquire (void);

  /**
   * Conditionally decrement the semaphore if count is greater than 0
   * (i.e., won't block).  Returns -1 on failure.  If we "failed"
   * because someone else already had the lock, <errno> is set to
   * <EBUSY>.
   */
  int tryacquire (void);

  /// Increment the semaphore, potentially unblocking a waiting thread.
  int release (void);

  /**
   * Acquire semaphore ownership.  This calls <acquire> and is only
   * here to make the <ACE_Process_Semaphore> interface consistent
   * with the other synchronization APIs.
   */
  int acquire_read (void);

  /**
   * Acquire semaphore ownership.  This calls <acquire> and is only
   * here to make the <ACE_Process_Semaphore> interface consistent
   * with the other synchronization APIs.
   */
  int acquire_write (void);

  /**
   * Conditionally acquire semaphore (i.e., won't block).  This calls
   * <tryacquire> and is only here to make the <ACE_Process_Semaphore>
   * interface consistent with the other synchronization APIs.
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, <errno> is set to <EBUSY>.
   */
  int tryacquire_read (void);

  /**
   * Conditionally acquire semaphore (i.e., won't block).  This calls
   * <tryacquire> and is only here to make the <ACE_Process_Semaphore>
   * interface consistent with the other synchronization APIs.
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, <errno> is set to <EBUSY>.
   */
  int tryacquire_write (void);

  /**
   * This is only here to make the <ACE_Process_Semaphore>
   * interface consistent with the other synchronization APIs.
   * Assumes the caller has already acquired the semaphore using one of
   * the above calls, and returns 0 (success) always.
   */
  int tryacquire_write_upgrade (void);

#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS)
  /// Return the underlying lock.
  const ACE_sema_t &lock (void) const;
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS */

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS)
  ACE_Semaphore lock_;
#else
  /// We need this to get the right semantics...
  ACE_SV_Semaphore_Complex lock_;
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS */
};

#if defined (__ACE_INLINE__)
#include "ace/Threads/Process_Semaphore.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_PROCESS_SEMAPHORE_H */
