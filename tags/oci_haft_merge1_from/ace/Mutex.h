// -*- C++ -*-

//==========================================================================
/**
 *  @file    Mutex.h
 *
 *  $Id$
 *
 *   Moved from Synch.h.
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//==========================================================================

#ifndef ACE_MUTEX_H
#define ACE_MUTEX_H
#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS.h"

class ACE_Time_Value;

/**
 * @class ACE_Mutex
 *
 * @brief <ACE_Mutex> wrapper (valid in same process or across
 * processes (depending on TYPE flag)).
 */
class ACE_Export ACE_Mutex
{
public:
  /// Initialize the mutex.
  ACE_Mutex (int type = USYNC_THREAD,
             const ACE_TCHAR *name = 0,
             ACE_mutexattr_t *arg = 0,
             mode_t mode = ACE_DEFAULT_FILE_PERMS);

  /// Implicitly destroy the mutex.
  ~ACE_Mutex (void);

  /**
   * Explicitly destroy the mutex.  Note that only one thread should
   * call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /// Acquire lock ownership (wait on queue if necessary).
  int acquire (void);

  /**
   * Block the thread until the mutex is acquired or <tv> times out,
   * in which case -1 is returned and <errno> == <ETIME>.  Note that
   * <tv> is assumed  to be in "absolute" rather than "relative" time.
   * The value of <tv> is updated upon return to show the actual
   * (absolute) acquisition time.
   */
  int acquire (ACE_Time_Value &tv);

  /**
   * If <tv> == 0 then call <acquire()> directly.  Otherwise, block
   * the thread until the mutex is acquired or <tv> times out, in
   * which case -1 is returned and <errno> == <ETIME>.  Note that
   * <*tv> is assumed to be in "absolute" rather than "relative" time.
   * The value of <*tv> is updated upon return to show the actual
   * (absolute) acquisition time.  */
  int acquire (ACE_Time_Value *tv);

  /**
   * Conditionally acquire lock (i.e., don't wait on queue).  Returns
   * -1 on failure.  If we "failed" because someone else already had
   * the lock, <errno> is set to <EBUSY>.
   */
  int tryacquire (void);

  /// Release lock and unblock a thread at head of queue.
  int release (void);

  /**
   * Acquire mutex ownership.  This calls <acquire> and is only
   * here to make the <ACE_Mutex> interface consistent with the
   * other synchronization APIs.
   */
  int acquire_read (void);

  /**
   * Acquire mutex ownership.  This calls <acquire> and is only
   * here to make the <ACE_Mutex> interface consistent with the
   * other synchronization APIs.
   */
  int acquire_write (void);

  /**
   * Conditionally acquire mutex (i.e., won't block).  This calls
   * <tryacquire> and is only here to make the <ACE_Mutex> interface
   * consistent with the other synchronization APIs.  Returns -1 on
   * failure.  If we "failed" because someone else already had the
   * lock, <errno> is set to <EBUSY>.
   */
  int tryacquire_read (void);

  /**
   * Conditionally acquire mutex (i.e., won't block).  This calls
   * <tryacquire> and is only here to make the <ACE_Mutex> interface
   * consistent with the other synchronization APIs.  Returns -1 on
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

  /// Return the underlying mutex.
  const ACE_mutex_t &lock (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  // = This should be protected but some C++ compilers complain...
public:
#if defined (CHORUS) || defined(ACE_HAS_PTHREADS) || defined(ACE_HAS_STHREADS)
  /// This lock resides in shared memory.
  ACE_mutex_t *process_lock_;

  /**
   * Remember the name of the mutex if we created it so we can unlink
   * it when we go away (only the actor that initialized the memory
   * can destroy it).
   */
  const ACE_TCHAR *lockname_;
#endif /* CHORUS || ACE_HAS_PTHREADS */

  /// Mutex type supported by the OS.
  ACE_mutex_t lock_;

  /// Keeps track of whether <remove> has been called yet to avoid
  /// multiple <remove> calls, e.g., explicitly and implicitly in the
  /// destructor.  This flag isn't protected by a lock, so make sure
  /// that you don't have multiple threads simultaneously calling
  /// <remove> on the same object, which is a bad idea anyway...
  int removed_;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Mutex &);
  ACE_Mutex (const ACE_Mutex &);
};

#if defined (__ACE_INLINE__)
#include "ace/Mutex.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_MUTEX_H */
