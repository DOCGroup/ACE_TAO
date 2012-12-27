//$Id$
#include "Thread_Task.h"

#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_unistd.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"

#include "Activity.h"
#include "Task_Stats.h"
#include "ace/Barrier.h"

Thread_Task::Thread_Task (void)
{
}

int
Thread_Task::activate_task (ACE_Barrier* barrier, RTCORBA::PriorityMapping *priority_mapping)
{
  barrier_ = barrier;

  // Convert the priority specified to this class to its native number.
  RTCORBA::NativePriority native_priority;

  if (priority_mapping->to_native (this->task_priority_, native_priority) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot convert CORBA priority %d to native priority\n",
                       this->task_priority_),
                      -1);

  long flags =
    THR_NEW_LWP |
    THR_JOINABLE |
    ACTIVITY::instance()->orb ()->orb_core ()->orb_params ()->thread_creation_flags ();

  // Become an active object.
  if (this->activate (flags,
                      1,
                      0,
                      native_priority) == -1)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Insufficient privilege to run this test.\n")),
                          -1);
      else
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("(%t) task activation at priority %d failed, ")
                    ACE_TEXT ("exiting!\n%a"),
                    native_priority,
                    -1));
    }
  return 0;
}

int
Thread_Task::svc (void)
{
  // if debugging, dump the priority that we're actually at.
  if (TAO_debug_level > 0)
    {

      // Get the priority of the current thread.
      RTCORBA::Priority prio =
        ACTIVITY::instance()->current ()->the_priority ();

      if (prio == this->task_priority_)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) actual prio of %d equals desired priority\n"),
                    prio));
      else
        {
          ACE_DEBUG ((LM_ERROR,
                      ACE_TEXT ("(%t) actual prio = %d, desired priority_ = %d!\n"),
                      prio,
                      this->task_priority_));
        }
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Thread_Task (%t) - wait\n"));

  // First, wait for other threads.
  this->barrier_->wait ();

  // first thread here inits the Base_Time.
  task_stats_->base_time (BASE_TIME::instance ()->base_time_);

  // now wait till the phase_ period expires.
  ACE_OS::sleep (ACE_Time_Value (0, phase_));

  ACE_High_Res_Timer::global_scale_factor_type gsf =
    ACE_High_Res_Timer::global_scale_factor ();

  ACE_hrtime_t before, after;

  for (int i = 0; i < iter_ ; ++i)
    {
      before = ACE_OS::gethrtime ();

      job_->work (load_);

      after = ACE_OS::gethrtime ();

      task_stats_->sample (before, after);

      if (period_ != 0) // blast mode, no sleep.
        {
          // convert to microseconds
          ACE_UINT32 elapsed_microseconds = ACE_UINT32((after - before) / gsf);

#if defined (ACE_WIN32)
          elapsed_microseconds*=1000; // convert to uSec on Win32
#endif /* ACE_WIN32 */

          // did we miss any deadlines?

          int const missed =
            elapsed_microseconds > period_ ? elapsed_microseconds/period_ : 0;

          long sleep_time = (missed + 1)*period_ ;
          sleep_time -= elapsed_microseconds;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "(%t) sleep time = %d\n", sleep_time));

          ACE_Time_Value t_sleep (0, sleep_time);
          ACE_OS::sleep (t_sleep);
        } /* period != 0 */
    } /* for */

  task_stats_->end_time (ACE_OS::gethrtime ());

  job_->shutdown (); // tell the job that we're done.

  ACTIVITY::instance ()->task_ended (this);

  return 0;
}
