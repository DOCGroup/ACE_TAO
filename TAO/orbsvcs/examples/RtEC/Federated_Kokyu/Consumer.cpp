// $Id$

#include "Consumer.h"
#include "Supplier.h"

#include "ace/High_Res_Timer.h"
#include "ace/Time_Value.h"
#include "ace/ACE.h" //for is_prime()
#include "orbsvcs/orbsvcs/Time_Utilities.h" //ORBSVCS_Time
#include <ace/Counter.h>

#if defined (ACE_HAS_DSUI)
#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"
#endif /* ACE_HAS_DSUI */

ACE_RCSID(EC_Examples, Consumer, "$Id$")

Consumer::Consumer (Supplier *fwddest)
  : worktime_(0,0),
    fwddest_(fwddest)
{
}

Consumer::Consumer (ACE_Time_Value& worktime, Supplier *fwddest)
  : worktime_(worktime),
    fwddest_(fwddest)
{
}

void
Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL)
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
  ACE_Time_Value tv = ACE_OS::gettimeofday();
  ACE_DEBUG((LM_DEBUG,"Consumer in thread %t START_SERVICE at %u\n",tv.msec()));

  Object_ID oid;
  oid.id = events[0].header.eid.id;
  oid.tid = events[0].header.eid.tid;
  oid.queue_id = events[0].header.eid.queue_id;

  DSUI_EVENT_LOG (TEST_ONE_FAM, START_SERVICE, 0, sizeof(Object_ID), (char*)&oid);

  //TODO: do work on push()
  ACE_High_Res_Timer timer;
  ACE_Time_Value elapsed_time;


  static CORBA::ULong prime_number = 9619899;

  ACE_Time_Value compute_count_down_time (this->worktime_);
  ACE_Countdown_Time compute_count_down (&compute_count_down_time);

  timer.start ();
  int j=0;
  while (compute_count_down_time > ACE_Time_Value::zero)
    {
      ACE::is_prime (prime_number,
                     2,
                     prime_number / 2);

      ++j;

      compute_count_down.update ();
    }

  TimeBase::TimeT current;
  ORBSVCS_Time::Time_Value_to_TimeT (current, ACE_OS::gettimeofday ());
  CORBA::Long temp = (long) current;
  //TODO: How do I know when the deadline was?
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
  char* extra_info = (char*) ACE_Allocator::instance()->malloc(strlen(format) + sizeof(this->deadline_missed_) - 1);
  if (extra_info != 0)
    {
      ACE_OS::sprintf(extra_info, "Deadline missed: %d", this->deadline_missed_);

      //@BT INSTRUMENT with event ID: EVENT_WORK_DEADLINE_MISSED Measure time when
      //work triggered by event finishes and deadline missed.
      //DSUI_EVENT_LOG (TEST_ONE_FAM, DEADLINE_MISSED, guid, strlen(extra_info), extra_info);
      tv = ACE_OS::gettimeofday();
      ACE_DEBUG((LM_DEBUG,"Consumer in thread %t STOP_SERVICE (DEADLINE_MISSED) at %u\n",tv.msec()));
      DSUI_EVENT_LOG (TEST_ONE_FAM, DEADLINE_MISSED, 0, sizeof(Object_ID), (char*)&oid);

    }
  ACE_Allocator::instance()->free(extra_info);

  //@BT INSTRUMENT with event ID: EVENT_WORK_END Measure time when
  //work triggered by event finishes.
  //DSUI_EVENT_LOG (TEST_ONE_FAM, STOP_SERVICE, guid,0,NULL);
  tv = ACE_OS::gettimeofday();
  ACE_DEBUG((LM_DEBUG,"Consumer in thread %t STOP_SERVICE (DEADLINE_MADE) at %u\n",tv.msec()));
  DSUI_EVENT_LOG (TEST_ONE_FAM, STOP_SERVICE, 0, sizeof(Object_ID), (char*)&oid);

  //now, trigger the next subtask if any
  if (this->fwddest_ != 0)
    {
      //trigger next subtask
      this->fwddest_->timeout_occured(ACE_ENV_SINGLE_ARG_PARAMETER);
    }
}

void
Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Consumer::setWorkTime(ACE_Time_Value& worktime)
{
  this->worktime_.set(worktime.sec(),worktime.usec());
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
