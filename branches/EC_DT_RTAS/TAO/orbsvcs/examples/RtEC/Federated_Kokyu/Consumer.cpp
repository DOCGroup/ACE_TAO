// $Id$

#include "Consumer.h"

#include "ace/High_Res_Timer.h"
#include "ace/ACE.h" //for is_prime()
#include "orbsvcs/orbsvcs/Time_Utilities.h" //ORBSVCS_Time

#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"

ACE_RCSID(EC_Examples, Consumer, "$Id$")

Consumer::Consumer (void)
{
}

void
Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t) no events\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG, "Consumer (%P|%t) we received event type %d\n",
              events[0].header.type));

  //@BT INSTRUMENT with event ID: EVENT_WORK_START Measure time
  //when work triggered by event starts.
  //DSUI_EVENT_LOG (TEST_ONE_FAM, START_SERVICE, guid, 0, NULL);
  DSUI_EVENT_LOG (TEST_ONE_FAM, START_SERVICE, 0, 0, NULL);

  //TODO: do work on push()
  ACE_High_Res_Timer timer;
  ACE_Time_Value elapsed_time;

//   ACE_DEBUG ((LM_DEBUG, "Request in thread %t\n"));

//   if (ACE_Thread::getprio (thr_handle, prio) == -1)
//     {
//       if (errno == ENOTSUP)
//      {
//        ACE_DEBUG((LM_DEBUG,
//                   ACE_TEXT ("getprio not supported on this platform\n")
//                   ));
//        return;
//      }
//       ACE_ERROR ((LM_ERROR,
//                       ACE_TEXT ("%p\n"),
//                       ACE_TEXT ("getprio failed"))
//                      );
//     }

//   ACE_DEBUG ((LM_DEBUG,
//               "Request in thread %t, prio = %d,"
//               "exec duration = %u\n", prio, exec_duration));

  static CORBA::ULong prime_number = 9619899;

  CORBA::Long exec_duration(1); //one second
  ACE_Time_Value compute_count_down_time (exec_duration, 0);
  ACE_Countdown_Time compute_count_down (&compute_count_down_time);

  //Applicable only for CV based implementations
  //yield every 1 sec
  //ACE_Time_Value yield_interval (0,100000);

  //ACE_Time_Value yield_count_down_time (yield_interval);
  //ACE_Countdown_Time yield_count_down (&yield_count_down_time);

  timer.start ();
  int j=0;
  while (compute_count_down_time > ACE_Time_Value::zero)
    {
      ACE::is_prime (prime_number,
                     2,
                     prime_number / 2);

      ++j;

//       if (j%1000 == 0)
//         {
//           ACE_Time_Value run_time = ACE_OS::gettimeofday ();
//           task_stats_.sample (ACE_UINT64 (run_time.msec ()), guid);
//         }

      compute_count_down.update ();

//       if (enable_yield_)
//         {
//           yield_count_down.update ();
//           if (yield_count_down_time <= ACE_Time_Value::zero)
//             {
//               CORBA::Policy_var sched_param_policy =
//                 CORBA::Policy::_duplicate (current_->
//                                            scheduling_parameter(ACE_ENV_SINGLE_ARG_PARAMETER));

//               const char * name = 0;

//               CORBA::Policy_var implicit_sched_param = sched_param_policy;
//               current_->update_scheduling_segment (name,
//                                                    sched_param_policy.in (),
//                                                    implicit_sched_param.in ()
//                                                    ACE_ENV_ARG_PARAMETER);
//               yield_count_down_time = yield_interval;
//               yield_count_down.start ();
//             }
//         }
    }

  TimeBase::TimeT current;
  ORBSVCS_Time::Time_Value_to_TimeT (current, ACE_OS::gettimeofday ());
  CORBA::Long temp = (long) current;
  CORBA::Long deadline(0); //no deadline specified
  if(temp > deadline )
    {
      this->deadline_missed_++;
    }

  timer.stop ();
  timer.elapsed_time (elapsed_time);

  ACE_DEBUG ((LM_DEBUG, "Consumer (%P|%t) request processing for %d done, "
              "elapsed time = %umsec, deadline_missed_=%d\n",
              events[0].header.type,elapsed_time.msec(),this->deadline_missed_));

//   ACE_DEBUG ((LM_DEBUG,
//               "Request processing in thread %t done, "
//               "prio = %d, load = %d, elapsed time = %umsec, deadline_missed = %d\n",
//               prio, exec_duration, elapsed_time.msec (),Deadline_missed ));

/*DTTIME:
  recording the finishing time on the server side. please also record the deadline_missed_ variable.
*/
  char* format = "Deadline missed: %d";
  char* extra_info = (char*) ACE_Allocator::instance()->malloc(strlen(format) + sizeof(this->deadline_missed_) - 2);
  if (extra_info != 0)
    {
      ACE_OS::sprintf(extra_info, "Deadline missed: %d", this->deadline_missed_);

      //@BT INSTRUMENT with event ID: EVENT_WORK_DEADLINE_MISSED Measure time when
      //work triggered by event finishes and deadline missed.
      //DSUI_EVENT_LOG (TEST_ONE_FAM, DEADLINE_MISSED, guid, strlen(extra_info), extra_info);
	DSUI_EVENT_LOG (TEST_ONE_FAM, DEADLINE_MISSED, 0, 0, NULL);

    }
  ACE_Allocator::instance()->free(extra_info);

  //@BT INSTRUMENT with event ID: EVENT_WORK_END Measure time when
  //work triggered by event finishes.
  //DSUI_EVENT_LOG (TEST_ONE_FAM, STOP_SERVICE, guid,0,NULL);
  DSUI_EVENT_LOG (TEST_ONE_FAM, STOP_SERVICE, 0, 0, NULL);

}

void
Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
