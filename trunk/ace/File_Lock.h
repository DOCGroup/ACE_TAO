/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    File_Lock.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_FILE_LOCK_H
#define ACE_FILE_LOCK_H
#include "ace/pre.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_File_Lock
{
  // = TITLE
  //     A wrapper around the UNIX file locking mechanism.
  //
  // = DESCRIPTION
  //     Allows us to "adapt" the UNIX file locking mechanisms to work
  //     with all of our Guard stuff...
public:
  ACE_File_Lock (ACE_HANDLE handle = ACE_INVALID_HANDLE);
  // Set the <handle_> of the File_Lock to <handle>.  Note that this
  // constructor assumes ownership of the <handle> and will close it
  // down in <remove>.  If you want the <handle> stays open when
  // <remove> is called make sure to call <dup> on the <handle> before
  // closing it.

  ACE_File_Lock (const ACE_TCHAR *filename, int flags, mode_t mode = 0);
  // Open the <filename> with <flags> and <mode> and set the result to
  // <handle_>.

  int open (const ACE_TCHAR *filename, int flags, mode_t mode = 0);
  // Open the <filename> with <flags> and <mode> and set the result to
  // <handle_>.

  ~ACE_File_Lock (void);
  // Remove a File lock by releasing it and closing down the <handle_>.

  int remove (int unlink_file = 1);
  // Remove a File lock by releasing it and closing down the
  // <handle_>.  If <unlink_file> is non-0 then we unlink the file.

  int acquire (short whence = 0, off_t start = 0, off_t len = 1);
  // Note, for interface uniformity with other synchronization
  // wrappers we include the <acquire> method.  This is implemented as
  // a write-lock to be on the safe-side...

  int tryacquire (short whence = 0, off_t start = 0, off_t len = 1);
  // Note, for interface uniformity with other synchronization
  // wrappers we include the <tryacquire> method.  This is implemented
  // as a write-lock to be on the safe-side...  Returns -1 on failure.
  // If we "failed" because someone else already had the lock, <errno>
  // is set to <EBUSY>.

  int release (short whence = 0, off_t start = 0, off_t len = 1);
  // Unlock a readers/writer lock.

  int acquire_write (short whence = 0, off_t start = 0, off_t len = 1);
  // Acquire a write lock, but block if any readers or a
  // writer hold the lock.

  int tryacquire_write (short whence = 0, off_t start = 0, off_t len = 1);
  // Conditionally acquire a write lock (i.e., won't block).  Returns
  // -1 on failure.  If we "failed" because someone else already had
  // the lock, <errno> is set to <EBUSY>.

  int tryacquire_write_upgrade (short whence = 0,
                                off_t start = 0,
                                off_t len = 1);
  // Conditionally upgrade to a write lock (i.e., won't block).  Returns
  // -1 on failure.  If we "failed" because someone else already had
  // the lock, <errno> is set to <EBUSY>.

  int acquire_read (short whence = 0, off_t start = 0, off_t len = 1);
  // Acquire a read lock, but block if a writer hold the lock.
  // Returns -1 on failure.  If we "failed" because someone else
  // already had the lock, <errno> is set to <EBUSY>.

  int tryacquire_read (short whence = 0, off_t start = 0, off_t len = 1);
  // Conditionally acquire a read lock (i.e., won't block).  Returns
  // -1 on failure.  If we "failed" because someone else already had
  // the lock, <errno> is set to <EBUSY>.

  ACE_HANDLE get_handle (void);
  // Get underlying <ACE_HANDLE> for the file.

  void set_handle (ACE_HANDLE);
  // Set underlying <ACE_HANDLE>.  Note that this method assumes
  // ownership of the <handle> and will close it down in <remove>.  If
  // you want the <handle> to stay open when <remove> is called make
  // sure to call <dup> on the <handle> before closing it. You are
  // responsible for the closing the existing <handle> before
  // overwriting it.

  void dump (void) const;
  // Dump state of the object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_OS::ace_flock_t lock_;
  // Locking structure for OS record locks.

  int removed_;
  // Keeps track of whether <remove> has been called yet to avoid
  // multiple <remove> calls, e.g., explicitly and implicitly in the
  // destructor.  This flag isn't protected by a lock, so make sure
  // that you don't have multiple threads simultaneously calling
  // <remove> on the same object, which is a bad idea anyway...

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_File_Lock &);
  ACE_File_Lock (const ACE_File_Lock &);
};

#if defined (__ACE_INLINE__)
#include "ace/File_Lock.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_FILE_LOCK_H */
