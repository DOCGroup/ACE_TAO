// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    TSS_Test.cpp
//
// = DESCRIPTION
//     This program tests thread specific storage of data. The ACE_TSS
//     wrapper transparently ensures that the objects of this class
//     will be placed in thread-specific storage. All calls on
//     ACE_TSS::operator->() are delegated to the appropriate method
//     in the Errno class.
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
//
// ============================================================================

#include "ace/Service_Config.h"
#include "ace/Synch.h"
#include "TSS_Test_Errno.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

#if defined (ACE_DEFAULT_THREAD_KEYS)
// If ACE_DEFAULT_THREAD_KEYS is defined, it is probably set to a small
// value.  So that the test doesn't run out of keys quickly in the
// first thread, set the number of ITERATIONS to be small as well.
static const int ITERATIONS = ((ACE_DEFAULT_THREAD_KEYS/8) < 2 ? 1 : ACE_DEFAULT_THREAD_KEYS/8);
#else
static const int ITERATIONS = 100;
#endif /* ACE_DEFAULT_THREAD_KEYS */

// Static variables.
ACE_MT (ACE_Thread_Mutex Errno::lock_);
int Errno::flags_;

// This is our thread-specific error handler...
// See comment below about why it's dynamically allocated.
static ACE_TSS<Errno> *TSS_Error;

#if defined (ACE_HAS_THREADS)
// Serializes output via cout.
static ACE_Thread_Mutex cout_lock;

typedef ACE_TSS_Guard<ACE_Thread_Mutex> GUARD;
#else
// Serializes output via cout.
static ACE_Null_Mutex cout_lock;

typedef ACE_Guard<ACE_Null_Mutex> GUARD;
#endif /* ACE_HAS_THREADS */

extern "C" void
cleanup (void *ptr)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) in cleanup, ptr = %x\n", ptr));

  operator delete (ptr);
}

// This worker function is the entry point for each thread.

static void *
worker (void *c)
{
  ACE_NEW_THREAD;

  int count = int (c);

  ACE_thread_key_t key = ACE_OS::NULL_key;
  int *ip = 0;

  // Make one key that will be available when the thread exits so that
  // we'll have something to cleanup!

  if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
    {
      ACE_ERROR ((LM_ERROR, "(%t) %p (no keys available)\n",
                  "ACE_OS::thr_keycreate"));
      return (void *) -1;
    }

  ACE_NEW_RETURN (ip, int, 0);

  if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

  for (int i = 0; i < count; i++)
    {
      if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
        {
	  ACE_ERROR ((LM_ERROR, "(%t) %p (no more keys)\n",
                      "ACE_OS::thr_keycreate"));
          break;
        }

      ACE_NEW_RETURN (ip, int, 0);

      ACE_DEBUG ((LM_DEBUG, "(%t) in worker 1, key = %d, ip = %x\n", key, ip));

      if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      if (ACE_OS::thr_getspecific (key, (void **) &ip) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      if (ACE_OS::thr_setspecific (key, (void *) 0) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      delete ip;

      if (ACE_OS::thr_keyfree (key) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keyfree"));

      // Cause an error.
      ACE_OS::read (ACE_INVALID_HANDLE, 0, 0);

      // The following two lines set the thread-specific state.
      (*TSS_Error)->error (errno);
      (*TSS_Error)->line (__LINE__);

      // This sets the static state (note how C++ makes it easy to do
      // both).
      (*TSS_Error)->flags (count);

      {
	// Use the guard to serialize access
	ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, cout_lock, 0));
	ACE_ASSERT ((*TSS_Error)->flags () == ITERATIONS);
      }

      key = ACE_OS::NULL_key;

      if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
        {
	  ACE_ERROR ((LM_ERROR, "(%t) %p (no more keys)\n",
                      "ACE_OS::thr_keycreate"));
          break;
        }

      ACE_NEW_RETURN (ip, int, 0);

      ACE_DEBUG ((LM_DEBUG, "(%t) in worker 2, key = %d, ip = %x\n", key, ip));

      if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      if (ACE_OS::thr_getspecific (key, (void **) &ip) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      if (ACE_OS::thr_setspecific (key, (void *) 0) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

      delete ip;

      if (ACE_OS::thr_keyfree (key) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keyfree"));
    }

  return 0;
}

extern "C" void
handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, "signal = %S\n", signum));
  ACE_Thread_Manager::instance ()->exit (0);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_TSS<Errno>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS<Errno>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif /* ACE_HAS_THREADS */

int
main (int, char *[])
{
  ACE_START_TEST ("TSS_Test");

  const u_int threads = ACE_MAX_THREADS;

  // Dynamically allocate TSS_Error so that we can control when
  // it gets deleted.  Specifically, we need to delete it before
  // the ACE_Object_Manager destroys the ACE_Allocator.  That's
  // because deletion of TSS_Error causes the internal structures
  // of ACE_TSS_Cleanup to be modified, and which in turn uses
  // ACE_Allocator.
  ACE_NEW_RETURN (TSS_Error, ACE_TSS<Errno>, 1);

#if defined (ACE_HAS_THREADS)

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);
  ACE_hthread_t *thread_handles;

  ACE_NEW_RETURN (thread_handles, ACE_hthread_t[threads], -1);

  if (ACE_Thread_Manager::instance ()->spawn_n
      ((ACE_thread_t *) 0,
       threads,
       ACE_THR_FUNC (worker),
       (void *) ITERATIONS,
       THR_BOUND,
       ACE_DEFAULT_THREAD_PRIORITY,
       -1,
       0,
       0,
       thread_handles) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), 1);

#if defined (VXWORKS)
      // VxWorks doesn't support thr_join() semantics...  Someday
      // we'll fix this.
      ACE_Thread_Manager::instance ()->wait ();
#else
      // Wait for all the threads to reach their exit point and then join
      // with all the exiting threads.
      for (u_int i = 0;
	   i < threads;
	   i++)
	if (ACE_Thread::join (thread_handles[i]) == -1)
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "join"), -1);
#endif /* VXWORKS */

  delete [] thread_handles;

#else
  ACE_ERROR ((LM_ERROR,
	      "threads are not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */

  delete TSS_Error;

  ACE_END_TEST;
  return 0;
}
