/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Synch.h
//
// = DESCRIPTION
//     Wrappers for various synchronization routines.
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SYNCH_H
#define ACE_SYNCH_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !(defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM))
#include "ace/SV_Semaphore_Complex.h"
#endif /* !(defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)) */

// Forward declarations.
class ACE_Time_Value;
// template <class ACE_COND_MUTEX> class ACE_Condition;

class ACE_Export ACE_Lock
{
  // = TITLE
  //     This is the abstract base class that contains the uniform
  //     locking API that is supported by all the ACE synchronization
  //     mechanisms.
  //
  // = DESCRIPTION
  //     This class is typically used in conjunction with the
  //     <ACE_Lock_Adapter> in order to provide a polymorphic
  //     interface to the ACE synchronization mechanisms (e.g.,
  //     <ACE_Mutex>, <ACE_Semaphore>, <ACE_RW_Mutex>, etc).  Note that
  //     the reason that all of ACE doesn't use polymorphic locks is
  //     that (1) they add ~20% extra overhead for virtual function
  //     calls and (2) objects with virtual functions can't be placed
  //     into shared memory.
public:
  ACE_Lock (void);
  // CE needs a default ctor here.

  virtual ~ACE_Lock (void);
  // Noop virtual destructor

  virtual int remove (void) = 0;
  // Explicitly destroy the lock.

  virtual int acquire (void) = 0;
  // Block the thread until the lock is acquired.  Returns -1 on
  // failure.

  virtual int tryacquire (void) = 0;
  // Conditionally acquire the lock (i.e., won't block).  Returns -1
  // on failure.  If we "failed" because someone else already had the
  // lock, <errno> is set to <EBUSY>.

  virtual int release (void) = 0;
  // Release the lock.  Returns -1 on failure.

  virtual int acquire_read (void) = 0;
  // Block until the thread acquires a read lock.  If the locking
  // mechanism doesn't support read locks then this just calls
  // <acquire>.  Returns -1 on failure.

  virtual int acquire_write (void) = 0;
  // Block until the thread acquires a write lock.  If the locking
  // mechanism doesn't support read locks then this just calls
  // <acquire>.  Returns -1 on failure.

  virtual int tryacquire_read (void) = 0;
  // Conditionally acquire a read lock.  If the locking mechanism
  // doesn't support read locks then this just calls <acquire>.
  // Returns -1 on failure.  If we "failed" because someone else
  // already had the lock, <errno> is set to <EBUSY>.

  virtual int tryacquire_write (void) = 0;
  // Conditionally acquire a write lock.  If the locking mechanism
  // doesn't support read locks then this just calls <acquire>.
  // Returns -1 on failure.  If we "failed" because someone else
  // already had the lock, <errno> is set to <EBUSY>.
};

class ACE_Export ACE_Adaptive_Lock : public ACE_Lock
{
  // = TITLE
  //    An adaptive general locking class that defers the decision of
  //    lock type to run time.
  //
  // = DESCRIPTION
  //    This class, as ACE_Lock, provide a set of general locking APIs.
  //    However, it defers our decision of what kind of lock to use
  //    to the run time and delegates all locking operations to the actual
  //    lock.  Users must define a constructor in their subclass to
  //    initialize <lock_>.
public:
  virtual ~ACE_Adaptive_Lock (void);
  // You must also override the destructor function to match with how
  // you construct the underneath <lock_>.

  // = Lock/unlock operations.

  virtual int remove (void);
  virtual int acquire (void);
  virtual int tryacquire (void);
  virtual int release (void);
  virtual int acquire_read (void);
  virtual int acquire_write (void);
  virtual int tryacquire_read (void);
  virtual int tryacquire_write (void);
  void dump (void) const;

protected:
  ACE_Adaptive_Lock (void);
  // Create and initialize create the actual lcok used in the class.
  // The default constructor simply set the <lock_> to 0 (null).  You
  // must overwrite this method for this class to work.

  ACE_Lock *lock_;

private:
  ACE_Adaptive_Lock (const ACE_Adaptive_Lock &);
  ACE_Adaptive_Lock & operator= (const ACE_Adaptive_Lock & rhs);
  // Explicitly disallow use of implicitly generated copy
  // constructor and assignment operator to prevent inadvertent
  // memory leaks.
};

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

  ACE_File_Lock (LPCTSTR filename, int flags, mode_t mode = 0);
  // Open the <filename> with <flags> and <mode> and set the result to
  // <handle_>.

  int open (LPCTSTR filename, int flags, mode_t mode = 0);
  // Open the <filename> with <flags> and <mode> and set the result to
  // <handle_>.

  ~ACE_File_Lock (void);
  // Remove a File lock by releasing it and closing down the <handle_>.

  int remove (void);
  // Remove a File lock by releasing it and closing down the <handle_>.

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

class ACE_Export ACE_Semaphore
{
  // = TITLE
  //     Wrapper for Dijkstra style general semaphores.
public:
  // = Initialization and termination.
  ACE_Semaphore (u_int count = 1, // By default make this unlocked.
                 int type = USYNC_THREAD,
                 LPCTSTR name = 0,
                 void * = 0,
                 int max = 0x7fffffff);
  // Initialize the semaphore, with initial value of "count".

