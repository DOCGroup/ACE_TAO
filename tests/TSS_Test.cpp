// ============================================================================
// $Id$

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
#include "ace/Log_Msg.h"
#include "test_config.h"

static int iterations = 100;

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
static ACE_Thread_Mutex lock;

typedef ACE_TSS_Guard<ACE_Thread_Mutex> GUARD;
#else
// Serializes output via cout.
static ACE_Null_Mutex lock;

typedef ACE_Guard<ACE_Null_Mutex> GUARD;
#endif /* ACE_HAS_THREADS */

static void 
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

  ip = new int;

  if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_setspecific"));

  for (int i = 0; i < count; i++)
    {
      if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keycreate"));

      ip = new int;

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
	ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, lock, 0));
	ACE_ASSERT (TSS_Error->flags () == iterations);
      }
      key = 0;

      if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "ACE_OS::thr_keycreate"));

      ip = new int;

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

static void 
handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, "signal = %S\n", signum));
  ACE_Service_Config::thr_mgr ()->exit (0);
}

int 
main (int argc, char *argv[])
{
  ACE_START_TEST;
  
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());
  int threads = ACE_MAX_THREADS;

  signal (SIGINT, ACE_SignalHandler (handler));
  
#if defined (ACE_HAS_THREADS)
  if (ACE_Service_Config::thr_mgr ()->spawn_n (threads, 
					       ACE_THR_FUNC (&worker), 
					       (void *) iterations,
					       THR_BOUND | THR_DETACHED) == -1)
    ACE_OS::perror ("ACE_Thread_Manager::spawn_n");

  ACE_Service_Config::thr_mgr ()->wait ();
#else
  worker ((void *) iterations);
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_TSS<Errno>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
