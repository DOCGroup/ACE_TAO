// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Thread_Manager_Test.cpp
//
// = DESCRIPTION
//      This program tests the group management mechanisms provided by
//      the <ACE_Thread_Manager>, including the group signal handling,
//      group suspension and resumption, and cooperative thread
//      cancellation mechanisms.
//
// = AUTHOR
//    Prashant Jain and Douglas C. Schmidt
//
// ============================================================================

#include "test_config.h"
#include "ace/Thread_Manager.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Signal.h"

ACE_RCSID(tests, Thread_Manager_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

#include "Thread_Manager_Test.h"

#if defined (DIGITAL_UNIX)
class ACE_Export ACE_Hash<ACE_hthread_t>
{
  // = TITLE
  //     Function object for hashing an ACE_hthread_t.
public:
  u_long operator () (const ACE_hthread_t &t_id) const;
};

u_long
ACE_Hash<ACE_hthread_t>::operator () (const ACE_hthread_t &t_id) const
{
  return ACE::hash_pjw (ACE_reinterpret_cast (const char *, &t_id),
                        sizeof t_id);
}
#endif /* DIGITAL_UNIX */

// Each thread keeps track of whether it has been signalled by using a
// global hash map.  See comment below about why it's allocated
// dynamically.
typedef ACE_Hash_Map_Manager_Ex<ACE_hthread_t,
                                int,
                                ACE_Hash<ACE_hthread_t>,
                                ACE_Equal_To<ACE_hthread_t>,
                                ACE_Null_Mutex> SIGNALLED_MAP;
static SIGNALLED_MAP *signalled = 0;

// Synchronize starts of threads, so that they all start before the
// main thread cancels them.  To avoid creating a static object, it
// is dynamically allocated, before spawning any threads.
static ACE_Barrier *thread_start;

extern "C" void
handler (int signum)
{
  if (signalled)
    {
      // No printout here, to be safe.  Signal handlers must not
      // acquire locks, etc.
      ACE_hthread_t t_id;
      ACE_OS::thr_self (t_id);

      // There's not much we can do if the bind () fails.
      signalled->bind (t_id, ACE_static_cast (sig_atomic_t, signum));
    }
}

static void *
worker (int iterations)
{
#if defined (VXWORKS)
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%P|%t) %s: stack size is %u\n"),
              ACE_OS::thr_self (), ACE_OS::thr_min_stack ()));
#endif /* VXWORKS */

#if !defined (ACE_LACKS_UNIX_SIGNALS)
  // Cache this thread's ID.
  ACE_hthread_t t_id;
  ACE_OS::thr_self (t_id);

  if (! signalled)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) (worker): signal catcher is 0!!!!\n")));
      return ACE_reinterpret_cast (void *, -1);
    }

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();
#endif /* ! ACE_LACKS_UNIX_SIGNAL */

  // After setting up the signal catcher, block on the start barrier.
  thread_start->wait ();

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) worker starting loop\n")));

  for (int i = 0; i < iterations; i++)
    {
      if ((i % 1000) == 0)
        {
#if !defined (ACE_LACKS_UNIX_SIGNALS)
          sig_atomic_t signum;
          if (signalled->find (t_id, signum) == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ASYS_TEXT ("(%t) had received signal %d\n"),
                          signum));

              // Only test for cancellation after we've been signaled,
              // to avoid race conditions for suspend() and resume().
              if (thr_mgr->testcancel (ACE_Thread::self ()) != 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ASYS_TEXT ("(%t) has been cancelled "
                                         "before iteration %d!\n"),
                              i));
                  break;
                }
            }
#endif /* ! ACE_LACKS_UNIX_SIGNAL */
          ACE_OS::sleep (1);
        }
    }

  // Destructor removes thread from Thread_Manager.
  return 0;
}

static const int DEFAULT_THREADS = ACE_MAX_THREADS;
static const int DEFAULT_ITERATIONS = 10000;

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Equal_To<ACE_hthread_t>;
template class ACE_Hash_Map_Entry<ACE_hthread_t, sig_atomic_t>;
template class ACE_Hash_Map_Manager_Ex<ACE_hthread_t,
  sig_atomic_t,
  ACE_Hash<ACE_hthread_t>,
  ACE_Equal_To<ACE_hthread_t>,
  ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_hthread_t,
  sig_atomic_t,
  ACE_Hash<ACE_hthread_t>,
  ACE_Equal_To<ACE_hthread_t>,
  ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_hthread_t,
  sig_atomic_t,
  ACE_Hash<ACE_hthread_t>,
  ACE_Equal_To<ACE_hthread_t>,
  ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Equal_To<ACE_hthread_t>
#pragma instantiate ACE_Hash_Map_Entry<ACE_hthread_t, sig_atomic_t>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_hthread_t,
  sig_atomic_t,
  ACE_Hash<ACE_hthread_t>,
  ACE_Equal_To<ACE_hthread_t>,
  ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_hthread_t,
  sig_atomic_t,
  ACE_Hash<ACE_hthread_t>,
  ACE_Equal_To<ACE_hthread_t>,
  ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_hthread_t,
  sig_atomic_t,
  ACE_Hash<ACE_hthread_t>,
  ACE_Equal_To<ACE_hthread_t>,
  ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Thread_Manager_Test"));
  int status = 0;

