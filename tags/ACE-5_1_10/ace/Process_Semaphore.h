/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Process_Semaphore.h
//
// = DESCRIPTION
//     Wrapper for Dijkstra style general semaphores that work
//     across processes.
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_PROCESS_SEMAPHORE_H
#define ACE_PROCESS_SEMAPHORE_H
#include "ace/pre.h"

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !(defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS))
#include "ace/SV_Semaphore_Complex.h"
#endif /* !(ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS) */

class ACE_Export ACE_Process_Semaphore
{
  // = TITLE
  //     Wrapper for Dijkstra style general semaphores that work
  //     across processes.
public:
  ACE_Process_Semaphore (u_int count = 1, // By default make this unlocked.
                         const ACE_TCHAR *name = 0,
                         void * = 0,
                         int max = 0x7FFFFFFF);
  // Initialize the semaphore, with an initial value of <count> and a
  // maximum value of <max>.

  ~ACE_Process_Semaphore (void);
  // This method is a no-op, i.e., it doesn't remove the semaphore.
  // If you want to remove the semaphore, you must call the <remove>
  // method explicitly.

  int remove (void);
  // Explicitly destroy the semaphore.  Note that only one thread
  // should call this method since it doesn't protect against race
  // conditions.

  int acquire (void);
  // Block the thread until the semaphore count becomes greater than
  // 0, then decrement it.

  int tryacquire (void);
  // Conditionally decrement the semaphore if count is greater than 0
  // (i.e., won't block).  Returns -1 on failure.  If we "failed"
  // because someone else already had the lock, <errno> is set to
  // <EBUSY>.

  int release (void);
  // Increment the semaphore, potentially unblocking a waiting thread.

  int acquire_read (void);
  // Acquire semaphore ownership.  This calls <acquire> and is only
  // here to make the <ACE_Process_Semaphore> interface consistent
  // with the other synchronization APIs.

  int acquire_write (void);
  // Acquire semaphore ownership.  This calls <acquire> and is only
  // here to make the <ACE_Process_Semaphore> interface consistent
  // with the other synchronization APIs.

  int tryacquire_read (void);
  // Conditionally acquire semaphore (i.e., won't block).  This calls
  // <tryacquire> and is only here to make the <ACE_Process_Semaphore>
  // interface consistent with the other synchronization APIs.
  // Returns -1 on failure.  If we "failed" because someone else
  // already had the lock, <errno> is set to <EBUSY>.

  int tryacquire_write (void);
  // Conditionally acquire semaphore (i.e., won't block).  This calls
  // <tryacquire> and is only here to make the <ACE_Process_Semaphore>
  // interface consistent with the other synchronization APIs.
  // Returns -1 on failure.  If we "failed" because someone else
  // already had the lock, <errno> is set to <EBUSY>.

  int tryacquire_write_upgrade (void);
  // This is only here to make the <ACE_Process_Semaphore>
  // interface consistent with the other synchronization APIs.
  // Assumes the caller has already acquired the semaphore using one of
  // the above calls, and returns 0 (success) always.

#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS)
  const ACE_sema_t &lock (void) const;
  // Return the underlying lock.
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS)
  ACE_Semaphore lock_;
#else
  ACE_SV_Semaphore_Complex lock_;
  // We need this to get the right semantics...
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS */
};

#if defined (__ACE_INLINE__)
#include "ace/Process_Semaphore.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_PROCESS_SEMAPHORE_H */
