/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Token.h
//
// = AUTHOR
//    Original author -- Karl-Heinz Dorn (kdorn@erlh.siemens.de)
//    Ported to ACE by Douglas C. Schmidt (schmidt@cs.wustl.edu)
//
// ============================================================================

#ifndef ACE_TOKEN_H
#define ACE_TOKEN_H

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_THREADS)

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || defined (VXWORKS) || defined (ACE_PSOS)
// If platforms support semaphores with timed wait, then we use semaphores instead of c.v.
# define ACE_TOKEN_USES_SEMAPHORE
#endif /* (ACE_WIN32 && !ACE_HAS_WINCE) || VXWORKS || ACE_PSOS */

class ACE_Export ACE_Token
{
  // = TITLE
  //    Class that acquires, renews, and releases a synchronization
  //    token that is serviced in strict FIFO ordering and that also
  //    supports readers/writer semantics.
  //
  // = DESCRIPTION
  //    This class is a more general-purpose synchronization mechanism
  //    than SunOS 5.x mutexes.  For example, it implements "recursive
  //    mutex" semantics, where a thread that owns the token can
  //    reacquire it without deadlocking.  In addition, threads that
  //    are blocked awaiting the token are serviced in strict FIFO
  //    order as other threads release the token (Solaris and Pthread
  //    mutexes don't strictly enforce an acquisition order).  There
  //    are two FIFO lists within the class.  Write acquires always
  //    have higher priority over read acquires.  Which means, if you
  //    use both write/read operations, care must be taken to avoid
  //    starvation on the readers.  Notice that the read/write acquire
  //    operations do not have the usual semantic of reader/writer
  //    locks.  Only one reader can acquire the token at a time (which
  //    is different from the usual reader/writer locks where several
  //    readers can acquire a lock at the same time as long as there
  //    is no writer waiting for the lock.)  We choose the names 1.)
  //    to borrow the semantic to give writers higher priority, and,
  //    2.) to support a common interface over all locking classes in
  //    ACE.
public:
  // = Initialization and termination.

  ACE_Token (LPCTSTR name = 0, void * = 0);
  virtual ~ACE_Token (void);

  // = Synchronization operations.

  int acquire (void (*sleep_hook)(void *),
               void *arg = 0,
               ACE_Time_Value *timeout = 0);
  // Acquire the token, sleeping until it is obtained or until
  // <timeout> expires.  If some other thread currently holds the
  // token then <sleep_hook> is called before our thread goes to
  // sleep.  This <sleep_hook> can be used by the requesting thread to
  // unblock a token-holder that is sleeping, e.g., by means of
  // writing to a pipe (the ACE ACE_Reactor uses this functionality).
  // Return values:
  // 0 if acquires without calling <sleep_hook>
  // 1 if <sleep_hook> is called.
  // 2 if the token is signaled.
  // -1 if failure or timeout occurs (if timeout occurs errno == ETIME)
  // If <timeout> == <&ACE_Time_Value::zero> then acquire has polling
  // semantics (and does *not* call <sleep_hook>).

  int acquire (ACE_Time_Value *timeout = 0);
  // This behaves just like the previous <acquire> method, except
  // that it invokes the virtual function called <sleep_hook>
  // that can be overridden by a subclass of ACE_Token.

  virtual void sleep_hook (void);
  // This should be overridden by a subclass to define
  // the appropriate behavior before <acquire> goes to sleep.
  // By default, this is a no-op...

  int renew (int requeue_position = 0, ACE_Time_Value *timeout = 0);
  // An optimized method that efficiently reacquires the token if no
  // other threads are waiting.  This is useful for situations where
  // you don't want to degrad the quality of service if there are
  // other threads waiting to get the token.  If <requeue_position> ==
  // -1 and there are other threads waiting to obtain the token we are
  // queued at the end of the list of waiters.  If <requeue_position>
  // > -1 then it indicates how many entries to skip over before
  // inserting our thread into the list of waiters (e.g.,
  // <requeue_position> == 0 means "insert at front of the queue").
  // Renew has the rather odd semantics such that if there are other
  // waiting threads it will give up the token even if the
  // nesting_level_ > 1.  I'm not sure if this is really the right
  // thing to do (since it makes it possible for shared data to be
  // changed unexpectedly) so use with caution...
  // This method maintians the original token priority.

  int tryacquire (void);
  // Become interface-compliant with other lock mechanisms (implements
  // a non-blocking <acquire>).

  int remove (void);
  // Shuts down the ACE_Token instance.

