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

#include "test_config.h"
#include "ace/Synch.h"
#include "ace/Thread_Manager.h"
#include "ace/Signal.h"
#include "TSS_Test_Errno.h"

ACE_RCSID(tests, TSS_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

static u_int errors = 0;

#if defined (ACE_HAS_THREADS)

#if defined (ACE_DEFAULT_THREAD_KEYS)
// If ACE_DEFAULT_THREAD_KEYS is defined, it is probably set to a small
// value.  So that the test doesn't run out of keys quickly in the
// first thread, set the number of ITERATIONS to be small as well.
static const int ITERATIONS = ((ACE_DEFAULT_THREAD_KEYS/8) < 2 ? 1 : ACE_DEFAULT_THREAD_KEYS/8);
#elif defined (__Lynx__)
static const int ITERATIONS = 1;
#else
static const int ITERATIONS = 100;
#endif /* ACE_DEFAULT_THREAD_KEYS */

// Static variables.
int Errno::flags_;
ACE_Thread_Mutex *Errno::lock_ = 0;

// This is our thread-specific error handler...
// See comment below about why it's dynamically allocated.
static ACE_TSS<Errno> *tss_error;
// This is for testing/demonstrating ACE_TSS_Type_Adapter.  It's
// dynamically allocated to avoid static objects, also.
static ACE_TSS<ACE_TSS_Type_Adapter<u_int> > *u;

// Serializes output.
static ACE_Thread_Mutex output_lock;

extern "C" void
cleanup (void *ptr)
{
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) in cleanup, ptr = %x\n"), ptr));

  operator delete (ptr);
}

// This worker function is the entry point for each thread.

static void *
worker (void *c)
{
  // Cast the arg to a long, first, because a pointer is the same
  // size as a long on all current ACE platforms.
  int count = (int) (long) c;

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) worker, iterations = %d\n"), count));

  ACE_thread_key_t key = ACE_OS::NULL_key;
  int *ip = 0;

  // Make one key that will be available when the thread exits so that
  // we'll have something to cleanup!

  if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
    {
      ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p (no keys available)\n"),
                  ASYS_TEXT ("ACE_OS::thr_keycreate")));
      return (void *) -1;
    }

  ACE_NEW_RETURN (ip, int, 0);

  if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p\n"),
                ASYS_TEXT ("ACE_OS::thr_setspecific")));

  for (int i = 0; i < count; i++)
    {
      if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
        {
          ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p (no more keys)\n"),
                      ASYS_TEXT ("ACE_OS::thr_keycreate")));
          break;
        }

      ACE_NEW_RETURN (ip, int, 0);

      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) in worker at location 1, ")
                            ASYS_TEXT ("key = %d, ip = %x\n"),
                  key, ip));

      if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
        ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p\n"),
                    ASYS_TEXT ("ACE_OS::thr_setspecific")));

      if (ACE_OS::thr_getspecific (key, (void **) &ip) == -1)
        ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p\n"),
                    ASYS_TEXT ("ACE_OS::thr_setspecific")));

      if (ACE_OS::thr_setspecific (key, (void *) 0) == -1)
        ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p\n"),
                    ASYS_TEXT ("ACE_OS::thr_setspecific")));

      delete ip;

      if (ACE_OS::thr_keyfree (key) == -1)
        ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p\n"),
                    ASYS_TEXT ("ACE_OS::thr_keyfree")));

      // Cause an error.
      ACE_OS::read (ACE_INVALID_HANDLE, 0, 0);

      // The following two lines set the thread-specific state.
      (*tss_error)->error (errno);
      (*tss_error)->line (__LINE__);

      // This sets the static state (note how C++ makes it easy to do
      // both).
      (*tss_error)->flags (count);

      {
        // Use the guard to serialize access
        ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, output_lock, 0));
        ACE_ASSERT ((*tss_error)->flags () == ITERATIONS);
      }

      // Demonstrate use of ACE_TSS_Type_Adapter to wrap built-in
      // types when used with ACE_TSS.  See DESCRIPTION of template
      // class ACE_TSS_Type_Adapter in ace/Synch_T.h for what this
      // should look like.  Unfortunately, some compilers have trouble
      // with the implicit type conversions.
      (*u)->operator u_int & () = 37;
      if ((*u)->operator u_int () != 37)
        {
          // Use the guard to serialize access to errors.
          ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, output_lock,
                                    0));
          ACE_DEBUG ((LM_ERROR, ASYS_TEXT ("use of ACE_TSS_Type_Adapter failed, value ")
                      ASYS_TEXT ("is %u, it should be 37!\n"), (*u)->operator u_int ()));
          ++errors;
        }