#if defined (ACE_HAS_THREADS)
  int n_threads = DEFAULT_THREADS;
  int n_iterations = DEFAULT_ITERATIONS;

  // Dynamically allocate signalled so that we can control when it
  // gets deleted.  Specifically, we need to delete it before the main
  // thread's TSS is cleaned up.
  ACE_NEW_RETURN (signalled, SIGNALLED_MAP, 1);

  // And similarly, dynamically allocate the thread_start barrier.
  ACE_NEW_RETURN (thread_start, ACE_Barrier (n_threads + 1), -1);

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();

#if defined (VXWORKS)
  // Assign thread (VxWorks task) names to test that feature.
  ACE_thread_t *thread_name;
  ACE_NEW_RETURN (thread_name, ACE_thread_t[n_threads], -1);

  // And test the ability to specify stack size.
  size_t *stack_size;
  ACE_NEW_RETURN (stack_size, size_t[n_threads], -1);

  int i;

  for (i = 0; i < n_threads; ++i)
    {
      if (i < n_threads - 1)
        {
          ACE_NEW_RETURN (thread_name[i], char[32], -1);
          ACE_OS::sprintf (thread_name[i], ASYS_TEXT ("thread%u"), i);
        }
      else
        // Pass an ACE_thread_t pointer of 0 for the last thread name.
        thread_name[n_threads - 1] = 0;

      stack_size[i] = 40000;
    }
#endif /* VXWORKS */

  int grp_id = thr_mgr->spawn_n
                 (
#if defined (VXWORKS)
                  thread_name,
#endif /* VXWORKS */
                  n_threads,
                  (ACE_THR_FUNC) worker,
                  (void *) n_iterations,
                  THR_BOUND | THR_DETACHED
#if defined (VXWORKS)
                  , ACE_DEFAULT_THREAD_PRIORITY
                  , -1
                  , 0
                  , stack_size
#endif /* VXWORKS */
                  );

  ACE_ASSERT (grp_id != -1);
  thread_start->wait ();

  // Wait for 1 second and then suspend every thread in the group.
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) suspending group\n")));
  if (thr_mgr->suspend_grp (grp_id) == -1)
    {
      // Pthreads w/o UNIX 98 extensions doesn't support suspend/resume,
      // so it's allowed to ENOTSUP; anything else is a hard fail.
      ACE_ASSERT (errno == ENOTSUP);
      ACE_DEBUG((LM_DEBUG,
                 ASYS_TEXT (" OK: suspend_grp isn't supported with Pthreads\n")));
    }

  // Wait for 1 more second and then resume every thread in the
  // group.
  ACE_OS::sleep (ACE_Time_Value (1));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) resuming group\n")));

  if (thr_mgr->resume_grp (grp_id) == -1)
    {
      ACE_ASSERT (errno == ENOTSUP);
      ACE_DEBUG((LM_DEBUG, ASYS_TEXT (" OK: resume_grp isn't supported with Pthreads\n")));
    }

  // Wait for 1 more second and then send a SIGINT to every thread in
  // the group.
  ACE_OS::sleep (ACE_Time_Value (1));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) signaling group\n")));

#if defined (ACE_HAS_WTHREADS)
  thr_mgr->kill_grp (grp_id, SIGINT);
#elif !defined (ACE_HAS_PTHREADS_DRAFT4)
  ACE_ASSERT (thr_mgr->kill_grp (grp_id, SIGINT) != -1);
#else
  if (thr_mgr->kill_grp (grp_id, SIGINT) == -1)
    ACE_ASSERT (errno == ENOTSUP);
#endif /* ACE_HAS_WTHREADS */

  // Wait and then cancel all the threads.
  ACE_OS::sleep (ACE_Time_Value (2));

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) cancelling group\n")));

  ACE_ASSERT (thr_mgr->cancel_grp (grp_id) != -1);

  // Perform a barrier wait until all the threads have shut down.
  // But, wait for a limited time because sometimes the test hangs
  // on SunOS 5.5.1 and 5.7.
  const ACE_Time_Value max_wait (60);
  const ACE_Time_Value wait_time (ACE_OS::gettimeofday () + max_wait);
  if (thr_mgr->wait (&wait_time) == -1)
    {
      if (errno == ETIME)
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("maximum wait time of %d msec exceeded\n"),
                               max_wait.msec ()));
      else
        ACE_OS::perror ("wait");

      status = -1;
    }

  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) main thread finished\n")));

#if defined (VXWORKS)
  for (i = 0; i < n_threads - 1; ++i)
    {
      delete [] thread_name[i];
      // Don't delete the last thread_name, because it points
      // to the name in the TCB.  It was initially 0.
    }
  delete [] thread_name;
  delete [] stack_size;
#endif /* VXWORKS */

  delete thread_start;
  thread_start = 0;
  delete signalled;
  signalled = 0;

#else
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return status;
}