  int release (void);
  // Relinquish the token.  If there are any waiters then the next one
  // in line gets it.

  int acquire_read (void);
  // Behave like acquire but in a lower priority.  It should probably
  // be called acquire_yield.

  int acquire_read (void (*sleep_hook)(void *),
               void *arg = 0,
               ACE_Time_Value *timeout = 0);
  // More sophisticate version of acquire_read.

  int acquire_write (void);
  // Just calls <acquire>.

  int acquire_write (void (*sleep_hook)(void *),
               void *arg = 0,
               ACE_Time_Value *timeout = 0);
  // More sophisticate version of acquire_write.

  int tryacquire_read (void);
  // Lower priority try_acquire.

  int tryacquire_write (void);
  // Just calls <tryacquire>.

  // = Accessor methods.

  int waiters (void);
  // Return the number of threads that are currently waiting to get
  // the token.

  ACE_thread_t current_owner (void);
  // Return the id of the current thread that owns the token.

  int signal_all_threads ();
  // Force all threads waiting to acquire the token to return one by
  // one.  The method sets the <signal_all_thread_> to non-zero if
  // there're threads waiting, and returns the number of threads
  // waiting.  If there's no thread waiting for the token, the call
  // returns 0 and doesn't do anything.  The last thread releases the
  // token also reset the <singal_all_thread_> flag to 0.  This means,
  // any threads that try to acquire the token after the call is
  // issued will also get "signaled" and the number of threads waiting
  // the token is only a snapshot.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  // = The following structure implements a ACE_FIFO of waiter threads
  // that are asleep waiting to obtain the token.

  struct ACE_Token_Queue_Entry
  {
    ACE_Token_Queue_Entry (ACE_Thread_Mutex &m, ACE_thread_t t_id);

    int wait (ACE_Time_Value *timeout, ACE_Thread_Mutex &lock);
    // Entry blocks on the token.

    int signal (void);
    // Notify (unblock) the entry.

    ACE_Token_Queue_Entry *next_;
    // Pointer to next waiter.

    ACE_thread_t thread_id_;
    // ACE_Thread id of this waiter.

#if defined (ACE_TOKEN_USES_SEMAPHORE)
    ACE_Semaphore cv_;
    // ACE_Semaphore object used to wake up waiter when it can run again.
#else
    ACE_Condition_Thread_Mutex cv_;
    // ACE_Condition object used to wake up waiter when it can run again.
#endif /* ACE_TOKEN_USES_SEMAPHORE */

    int runable_;
    // Ok to run.
  };

private:
  enum ACE_Token_Op_Type
  {
    READ_TOKEN = 1,
    WRITE_TOKEN
  };

  struct ACE_Token_Queue
  {
    ACE_Token_Queue (void);

    void remove_entry (ACE_Token_Queue_Entry *);
    // Remove a waiter from the queue (used when a timeout occurs).

    ACE_Token_Queue_Entry *head_;
    // Head of the list of waiting threads.

    ACE_Token_Queue_Entry *tail_;
    // Tail of the list of waiting threads.
  };

  int shared_acquire (void (*sleep_hook_func)(void *),
                      void *arg,
                      ACE_Time_Value *timeout,
                      ACE_Token_Op_Type op_type);
  // Implements the <acquire> and <tryacquire> methods above.

  ACE_Token_Queue writers_;
  // A queue of writer threads.

  ACE_Token_Queue readers_;
  // A queue of reader threads.

  ACE_Thread_Mutex lock_;
  // ACE_Thread_Mutex used to lock internal data structures.

  ACE_thread_t owner_;
  // Current owner of the token.

  int in_use_;
  // Some thread (i.e., <owner_>) is using the token.  We need this
  // extra variable to deal with POSIX pthreads madness...

  int waiters_;
  // Number of waiters.

  int nesting_level_;
  // Current nesting level.

  int signal_all_threads_;
  // Whether we are "signaling" all threads or not.
};

#if defined (__ACE_INLINE__)
#include "ace/Synch_T.h"
#include "ace/Token.i"
#endif /* __ACE_INLINE__ */
#else
class ACE_Export ACE_Token
{
public:
  int acquire (ACE_Time_Value * = 0) { ACE_NOTSUP_RETURN (-1); }
  int tryacquire (void) { ACE_NOTSUP_RETURN (-1); }
  int remove (void) { ACE_NOTSUP_RETURN (-1); }
  int release (void) { ACE_NOTSUP_RETURN (-1); }
};
#endif /* ACE_HAS_THREADS */
#endif /* ACE_TOKEN_H */
