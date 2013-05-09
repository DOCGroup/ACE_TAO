
//=============================================================================
/**
 *  @file    Thread_Manager_Test.cpp
 *
 *  $Id$
 *
 *    This program tests the group management mechanisms provided by
 *    the <ACE_Thread_Manager>, including the group signal handling,
 *    group suspension and resumption, and cooperative thread
 *    cancellation mechanisms.
 *
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu> and Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Thread_Manager.h"
#include "ace/Signal.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"



#if defined (ACE_HAS_THREADS)
#include "ace/Barrier.h"

// Each thread keeps track of whether it has been signalled by using a
// global array.  It must be dynamically allocated to allow sizing at
// runtime, based on the number of threads.
static ACE_thread_t *signalled = 0;
static size_t n_threads = ACE_MAX_THREADS;

// Helper function that looks for an existing entry in the signalled
// array.  Also finds the position of the first unused entry in the
// array, and updates if requested with the t_id.
extern "C" int
been_signalled (const ACE_thread_t t_id,
                const u_int update = 0)
{
  u_int unused_slot = n_threads;

  for (u_int i = 0; i < n_threads; ++i)
    {
      if (ACE_OS::thr_equal (signalled[i], t_id))
        // Already signalled.
        return 1;

      if (update  &&
          unused_slot == n_threads  &&
          ACE_OS::thr_equal (signalled[i],
                             ACE_OS::NULL_thread))
        unused_slot = i;
    }

  if (update  &&  unused_slot < n_threads)
    // Update the array using the first unused_slot.
    signalled[unused_slot] = t_id;

  return 0;
}

// Synchronize starts of threads, so that they all start before the
// main thread cancels them.  To avoid creating a static object, it is
// dynamically allocated, before spawning any threads.
static ACE_Barrier *thread_start = 0;

extern "C" void
handler (int /* signum */)
{
  if (signalled)
    {
      // No printout here, to be safe.  Signal handlers must not
      // acquire locks, etc.
      const ACE_thread_t t_id = ACE_OS::thr_self ();

      // Update the signalled indication.
      (void) been_signalled (t_id, 1u /* update */);
    }
}

static void *
worker (int iterations)
{
#if 0
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) %s: stack size is %u\n"),
              ACE_OS::thr_self (),
              ACE_OS::thr_min_stack ()));
#endif /* ACE_HAS_VXTHREADS */

#if !defined (ACE_LACKS_UNIX_SIGNALS)
  // Cache this thread's ID.
  const ACE_thread_t t_id = ACE_OS::thr_self ();
#endif /* ! ACE_LACKS_UNIX_SIGNAL */

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();

  // After setting up the signal catcher, block on the start barrier.
  thread_start->wait ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) worker starting loop\n")));

  for (int i = 0; i < iterations; i++)
    {
      if ((i % 1000) == 0)
        {
#if !defined (ACE_LACKS_UNIX_SIGNALS)
          if (been_signalled (t_id))
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%t) had received signal\n")));

              // Only test for cancellation after we've been signaled,
              // to avoid race conditions for suspend() and resume().
              if (thr_mgr->testcancel (ACE_Thread::self ()) != 0)
                {
                  char thr_id[BUFSIZ];
                  // Test out the ACE_OS::thr_id() method.
                  ACE_OS::thr_id (thr_id, sizeof thr_id);
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%s) has been cancelled ")
                              ACE_TEXT ("before iteration %d!\n"),
                              thr_id,
                              i));
                  break;
                }
            }
#else
          if (thr_mgr->testcancel (ACE_Thread::self ()) != 0)
            {
              char thr_id[BUFSIZ];
              // Test out the ACE_OS::thr_id() method.
              ACE_OS::thr_id (thr_id, sizeof thr_id);
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%s) has been cancelled ")
                          ACE_TEXT ("before iteration %d!\n"),
                          thr_id,
                          i));
              break;
            }
#endif /* ! ACE_LACKS_UNIX_SIGNAL */
          ACE_OS::sleep (1);
        }
    }

  // Destructor removes thread from Thread_Manager.
  return 0;
}

static const int DEFAULT_ITERATIONS = 10000;

// Define a ACE_Task that will serve in the tests related to tasks.

class ThrMgr_Task : public ACE_Task_Base {
public:
  ThrMgr_Task (ACE_Thread_Manager *);

  virtual int svc (void);

  static int errors;
};

int ThrMgr_Task::errors = 0;