  virtual ~ACE_Semaphore (void);
  // Implicitly destroy the semaphore.

  int remove (void);
  // Explicitly destroy the semaphore.

  int acquire (void);
  // Block the thread until the semaphore count becomes
  // greater than 0, then decrement it.

  int acquire (ACE_Time_Value &tv);
  // Block the thread until <tv> times out or until the semaphore
  // count becomes greater than 0 (at which point it is decremented).
  // Note that <tv> is assumed to be in "absolute" rather than
  // "relative" time.  The value of <tv> is updated upon return, i.e.,
  // the caller gets the amount of time that has elapsed while waiting
  // to acquire the semaphore.

  int tryacquire (void);
  // Conditionally decrement the semaphore if count is greater than 0
  // (i.e., won't block).  Returns -1 on failure.  If we "failed"
  // because someone else already had the lock, <errno> is set to
  // <EBUSY>.

  int release (void);
  // Increment the semaphore by 1, potentially unblocking a waiting
  // thread.

  int release (size_t release_count);
  // Increment the semaphore by <release_count>, potentially
  // unblocking waiting threads.

  int acquire_read (void);
  // Acquire semaphore ownership.  This calls <acquire> and is only
  // here to make the <ACE_Semaphore> interface consistent with the
  // other synchronization APIs.

  int acquire_write (void);
  // Acquire semaphore ownership.  This calls <acquire> and is only
  // here to make the <ACE_Semaphore> interface consistent with the
  // other synchronization APIs.

  int tryacquire_read (void);
  // Conditionally acquire semaphore (i.e., won't block).  This calls
  // <tryacquire> and is only here to make the <ACE_Semaphore>
  // interface consistent with the other synchronization APIs.
  // Returns -1 on failure.  If we "failed" because someone else
  // already had the lock, <errno> is set to <EBUSY>.

  int tryacquire_write (void);
  // Conditionally acquire semaphore (i.e., won't block).  This calls
  // <tryacquire> and is only here to make the <ACE_Semaphore>
  // interface consistent with the other synchronization APIs.
  // Returns -1 on failure.  If we "failed" because someone else
  // already had the lock, <errno> is set to <EBUSY>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  const ACE_sema_t &lock (void) const;
  // Return the underlying lock.

protected:
  ACE_sema_t semaphore_;

  int removed_;
  // Keeps track of whether <remove> has been called yet to avoid
  // multiple <remove> calls, e.g., explicitly and implicitly in the
  // destructor.  This flag isn't protected by a lock, so make sure
  // that you don't have multiple threads simultaneously calling
  // <remove> on the same object, which is a bad idea anyway...

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Semaphore &);
  ACE_Semaphore (const ACE_Semaphore &);
};

class ACE_Export ACE_Process_Semaphore
{
  // = TITLE
  //     Wrapper for Dijkstra style general semaphores that work
  //     across processes.
public:
  ACE_Process_Semaphore (u_int count = 1, // By default make this unlocked.
                         LPCTSTR name = 0,
                         void * = 0,
                         int max = 0x7FFFFFFF);
  // Initialize the semaphore, with an initial value of <count> and a
  // maximum value of <max>.

  ~ACE_Process_Semaphore (void);
  // Implicitly destroy the semaphore.

  int remove (void);
  // Explicitly destroy the semaphore.

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

#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)
  const ACE_sema_t &lock (void) const;
  // Return the underlying lock.
#endif /* ACE_WIN32 */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)
  ACE_Semaphore lock_;
#else
  ACE_SV_Semaphore_Complex lock_;
  // We need this to get the right semantics...
#endif /* ACE_WIN32 */
};

class ACE_Export ACE_Null_Semaphore
{
  // = TITLE
  //     Implement a do nothing <ACE_Semaphore>, i.e., all the methods are
  //     no ops.
public:
  ACE_Null_Semaphore (u_int count = 1, // By default make this unlocked.
		       int type = USYNC_THREAD,
		       LPCTSTR name = 0,
		       void * = 0,
		       int max = 0x7fffffff);
  ~ACE_Null_Semaphore (void);
  int remove (void);

