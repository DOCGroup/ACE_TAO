/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Synch.h
 *
 *  $Id$
 *
 *   Wrapper Facades for various synchronization mechanisms.
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_SYNCH_H
#define ACE_SYNCH_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations.
/**
 * @class ACE_Time_Value;
 template <class ACE_COND_MUTEX> class ACE_Condition;
 */
class ACE_Time_Value;

/**
 * @class ACE_Lock
 *
 * @brief This is the abstract base class that contains the uniform
 * locking API that is supported by all the ACE synchronization
 * mechanisms.
 *
 * This class is typically used in conjunction with the
 * <ACE_Lock_Adapter> in order to provide a polymorphic
 * interface to the ACE synchronization mechanisms (e.g.,
 * <ACE_Mutex>, <ACE_Semaphore>, <ACE_RW_Mutex>, etc).  Note that
 * the reason that all of ACE doesn't use polymorphic locks is
 * that (1) they add ~20% extra overhead for virtual function
 * calls and (2) objects with virtual functions can't be placed
 * into shared memory.
 */
class ACE_Export ACE_Lock
{
public:
  /// CE needs a default ctor here.
  ACE_Lock (void);

  /// Noop virtual destructor
  virtual ~ACE_Lock (void);

  /**
   * Explicitly destroy the lock.  Note that only one thread should
   * call this method since it doesn't protect against race
   * conditions.
   */
  virtual int remove (void) = 0;

  /// Block the thread until the lock is acquired.  Returns -1 on
  /// failure.
  virtual int acquire (void) = 0;

  /**
   * Conditionally acquire the lock (i.e., won't block).  Returns -1
   * on failure.  If we "failed" because someone else already had the
   * lock, <errno> is set to <EBUSY>.
   */
  virtual int tryacquire (void) = 0;

  /// Release the lock.  Returns -1 on failure.
  virtual int release (void) = 0;

  /**
   * Block until the thread acquires a read lock.  If the locking
   * mechanism doesn't support read locks then this just calls
   * <acquire>.  Returns -1 on failure.
   */
  virtual int acquire_read (void) = 0;

  /**
   * Block until the thread acquires a write lock.  If the locking
   * mechanism doesn't support read locks then this just calls
   * <acquire>.  Returns -1 on failure.
   */
  virtual int acquire_write (void) = 0;

  /**
   * Conditionally acquire a read lock.  If the locking mechanism
   * doesn't support read locks then this just calls <acquire>.
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, <errno> is set to <EBUSY>.
   */
  virtual int tryacquire_read (void) = 0;

  /**
   * Conditionally acquire a write lock.  If the locking mechanism
   * doesn't support read locks then this just calls <acquire>.
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, <errno> is set to <EBUSY>.
   */
  virtual int tryacquire_write (void) = 0;

  /**
   * Conditionally try to upgrade a lock held for read to a write lock.
   * If the locking mechanism doesn't support read locks then this just
   * calls <acquire>. Returns 0 on success, -1 on failure.
   */
  virtual int tryacquire_write_upgrade (void) = 0;
};

/**
 * @class ACE_Adaptive_Lock
 *
 * @brief An adaptive general locking class that defers the decision of
 * lock type to run time.
 *
 * This class, as ACE_Lock, provide a set of general locking APIs.
 * However, it defers our decision of what kind of lock to use
 * to the run time and delegates all locking operations to the actual
 * lock.  Users must define a constructor in their subclass to
 * initialize <lock_>.
 */
class ACE_Export ACE_Adaptive_Lock : public ACE_Lock
{
public:
  /// You must also override the destructor function to match with how
  /// you construct the underneath <lock_>.
  virtual ~ACE_Adaptive_Lock (void);

  // = Lock/unlock operations.

  virtual int remove (void);
  virtual int acquire (void);
  virtual int tryacquire (void);
  virtual int release (void);
  virtual int acquire_read (void);
  virtual int acquire_write (void);
  virtual int tryacquire_read (void);
  virtual int tryacquire_write (void);
  virtual int tryacquire_write_upgrade (void);
  void dump (void) const;

protected:
  /**
   * Create and initialize create the actual lcok used in the class.
   * The default constructor simply set the <lock_> to 0 (null).  You
   * must overwrite this method for this class to work.
   */
  ACE_Adaptive_Lock (void);

  ACE_Lock *lock_;
};

/**
 * @class ACE_Semaphore
 *
 * @brief Wrapper for Dijkstra style general semaphores.
 */
class ACE_Export ACE_Semaphore
{
public:
  // = Initialization and termination.
  /// Initialize the semaphore, with initial value of "count".
  ACE_Semaphore (u_int count = 1, // By default make this unlocked.
                 int type = USYNC_THREAD,
                 const ACE_TCHAR *name = 0,
                 void * = 0,
                 int max = 0x7fffffff);

  /// Implicitly destroy the semaphore.
  ~ACE_Semaphore (void);

  /**
   * Explicitly destroy the semaphore.  Note that only one thread
   * should call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /// Block the thread until the semaphore count becomes
  /// greater than 0, then decrement it.
  int acquire (void);

  /**
   * Block the thread until the semaphore count becomes greater than 0
   * (at which point it is decremented) or until <tv> times out (in
   * which case -1 is returned and <errno> == <ETIME>).  Note that <tv>
   * is assumed to be in "absolute" rather than "relative" time.  The
   * value of <tv> is updated upon return to show the actual
   * (absolute) acquisition time.
   *
   * NOTE: Solaris threads do not support timed semaphores.
   * Therefore, if you're running on Solaris you might want to
   * consider using the ACE POSIX pthreads implementation instead,
   * which can be enabled by compiling ACE with
   * -DACE_HAS_PTHREADS, rather than -DACE_HAS_STHREADS or
   * -DACE_HAS_POSIX_SEM. */  
  int acquire (ACE_Time_Value &tv);

