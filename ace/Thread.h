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

#if !defined (ACE_HAS_THREADS) 
class ACE_Export ACE_Thread
{
public:
  static ACE_thread_t self (void);
  // Return the unique ID of the thread.

  static void self (ACE_hthread_t &t_id);
  // Return the unique kernel ID of the thread.

  static void exit (void *status = 0);
  // Exit the current thread and return "status".

private:
  ACE_Thread (void);
  // Ensure that we don't get instantiated.
};
#else
class ACE_Export ACE_Thread
{
  // = TITLE
  //   Provides a wrapper for threads.
  //
  // = DESCRIPTION
  //   This class provides a common interface that is mapped onto
  //   either POSIX Pthreads, Solaris threads, or Win32 threads.

public:
  static int spawn (ACE_THR_FUNC func, 
		    void *arg = 0,
		    long flags = THR_NEW_LWP,
		    ACE_thread_t *t_id = 0, 
		    ACE_hthread_t *t_handle = 0,
		    u_int priority = 0,
		    void *stack = 0, 
		    size_t stack_size = 0);
  // Spawn a new thread, which executes "func" with argument "arg".

  static int spawn_n (size_t n, 
		      ACE_THR_FUNC func,
		      void *arg = 0, 
		      long flags = THR_NEW_LWP,
		      u_int priority = 0,
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

  static int spawn_n (ACE_thread_t thread_ids[], 
		      size_t n, 
		      ACE_THR_FUNC func, 
		      void *arg, 
		      long flags, 
		      u_int priority = 0,
		      void *stack[] = 0,
		      size_t stack_size[] = 0);
  // Spawn N new threads, which execute <func> with argument <arg>.
  // The thread_ids of successfully spawned threads will be placed
  // into the <thread_ids> buffer, which must be the same size as <n>.
  // If <stack> != 0 it is assumed to be an array of <n> pointers to
  // the base of the stacks to use for the threads being spawned.
  // Likewise, if <stack_size> != 0 it is assumed to be an array of
  // <n> values indicating how big each of the corresponding <stack>s
  // are.  Returns the number of threads actually spawned (if this
  // doesn't equal the number requested then something has gone wrong
  // and <errno> will explain...).

  static int join (ACE_thread_t,
		   ACE_thread_t *,
		   void ** = 0);
  // Wait for one or more threads to exit.

  static int join (ACE_hthread_t, 
		   void ** = 0);
  // Wait for one thread to exit.

  static int resume (ACE_hthread_t);
  // Continue the execution of a previously suspended thread.

  static int suspend (ACE_hthread_t);
  // Suspend the execution of a particular thread.

  static int kill (ACE_thread_t, int signum);
  // Send a signal to the thread.

  static void yield (void);
  // Yield the thread to another.

  static void self (ACE_hthread_t &t_id);
  // Return the unique kernel ID of the thread.

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
			void (*destructor)(void *value),
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
#endif /* ACE_HAS_THREADS */

#if defined (__ACE_INLINE__)
#include "ace/Thread.i"
#endif /* __ACE_INLINE__ */
#endif /* ACE_THREAD_H */