  int acquire (ACE_Time_Value &);
  int acquire (void);
  int tryacquire (void);
  int release (void);
  int release (size_t);
  int acquire_write (void);
  int tryacquire_write (void);
  int acquire_read (void);
  int tryacquire_read (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_RW_Mutex
{
  // = TITLE
  //     Wrapper for readers/writer locks.
  //
  // = DESCRIPTION
  //     These are most useful for applications that have many more
  //     parallel readers than writers...
public:
  ACE_RW_Mutex (int type = USYNC_THREAD,
                LPCTSTR name = 0,
                void *arg = 0);
  // Initialize a readers/writer lock.

  virtual ~ACE_RW_Mutex (void);
  // Implicitly destroy a readers/writer lock

  int remove (void);
  // Explicitly destroy a readers/writer lock.

  int acquire_read (void);
  // Acquire a read lock, but block if a writer hold the lock.

  int acquire_write (void);
  // Acquire a write lock, but block if any readers or a
  // writer hold the lock.

  int tryacquire_read (void);
  // Conditionally acquire a read lock (i.e., won't block).  Returns
  // -1 on failure.  If we "failed" because someone else already had
  // the lock, <errno> is set to <EBUSY>.

  int tryacquire_write (void);
  // Conditionally acquire a write lock (i.e., won't block).

  int acquire (void);
  // Note, for interface uniformity with other synchronization
  // wrappers we include the <acquire> method.  This is implemented as
  // a write-lock to safe...

  int tryacquire (void);
  // Note, for interface uniformity with other synchronization
  // wrappers we include the <tryacquire> method.  This is implemented
  // as a write-lock to be safe...  Returns -1 on failure.  If we
  // "failed" because someone else already had the lock, <errno> is
  // set to <EBUSY>.

  int release (void);
  // Unlock a readers/writer lock.

  const ACE_rwlock_t &lock (void) const;
  // Return the underlying lock.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_rwlock_t lock_;
  // Readers/writer lock.

  int removed_;
  // Keeps track of whether <remove> has been called yet to avoid
  // multiple <remove> calls, e.g., explicitly and implicitly in the
  // destructor.  This flag isn't protected by a lock, so make sure
  // that you don't have multiple threads simultaneously calling
  // <remove> on the same object, which is a bad idea anyway...
private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_RW_Mutex &);
  ACE_RW_Mutex (const ACE_RW_Mutex &);
};

class ACE_Export ACE_Mutex
{
  // = TITLE
  //     <ACE_Mutex> wrapper (valid in same process or across
  //     processes (depending on TYPE flag)).
public:
  ACE_Mutex (int type = USYNC_THREAD,
             LPCTSTR name = 0,
             void *arg = 0);
  // Initialize the mutex.

  ~ACE_Mutex (void);
  // Implicitly destroy the mutex.

  int remove (void);
  // Explicitly destroy the mutex.

  int acquire (void);
  // Acquire lock ownership (wait on queue if necessary).

  int tryacquire (void);
  // Conditionally acquire lock (i.e., don't wait on queue).  Returns
  // -1 on failure.  If we "failed" because someone else already had
  // the lock, <errno> is set to <EBUSY>.

  int release (void);
  // Release lock and unblock a thread at head of queue.

  int acquire_read (void);
  // Acquire mutex ownership.  This calls <acquire> and is only
  // here to make the <ACE_Mutex> interface consistent with the
  // other synchronization APIs.

  int acquire_write (void);
  // Acquire mutex ownership.  This calls <acquire> and is only
  // here to make the <ACE_Mutex> interface consistent with the
  // other synchronization APIs.

  int tryacquire_read (void);
  // Conditionally acquire mutex (i.e., won't block).  This calls
  // <tryacquire> and is only here to make the <ACE_Mutex> interface
  // consistent with the other synchronization APIs.  Returns -1 on
  // failure.  If we "failed" because someone else already had the
  // lock, <errno> is set to <EBUSY>.

  int tryacquire_write (void);
  // Conditionally acquire mutex (i.e., won't block).  This calls
  // <tryacquire> and is only here to make the <ACE_Mutex> interface
  // consistent with the other synchronization APIs.  Returns -1 on
  // failure.  If we "failed" because someone else already had the
  // lock, <errno> is set to <EBUSY>.

  const ACE_mutex_t &lock (void) const;
  // Return the underlying mutex.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
#if defined (CHORUS)
  ACE_mutex_t *process_lock_;
  // This lock resides in shared memory.

  LPCTSTR lockname_;
  // Remember the name of the mutex if we created it so we can unlink
  // it when we go away (only the actor that initialized the memory
  // can destroy it).
#endif /* CHORUS */

  ACE_mutex_t lock_;
  // Mutex type supported by the OS.

  int removed_;
  // Keeps track of whether <remove> has been called yet to avoid
  // multiple <remove> calls, e.g., explicitly and implicitly in the
  // destructor.  This flag isn't protected by a lock, so make sure
  // that you don't have multiple threads simultaneously calling
  // <remove> on the same object, which is a bad idea anyway...

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Mutex &);
  ACE_Mutex (const ACE_Mutex &);
};

class ACE_Export ACE_Process_Mutex
{
  // = TITLE
  //     A wrapper for mutexes that can be used across processes on
  //     the same host machine, as well as within a process, of
  //     course.
public:
  ACE_Process_Mutex (LPCTSTR name = 0,
                     void *arg = 0);
  // Create a Process_Mutex, passing in the optional <name>.

  ~ACE_Process_Mutex (void);

  int remove (void);
  // Explicitly destroy the mutex.

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

#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)
  const ACE_mutex_t &lock (void) const;
  // Return the underlying mutex.
#endif /* ACE_WIN32 */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
#if defined (ACE_WIN32) || defined (ACE_HAS_POSIX_SEM)
  ACE_Mutex lock_;
#else
  TCHAR name_[ACE_UNIQUE_NAME_LEN];
  // If the user does not provide a name we generate a unique name in
  // this buffer.

  LPCTSTR unique_name (void);
  // Create and return the unique name.

  ACE_SV_Semaphore_Complex lock_;
  // We need this to get the right semantics...
#endif /* ACE_WIN32 */
};

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
  ACE_RW_Process_Mutex (LPCTSTR name = 0,
                        int flags = O_CREAT|O_RDWR);
  // Create a readers/writer <Process_Mutex>, passing in the optional
  // <name>.

  ~ACE_RW_Process_Mutex (void);

  int remove (void);
  // Explicitly destroy the mutex.

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

