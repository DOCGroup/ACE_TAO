/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    RW_Process_Mutex.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_RW_PROCESS_MUTEX_H
#define ACE_RW_PROCESS_MUTEX_H
#include "ace/pre.h"

#include "ace/File_Lock.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_RW_Process_Mutex
 *
 * @brief Wrapper for readers/writer locks that exist across processes.
 *
 * Note that because this class uses the
 * <ACE_File_Lock> as its implementation it only can be reliably
 * used between separate processes, rather than threads in the
 * same process.  This isn't a limitation of ACE, it's simply
 * the file lock semantics on UNIX and Win32.
 */
class ACE_Export ACE_RW_Process_Mutex
{
public:
  /// Create a readers/writer <Process_Mutex>, passing in the optional
  /// <name>.  If not specified, a name is generated.
  ACE_RW_Process_Mutex (const ACE_TCHAR *name = 0,
                        int flags = O_CREAT|O_RDWR);

  ~ACE_RW_Process_Mutex (void);

  /**
   * Explicitly destroy the mutex.  Note that only one thread should
   * call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /// Acquire lock ownership (wait on queue if necessary).
  int acquire (void);

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

  /// Attempt to upgrade a read lock to a write lock. Returns 0 on
  /// success, -1 on failure.
  int tryacquire_write_upgrade (void);

  /// Return the underlying lock.
  const ACE_File_Lock &lock (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// If the user does not provide a name we generate a unique name in
  /// this buffer.
  ACE_TCHAR name_[ACE_UNIQUE_NAME_LEN];

  /// Create and return the unique name.
  const ACE_TCHAR *unique_name (void);

  /// We need this to get the readers/writer semantics...
  ACE_File_Lock lock_;
};

#if defined (__ACE_INLINE__)
#include "ace/RW_Process_Mutex.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RW_PROCESS_MUTEX_H */