#if !defined (__Lynx__) && !defined (ACE_HAS_TSS_EMULATION)
      key = ACE_OS::NULL_key;

      if (ACE_OS::thr_keycreate (&key, cleanup) == -1)
        {
          ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p (no more keys)\n"),
                      ASYS_TEXT ("ACE_OS::thr_keycreate")));
          break;
        }

      ACE_NEW_RETURN (ip, int, 0);

      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) in worker at location 2, ")
                            ASYS_TEXT ("key = %d, ip = %x\n"),
                  key, ip));

      if (ACE_OS::thr_setspecific (key, (void *) ip) == -1)
        ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p\n"),
                    ASYS_TEXT ("ACE_OS::thr_setspecific")));

      if (ACE_OS::thr_getspecific (key, (void **) &ip) == -1)
        ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p\n"),
                    ASYS_TEXT ("ACE_OS::thr_setspecific")));

      if (ACE_OS::thr_setspecific (key, (void *) 0) == -1)
        ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p\n"),
                    ASYS_TEXT ("ACE_OS::thr_setspecific")));

      delete ip;

      if (ACE_OS::thr_keyfree (key) == -1)
        ACE_ERROR ((LM_ERROR, ASYS_TEXT ("(%t) %p\n"),
                    ASYS_TEXT ("ACE_OS::thr_keyfree")));
#endif /* ! __Lynx__) && ! ACE_HAS_TSS_EMULATION */
    }

  return 0;
}

extern "C" void
handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("signal = %S\n"), signum));
  ACE_Thread_Manager::instance ()->exit (0);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_TSS<Errno>;
template class ACE_TSS_Type_Adapter<u_int>;
template class ACE_TSS<ACE_TSS_Type_Adapter<u_int> >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS<Errno>
#pragma instantiate ACE_TSS_Type_Adapter<u_int>
#pragma instantiate ACE_TSS<ACE_TSS_Type_Adapter<u_int> >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif /* ACE_HAS_THREADS */

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("TSS_Test"));

#if defined (ACE_HAS_THREADS)
  Errno::allocate_lock ();

#if defined (__Lynx__)
  const u_int threads = 2;
#else  /* ! __Lynx__ */
  const u_int threads = ACE_MAX_THREADS;
#endif /* ! __Lynx__ */

  // Dynamically allocate TSS_Error so that we can control when it
  // gets deleted.  Specifically, we need to delete it before the
  // ACE_Object_Manager destroys the ACE_Allocator.  That's because
  // deletion of TSS_Error causes the internal structures of
  // ACE_TSS_Cleanup to be modified, and which in turn uses
  // ACE_Allocator.
  ACE_NEW_RETURN (tss_error, ACE_TSS<Errno>, 1);

  // Similarly, dynamically allocate u.
  ACE_NEW_RETURN (u, ACE_TSS<ACE_TSS_Type_Adapter<u_int> >, 1);

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  if (ACE_Thread_Manager::instance ()->spawn_n
      (threads,
       ACE_THR_FUNC (worker),
       (void *) ITERATIONS,
       THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("%p\n"), ASYS_TEXT ("spawn_n")), 1);

  ACE_Thread_Manager::instance ()->wait ();

  delete u;
  u = 0;
  delete tss_error;
  tss_error = 0;

  Errno::deallocate_lock ();
#else
  ACE_ERROR ((LM_ERROR,
              ASYS_TEXT ("threads are not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return errors ? -1 : 0;
}