class ACE_Export ACE_Null_Barrier
{
  // = TITLE
  //     Implements "NULL barrier synchronization".
public:
  ACE_Null_Barrier (u_int,
                    const char * = 0,
                    void * = 0);
  // Initialize the barrier to synchronize <count> threads.

  ~ACE_Null_Barrier (void);
  // Default dtor.

  int wait (void);
  // Block the caller until all <count> threads have called <wait> and
  // then allow all the caller threads to continue in parallel.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Null_Barrier &);
  ACE_Null_Barrier (const ACE_Null_Barrier &);
};

class ACE_Export ACE_Null_Mutex
{
  // = TITLE
  //     Implement a do nothing <ACE_Mutex>, i.e., all the methods are
  //     no ops.
public:
  ACE_Null_Mutex (LPCTSTR = 0);
  virtual ~ACE_Null_Mutex (void);
  int remove (void);

  int acquire (void);
  int tryacquire (void);
  int release (void);
  int acquire_write (void);
  int tryacquire_write (void);
  int acquire_read (void);
  int tryacquire_read (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_Noop_Token : public ACE_Null_Mutex
{
public:
  int renew (int = 0, ACE_Time_Value * =0);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_Null_Condition
{
  // = TITLE
  //     Implement a do nothing <ACE_Condition> variable wrapper,
  //     i.e., all methods are no ops.  This class is necessary since
  //     some C++ compilers are *very* lame...
public:
  ACE_Null_Condition (const ACE_Null_Mutex &m,
                      LPCTSTR = 0,
                      void * = 0);
  ~ACE_Null_Condition (void);
  int remove (void);
  int wait (ACE_Time_Value * = 0);
  int signal (void);
  int broadcast (void);
  ACE_Null_Mutex &mutex (void);

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Null_Mutex &mutex_; // Reference to mutex lock.

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Null_Condition &);
  ACE_Null_Condition (const ACE_Null_Condition &);
};

#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
class ACE_Export ACE_Null_Mutex_Guard
{
  // = TITLE
  //     This data structure is meant to be used within a method or
  //     function...  It performs automatic aquisition and release of
  //     an ACE_Null_Mutex.
  //
  // = DESCRIPTION
  //     This should be a specialization of ACE_Guard, but compiler
  //     bugs preclude this...
public:
  ACE_Null_Mutex_Guard (ACE_Null_Mutex &);
  ~ACE_Null_Mutex_Guard (void);
  int remove (void);
  int locked (void);
  int acquire (void);
  int tryacquire (void);
  int release (void);
  void dump (void) const;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Null_Mutex_Guard &);
  ACE_Null_Mutex_Guard (const ACE_Null_Mutex_Guard &);
};
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */

class ACE_Export ACE_TSS_Adapter
{
  // = TITLE
  //     This class encapsulates a TSS object and its associated
  //     C++ destructor function.  It is used by the ACE_TSS...
  //     methods (in Synch_T.cpp) in order to allow an extern
  //     "C" cleanup routine to be used.  Needed by the "frigging"
  //     MVS C++ compiler.
  //
  // = DESCRIPTION
  //     Objects of this class are stored in thread specific
  //     storage. ts_obj_ points to the "real" object and
  //     func_ is a pointer to the C++ cleanup function for ts_obj_.
  //
public:
  ACE_TSS_Adapter (void *object, ACE_THR_DEST f);
  // Initialize the adapter.

  ~ACE_TSS_Adapter (void);
  // Default dtor.

  void cleanup (void);
  // Perform the cleanup operation.

//private:

  void *ts_obj_;
  // The real TS object.

  ACE_THR_DEST func_;
  // The real cleanup routine for ts_obj;
private:
  ACE_TSS_Adapter (const ACE_TSS_Adapter &);
  ACE_TSS_Adapter & operator= (const ACE_TSS_Adapter & rhs);
  // Explicitly disallow use of implicitly generated copy
  // constructor and assignment operator to prevent inadvertent
  // memory leaks.
};

class ACE_Export ACE_Event
{
  // = TITLE
  //     A wrapper around the Win32 event locking mechanism.
  //
  // = DESCRIPTION
  //     Portable implementation of an Event mechanism, which is
  //     native to Win32, but must be emulated on UNIX.
public:
  ACE_Event (int manual_reset = 0,
             int initial_state = 0,
             int type = USYNC_THREAD,
             LPCTSTR name = 0,
             void *arg = 0);
  // Constructor which will create event.

  virtual ~ACE_Event (void);
  // Implicitly destroy the event variable.

  int remove (void);
  // Explicitly destroy the event variable.

  ACE_event_t handle (void) const;
  // Underlying handle to event.

  void handle (ACE_event_t new_handle);
  // Set the underlying handle to event. Note that this method assumes
  // ownership of the <handle> and will close it down in <remove>.  If
  // you want the <handle> to stay open when <remove> is called make
  // sure to call <dup> on the <handle> before closing it.  You are
  // responsible for the closing the existing <handle> before
  // overwriting it.

  int wait (void);
  // if MANUAL reset
  //    sleep till the event becomes signaled
  //    event remains signaled after wait() completes.
  // else AUTO reset
  //    sleep till the event becomes signaled
  //    event resets wait() completes.