  /**
   * If <tv> == 0 then call <acquire()> directly.  Otherwise, Block
   * the thread until the semaphore count becomes greater than 0 
   * (at which point it is decremented) or until <tv> times out (in
   * which case -1 is returned and <errno> == <ETIME>).  Note that
   * <*tv> is assumed to be in "absolute" rather than "relative" time.
   * The value of <*tv> is updated upon return to show the actual
   * (absolute) acquisition time. 
   *
   * NOTE: Solaris threads do not support timed semaphores.
   * Therefore, if you're running on Solaris you might want to
   * consider using the ACE POSIX pthreads implementation instead,
   * which can be enabled by compiling ACE with
   * -DACE_HAS_PTHREADS, rather than -DACE_HAS_STHREADS or 
   * -DACE_HAS_POSIX_SEM.  */
  int acquire (ACE_Time_Value *tv);

  /**
   * Conditionally decrement the semaphore if count is greater than 0
   * (i.e., won't block).  Returns -1 on failure.  If we "failed"
   * because someone else already had the lock, <errno> is set to
   * <EBUSY>.
   */
  int tryacquire (void);

  /// Increment the semaphore by 1, potentially unblocking a waiting
  /// thread.
  int release (void);

  /// Increment the semaphore by <release_count>, potentially
  /// unblocking waiting threads.
  int release (size_t release_count);

  /**
   * Acquire semaphore ownership.  This calls <acquire> and is only
   * here to make the <ACE_Semaphore> interface consistent with the
   * other synchronization APIs.
   */
  int acquire_read (void);

  /**
   * Acquire semaphore ownership.  This calls <acquire> and is only
   * here to make the <ACE_Semaphore> interface consistent with the
   * other synchronization APIs.
   */
  int acquire_write (void);

  /**
   * Conditionally acquire semaphore (i.e., won't block).  This calls
   * <tryacquire> and is only here to make the <ACE_Semaphore>
   * interface consistent with the other synchronization APIs.
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, <errno> is set to <EBUSY>.
   */
  int tryacquire_read (void);

  /**
   * Conditionally acquire semaphore (i.e., won't block).  This calls
   * <tryacquire> and is only here to make the <ACE_Semaphore>
   * interface consistent with the other synchronization APIs.
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, <errno> is set to <EBUSY>.
   */
  int tryacquire_write (void);

  /**
   * This is only here to make the <ACE_Semaphore>
   * interface consistent with the other synchronization APIs.
   * Assumes the caller has already acquired the semaphore using one of
   * the above calls, and returns 0 (success) always.
   */
  int tryacquire_write_upgrade (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /// Return the underlying lock.
  const ACE_sema_t &lock (void) const;

protected:
  ACE_sema_t semaphore_;

  /// Keeps track of whether <remove> has been called yet to avoid
  /// multiple <remove> calls, e.g., explicitly and implicitly in the
  /// destructor.  This flag isn't protected by a lock, so make sure
  /// that you don't have multiple threads simultaneously calling
  /// <remove> on the same object, which is a bad idea anyway...
  int removed_;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Semaphore &);
  ACE_Semaphore (const ACE_Semaphore &);
};

/**
 * @class ACE_Null_Semaphore
 *
 * @brief Implement a do nothing <ACE_Semaphore>, i.e., all the methods are
 * no ops.
 *
 * Although the methods are no-ops, the return values are different for
 * the blocking as opposed to timed acquires.  The blocking version of
 * acquire() is often used to serialize access to a critical section,
 * whereas the timed version is often used to wait for another thread
 * to update some condition or change some shared state.  When using an
 * ACE_Null_Semaphore, however, there's no other thread involved to
 * change a state or condition (otherwise, a null semaphore would be
 * inappropriate).  Returning an error value signifies that the
 * state or condition has not been (and can't be) changed, which is
 * consistent with the behavior of the threaded case where a timeout
 * occurs before the state or condition is changed.
 */
class ACE_Export ACE_Null_Semaphore
{
public:
  ACE_Null_Semaphore (u_int count = 1, // By default make this unlocked.
                       int type = USYNC_THREAD,
                       const ACE_TCHAR *name = 0,
                       void * = 0,
                       int max = 0x7fffffff);
  ~ACE_Null_Semaphore (void);
  /// Return 0.
  int remove (void);

  /// Return 0.
  int acquire (void);

  /// Return -1 with <errno> == <ETIME>.
  int acquire (ACE_Time_Value &);

  /// Return -1 with <errno> == <ETIME>.
  int acquire (ACE_Time_Value *);

  /// Return 0.
  int tryacquire (void);

  /// Return 0.
  int release (void);

  /// Return 0.
  int release (size_t);

  /// Return 0.
  int acquire_write (void);

  /// Return 0.
  int tryacquire_write (void);

  /// Return 0.
  int tryacquire_write_upgrade (void);

  /// Return 0.
  int acquire_read (void);

