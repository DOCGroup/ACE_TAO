// $Id$

#include "Consumer.h"
#include "Supplier.h"
#include "Service_Handler.h"

#include "ace/Timeprobe.h"
#include "ace/High_Res_Timer.h"
#include "ace/Time_Value.h"
#include "ace/ACE.h" //for is_prime()
#include "orbsvcs/orbsvcs/Time_Utilities.h" //ORBSVCS_Time
#include <ace/Counter.h>
#include <ace/OS_NS_sys_time.h>

#if defined (ACE_HAS_DSUI)
#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"
#endif /* ACE_HAS_DSUI */

ACE_RCSID(EC_Examples, Consumer, "$Id$")

Consumer::Consumer (Supplier *fwddest, Service_Handler * handler)
  : worktime_(0,0),
    fwddest_(fwddest),
    handler_(handler)
{
}

Consumer::Consumer (ACE_Time_Value& worktime, Supplier *fwddest, Service_Handler *handler)
  : worktime_(worktime),
    fwddest_(fwddest),
    handler_(handler)
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

  if (this->handler_ != 0)
    {
      ACE_DEBUG((LM_DEBUG,"Consumer (%P|%t) calling handle_service_start()\n"));
      this->handler_->handle_service_start(events ACE_ENV_ARG_PARAMETER);
      ACE_DEBUG((LM_DEBUG,"Consumer (%P|%t) handle_service_start() DONE\n"));
    }

  //@BT INSTRUMENT with event ID: EVENT_WORK_START Measure time
  //when work triggered by event starts.
  //DSTRM_EVENT (TEST_ONE_FAM, START_SERVICE, guid, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Consumer (%P|%t) START_SERVICE at %u\n",ACE_OS::gettimeofday().msec()));

  Object_ID oid;
  oid.id = events[0].header.eid.id;
//  oid.tid = events[0].header.eid.tid;
//  oid.pid = events[0].header.eid.pid;
  oid.queue_id = events[0].header.eid.queue_id;
  oid.type = events[0].header.type;
  oid.pid = ACE_OS::getpid();
  oid.tid = ACE_OS::thr_self();

  ACE_TIMEPROBE("START_SERVICE");
  DSTRM_EVENT (TEST_ONE_FAM, START_SERVICE, 0, sizeof(Object_ID), (char*)&oid);

  ACE_High_Res_Timer timer;
  ACE_Time_Value elapsed_time;

  static CORBA::ULong prime_number = 9619899;

  ACE_DEBUG((LM_DEBUG,"Consumer (%P|%t) worktime is %isec %iusec\n",
             this->worktime_.sec(),this->worktime_.usec()));

  //There seems to be something wrong with using ACE_Countdown_Time, so we don't use it.
  //ACE_Time_Value compute_count_down_time (this->worktime_);
  //ACE_Countdown_Time compute_count_down (&compute_count_down_time); //auto-starts

  ACE_Time_Value start_time(ACE_OS::gettimeofday());
  timer.start();
  int j=0;
  while (elapsed_time <= this->worktime_)
    {
      //ACE_DEBUG((LM_DEBUG,"%isec %iusec elapsed\n",elapsed_time.sec(),elapsed_time.usec()));

      ACE::is_prime (prime_number,
                     2,
                     prime_number / 2);

      ++j;
      elapsed_time = ACE_OS::gettimeofday() - start_time;
    }

  //ACE_DEBUG((LM_DEBUG,"Consumer (%P|%t) elapsed %isec %iusec\n",elapsed_time.sec(),elapsed_time.usec()));

  timer.stop ();
  timer.elapsed_time (elapsed_time); //total elapsed time

  TimeBase::TimeT now;
  ORBSVCS_Time::Time_Value_to_TimeT (now, ACE_OS::gettimeofday ());
  if(now > events[0].header.deadline )
    {
      this->deadline_missed_++;

      //@BT INSTRUMENT with event ID: EVENT_WORK_DEADLINE_MISSED Measure time when
      //work triggered by event finishes and deadline missed.
      //DSTRM_EVENT (TEST_ONE_FAM, DEADLINE_MISSED, guid, strlen(extra_info), extra_info);
      ACE_DEBUG((LM_DEBUG,"Consumer in thread %t STOP_SERVICE (DEADLINE_MISSED) at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (TEST_ONE_FAM, DEADLINE_MISSED, 0, sizeof(Object_ID), (char*)&oid);
    }

  //@BT INSTRUMENT with event ID: EVENT_WORK_END Measure time when
  //work triggered by event finishes.
  //DSTRM_EVENT (TEST_ONE_FAM, STOP_SERVICE, guid,0,NULL);
  ACE_DEBUG((LM_DEBUG,"Consumer in thread %t STOP_SERVICE for event type %d at %u\n",events[0].header.type,ACE_OS::gettimeofday().msec()));
  DSTRM_EVENT (TEST_ONE_FAM, STOP_SERVICE, 0, sizeof(Object_ID), (char*)&oid);

  ACE_TIMEPROBE("STOP_SERVICE");

  //now print timeprobe values
  ACE_TIMEPROBE_PRINT;
  ACE_TIMEPROBE_RESET;

  ACE_DEBUG ((LM_DEBUG, "Consumer (%P|%t) request processing for %d done, "
              "elapsed time = %isec %iusec, deadline_missed_=%d\n",
              events[0].header.type,elapsed_time.sec(),elapsed_time.usec(),
              this->deadline_missed_));
  ACE_DEBUG((LM_DEBUG,"Consumer (%P|%t) processing took %d iterations\n",j));
  ACE_DEBUG((LM_DEBUG,"Consumer (%P|%t) event had deadline %i\n",
             events[0].header.deadline));

//   ACE_DEBUG ((LM_DEBUG,
//               "Request processing in thread %t done, "
//               "prio = %d, load = %d, elapsed time = %umsec, deadline_missed = %d\n",
//               prio, exec_duration, elapsed_time.msec (),Deadline_missed ));

  //now, trigger the next subtask if any
  if (this->fwddest_ != 0)
    {
      //trigger next subtask
      this->fwddest_->timeout_occured(oid ACE_ENV_ARG_PARAMETER);
    }

  if (this->handler_ != 0)
    {
      ACE_DEBUG((LM_DEBUG,"Consumer (%P|%t) calling handle_service_stop()\n"));
      this->handler_->handle_service_stop(events ACE_ENV_ARG_PARAMETER);
      ACE_DEBUG((LM_DEBUG,"Consumer (%P|%t) handle_service_stop() DONE\n"));
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

void
Consumer::rt_info(RtecScheduler::handle_t consumer_rt_info)
{
  rt_info_ = consumer_rt_info;
}

RtecScheduler::handle_t
Consumer::rt_info(void) const
{
  return rt_info_;
}

void
Consumer::handler(Service_Handler * handler)
{
  this->handler_ = handler;
}

Service_Handler *
Consumer::handler(void) const
{
  return this->handler_;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