  int wait (const ACE_Time_Value *abstime);
  // Same as wait() above, but this one can be timed
  // <abstime> is absolute time-of-day.

  int signal (void);
  // if MANUAL reset
  //    wake up all waiting threads
  //    set to signaled state
  // else AUTO reset
  //    if no thread is waiting, set to signaled state
  //    if thread(s) are waiting, wake up one waiting thread and
  //    reset event

  int pulse (void);
  // if MANUAL reset
  //    wakeup all waiting threads and
  //    reset event
  // else AUTO reset
  //    wakeup one waiting thread (if present) and
  //    reset event

  int reset (void);
  // Set to nonsignaled state.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks

protected:
  ACE_event_t handle_;
  // The underlying handle.

  int removed_;
  // Keeps track of whether <remove> has been called yet to avoid
  // multiple <remove> calls, e.g., explicitly and implicitly in the
  // destructor.  This flag isn't protected by a lock, so make sure
  // that you don't have multiple threads simultaneously calling
  // <remove> on the same object, which is a bad idea anyway...

private:
  // = Prevent copying.
  ACE_Event (const ACE_Event& event);
  const ACE_Event &operator= (const ACE_Event &rhs);
};

class ACE_Export ACE_Manual_Event : public ACE_Event
{
  // = TITLE
  //     Manual Events.
  //
  // = DESCRIPTION
  //     Specialization of Event mechanism which wakes up all waiting
  //     threads on signal()
public:
  ACE_Manual_Event (int initial_state = 0,
                    int type = USYNC_THREAD,
                    LPCTSTR name = 0,
                    void *arg = 0);
  // constructor which will create manual event

  ~ACE_Manual_Event (void);
  // Default dtor.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks
};

class ACE_Export ACE_Auto_Event : public ACE_Event
{
  // = TITLE
  //     Auto Events.
  //
  // = DESCRIPTION
  //     Specialization of Event mechanism which wakes up one waiting
  //     thread on signal()
public:
  ACE_Auto_Event (int initial_state = 0,
                  int type = USYNC_THREAD,
                  LPCTSTR name = 0,
                  void *arg = 0);
  // constructor which will create auto event

  ~ACE_Auto_Event (void);
  // Default dtor.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks
};

// ACE platform supports some form of threading.
#if !defined (ACE_HAS_THREADS)
class ACE_Barrier
{
  // = TITLE
  //   This is a no-op to make ACE "syntactically consistent."
public:
  ACE_Barrier (u_int, LPCTSTR = 0, void * = 0) {}
  virtual ~ACE_Barrier (void) {}
  int wait (void) { ACE_NOTSUP_RETURN (-1); }
  void dump (void) const {}
};
#else
class ACE_Export ACE_Thread_Mutex
{
  // = TITLE
  //     ACE_Thread_Mutex wrapper (only valid for threads in the same
  //     process).
  //
  // = DESCRIPTION
  //     This implementation is optimized for locking threads that are
  //     in the same process.  It maps to <CRITICAL_SECTION>s on NT
  //     and <ACE_mutex_t> with <type> set to <USYNC_THREAD> on UNIX.
  //
  //     ACE_Thread_Mutex is recursive on some platforms (like
  //     Win32). However, on most platforms (like Solaris) it is not
  //     recursive.  To be totally safe and portable, developers
  //     should use ACE_Recursive_Thread_Mutex when they need a
  //     recursive mutex.
  friend class ACE_Condition_Thread_Mutex;
public:
  ACE_Thread_Mutex (LPCTSTR name = 0, void *arg = 0);

  ~ACE_Thread_Mutex (void);
  // Implicitly destroy the mutex.

  int remove (void);
  // Explicitly destroy the mutex.

  int acquire (void);
  // Acquire lock ownership (wait on queue if necessary).

  int tryacquire (void);
  // Conditionally acquire lock (i.e., don't wait on queue).  Returns
  // -1 on failure.  If we "failed" because someone else already had
  // the lock, <errno> is set to <EBUSY>.

  int release (void);
  // Release lock and unblock a thread at head of queue.

  int acquire_read (void);
  // Acquire mutex ownership.  This calls <acquire> and is only here
  // to make the <ACE_Thread_Mutex> interface consistent with the
  // other synchronization APIs.

  int acquire_write (void);
  // Acquire mutex ownership.  This calls <acquire> and is only here
  // to make the <ACE_Thread_Mutex> interface consistent with the
  // other synchronization APIs.

  int tryacquire_read (void);
  // Conditionally acquire mutex (i.e., won't block).  This calls
  // <tryacquire> and is only here to make the <ACE_Thread_Mutex>
  // interface consistent with the other synchronization APIs.
  // Returns -1 on failure.  If we "failed" because someone else
  // already had the lock, <errno> is set to <EBUSY>.

  int tryacquire_write (void);
  // Conditionally acquire mutex (i.e., won't block).  This calls
  // <tryacquire> and is only here to make the <ACE_Thread_Mutex>
  // interface consistent with the other synchronization APIs.
  // Returns -1 on failure.  If we "failed" because someone else
  // already had the lock, <errno> is set to <EBUSY>.

