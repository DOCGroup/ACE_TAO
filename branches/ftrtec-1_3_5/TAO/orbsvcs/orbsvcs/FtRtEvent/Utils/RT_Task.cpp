//$Id$
#include "RT_Task.h"
#include "ace/Task.h"
#include "ace/Sched_Params.h"

void RT_Task::set_current()
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  long prio = ACE_Sched_Params::priority_max(ACE_SCHED_FIFO);
  if (ACE_OS::thr_setprio(thr_handle, prio, ACE_SCHED_FIFO) == -1){
    ACE_DEBUG((LM_DEBUG, "Cannot set the thread to RT class\n"));
  }
}

int RT_Task::activate(ACE_Task_Base* task)
{
  long priority = ACE_Sched_Params::priority_max(ACE_SCHED_FIFO);
  long flags = THR_NEW_LWP;

  // To get FIFO scheduling with PTHREADS.
  ACE_SET_BITS (flags,
                THR_SCHED_FIFO);

 // Become an active object.
  if (task->activate (flags,
                      1,
                      0,
                      priority) == -1)
    {
      ACE_DEBUG((LM_DEBUG, "Cannot activate the thread in RT class\n"));

      // On Linux, for example, only the superuser can set the policy
      // to other than ACE_SCHED_OTHER.  But with ACE_SCHED_OTHER,
      // there is only one thread priority value, for example, 0.  So,
      // let the superuser run an interesting test, but for other
      // users use the minimum ACE_SCHED_OTHER thread priority.

      long fallback_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_OTHER,
                                        ACE_SCOPE_THREAD);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) task activation at priority %d with ")
                  ACE_TEXT ("flags 0x%X failed; retry at priority %d with ")
                  ACE_TEXT ("flags 0x%X (errno is %d%p)\n"),
                  priority,
                  flags,
                  fallback_priority,
                  THR_NEW_LWP,
                  errno,
                  ACE_TEXT ("")));

      flags = THR_NEW_LWP;
      priority = fallback_priority;

      if (task->activate (flags,
                          1,
                          1,
                          priority) == -1)
        {

            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%t) task activation at priority %d failed, ")
                        ACE_TEXT ("exiting!\n%a"),
                        priority,
                        -1));
        }
    }
  return 0;
}
