
//=============================================================================
/**
 *  @file    Task_Group_Test.cpp
 *
 *    This test program illustrates the logic of the grp_id handling
 *    mechanism in <ACE_Task_Base> and the <ACE_Thread_Manager>.
 *    Partially reuses test submitted by Paxton Mason in Bugzilla #2366
 *
 *  @author Martin Corino <mcorino@remedy.nl>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Atomic_Op.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

#if defined (ACE_HAS_THREADS)

const int MAX_GROUP_THREADS = ACE_MAX_THREADS * 2;

static ACE_Atomic_Op<ACE_Thread_Mutex, int> run_count (0);

class Waiter_Task : public ACE_Task_Base
{
public:
  Waiter_Task (ACE_Thread_Manager* tm = 0)
    : ACE_Task_Base (tm) {}
  virtual int svc(void);
};

int
Waiter_Task::svc(void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t|grp_id=%d) waiting for thread cancellation.\n"), this->grp_id ()));

  run_count++;

  // wait until canceled
  while (!ACE_Task_Base::thr_mgr ()->testcancel (ACE_OS::thr_self ()))
    ACE_OS::sleep (0);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t|grp_id=%d) thread cancelled.\n"), this->grp_id ()));

  run_count--;

  return 0;
}

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Task_Group_Test"));

#if defined (ACE_HAS_THREADS)

  ACE_thread_t thread_list[MAX_GROUP_THREADS] = {0};
  int group_id = -1;

  Waiter_Task waiter_task (ACE_Thread_Manager::instance ());

  // activate a single thread
  waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 1);
  // get the assigned grp_id
  group_id = waiter_task.grp_id ();
  // attempt to add a thread -> this should fail without force_activate
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 1) != 1)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have failed to add new thread.")));
  }
  // force addition of new thread
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 1, 1) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have succeeded to force addition of new thread.")));
  }
  // force addition of new thread ignoring specific group id
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 1, 1, ACE_DEFAULT_THREAD_PRIORITY, group_id+1) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have succeeded to force addition of new thread.")));
  }
  // we should now have 3 threads in group 'group_id'
  if (waiter_task.thr_mgr ()->thread_grp_list (group_id, &thread_list[0], MAX_GROUP_THREADS) != 3)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Should have found 3 threads in group %d."), group_id));
  }

  // cancel all threads
  waiter_task.thr_mgr ()->cancel_all ();
  // wait for threads to exit cleanly
  while (waiter_task.thr_mgr ()->num_threads_in_task (&waiter_task) > 0)
    ACE_OS::sleep (0);

  // restart 2 threads reusing the group id
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 2) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have succeeded to add new threads.")));
  }
  // attempt to add a thread -> this should fail without force_activate
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 1) != 1)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have failed to add new thread.")));
  }
  // force addition of new thread
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 1, 1) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have succeeded to force addition of new thread.")));
  }
  // force addition of new thread ignoring specific group id
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 1, 1, ACE_DEFAULT_THREAD_PRIORITY, group_id+1) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have succeeded to force addition of new thread.")));
  }
  // we should now have 4 threads in group 'group_id'
  if (waiter_task.thr_mgr ()->thread_grp_list (group_id, &thread_list[0], MAX_GROUP_THREADS) != 4)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Should have found 4 threads in group %d."), group_id));
  }

  // wait till the threads actually started up
  while (run_count != 4)
    ACE_OS::sleep (0);

  // change the group id of all threads of the task
  group_id += 1;
  waiter_task.grp_id (group_id);

  // we should still have 4 threads in group with new 'group_id'
  if (waiter_task.thr_mgr ()->thread_grp_list (group_id, &thread_list[0], MAX_GROUP_THREADS) != 4)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Should have found 4 threads in group %d."), group_id));
  }

  // cancel all threads
  waiter_task.thr_mgr ()->cancel_all ();
  // wait for threads to exit cleanly
  while (waiter_task.thr_mgr ()->num_threads_in_task (&waiter_task) > 0)
    ACE_OS::sleep (0);

  // restart 3 threads using explicit new group id
  group_id = 33;
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 3, 0, ACE_DEFAULT_THREAD_PRIORITY, group_id) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have succeeded to add new threads.")));
  }
  // attempt to add a thread -> this should fail without force_activate
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 1) != 1)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have failed to add new thread.")));
  }
  // force addition of new thread
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 1, 1) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have succeeded to force addition of new thread.")));
  }
  // force addition of new thread ignoring specific group id
  if (waiter_task.activate (THR_NEW_LWP | THR_DETACHED, 1, 1, ACE_DEFAULT_THREAD_PRIORITY, group_id+1) != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Task_Base::activate should have succeeded to force addition of new thread.")));
  }
  // we should now have 5 threads in group 'group_id'
  if (waiter_task.thr_mgr ()->thread_grp_list (group_id, &thread_list[0], MAX_GROUP_THREADS) != 5)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Should have found 5 threads in group %d."), group_id));
  }

  // cancel all threads
  waiter_task.thr_mgr ()->cancel_all ();
  // wait for threads to exit cleanly
  while (waiter_task.thr_mgr ()->num_threads_in_task (&waiter_task) > 0)
    ACE_OS::sleep (0);

#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