  const ACE_thread_mutex_t &lock (void) const;
  // Return the underlying mutex.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  // protected:
  ACE_thread_mutex_t lock_;
  // Mutex type that supports single-process locking efficiently.

  int removed_;
  // Keeps track of whether <remove> has been called yet to avoid
  // multiple <remove> calls, e.g., explicitly and implicitly in the
  // destructor.  This flag isn't protected by a lock, so make sure
  // that you don't have multiple threads simultaneously calling
  // <remove> on the same object, which is a bad idea anyway...

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Thread_Mutex &);
  ACE_Thread_Mutex (const ACE_Thread_Mutex &);
};

#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
class ACE_Export ACE_Thread_Mutex_Guard
{
  // = TITLE
  //     This data structure is meant to be used within a method or
  //     function...  It performs automatic aquisition and release of
  //     an <ACE_Thread_Mutex>.
  //
  // = DESCRIPTION
  //     This should be a specialization of <ACE_Guard>, but compiler
  //     bugs in older C++ compilers preclude this...
public:
  ACE_Thread_Mutex_Guard (ACE_Thread_Mutex &m, int block = 1);
  // Implicitly and automatically acquire the lock.

  ~ACE_Thread_Mutex_Guard (void);
  // Implicitly release the lock.

  int locked (void);
  // 1 if locked, 0 if couldn't acquire the lock (errno will contain
  // the reason for this).

  int remove (void);
  // Explicitly release the lock.

  int acquire (void);
  // Explicitly acquire the lock.

  int tryacquire (void);
  // Conditionally acquire the lock (i.e., won't block).  Returns -1
  // on failure.  If we "failed" because someone else already had the
  // lock, <errno> is set to <EBUSY>.

  int release (void);
  // Explicitly release the lock.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Thread_Mutex &lock_;
  // Reference to the mutex.

  int owner_;
  // Keeps track of whether we acquired the lock or failed.

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Thread_Mutex_Guard &);
  ACE_Thread_Mutex_Guard (const ACE_Thread_Mutex_Guard &);
};
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */

class ACE_Export ACE_Condition_Thread_Mutex
{
  // = TITLE
  //     ACE_Condition variable wrapper written using ACE_Mutexes This
  //     allows threads to block until shared data changes state.
  //
  //     A condition variable enables threads to atomically block and
  //     test the condition under the protection of a mutual exclu-
  //     sion lock (mutex) until the condition is satisfied.  That is,
  //     the mutex must have been held by the thread before calling
  //     wait or signal on the condition.  If the condition is false,
  //     a thread blocks on a condition variable and atomically
  //     releases the mutex that is waiting for the condition to
  //     change.  If another thread changes the condition, it may wake
  //     up waiting threads by signaling the associated condition
  //     variable.  The waiting threads, upon awakening, reacquire the
  //     mutex and re-evaluate the condition.
  //
  // = DESCRIPTION
  //     This should be an instantiation of ACE_Condition but problems
  //     with compilers precludes this...
public:
  ACE_Condition_Thread_Mutex (const ACE_Thread_Mutex &m,
                              LPCTSTR name = 0,
                              void *arg = 0);
  // Initialize the condition variable.

  ~ACE_Condition_Thread_Mutex (void);
  // Implicitly destroy the condition variable.

  int remove (void);
  // Explicitly destroy the condition variable.

  int wait (const ACE_Time_Value *abstime);
  // Block on condition, or until absolute time-of-day has passed.  If
  // abstime == 0 use "blocking" <wait> semantics.  Else, if <abstime>
  // != 0 and the call times out before the condition is signaled
  // <wait> returns -1 and sets errno to ETIME.

  int wait (void);
  // Block on condition.

  int wait (ACE_Thread_Mutex &mutex, const ACE_Time_Value *abstime = 0);
  // Block on condition or until absolute time-of-day has passed.  If
  // abstime == 0 use "blocking" wait() semantics on the <mutex>
  // passed as a parameter (this is useful if you need to store the
  // <Condition> in shared memory).  Else, if <abstime> != 0 and the
  // call times out before the condition is signaled <wait> returns -1
  // and sets errno to ETIME.

  int signal (void);
  // Signal one waiting thread.

  int broadcast (void);
  // Signal *all* waiting threads.

  ACE_Thread_Mutex &mutex (void);
  // Returns a reference to the underlying mutex_;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_cond_t cond_;
  // Condition variable.

  ACE_Thread_Mutex &mutex_;
  // Reference to mutex lock.

  int removed_;
  // Keeps track of whether <remove> has been called yet to avoid
  // multiple <remove> calls, e.g., explicitly and implicitly in the
  // destructor.  This flag isn't protected by a lock, so make sure
  // that you don't have multiple threads simultaneously calling
  // <remove> on the same object, which is a bad idea anyway...

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Condition_Thread_Mutex &);
  ACE_Condition_Thread_Mutex (const ACE_Condition_Thread_Mutex &);
};

class ACE_Export ACE_Recursive_Thread_Mutex
{
  // = TITLE
  //     Implement a C++ wrapper that allows nested acquisition and
  //     release of a mutex that occurs in the same thread.
public:
  ACE_Recursive_Thread_Mutex (LPCTSTR name = 0,
                              void *arg = 0);
  // Initialize a recursive mutex.

