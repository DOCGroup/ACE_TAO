/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Process_Mutex.h
//
// = DESCRIPTION
//     A wrapper for mutexes that can be used across processes on
//     the same host machine, as well as within a process, of
//     course.
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

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

class ACE_Export ACE_Process_Mutex
{
  // = TITLE
  //     A wrapper for mutexes that can be used across processes on
  //     the same host machine, as well as within a process, of
  //     course.
public:
  ACE_Process_Mutex (const ACE_TCHAR *name = 0,
                     void *arg = 0);
  // Create a Process_Mutex, passing in the optional <name>.

  ~ACE_Process_Mutex (void);

  int remove (void);
  // Explicitly destroy the mutex.  Note that only one thread should
  // call this method since it doesn't protect against race
  // conditions.

  int acquire (void);
  // Acquire lock ownership (wait on queue if necessary).

  int tryacquire (void);
  // Conditionally acquire lock (i.e., don't wait on queue).  Returns
  // -1 on failure.  If we "failed" because someone else already had
  // the lock, <errno> is set to <EBUSY>.

  int release (void);
  // Release lock and unblock a thread at head of queue.

  int acquire_read (void);
  // Acquire lock ownership (wait on queue if necessary).

  int acquire_write (void);
  // Acquire lock ownership (wait on queue if necessary).

  int tryacquire_read (void);
  // Conditionally acquire a lock (i.e., won't block).  Returns -1 on
  // failure.  If we "failed" because someone else already had the
  // lock, <errno> is set to <EBUSY>.

  int tryacquire_write (void);
  // Conditionally acquire a lock (i.e., won't block).  Returns -1 on
  // failure.  If we "failed" because someone else already had the
  // lock, <errno> is set to <EBUSY>.

  int tryacquire_write_upgrade (void);
  // This is only here for consistency with the other synchronization
  // APIs and usability with Lock adapters. Assumes the caller already has
  // acquired the mutex and returns 0 in all cases.

#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS)
  const ACE_mutex_t &lock (void) const;
  // Return the underlying mutex.
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM) || defined (ACE_PSOS)
  ACE_Mutex lock_;
#else
  ACE_TCHAR name_[ACE_UNIQUE_NAME_LEN];
  // If the user does not provide a name we generate a unique name in
  // this buffer.

  const ACE_TCHAR *unique_name (void);
  // Create and return the unique name.

  ACE_SV_Semaphore_Complex lock_;
  // We need this to get the right semantics...
#endif /* ACE_WIN32 || ACE_HAS_POSIX_SEM || ACE_PSOS */
};

#if defined (__ACE_INLINE__)
#include "ace/Process_Mutex.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_PROCESS_MUTEX_H */