// Just be sure to set the ACE_Thread_Manager correctly.
ThrMgr_Task::ThrMgr_Task (ACE_Thread_Manager *mgr)
  : ACE_Task_Base (mgr)
{
}

// svc just waits til it's been cancelled, then exits.
int
ThrMgr_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Task 0x%x, thread %t waiting to be cancelled\n"),
              this));
  ACE_thread_t me = ACE_Thread::self ();
  for (int i = 0; i < 30 && !this->thr_mgr ()->testcancel (me); ++i)
    ACE_OS::sleep (1);

  if (this->thr_mgr ()->testcancel (me))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Task 0x%x, thread %t cancelled; exiting\n"),
                  this));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Task 0x%x, thread %t was not cancelled\n"),
                  this));
      ++ThrMgr_Task::errors;
    }
  return 0;
}


// This function tests the task-based record keeping functions.
static int
test_task_record_keeping (ACE_Thread_Manager *mgr)
{

  int status = 0;

  ThrMgr_Task t1 (mgr), t2 (mgr);
  int t1_grp (20), t2_grp (30);

  // Start two tasks, with multiple threads per task. Make sure that
  // task_all_list() works.
  if (-1 == t1.activate (THR_JOINABLE, 2, 0,
                         ACE_DEFAULT_THREAD_PRIORITY, t1_grp))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("activate")), 1);
  if (-1 == t2.activate (THR_JOINABLE, 3, 0,
                         ACE_DEFAULT_THREAD_PRIORITY, t2_grp))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("activate")), 1);

  ACE_Task_Base *task_list[10];
  int num_tasks = mgr->task_all_list (task_list, 10);
  if (2 != num_tasks)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected 2 tasks; got %d\n"),
                  num_tasks));
      status = 1;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Got %d tasks - correct\n"), num_tasks));
      if (((task_list[0] == &t1 && task_list[1] == &t2)
           || (task_list[1] == &t1 && task_list[0] == &t2))
          && task_list[0] != task_list[1])
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The Task IDs are correct\n")));
      else
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("But Task ID values are wrong!\n")));
    }
  ACE_DEBUG ((LM_DEBUG, "Canceling grp %d\n", t1_grp));
  if (-1 == mgr->cancel_grp (t1_grp))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("cancel_grp")),
                      1);
  ACE_DEBUG ((LM_DEBUG, "Canceling grp %d\n", t2_grp));
  if (-1 == mgr->cancel_grp (t2_grp))
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("cancel_grp")),
                      1);

  mgr->wait ();
  if (ThrMgr_Task::errors > 0 && status == 0)
    status = 1;

  return status;
}

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Thread_Manager_Test"));
  int status = 0;

#if defined (ACE_HAS_THREADS)
  size_t n_iterations = DEFAULT_ITERATIONS;

  u_int i;

  // Dynamically allocate signalled so that we can control when it
  // gets deleted.  Specifically, we need to delete it before the main
  // thread's TSS is cleaned up.
  ACE_NEW_RETURN (signalled,
                  ACE_thread_t[n_threads],
                  1);
  // Initialize each ACE_thread_t to avoid Purify UMR's.
  for (i = 0; i < n_threads; ++i)
    signalled[i] = ACE_OS::NULL_thread;

  // And similarly, dynamically allocate the thread_start barrier.
  ACE_NEW_RETURN (thread_start,
                  ACE_Barrier (n_threads + 1),
                  -1);

  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  ACE_Thread_Manager *thr_mgr = ACE_Thread_Manager::instance ();

#if 0
  // Assign thread (VxWorks task) names to test that feature.
  ACE_hthread_t *thread_name = 0;
  ACE_NEW_RETURN (thread_name,
                  ACE_hthread_t[n_threads],
                  -1);

  // And test the ability to specify stack size.
  size_t *stack_size = 0;
  ACE_NEW_RETURN (stack_size,
                  size_t[n_threads],
                  -1);

  for (i = 0; i < n_threads; ++i)
    {
      if (i < n_threads - 1)
        {
          ACE_NEW_RETURN (thread_name[i],
                          char[32],
                          -1);
          ACE_OS::sprintf (thread_name[i],
                           ACE_TEXT ("thread%u"),
                           i);
        }
      else
        // Pass an ACE_thread_t pointer of 0 for the last thread name.
        thread_name[n_threads - 1] = 0;

      stack_size[i] = 40000;
    }