  /// Return 0.
  int tryacquire_read (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

/**
 * @class ACE_RW_Mutex
 *
 * @brief Wrapper for readers/writer locks.
 *
 * These are most useful for applications that have many more
 * parallel readers than writers...
 */
class ACE_Export ACE_RW_Mutex
{
public:
  /// Initialize a readers/writer lock.
  ACE_RW_Mutex (int type = USYNC_THREAD,
                const ACE_TCHAR *name = 0,
                void *arg = 0);

  /// Implicitly destroy a readers/writer lock
  ~ACE_RW_Mutex (void);

  /**
   * Explicitly destroy a readers/writer lock.  Note that only one
   * thread should call this method since it doesn't protect against
   * race conditions.
   */
  int remove (void);

  /// Acquire a read lock, but block if a writer hold the lock.
  int acquire_read (void);

  /// Acquire a write lock, but block if any readers or a
  /// writer hold the lock.
  int acquire_write (void);

  /**
   * Conditionally acquire a read lock (i.e., won't block).  Returns
   * -1 on failure.  If we "failed" because someone else already had
   * the lock, <errno> is set to <EBUSY>.
   */
  int tryacquire_read (void);

  /// Conditionally acquire a write lock (i.e., won't block).
  int tryacquire_write (void);

  /**
   * Conditionally upgrade a read lock to a write lock.  This only
   * works if there are no other readers present, in which case the
   * method returns 0.  Otherwise, the method returns -1 and sets
   * <errno> to <EBUSY>.  Note that the caller of this method *must*
   * already possess this lock as a read lock (but this condition is
   * not checked by the current implementation).
   */
  int tryacquire_write_upgrade (void);

  /**
   * Note, for interface uniformity with other synchronization
   * wrappers we include the <acquire> method.  This is implemented as
   * a write-lock to safe...
   */
  int acquire (void);

  /**
   * Note, for interface uniformity with other synchronization
   * wrappers we include the <tryacquire> method.  This is implemented
   * as a write-lock to be safe...  Returns -1 on failure.  If we
   * "failed" because someone else already had the lock, <errno> is
   * set to <EBUSY>.
   */
  int tryacquire (void);

  /// Unlock a readers/writer lock.
  int release (void);

  /// Return the underlying lock.
  const ACE_rwlock_t &lock (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Readers/writer lock.
  ACE_rwlock_t lock_;

  /// Keeps track of whether <remove> has been called yet to avoid
  /// multiple <remove> calls, e.g., explicitly and implicitly in the
  /// destructor.  This flag isn't protected by a lock, so make sure
  /// that you don't have multiple threads simultaneously calling
  /// <remove> on the same object, which is a bad idea anyway...
  int removed_;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_RW_Mutex &);
  ACE_RW_Mutex (const ACE_RW_Mutex &);
};

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
             ACE_mutexattr_t *arg = 0);

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

/**
 * @class ACE_Null_Barrier
 *
 * @brief Implements "NULL barrier synchronization".
 */
class ACE_Export ACE_Null_Barrier
{
public:
  /// Initialize the barrier to synchronize <count> threads.
  ACE_Null_Barrier (u_int,
                    const char * = 0,
                    void * = 0);

  /// Default dtor.
  ~ACE_Null_Barrier (void);

  /// Block the caller until all <count> threads have called <wait> and
  /// then allow all the caller threads to continue in parallel.
  int wait (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Null_Barrier &);
  ACE_Null_Barrier (const ACE_Null_Barrier &);
};

/**
 * @class ACE_Null_Mutex
 *
 * @brief Implement a do nothing <ACE_Mutex>, i.e., all the methods are
 * no ops.
 */
class ACE_Export ACE_Null_Mutex
{
public:
  ACE_Null_Mutex (const ACE_TCHAR * = 0);
  ~ACE_Null_Mutex (void);
  /// Return 0.
  int remove (void);

  /// Return 0.
  int acquire (void);

  /// Return -1 with <errno> == <ETIME>.
  int acquire (ACE_Time_Value &timeout);

  /// Return -1 with <errno> == <ETIME>.
  int acquire (ACE_Time_Value *timeout);

  /// Return 0.
  int tryacquire (void);

  /// Return 0.
  int release (void);

  /// Return 0.
  int acquire_write (void);

  /// Return 0.
  int tryacquire_write (void);

  /// Return 0.
  int tryacquire_write_upgrade (void);

  /// Return 0.
  int acquire_read (void);

