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

#include "test_config.h"

#if defined (ACE_HAS_THREADS)

static const int ITERATIONS = 100;

class Errno
{
public:
  int error (void) { return this->errno_; }
  void error (int i) { this->errno_ = i; }
  
  int line (void) { return this->lineno_; }
  void line (int l) { this->lineno_ = l; }

  // Errno::flags_ is a static variable, so we've got to protect it
  // with a mutex since it isn't kept in thread-specific storage.
  int flags (void) { 
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_Mon, Errno::lock_, -1));

    return Errno::flags_;
  }
  int flags (int f)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, Errno::lock_, -1));

    Errno::flags_ = f;
    return 0;
  }

private:
  // = errno_ and lineno_ will be thread-specific data so they don't
  // need a lock.  
  int errno_;
  int lineno_;

  static int flags_;
#if defined (ACE_HAS_THREADS)
  // flags_ needs a lock. 
  static ACE_Thread_Mutex lock_;
#endif /* ACE_HAS_THREADS */
};

// Static variables.
ACE_MT (ACE_Thread_Mutex Errno::lock_);
int Errno::flags_;

// This is our thread-specific error handler...
static ACE_TSS<Errno> TSS_Error;

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

  delete ptr;
}

// This worker function is the entry point for each thread.

static void *
worker (void *c)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());
  ACE_NEW_THREAD;

  int count = int (c);

  ACE_thread_key_t key = 0;
  int *ip = 0;

  // Make one key that will be available when the thread exits so that
  // we'll have something to cleanup!

  if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keycreate"));

  ACE_NEW_RETURN (ip, int, 0);

  if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

  for (int i = 0; i < count; i++)
    {
      if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keycreate"));

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
      TSS_Error->error (errno);
      TSS_Error->line (__LINE__);

      // This sets the static state (note how C++ makes it easy to do
      // both).
      TSS_Error->flags (count);

      {
	// Use the guard to serialize access
	ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, cout_lock, 0));
	ACE_ASSERT (TSS_Error->flags () == ITERATIONS);
      }
      key = 0;

      if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keycreate"));

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
  ACE_Service_Config::thr_mgr ()->exit (0);
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_TSS<Errno>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#endif /* ACE_HAS_THREADS */

int 
main (int, char *argv[])
{
  ACE_START_TEST ("TSS_Test.cpp");
  
#if defined (ACE_MT_SAFE)
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);

  if (ACE_Service_Config::thr_mgr ()->spawn_n (ACE_MAX_THREADS, 
					       ACE_THR_FUNC (&worker), 
					       (void *) ITERATIONS,
					       THR_BOUND | THR_DETACHED) == -1)
    ACE_OS::perror ("ACE_Thread_Manager::spawn_n");

  ACE_Service_Config::thr_mgr ()->wait ();
#else
  ACE_ERROR ((LM_ERROR, 
	      "threads are not supported on this platform\n"));
#endif /* defined (ACE_MT_SAFE) */
  ACE_END_TEST;
  return 0;
}
