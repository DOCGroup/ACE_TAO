// $Id$

#include "test_i.h"
#include "ace/High_Res_Timer.h"
#include "ace/Task.h"
#include "ace/ACE.h"
#include "tao/debug.h"
#include "ace/Countdown_Time.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

CORBA::Long
Simple_Server_i::test_method (CORBA::Long exec_duration)
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;
  int guid;
  RTScheduling::Current::IdType_var id = this->current_->id ();

  ACE_OS::memcpy (&guid,
                  id->get_buffer (),
                  sizeof (id->length ()));

  ACE_High_Res_Timer timer;
  ACE_Time_Value elapsed_time;

  ACE_DEBUG ((LM_DEBUG, "Request in thread %t\n"));

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
        {
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT ("getprio not supported on this platform\n")));
          return 0;
        }

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("getprio failed")),
                        -1);
    }

  ACE_DEBUG ((LM_DEBUG,
              "Request in thread %t, prio = %d,"
              "exec duration = %u\n", prio, exec_duration));

  static CORBA::ULong prime_number = 9619899;

  ACE_Time_Value compute_count_down_time (exec_duration, 0);
  ACE_Countdown_Time compute_count_down (&compute_count_down_time);

  //Applicable only for CV based implementations
  //yield every 1 sec
  ACE_Time_Value yield_interval (1,0);

  ACE_Time_Value yield_count_down_time (yield_interval);
  ACE_Countdown_Time yield_count_down (&yield_count_down_time);

  timer.start ();
  int j=0;
  while (compute_count_down_time > ACE_Time_Value::zero)
    {
      ACE::is_prime (prime_number,
                     2,
                     prime_number / 2);

      ++j;

#ifdef KOKYU_DSRT_LOGGING
      if (j%1000 == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
            "(%t|%T) loop # = %d, load = %usec\n", j, exec_duration));
        }
#endif
      if (j%1000 == 0)
        {
          ACE_Time_Value run_time = ACE_OS::gettimeofday ();
          task_stats_.sample (ACE_UINT32 (run_time.msec ()), guid);
        }

      compute_count_down.update ();

      if (enable_yield_)
        {
          yield_count_down.update ();
          if (yield_count_down_time <= ACE_Time_Value::zero)
            {
              CORBA::Policy_var sched_param_policy =
                current_->scheduling_parameter();

              const char * name = 0;

              CORBA::Policy_ptr implicit_sched_param = 0;
              current_->update_scheduling_segment (name,
                                                   sched_param_policy.in (),
                                                   implicit_sched_param);
              yield_count_down_time = yield_interval;
              yield_count_down.start ();
            }
        }
    }

  timer.stop ();
  timer.elapsed_time (elapsed_time);

  ACE_DEBUG ((LM_DEBUG,
              "Request processing in thread %t done, "
              "prio = %d, load = %d, elapsed time = %umsec\n",
              prio, exec_duration, elapsed_time.msec () ));

  return exec_duration;
}

void
Simple_Server_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "shutdown request from client\n"));
  this->orb_->shutdown (0);
}