  ~ACE_Recursive_Thread_Mutex (void);
  // Implicitly release a recursive mutex.

  int remove (void);
  // Implicitly release a recursive mutex.

  int acquire (void);
  // Acquire a recursive mutex (will increment the nesting level and
  // not deadmutex if the owner of the mutex calls this method more
  // than once).

  int tryacquire (void);
  // Conditionally acquire a recursive mutex (i.e., won't block).
  // Returns -1 on failure.  If we "failed" because someone else
  // already had the lock, <errno> is set to <EBUSY>.

  int acquire_read (void);
  // Acquire mutex ownership.  This calls <acquire> and is only
  // here to make the <ACE_Recusive_Thread_Mutex> interface consistent
  // with the other synchronization APIs.

  int acquire_write (void);
  // Acquire mutex ownership.  This calls <acquire> and is only
  // here to make the <ACE_Recusive_Thread_Mutex> interface consistent
  // with the other synchronization APIs.

  int tryacquire_read (void);
  // Conditionally acquire mutex (i.e., won't block).  This calls
  // <tryacquire> and is only here to make the
  // <ACE_Recusive_Thread_Mutex> interface consistent with the other
  // synchronization APIs.  Returns -1 on failure.  If we "failed"
  // because someone else already had the lock, <errno> is set to
  // <EBUSY>.

  int tryacquire_write (void);
  // Conditionally acquire mutex (i.e., won't block).  This calls
  // <tryacquire> and is only here to make the
  // <ACE_Recusive_Thread_Mutex> interface consistent with the other
  // synchronization APIs.  Returns -1 on failure.  If we "failed"
  // because someone else already had the lock, <errno> is set to
  // <EBUSY>.

  int release (void);
  // Releases a recursive mutex (will not release mutex until all the
  // nesting level drops to 0, which means the mutex is no longer
  // held).

  ACE_thread_t get_thread_id (void);
  // Return the id of the thread that currently owns the mutex.

  int get_nesting_level (void);
  // Return the nesting level of the recursion.  When a thread has
  // acquired the mutex for the first time, the nesting level == 1.
  // The nesting level is incremented every time the thread acquires
  // the mutex recursively.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = This method should *not* be public (they hold no locks...)
  void set_thread_id (ACE_thread_t t);

  ACE_recursive_thread_mutex_t recursive_mutex_;
  // Recursive mutex.

  int removed_;
  // Keeps track of whether <remove> has been called yet to avoid
  // multiple <remove> calls, e.g., explicitly and implicitly in the
  // destructor.  This flag isn't protected by a lock, so make sure
  // that you don't have multiple threads simultaneously calling
  // <remove> on the same object, which is a bad idea anyway...

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Recursive_Thread_Mutex &);
  ACE_Recursive_Thread_Mutex (const ACE_Recursive_Thread_Mutex &);
};

class ACE_Export ACE_RW_Thread_Mutex : public ACE_RW_Mutex
{
  // = TITLE
  //     Wrapper for readers/writer locks that exist within a process.
public:
  ACE_RW_Thread_Mutex (LPCTSTR name = 0,
                       void *arg = 0);

  ~ACE_RW_Thread_Mutex (void);
  // Default dtor.

