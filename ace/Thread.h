/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Thread.h 
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_THREAD_H)
#define ACE_THREAD_H

#include "ace/ACE.h"

class ACE_Export ACE_Thread
{
  // = TITLE
  //     Provides a wrapper for threads.
  //
  // = DESCRIPTION
  //     This class provides a common interface that is mapped onto
  //     either POSIX Pthreads, Solaris threads, or Win32 threads.

public:
  static int spawn (ACE_THR_FUNC func, 
		    void *arg = 0,
		    long flags = THR_NEW_LWP,
		    ACE_thread_t *t_id = 0, 
		    ACE_hthread_t *t_handle = 0,
		    long priority = -1,
		    void *stack = 0, 
		    size_t stack_size = 0);
  // Spawn a new thread having <{flags}> attributes and running
  // <{func}> with arguments <{args}>.  <{thr_id}> and <{t_handle}>
  // are set to the thread's ID and handle (?), respectively.  The
  // thread runs at <{priority}> priority (see below).
  //
  // The <{flags}> are a bitwise-OR of the following:
  // = BEGIN<INDENT>
  // THR_CANCEL_DISABLE, THR_CANCEL_ENABLE, THR_CANCEL_DEFERRED,
  // THR_CANCEL_ASYNCHRONOUS, THR_BOUND, THR_NEW_LWP, THR_DETACHED,
  // THR_SUSPENDED, THR_DAEMON, THR_JOINABLE, THR_SCHED_FIFO,
  // THR_SCHED_RR, THR_SCHED_DEFAULT
  // = END<INDENT>
  // 
  // By default, or if <{priority}> is set to -1, an "appropriate"
  // priority value for the given scheduling policy (specified in
  // <{flags}>, e.g., <THR_SCHED_DEFAULT>) is used.  This value is
  // calculated dynamically, and is the median value between the
  // minimum and maximum priority values for the given policy.  If an
  // explicit value is given, it is used.  Note that actual priority
  // values are EXTREMEMLY implementation-dependent, and are probably
  // best avoided.

  static int spawn_n (size_t n, 
		      ACE_THR_FUNC func,
		      void *arg = 0, 
		      long flags = THR_NEW_LWP,
		      long priority = -1,
		      void *stack[] = 0, 
		      size_t stack_size[] = 0);
  // Spawn N new threads, which execute <func> with argument <arg>.
  // If <stack> != 0 it is assumed to be an array of <n> pointers to
  // the base of the stacks to use for the threads being spawned.
  // Likewise, if <stack_size> != 0 it is assumed to be an array of
  // <n> values indicating how big each of the corresponding <stack>s
  // are.  Returns the number of threads actually spawned (if this
  // doesn't equal the number requested then something has gone wrong
  // and <errno> will explain...).
  //
  // See also <spawn>.

  static int spawn_n (ACE_thread_t thread_ids[], 
		      size_t n, 
		      ACE_THR_FUNC func, 
		      void *arg, 
		      long flags, 
		      long priority = -1,
		      void *stack[] = 0,
		      size_t stack_size[] = 0,
		      ACE_hthread_t thread_handles[] = 0);
  // Spawn N new threads, which execute <func> with argument <arg>.
  // The thread_ids of successfully spawned threads will be placed
  // into the <thread_ids> buffer (which must be the same size as
  // <n>).  If <stack> != 0 it is assumed to be an array of <n>
  // pointers to the base of the stacks to use for the threads being
  // spawned.  If <stack_size> != 0 it is assumed to be an array of
  // <n> values indicating how big each of the corresponding <stack>s
  // are.  If <thread_handles> != 0 it is assumed to be an array of
  // <n> thread_handles that will be assigned the values of the thread
  // handles being spawned.  Returns the number of threads actually
  // spawned (if this doesn't equal the number requested then
  // something has gone wrong and <errno> will explain...).
  //
  // See also <spawn>.

  static int join (const ACE_Thread_ID &,
		   void **status = 0);
  // Wait for one or more threads to exit.

  static int join (ACE_thread_t,
		   ACE_thread_t *,
		   void **status = 0);
  // Wait for one or more threads to exit.

  static int join (ACE_hthread_t, 
		   void ** = 0);
  // Wait for one thread to exit.

  static int resume (ACE_hthread_t);
  // Continue the execution of a previously suspended thread.

  static int suspend (ACE_hthread_t);
  // Suspend the execution of a particular thread.

  static int getprio (ACE_hthread_t, int &prio);
  // Get the priority of a particular thread.

  static int setprio (ACE_hthread_t, int prio);
  // Set the priority of a particular thread.

  static int kill (ACE_thread_t, int signum);
  // Send a signal to the thread.

  static void yield (void);
  // Yield the thread to another.

  static void self (ACE_hthread_t &t_handle);
  // Return the unique kernel handle of the thread.  Note that on
  // Win32 this is actually a pseudohandle, which cannot be shared
  // with other processes or waited on by threads.  To locate the real
  // handle, please use the <ACE_Thread_Manager::thr_self> method.

  static ACE_thread_t self (void);
  // Return the unique ID of the thread.

  static void exit (void *status = 0);
  // Exit the current thread and return "status".

  static int getconcurrency (void);
  // Get the LWP concurrency level of the process.

  static int setconcurrency (int new_level);
  // Set the LWP concurrency level of the process.

  static int sigsetmask (int how, 
			 const sigset_t *set, 
			 sigset_t *oset = 0);
  // Change and/or examine calling thread's signal mask.

  static int keycreate (ACE_thread_key_t *keyp,
#if defined (ACE_HAS_THR_C_DEST)
			ACE_THR_C_DEST destructor,
#else
			ACE_THR_DEST destructor,
#endif /* ACE_HAS_THR_C_DEST */
			void * = 0);
  // Allocates a <keyp> that is used to identify data that is specific
  // to each thread in the process.  The key is global to all threads
  // in the process.

  static int keyfree (ACE_thread_key_t key);
  // Free up the key so that other threads can reuse it.

  static int setspecific (ACE_thread_key_t key, 
			  void *value);
  // Bind value to the thread-specific data key, <key>, for the calling
  // thread.

  static int getspecific (ACE_thread_key_t key, 
			  void **valuep);
  // Stores the current value bound to <key> for the calling thread
  // into the location pointed to by <valuep>.

  static int disablecancel (struct cancel_state *old_state);
  // Disable thread cancellation.

  static int enablecancel (struct cancel_state *old_state, 
			   int flag);
  // Enable thread cancellation.

  static int setcancelstate (struct cancel_state &new_state,
			     struct cancel_state *old_state);
  // Set the cancellation state.

  static int cancel (ACE_thread_t t_id);
  // Cancel a thread.

  static void testcancel (void);
  // Test the cancel.

private:
  ACE_Thread (void);
  // Ensure that we don't get instantiated.
};

#if defined (__ACE_INLINE__)
#include "ace/Thread.i"
#endif /* __ACE_INLINE__ */
#endif /* ACE_THREAD_H */