  /// Return 0.
  int tryacquire_read (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

class ACE_Export ACE_Noop_Token : public ACE_Null_Mutex
{
public:
  /// Queueing strategy
  enum QUEUEING_STRATEGY
  {
    FIFO = -1,
    LIFO = 0
  };

  /// Get queueing strategy.
  int queueing_strategy (void);

  /// Set queueing strategy.
  void queueing_strategy (int queueing_strategy);

  int renew (int = 0, ACE_Time_Value * =0);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

/**
 * @class ACE_Null_Condition
 *
 * @brief Implement a do nothing <ACE_Condition> variable wrapper,
 * i.e., all methods are no ops.  This class is necessary since
 * some C++ compilers are *very* lame...
 */
class ACE_Export ACE_Null_Condition
{
public:
  ACE_Null_Condition (const ACE_Null_Mutex &m,
                      const ACE_TCHAR * = 0,
                      void * = 0);
  ~ACE_Null_Condition (void);

  /// Returns 0.
  int remove (void);

  /// Returns -1 with <errno> == <ETIME>.
  int wait (ACE_Time_Value * = 0);

  /// Returns 0.
  int signal (void);

  /// Returns 0.
  int broadcast (void);
  ACE_Null_Mutex &mutex (void);

  /// Dump the state of an object.
  void dump (void) const;

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
/**
 * @class ACE_Null_Mutex_Guard
 *
 * @brief This data structure is meant to be used within a method or
 * function...  It performs automatic aquisition and release of
 * an ACE_Null_Mutex.
 *
 * This class is obsolete and should be replaced by
 * ACE_Guard<ACE_Null_Mutex>.
 */
class ACE_Export ACE_Null_Mutex_Guard
{
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

/**
 * @class ACE_TSS_Adapter
 *
 * @brief This class encapsulates a TSS object and its associated
 * C++ destructor function.  It is used by the ACE_TSS...
 * methods (in Synch_T.cpp) in order to allow an extern
 * "C" cleanup routine to be used.  Needed by the "frigging"
 * MVS C++ compiler.
 *
 * Objects of this class are stored in thread specific
 * storage. ts_obj_ points to the "real" object and
 * func_ is a pointer to the C++ cleanup function for ts_obj_.
 */
class ACE_Export ACE_TSS_Adapter
{
public:
  /// Initialize the adapter.
  ACE_TSS_Adapter (void *object, ACE_THR_DEST f);

  /// Default dtor.
  ~ACE_TSS_Adapter (void);

  /// Perform the cleanup operation.
  void cleanup (void);

//private:

  /// The real TS object.
  void *ts_obj_;

  /// The real cleanup routine for ts_obj;
  ACE_THR_DEST func_;
};

/**
 * @class ACE_Event
 *
 * @brief A wrapper around the Win32 event locking mechanism.
 *
 * Portable implementation of an Event mechanism, which is native to
 * Win32, but must be emulated on UNIX.  All platforms support
 * process-scope locking support.  However, only Win32 platforms
 * support global naming and system-scope locking support.
 */
class ACE_Export ACE_Event
{
public:
  /// Constructor that creates event.
  ACE_Event (int manual_reset = 0,
             int initial_state = 0,
             int type = USYNC_THREAD,
             const ACE_TCHAR *name = 0,
             void *arg = 0);

  /// Implicitly destroy the event variable.
  ~ACE_Event (void);

  /**
   * Explicitly destroy the event variable.  Note that only one thread
   * should call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /// Underlying handle to event.
  ACE_event_t handle (void) const;

  /**
   * Set the underlying handle to event. Note that this method assumes
   * ownership of the <handle> and will close it down in <remove>.  If
   * you want the <handle> to stay open when <remove> is called make
   * sure to call <dup> on the <handle> before closing it.  You are
   * responsible for the closing the existing <handle> before
   * overwriting it.
   */
  void handle (ACE_event_t new_handle);

  /**
   * if MANUAL reset
   *    sleep till the event becomes signaled
   *    event remains signaled after wait() completes.
   * else AUTO reset
   *    sleep till the event becomes signaled
   *    event resets wait() completes.
   */
  int wait (void);

  /// Same as wait() above, but this one can be timed
  /// <abstime> is absolute time-of-day if if <use_absolute_time> 
  /// is non-0, else it is relative time.
  int wait (const ACE_Time_Value *abstime, 
            int use_absolute_time = 1);

  /**
   * if MANUAL reset
   *    wake up all waiting threads
   *    set to signaled state
   * else AUTO reset
   *    if no thread is waiting, set to signaled state
   *    if thread(s) are waiting, wake up one waiting thread and
   *    reset event
   */
  int signal (void);

  /**
   * if MANUAL reset
   *    wakeup all waiting threads and
   *    reset event
   * else AUTO reset
   *    wakeup one waiting thread (if present) and
   *    reset event
   */
  int pulse (void);

  /// Set to nonsignaled state.
  int reset (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// The underlying handle.
  ACE_event_t handle_;

  /// Keeps track of whether <remove> has been called yet to avoid
  /// multiple <remove> calls, e.g., explicitly and implicitly in the
  /// destructor.  This flag isn't protected by a lock, so make sure
  /// that you don't have multiple threads simultaneously calling
  /// <remove> on the same object, which is a bad idea anyway...
  int removed_;

private:
  // = Prevent copying.
  ACE_Event (const ACE_Event& event);
  const ACE_Event &operator= (const ACE_Event &rhs);
};

/**
 * @class ACE_Manual_Event
 *
 * @brief Manual Events.
 *
 * Specialization of Event mechanism which wakes up all waiting
 * thread on <signal>.  All platforms support process-scope locking
 * support.  However, only Win32 platforms support global naming and
 * system-scope locking support. 
 */
class ACE_Export ACE_Manual_Event : public ACE_Event
{
public:
  /// constructor which will create manual event
  ACE_Manual_Event (int initial_state = 0,
                    int type = USYNC_THREAD,
                    const char *name = 0,
                    void *arg = 0);

#if defined (ACE_HAS_WCHAR)
  /// constructor which will create manual event (wchar_t version)
  ACE_Manual_Event (int initial_state,
                    int type,
                    const wchar_t *name,
                    void *arg = 0);
#endif /* ACE_HAS_WCHAR */

  /// Default dtor.
  ~ACE_Manual_Event (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks
  ACE_ALLOC_HOOK_DECLARE;
};

/**
 * @class ACE_Auto_Event
 *
 * @brief Auto Events.
 *
 * Specialization of Event mechanism which wakes up one waiting
 * thread on <signal>.  All platforms support process-scope locking
 * support.  However, only Win32 platforms support global naming and
 * system-scope locking support. 
 */
class ACE_Export ACE_Auto_Event : public ACE_Event
{
public:
  /// constructor which will create auto event
  ACE_Auto_Event (int initial_state = 0,
                  int type = USYNC_THREAD,
                  const char *name = 0,
                  void *arg = 0);

#if defined (ACE_HAS_WCHAR)
  /// constructor which will create auto event (wchar_t version)
  ACE_Auto_Event (int initial_state,
                  int type,
                  const wchar_t *name,
                  void *arg = 0);
#endif /* ACE_HAS_WCHAR */

  /// Default dtor.
  ~ACE_Auto_Event (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks
  ACE_ALLOC_HOOK_DECLARE;
};

// ACE platform supports some form of threading.
#if !defined (ACE_HAS_THREADS)
/**
 * @class ACE_Barrier
 *
 * @brief This is a no-op to make ACE "syntactically consistent."
 */
class ACE_Barrier
{
public:
  ACE_Barrier (u_int, const ACE_TCHAR * = 0, void * = 0) {}
  ~ACE_Barrier (void) {}
  int wait (void) { ACE_NOTSUP_RETURN (-1); }
  void dump (void) const {}
};
#else
  /**
   * @class ACE_Thread_Mutex
   *
   * @brief ACE_Thread_Mutex wrapper (only valid for threads in the same
   * process).
   *
   * This implementation is optimized for locking threads that are
   * in the same process.  It maps to <CRITICAL_SECTION>s on NT
   * and <ACE_mutex_t> with <type> set to <USYNC_THREAD> on UNIX.
   * ACE_Thread_Mutex is recursive on some platforms (like
   * Win32). However, on most platforms (like Solaris) it is not
   * recursive.  To be totally safe and portable, developers
   * should use <ACE_Recursive_Thread_Mutex> when they need a
   * recursive mutex.
   */
class ACE_Export ACE_Thread_Mutex
{
  friend class ACE_Condition_Thread_Mutex;
public:
  /// Constructor.
  ACE_Thread_Mutex (const ACE_TCHAR *name = 0,
                    ACE_mutexattr_t *attributes = 0);

  /// Implicitly destroy the mutex.
  ~ACE_Thread_Mutex (void);

  /**
   * Explicitly destroy the mutex.  Note that only one thread should
   * call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /// Acquire lock ownership (wait on queue if necessary).
  int acquire (void);

  /**
   * Block the thread until we acquire the mutex or until <tv> times
   * out, in which case -1 is returned with <errno> == <ETIME>.  Note
   * that <tv> is assumed to be in "absolute" rather than "relative"
   * time.  The value of <tv> is updated upon return to show the
   * actual (absolute) acquisition time.
   */
  int acquire (ACE_Time_Value &tv);

  /**
   * If <tv> == 0 the call <acquire()> directly.  Otherwise, Block the
   * thread until we acquire the mutex or until <tv> times out, in
   * which case -1 is returned with <errno> == <ETIME>.  Note that
   * <*tv> is assumed to be in "absolute" rather than "relative" time.
   * The value of <*tv> is updated upon return to show the actual
   * (absolute) acquisition time.  
   */
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
   * Acquire mutex ownership.  This calls <acquire> and is only here
   * to make the <ACE_Thread_Mutex> interface consistent with the
   * other synchronization APIs.
   */
  int acquire_read (void);

  /**
   * Acquire mutex ownership.  This calls <acquire> and is only here
   * to make the <ACE_Thread_Mutex> interface consistent with the
   * other synchronization APIs.
   */
  int acquire_write (void);

  /**
   * Conditionally acquire mutex (i.e., won't block).  This calls
   * <tryacquire> and is only here to make the <ACE_Thread_Mutex>
   * interface consistent with the other synchronization APIs.
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, <errno> is set to <EBUSY>.
   */
  int tryacquire_read (void);

  /**
   * Conditionally acquire mutex (i.e., won't block).  This calls
   * <tryacquire> and is only here to make the <ACE_Thread_Mutex>
   * interface consistent with the other synchronization APIs.
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, <errno> is set to <EBUSY>.
   */
  int tryacquire_write (void);

  /**
   * This is only here to make the <ACE_Thread_Mutex>
   * interface consistent with the other synchronization APIs.
   * Assumes the caller has already acquired the mutex using one of
   * the above calls, and returns 0 (success) always.
   */
  int tryacquire_write_upgrade (void);

  /// Return the underlying mutex.
  const ACE_thread_mutex_t &lock (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  // protected:
  /// Mutex type that supports single-process locking efficiently.
  ACE_thread_mutex_t lock_;

  /// Keeps track of whether <remove> has been called yet to avoid
  /// multiple <remove> calls, e.g., explicitly and implicitly in the
  /// destructor.  This flag isn't protected by a lock, so make sure
  /// that you don't have multiple threads simultaneously calling
  /// <remove> on the same object, which is a bad idea anyway...
  int removed_;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Thread_Mutex &);
  ACE_Thread_Mutex (const ACE_Thread_Mutex &);
};

#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
/**
 * @class ACE_Thread_Mutex_Guard
 *
 * @brief This data structure is meant to be used within a method or
 * function...  It performs automatic aquisition and release of
 * an <ACE_Thread_Mutex>.
 *
 * This class is obsolete and should be replaced by
 * ACE_Guard<ACE_Thread_Mutex>.
 */
class ACE_Export ACE_Thread_Mutex_Guard
{
public:
  /// Implicitly and automatically acquire the lock.
  ACE_Thread_Mutex_Guard (ACE_Thread_Mutex &m, int block = 1);

  /// Implicitly release the lock.
  ~ACE_Thread_Mutex_Guard (void);

  /// 1 if locked, 0 if couldn't acquire the lock (errno will contain
  /// the reason for this).
  int locked (void);

  /**
   * Explicitly release the lock.  Note that only one thread should
   * call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /// Explicitly acquire the lock.
  int acquire (void);

  /**
   * Conditionally acquire the lock (i.e., won't block).  Returns -1
   * on failure.  If we "failed" because someone else already had the
   * lock, <errno> is set to <EBUSY>.
   */
  int tryacquire (void);

  /// Explicitly release the lock.
  int release (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Reference to the mutex.
  ACE_Thread_Mutex &lock_;

  /// Keeps track of whether we acquired the lock or failed.
  int owner_;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Thread_Mutex_Guard &);
  ACE_Thread_Mutex_Guard (const ACE_Thread_Mutex_Guard &);
};
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */

class ACE_Export ACE_Condition_Attributes
{
public:
  /// Constructor
  ACE_Condition_Attributes (int type = ACE_DEFAULT_SYNCH_TYPE);

  /// Destructor
  ~ACE_Condition_Attributes (void);

private:
  friend class ACE_Condition_Thread_Mutex;

  /// The attributes
  ACE_condattr_t attributes_;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Condition_Attributes &);
  ACE_Condition_Attributes (const ACE_Condition_Attributes &);
};

/**
 * @class ACE_Condition_Thread_Mutex
 *
 * @brief ACE_Condition variable wrapper written using ACE_Mutexes This
 * allows threads to block until shared data changes state.
 * A condition variable enables threads to atomically block and
 * test the condition under the protection of a mutual exclu-
 * sion lock (mutex) until the condition is satisfied.  That is,
 * the mutex must have been held by the thread before calling
 * wait or signal on the condition.  If the condition is false,
 * a thread blocks on a condition variable and atomically
 * releases the mutex that is waiting for the condition to
 * change.  If another thread changes the condition, it may wake
 * up waiting threads by signaling the associated condition
 * variable.  The waiting threads, upon awakening, reacquire the
 * mutex and re-evaluate the condition.
 *
 * This should be an instantiation of ACE_Condition but problems
 * with compilers precludes this...
 */
class ACE_Export ACE_Condition_Thread_Mutex
{
public:
  /// Initialize the condition variable.
  ACE_Condition_Thread_Mutex (const ACE_Thread_Mutex &m,
                              const ACE_TCHAR *name = 0,
                              void *arg = 0);

  /// Initialize the condition variable.
  ACE_Condition_Thread_Mutex (const ACE_Thread_Mutex &m,
                              ACE_Condition_Attributes &attributes,
                              const ACE_TCHAR *name = 0,
                              void *arg = 0);

  /// Implicitly destroy the condition variable.
  ~ACE_Condition_Thread_Mutex (void);

  /**
   * Explicitly destroy the condition variable.  Note that only one
   * thread should call this method since it doesn't protect against
   * race conditions.
   */
  int remove (void);

  /**
   * Block on condition, or until absolute time-of-day has passed.  If
   * abstime == 0 use "blocking" <wait> semantics.  Else, if <abstime>
   * != 0 and the call times out before the condition is signaled
   * <wait> returns -1 and sets errno to ETIME.
   */
  int wait (const ACE_Time_Value *abstime);

  /// Block on condition.
  int wait (void);

  /**
   * Block on condition or until absolute time-of-day has passed.  If
   * abstime == 0 use "blocking" wait() semantics on the <mutex>
   * passed as a parameter (this is useful if you need to store the
   * <Condition> in shared memory).  Else, if <abstime> != 0 and the
   * call times out before the condition is signaled <wait> returns -1
   * and sets errno to ETIME.
   */
  int wait (ACE_Thread_Mutex &mutex, const ACE_Time_Value *abstime = 0);

  /// Signal one waiting thread.
  int signal (void);

  /// Signal *all* waiting threads.
  int broadcast (void);

  /// Returns a reference to the underlying mutex_;
  ACE_Thread_Mutex &mutex (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Condition variable.
  ACE_cond_t cond_;

  /// Reference to mutex lock.
  ACE_Thread_Mutex &mutex_;

  /// Keeps track of whether <remove> has been called yet to avoid
  /// multiple <remove> calls, e.g., explicitly and implicitly in the
  /// destructor.  This flag isn't protected by a lock, so make sure
  /// that you don't have multiple threads simultaneously calling
  /// <remove> on the same object, which is a bad idea anyway...
  int removed_;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Condition_Thread_Mutex &);
  ACE_Condition_Thread_Mutex (const ACE_Condition_Thread_Mutex &);
};

/**
 * @class ACE_Recursive_Thread_Mutex
 *
 * @brief Implement a C++ wrapper that allows nested acquisition and
 * release of a mutex that occurs in the same thread.
 */
class ACE_Export ACE_Recursive_Thread_Mutex
{
public:
  /// Initialize a recursive mutex.
  ACE_Recursive_Thread_Mutex (const ACE_TCHAR *name = 0,
                              ACE_mutexattr_t *arg = 0);

  /// Implicitly release a recursive mutex.
  ~ACE_Recursive_Thread_Mutex (void);

  /**
   * Implicitly release a recursive mutex.  Note that only one thread
   * should call this method since it doesn't protect against race
   * conditions.
   */
  int remove (void);

  /**
   * Acquire a recursive mutex (will increment the nesting level and
   * not deadmutex if the owner of the mutex calls this method more
   * than once).
   */
  int acquire (void);

  /**
   * Conditionally acquire a recursive mutex (i.e., won't block).
   * Returns -1 on failure.  If we "failed" because someone else
   * already had the lock, <errno> is set to <EBUSY>.
   */
  int tryacquire (void);

  /**
   * Acquire mutex ownership.  This calls <acquire> and is only
   * here to make the <ACE_Recusive_Thread_Mutex> interface consistent
   * with the other synchronization APIs.
   */
  int acquire_read (void);

  /**
   * Acquire mutex ownership.  This calls <acquire> and is only
   * here to make the <ACE_Recusive_Thread_Mutex> interface consistent
   * with the other synchronization APIs.
   */
  int acquire_write (void);

  /**
   * Conditionally acquire mutex (i.e., won't block).  This calls
   * <tryacquire> and is only here to make the
   * <ACE_Recusive_Thread_Mutex> interface consistent with the other
   * synchronization APIs.  Returns -1 on failure.  If we "failed"
   * because someone else already had the lock, <errno> is set to
   * <EBUSY>.
   */
  int tryacquire_read (void);

  /**
   * Conditionally acquire mutex (i.e., won't block).  This calls
   * <tryacquire> and is only here to make the
   * <ACE_Recusive_Thread_Mutex> interface consistent with the other
   * synchronization APIs.  Returns -1 on failure.  If we "failed"
   * because someone else already had the lock, <errno> is set to
   * <EBUSY>.
   */
  int tryacquire_write (void);

  /**
   * This is only here to make the <ACE_Recursive_Thread_Mutex>
   * interface consistent with the other synchronization APIs.
   * Assumes the caller has already acquired the mutex using one of
   * the above calls, and returns 0 (success) always.
   */
  int tryacquire_write_upgrade (void);

  /**
   * Releases a recursive mutex (will not release mutex until all the
   * nesting level drops to 0, which means the mutex is no longer
   * held).
   */
  int release (void);

  /// Return the id of the thread that currently owns the mutex.
  ACE_thread_t get_thread_id (void);

  /**
   * Return the nesting level of the recursion.  When a thread has
   * acquired the mutex for the first time, the nesting level == 1.
   * The nesting level is incremented every time the thread acquires
   * the mutex recursively.
   */
  int get_nesting_level (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = This method should *not* be public (they hold no locks...)
  void set_thread_id (ACE_thread_t t);

  /// Recursive mutex.
  ACE_recursive_thread_mutex_t recursive_mutex_;

  /// Keeps track of whether <remove> has been called yet to avoid
  /// multiple <remove> calls, e.g., explicitly and implicitly in the
  /// destructor.  This flag isn't protected by a lock, so make sure
  /// that you don't have multiple threads simultaneously calling
  /// <remove> on the same object, which is a bad idea anyway...
  int removed_;

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Recursive_Thread_Mutex &);
  ACE_Recursive_Thread_Mutex (const ACE_Recursive_Thread_Mutex &);
};

/**
 * @class ACE_RW_Thread_Mutex
 *
 * @brief Wrapper for readers/writer locks that exist within a process.
 */
class ACE_Export ACE_RW_Thread_Mutex : public ACE_RW_Mutex
{
public:
  ACE_RW_Thread_Mutex (const ACE_TCHAR *name = 0,
                       void *arg = 0);

  /// Default dtor.
  ~ACE_RW_Thread_Mutex (void);

  /**
   * Conditionally upgrade a read lock to a write lock.  This only
   * works if there are no other readers present, in which case the
   * method returns 0.  Otherwise, the method returns -1 and sets
   * <errno> to <EBUSY>.  Note that the caller of this method *must*
   * already possess this lock as a read lock (but this condition is
   * not checked by the current implementation).
   */
  int tryacquire_write_upgrade (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

/**
 * @class ACE_Thread_Semaphore
 *
 * @brief Wrapper for Dijkstra style general semaphores that work
 * only within one process.
 */
class ACE_Export ACE_Thread_Semaphore : public ACE_Semaphore
{
public:
  /// Initialize the semaphore, with an initial value of <count>,
  /// maximum value of <max>, and unlocked by default.
  ACE_Thread_Semaphore (u_int count = 1, // By default make this unlocked.
                        const ACE_TCHAR *name = 0,
                        void * = 0,
                        int max = 0x7FFFFFFF);

  /// Default dtor.
  ~ACE_Thread_Semaphore (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

struct ACE_Export ACE_Sub_Barrier
{
  // = Initialization.
  ACE_Sub_Barrier (u_int count,
                   ACE_Thread_Mutex &lock,
                   const ACE_TCHAR *name = 0,
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

/**
 * @class ACE_Barrier
 *
 * @brief Implements "barrier synchronization".
 *
 * This class allows <count> number of threads to synchronize
 * their completion of (one round of) a task, which is known as
 * "barrier synchronization".  The implementation uses a
 * "sub-barrier generation numbering" scheme to avoid overhead
 * and to ensure that all threads wait to leave the barrier
 * correct.  This code is based on an article from SunOpsis
 * Vol. 4, No. 1 by Richard Marejka
 * (Richard.Marejka@canada.sun.com).
 */
class ACE_Export ACE_Barrier
{
public:
  /// Initialize the barrier to synchronize <count> threads.
  ACE_Barrier (u_int count,
               const ACE_TCHAR *name = 0,
               void *arg = 0);

  /// Default dtor.
  ~ACE_Barrier (void);

  /// Block the caller until all <count> threads have called <wait> and
  /// then allow all the caller threads to continue in parallel.
  int wait (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Serialize access to the barrier state.
  ACE_Thread_Mutex lock_;

  /// Either 0 or 1, depending on whether we are the first generation
  /// of waiters or the next generation of waiters.
  int current_generation_;

  /// Total number of threads that can be waiting at any one time.
  int count_;

  /**
   * We keep two <sub_barriers>, one for the first "generation" of
   * waiters, and one for the next "generation" of waiters.  This
   * efficiently solves the problem of what to do if all the first
   * generation waiters don't leave the barrier before one of the
   * threads calls wait() again (i.e., starts up the next generation
   * barrier).
   */
  ACE_Sub_Barrier sub_barrier_1_;
  ACE_Sub_Barrier sub_barrier_2_;
  ACE_Sub_Barrier *sub_barrier_[2];

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Barrier &);
  ACE_Barrier (const ACE_Barrier &);
};

#if 0
// The following two classes are commented out since there doesn't
// appear to be a portable and robust means of implementing this
// functionality across platforms.  If you know of a portable and
// robust way to implement this functionality please let us know.

/**
 * @class ACE_Process_Condition
 *
 * @brief ACE_Condition variable wrapper that works across processes.
 */
class ACE_Export ACE_Process_Condition
{
public:
  ACE_Process_Condition (MUTEX &m, const ACE_TCHAR *name = 0, void *arg = 0);

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};
#endif /* 0 */

#if 0
/**
 * @class ACE_Process_Barrier
 *
 * @brief Implements "barrier synchronization" using ACE_Process_Mutexes!
 *
 * This class is just a simple wrapper for ACE_Barrier that
 * selects the USYNC_PROCESS variant for the locks.
 */
class ACE_Export ACE_Process_Barrier : public ACE_Barrier
{
public:
  /// Create a Process_Barrier, passing in the optional <name>.
  ACE_Process_Barrier (u_int count, const ACE_TCHAR *name = 0);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};
#endif /* 0 */

/**
 * @class ACE_Thread_Barrier
 *
 * @brief Implements "barrier synchronization" using ACE_Thread_Mutexes!
 *
 * This class is just a simple wrapper for ACE_Barrier that
 * selects the USYNC_THREAD variant for the locks.
 */
class ACE_Export ACE_Thread_Barrier : public ACE_Barrier
{
public:
  /// Create a Thread_Barrier, passing in the optional <name>.
  ACE_Thread_Barrier (u_int count, const ACE_TCHAR *name = 0);

  /// Default dtor.
  ~ACE_Thread_Barrier (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};
#endif /* ACE_HAS_THREADS */

#if defined (__ACE_INLINE__)
#include "ace/Synch.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "ace/Synch_T.h"

template <class ACE_LOCK>
class ACE_Guard;

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Guard<ACE_Null_Mutex>
 *
 * @brief Template specialization of <ACE_Guard> for the
 * <ACE_Null_Mutex>.
 *
 * This specialization is useful since it helps to speedup
 * performance of the "Null_Mutex" considerably.
 */
class ACE_Export ACE_Guard<ACE_Null_Mutex>
{
public:
  // = Initialization and termination methods.
  ACE_Guard (ACE_Null_Mutex &) {}
  ACE_Guard (ACE_Null_Mutex &, int) {}
#if defined (ACE_WIN32)
  ~ACE_Guard (void) {}
#endif /* ACE_WIN32 */

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

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Write_Guard<ACE_Null_Mutex>
 *
 */
class ACE_Export ACE_Write_Guard<ACE_Null_Mutex> : public ACE_Guard<ACE_Null_Mutex>
{
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

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Read_Guard<ACE_Null_Mutex>
 *
 */
class ACE_Export ACE_Read_Guard<ACE_Null_Mutex> : public ACE_Guard<ACE_Null_Mutex>
{
public:
  ACE_Read_Guard (ACE_Null_Mutex &m)
    : ACE_Guard<ACE_Null_Mutex> (m) {}
  ACE_Read_Guard (ACE_Null_Mutex &m, int blocked)
    : ACE_Guard<ACE_Null_Mutex> (m, blocked) {}

  int acquire_read (void) { return 0; }
  int acquire (void) { return 0; }
  int tryacquire_read (void) { return 0; }
  int tryacquire (void) { return 0; }
  void dump (void) const {}
};

#if defined (ACE_LEGACY_MODE)
# include "ace/File_Lock.h"
# include "ace/Process_Semaphore.h"
# include "ace/Process_Mutex.h"
# include "ace/RW_Process_Mutex.h"
# include "ace/Test_and_Set.h"
#endif  /* ACE_LEGACY_MODE */

#include "ace/post.h"
#endif /* ACE_SYNCH_H */
