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
//    Prashant Jain <pjain@cs.wustl.edu> and Doug Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Guard_T.h"
#include "ace/Thread_Manager.h"
#include "ace/Thread_Mutex.h"
#include "ace/Signal.h"
#include "TSS_Test_Errno.h"



static u_int errors = 0;

#if defined (ACE_HAS_THREADS)

#if defined (ACE_DEFAULT_THREAD_KEYS)
  // If ACE_DEFAULT_THREAD_KEYS is defined, it is probably
  // set to a small value.  So that the test doesn't run out
  // of keys quickly in the first thread, set the number of
  // ITERATIONS to be small as well.
  static const int ITERATIONS =
    (ACE_DEFAULT_THREAD_KEYS - ACE_MAX_THREADS) / (2 * ACE_MAX_THREADS) < 2
      ?  1
      :  (ACE_DEFAULT_THREAD_KEYS - ACE_MAX_THREADS) / (2 * ACE_MAX_THREADS);
#else
  // POSIX requires at least _POSIX_THREAD_KEYS_MAX (128) keys.  25
  // iterations with 4 worker threads should be sufficient to check
  // the TSS wrappers without exceeding the minimum requirements.

  static const int ITERATIONS = 25;
#endif /* ACE_DEFAULT_THREAD_KEYS */

// Static variables.
int Errno::flags_;
int Errno::created_;
int Errno::deleted_;

ACE_Thread_Mutex *Errno::lock_ = 0;

// This is our thread-specific error handler . . .
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
  // Don't do this:  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) in cleanup, ptr = %x\n"), ptr));
  // The Log_Msg buffer is a TSS object, too, and it may be gone!
  // if you must say something here try:
  //  ACE_OS::fprintf (stderr, ACE_TEXT("(%d) in cleanup, ptr = %x\n"), ACE_Thread::self(), ptr);
  // and this:
  //    operator delete (ptr);
  // is nonsense when applied to a void *! (even tho the compilers accept it????
  delete static_cast <int *> (ptr);
}

// This worker function is the entry point for each thread.

static void *
worker (void *c)
{
  int count = *(static_cast<int*> (c));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) worker, iterations = %d\n"), count));

  ACE_thread_key_t key = ACE_OS::NULL_key;
  int *ip = 0;

  // Make one key that will be available when the thread exits so that
  // we'll have something to cleanup!

  if (ACE_Thread::keycreate (&key, cleanup) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p (no keys available)\n"),
                  ACE_TEXT ("ACE_Thread::keycreate")));
      return (void *) -1;
    }

  ACE_NEW_RETURN (ip, int, 0);

  if (ACE_Thread::setspecific (key, (void *) ip) == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                ACE_TEXT ("ACE_Thread::setspecific")));

  for (int i = 0; i < count; i++)
    {
      if (ACE_Thread::keycreate (&key, cleanup) == -1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p (no more keys)\n"),
                      ACE_TEXT ("ACE_Thread::keycreate")));
          break;
        }

      ACE_NEW_RETURN (ip, int, 0);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) in worker at location 1, ")
                            ACE_TEXT ("key = %d, ip = %x\n"),
                  key, ip));

      // Needed to work around (possibly broken?) strict aliasing warning in GCC.
      void *v_ip (reinterpret_cast <void *> (ip));

      if (ACE_Thread::setspecific (key, v_ip) == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("ACE_Thread::setspecific")));

      if (ACE_Thread::getspecific (key, &v_ip) == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("ACE_Thread::getspecific")));

      if (ACE_Thread::setspecific (key, (void *) 0) == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("ACE_Thread::setspecific")));

      // See comment in cleanup () above.
      delete ip;

#if defined (ACE_HAS_TSS_EMULATION)
      if (ACE_Thread::keyfree (key) == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("ACE_Thread::keyfree")));
#endif /* ACE_HAS_TSS_EMULATION */

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
      // class ACE_TSS_Type_Adapter for what this
      // should look like.  Unfortunately, some compilers have trouble
      // with the implicit type conversions.  Others have problems with
      // the *explicit* type conversions.
#if !defined (ACE_HAS_BROKEN_EXPLICIT_TYPECAST_OPERATOR_INVOCATION)
      (*u)->operator u_int & () = 37;
      if ((*u)->operator u_int () != 37)
        {
          // Use the guard to serialize access to errors.
          ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, output_lock,
                                    0));
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("use of ACE_TSS_Type_Adapter failed, value ")
                      ACE_TEXT ("is %u, it should be 37!\n"),
                      (*u)->operator u_int ()));
          ++errors;
        }
#endif /* !defined (ACE_HAS_BROKEN_EXPLICIT_TYPECAST_OPERATOR_INVOCATION) */

#if defined (ACE_HAS_TSS_EMULATION)
      key = ACE_OS::NULL_key;

      if (ACE_Thread::keycreate (&key, cleanup) == -1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p (no more keys)\n"),
                      ACE_TEXT ("ACE_Thread::keycreate")));
          break;
        }

      ACE_NEW_RETURN (ip, int, 0);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) in worker at location 2, ")
                            ACE_TEXT ("key = %d, ip = %x\n"),
                  key, ip));

      // Needed to work around (possibly broken?) strict aliasing warning in GCC.
      void *v_ip2 (reinterpret_cast <void *> (ip));

      if (ACE_Thread::setspecific (key, v_ip2) == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("ACE_Thread::setspecific")));

      if (ACE_Thread::getspecific (key, &v_ip2) == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("ACE_Thread::getspecific")));

      if (ACE_Thread::setspecific (key, (void *) 0) == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("ACE_Thread::setspecific")));

      // See comment in cleanup () above.
      delete ip;

      // ACE_HAS_TSS_EMULATION is turned on, then it should work.
#  if defined (ACE_HAS_TSS_EMULATION)
      if (ACE_Thread::keyfree (key) == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("ACE_Thread::keyfree")));
#  endif /* defined (ACE_HAS_TSS_EMULATION) */
#endif /* ACE_HAS_TSS_EMULATION */
    }
  return 0;
}

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("TSS_Test"));

#if defined (ACE_HAS_THREADS)
  Errno::allocate_lock ();

  const u_int threads = ACE_MAX_THREADS;

  // Dynamically allocate TSS_Error so that we can control when it
  // gets deleted.  Specifically, we need to delete it before the
  // ACE_Object_Manager destroys the ACE_Allocator.  That's because
  // deletion of TSS_Error causes the internal structures of
  // ACE_TSS_Cleanup to be modified, and which in turn uses
  // ACE_Allocator.
  ACE_NEW_RETURN (tss_error, ACE_TSS<Errno>, 1);

  // Similarly, dynamically allocate u.
  ACE_NEW_RETURN (u, ACE_TSS<ACE_TSS_Type_Adapter<u_int> >, 1);

  int iterations = ITERATIONS;
  if (ACE_Thread_Manager::instance ()->spawn_n
      (threads,
       ACE_THR_FUNC (worker),
       &iterations,
       THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"), ACE_TEXT ("spawn_n")), 1);

  ACE_Thread_Manager::instance ()->wait ();

  delete u;
  delete tss_error;

  Errno::deallocate_lock ();


  if (Errno::created () != Errno::deleted ())
    {
      //@@TODO: this should probably be promoted to an error rather than just a
      // warning.
      ACE_ERROR ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Warning: Number created (%d) != number deleted (%d)\n"),
        Errno::created (),
        Errno::deleted ()
        ));
    }

#else /* ACE_HAS_THREADS */
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads are not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return errors ? -1 : 0;
}