#endif /* ACE_HAS_VXTHREADS */

  int grp_id = thr_mgr->spawn_n
                 (
                  n_threads,
                  (ACE_THR_FUNC) worker,
                  reinterpret_cast <void *> (n_iterations),
                  THR_BOUND
                  , ACE_DEFAULT_THREAD_PRIORITY
                  , -1
#if 0
                  , 0
                  , thread_name
                  , 0
                  , stack_size
#endif /* ACE_HAS_VXTHREADS */
                  );

  ACE_TEST_ASSERT (grp_id != -1);
  thread_start->wait ();

  // Wait for 1 second and then suspend every thread in the group.
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) suspending group\n")));
  if (thr_mgr->suspend_grp (grp_id) == -1)
    {
      // Pthreads w/o UNIX 98 extensions doesn't support suspend/resume,
      // so it's allowed to ENOTSUP as long as the config indicates this.
      if (errno == ENOTSUP)
        {
#if defined (ACE_HAS_PTHREADS) && (defined (ACE_HAS_PTHREAD_SUSPEND) || \
                                   defined (ACE_HAS_PTHREAD_SUSPEND_NP))
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("suspend_grp: ENOTSUP but config ")
                      ACE_TEXT ("says it should work.\n")));
#else
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT (" OK: suspend_grp isn't supported with ")
                     ACE_TEXT ("Pthreads\n")));
#endif /* ACE_HAS_PTHREADS && should be able to suspend */
        }
      else
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("suspend_grp")));
        }
    }

  // Wait for 1 more second and then resume every thread in the
  // group.
  ACE_OS::sleep (ACE_Time_Value (1));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) resuming group\n")));

  if (thr_mgr->resume_grp (grp_id) == -1)
    {
      // Pthreads w/o UNIX 98 extensions doesn't support suspend/resume,
      // so it's allowed to ENOTSUP as long as the config indicates this.
      if (errno == ENOTSUP)
        {
#if defined (ACE_HAS_PTHREADS) && (defined (ACE_HAS_PTHREAD_CONTINUE) || \
                                   defined (ACE_HAS_PTHREAD_CONTINUE_NP) || \
                                   defined (ACE_HAS_PTHREAD_RESUME_NP))
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("resume_grp: ENOTSUP but config ")
                      ACE_TEXT ("says it should work.\n")));
#else
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT (" OK: resume_grp isn't supported with ")
                     ACE_TEXT ("Pthreads\n")));
#endif /* ACE_HAS_PTHREADS && should be able to continue/resume */
        }
      else
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("resume_grp")));
        }
    }

  // Wait for 1 more second and then send a SIGINT to every thread in
  // the group.
  ACE_OS::sleep (ACE_Time_Value (1));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) signaling group\n")));

#if defined (ACE_HAS_WTHREADS)
  thr_mgr->kill_grp (grp_id,
                     SIGINT);
#elif !defined (ACE_HAS_PTHREADS_DRAFT4) && !defined(ACE_LACKS_PTHREAD_KILL)
  ACE_TEST_ASSERT (thr_mgr->kill_grp (grp_id,
                                 SIGINT) != -1);
#else
  if (thr_mgr->kill_grp (grp_id,
                         SIGINT) == -1)
    ACE_TEST_ASSERT (errno == ENOTSUP);
#endif /* ACE_HAS_WTHREADS */

  // Wait and then cancel all the threads.
  ACE_OS::sleep (ACE_Time_Value (1));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) cancelling group\n")));

  ACE_TEST_ASSERT (thr_mgr->cancel_grp (grp_id) != -1);

  // Perform a barrier wait until all the threads have shut down.
  // But, wait for a limited time, just in case.
  ACE_Time_Value const max_wait (600);
  ACE_Time_Value const wait_time (ACE_OS::gettimeofday () + max_wait);
  if (thr_mgr->wait (&wait_time) == -1)
    {
      if (errno == ETIME)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("maximum wait time of %d msec exceeded\n"),
                               max_wait.msec ()));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"), ACE_TEXT ("wait")));
      status = -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) main thread finished\n")));

#if 0
  for (i = 0; i < n_threads - 1; ++i)
    {
      delete [] thread_name[i];
      // Don't delete the last thread_name, because it points to the
      // name in the TCB.  It was initially 0.
    }
  delete [] thread_name;
  delete [] stack_size;
#endif /* ACE_HAS_VXTHREADS */

  delete thread_start;
  thread_start = 0;
  delete [] signalled;
  signalled = 0;

  // Now test task record keeping
  if (test_task_record_keeping (thr_mgr) != 0)
    status = -1;

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return status;
}