  int tryacquire_write_upgrade (void);
  // Conditionally upgrade a read lock to a write lock.  This only
  // works if there are no other readers present, in which case the
  // method returns 0.  Otherwise, the method returns -1 and sets
  // <errno> to <EBUSY>.  Note that the caller of this method *must*
  // already possess this lock as a read lock (but this condition is
  // not checked by the current implementation).

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_Thread_Semaphore : public ACE_Semaphore
{
  // = TITLE
  //     Wrapper for Dijkstra style general semaphores that work
  //     only within one process.
public:
  ACE_Thread_Semaphore (u_int count = 1, // By default make this unlocked.
                        LPCTSTR name = 0,
                        void * = 0,
                        int max = 0x7FFFFFFF);
  // Initialize the semaphore, with an initial value of <count>,
  // maximum value of <max>, and unlocked by default.

  ~ACE_Thread_Semaphore (void);
  // Default dtor.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

struct ACE_Export ACE_Sub_Barrier
{
  // = Initialization.
  ACE_Sub_Barrier (u_int count,
                   ACE_Thread_Mutex &lock,
                   LPCTSTR name = 0,
                   void *arg = 0);

  ~ACE_Sub_Barrier (void);

  ACE_Condition_Thread_Mutex barrier_finished_;
  // True if this generation of the barrier is done.

  int running_threads_;
  // Number of threads that are still running.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_Barrier
{
  // = TITLE
  //     Implements "barrier synchronization".
  //
  // = DESCRIPTION
  //     This class allows <count> number of threads to synchronize
  //     their completion of (one round of) a task, which is known as
  //     "barrier synchronization".  The implementation uses a
  //     "sub-barrier generation numbering" scheme to avoid overhead
  //     and to ensure that all threads wait to leave the barrier
  //     correct.  This code is based on an article from SunOpsis
  //     Vol. 4, No. 1 by Richard Marejka
  //     (Richard.Marejka@canada.sun.com).
public:
  ACE_Barrier (u_int count,
               LPCTSTR name = 0,
               void *arg = 0);
  // Initialize the barrier to synchronize <count> threads.

  ~ACE_Barrier (void);
  // Default dtor.

  int wait (void);
  // Block the caller until all <count> threads have called <wait> and
  // then allow all the caller threads to continue in parallel.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Thread_Mutex lock_;
  // Serialize access to the barrier state.

  int current_generation_;
  // Either 0 or 1, depending on whether we are the first generation
  // of waiters or the next generation of waiters.

  int count_;
  // Total number of threads that can be waiting at any one time.

  ACE_Sub_Barrier sub_barrier_1_;
  ACE_Sub_Barrier sub_barrier_2_;
  ACE_Sub_Barrier *sub_barrier_[2];
  // We keep two <sub_barriers>, one for the first "generation" of
  // waiters, and one for the next "generation" of waiters.  This
  // efficiently solves the problem of what to do if all the first
  // generation waiters don't leave the barrier before one of the
  // threads calls wait() again (i.e., starts up the next generation
  // barrier).

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Barrier &);
  ACE_Barrier (const ACE_Barrier &);
};

#if 0
// The following two classes are commented out since there doesn't
// appear to be a portable and robust means of implementing this
// functionality across platforms.

class ACE_Export ACE_Process_Condition
{
  // = TITLE
  //     ACE_Condition variable wrapper that works across processes.
public:
  ACE_Process_Condition (MUTEX &m, LPCTSTR name = 0, void *arg = 0);

  void dump (void) const;
  // Dump the state of an object.

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};
#endif /* 0 */

#if 0
class ACE_Export ACE_Process_Barrier : public ACE_Barrier
{
  // = TITLE
  //     Implements "barrier synchronization" using ACE_Process_Mutexes!
  //
  // = DESCRIPTION
  //     This class is just a simple wrapper for ACE_Barrier that
  //     selects the USYNC_PROCESS variant for the locks.
public:
  ACE_Process_Barrier (u_int count, LPCTSTR name = 0);
  // Create a Process_Barrier, passing in the optional <name>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};
#endif /* 0 */

class ACE_Export ACE_Thread_Barrier : public ACE_Barrier
{
  // = TITLE
  //     Implements "barrier synchronization" using ACE_Thread_Mutexes!
  //
  // = DESCRIPTION
  //     This class is just a simple wrapper for ACE_Barrier that
  //     selects the USYNC_THREAD variant for the locks.
public:
  ACE_Thread_Barrier (u_int count, LPCTSTR name = 0);
  // Create a Thread_Barrier, passing in the optional <name>.

  ~ACE_Thread_Barrier (void);
  // Default dtor.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};
#endif /* ACE_HAS_THREADS */

#if defined (__ACE_INLINE__)
#include "ace/Synch.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "ace/Synch_T.h"

template <class ACE_LOCK>
class ACE_Guard;

class ACE_Export ACE_Guard<ACE_Null_Mutex>
{
  // = TITLE
  //     Template specialization of <ACE_Guard> for the
  //     <ACE_Null_Mutex>.
  //
  // = DESCRIPTION
  //     This specialization is useful since it helps to speedup
  //     performance of the "Null_Mutex" considerably.
public:
  // = Initialization and termination methods.
  ACE_Guard (ACE_Null_Mutex &) {}
  ACE_Guard (ACE_Null_Mutex &, int) {}
  virtual ~ACE_Guard (void) {}

  int acquire (void) { return 0; }
  int tryacquire (void) { return 0; }
  int release (void) { return 0; }
  int locked (void) { return 1; }
  int remove (void) { return 0; }
  void dump (void) const {}

private:
  // = Prevent assignment and initialization.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Guard<ACE_Null_Mutex> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Guard (const ACE_Guard<ACE_Null_Mutex> &))
};

template <class ACE_LOCK>
class ACE_Write_Guard;

class ACE_Export ACE_Write_Guard<ACE_Null_Mutex> : public ACE_Guard<ACE_Null_Mutex>
{
  // = TITLE
public:
  ACE_Write_Guard (ACE_Null_Mutex &m)
    : ACE_Guard<ACE_Null_Mutex> (m) {}
  ACE_Write_Guard (ACE_Null_Mutex &m, int blocked)
    : ACE_Guard<ACE_Null_Mutex> (m, blocked) {}

  int acquire_write (void) { return 0; }
  int acquire (void) { return 0; }
  int tryacquire_write (void) { return 0; }
  int tryacquire (void) { return 0; }
  void dump (void) const {}
};

template <class ACE_LOCK>
class ACE_Read_Guard;

class ACE_Export ACE_Read_Guard<ACE_Null_Mutex> : public ACE_Guard<ACE_Null_Mutex>
{
  // = TITLE
public:
  ACE_Read_Guard (ACE_Null_Mutex &m)
    : ACE_Guard<ACE_Null_Mutex> (m) {}
  ACE_Read_Guard (ACE_Null_Mutex &m, int blocked)
    : ACE_Guard<ACE_Null_Mutex> (m, blocked) {}

  int acquire_write (void) { return 0; }
  int acquire (void) { return 0; }
  int tryacquire_write (void) { return 0; }
  int tryacquire (void) { return 0; }
  void dump (void) const {}
};

#endif /* ACE_SYNCH_H */
