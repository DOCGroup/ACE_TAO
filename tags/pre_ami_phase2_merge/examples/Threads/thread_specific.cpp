// $Id$

#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "ace/Synch.h"

ACE_RCSID(Threads, thread_specific, "$Id$")

#if defined (ACE_HAS_THREADS)

#include "thread_specific.h"

// Static variables.
ACE_MT (ACE_Thread_Mutex Errno::lock_);
int Errno::flags_;

// This is our thread-specific error handler...
static ACE_TSS<Errno> tss_error;

// Serializes output via cout.
static ACE_SYNCH_MUTEX printf_lock;

#if defined (ACE_HAS_THREADS)
typedef ACE_TSS_Guard<ACE_Thread_Mutex> GUARD;
#else
typedef ACE_Guard<ACE_Null_Mutex> GUARD;
#endif /* ACE_HAS_THREADS */

extern "C" void
cleanup (void *ptr)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) in cleanup, ptr = %x\n",
              ptr));

  delete ACE_reinterpret_cast (char *, ptr);
}

// This worker function is the entry point for each thread.

static void *
worker (void *c)
{
  // Cast the arg to a long, first, because a pointer is the same size
  // as a long on all current ACE platforms.
  int count = (int) (long) c;

  ACE_thread_key_t key = ACE_OS::NULL_key;
  int *ip = 0;

  // Make one key that will be available when the thread exits so that
  // we'll have something to cleanup!

  if (ACE_Thread::keycreate (&key, cleanup) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) %p\n",
                "ACE_Thread::keycreate"));

  ACE_NEW_RETURN (ip,
                  int,
                  0);

  if (ACE_Thread::setspecific (key, (void *) ip) == -1)
    ACE_ERROR ((LM_ERROR, 
                "(%t) %p\n",
                "ACE_Thread::setspecific"));

  for (int i = 0; i < count; i++)
    {
      if (ACE_Thread::keycreate (&key, cleanup) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "ACE_Thread::keycreate"));

      ACE_NEW_RETURN (ip,
                      int,
                      0);

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) in worker 1, key = %d, ip = %x\n",
                  key,
                  ip));

      if (ACE_Thread::setspecific (key, (void *) ip) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "ACE_Thread::setspecific"));

      if (ACE_Thread::getspecific (key, (void **) &ip) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "ACE_Thread::setspecific"));

      if (ACE_Thread::setspecific (key, (void *) 0) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "ACE_Thread::setspecific"));
      delete ip;

      if (ACE_Thread::keyfree (key) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "ACE_Thread::keyfree"));

      // Cause an error.
      ACE_OS::read (ACE_INVALID_HANDLE, 0, 0);

      // The following two lines set the thread-specific state.
      tss_error->error (errno);
      tss_error->line (__LINE__);

      // This sets the static state (note how C++ makes it easy to do
      // both).
      tss_error->flags (count);

      {
        ACE_hthread_t handle;
        ACE_Thread::self (handle);

        // Use the guard to serialize access to printf...
        ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, printf_lock, 0);

        ACE_OS::printf ("(%u) errno = %d, lineno = %d, flags = %d\n",
                        handle,
                        tss_error->error (),
                        tss_error->line (),
                        tss_error->flags ());
      }
      key = ACE_OS::NULL_key;

      if (ACE_Thread::keycreate (&key, cleanup) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "ACE_Thread::keycreate"));

      ACE_NEW_RETURN (ip,
                      int,
                      0);

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) in worker 2, key = %d, ip = %x\n",
                  key,
                  ip));

      if (ACE_Thread::setspecific (key, (void *) ip) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "ACE_Thread::setspecific"));

      if (ACE_Thread::getspecific (key, (void **) &ip) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "ACE_Thread::setspecific"));

      if (ACE_Thread::setspecific (key, (void *) 0) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "ACE_Thread::setspecific"));
      delete ip;

      if (ACE_Thread::keyfree (key) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "ACE_Thread::keyfree"));
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%t) exiting\n"));
  return 0;
}

extern "C" void
handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG,
              "signal = %S\n", signum));
  ACE_Thread_Manager::instance ()->exit (0);
}

int
main (int argc, char *argv[])
{
  // The Service_Config must be the first object defined in main...
  ACE_Service_Config daemon (argv[0]);

  int threads = argc > 1 ? ACE_OS::atoi (argv[1]) : 4;
  int count = argc > 2 ? ACE_OS::atoi (argv[2]) : 10000;

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) (handler), SIGINT);
  ACE_UNUSED_ARG (sa);

#if defined (ACE_HAS_THREADS)
  if (ACE_Thread_Manager::instance ()->spawn_n (threads,
                                               ACE_THR_FUNC (&worker),
                                               (void *) count,
                                               THR_BOUND | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_Thread_Manager::spawn_n"),
                      -1);

  ACE_Thread_Manager::instance ()->wait ();
#else
  worker ((void *) count);
#endif /* ACE_HAS_THREADS */
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_TSS<Errno>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS<Errno>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#else
int
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "ACE doesn't support support threads on this platform (yet)\n"),
                    -1);
}
#endif /* ACE_HAS_THREADS */
