/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RW_Process_Mutex.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_RW_PROCESS_MUTEX_H
#define ACE_RW_PROCESS_MUTEX_H
#include "ace/pre.h"

#include "ace/File_Lock.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_RW_Process_Mutex
{
  // = TITLE
  //     Wrapper for readers/writer locks that exist across processes.
  //
  // = DESCRIPTION
  //     Note that because this class uses the
  //     <ACE_File_Lock> as its implementation it only can be reliably
  //     used between separate processes, rather than threads in the
  //     same process.  This isn't a limitation of ACE, it's simply
  //     the file lock semantics on UNIX and Win32.
public:
  ACE_RW_Process_Mutex (const ACE_TCHAR *name = 0,
                        int flags = O_CREAT|O_RDWR);
  // Create a readers/writer <Process_Mutex>, passing in the optional
  // <name>.

  ~ACE_RW_Process_Mutex (void);

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
  // Attempt to upgrade a read lock to a write lock. Returns 0 on
  // success, -1 on failure.

  const ACE_File_Lock &lock (void) const;
  // Return the underlying lock.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_File_Lock lock_;
  // We need this to get the readers/writer semantics...
};

#if defined (__ACE_INLINE__)
#include "ace/RW_Process_Mutex.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RW_PROCESS_MUTEX_H */
