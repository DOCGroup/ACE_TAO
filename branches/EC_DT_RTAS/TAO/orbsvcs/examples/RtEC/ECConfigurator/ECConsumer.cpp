// $Id$

#include "ECConsumer.h"
#include "ECSupplier.h"
#include "Service_Handler.h"

#include "ace/Timeprobe.h"
#include "ace/High_Res_Timer.h"
#include "ace/Time_Value.h"
#include "ace/ACE.h" //for is_prime()
#include "orbsvcs/orbsvcs/Time_Utilities.h" //ORBSVCS_Time
#include <ace/Counter.h>
#include <ace/OS_NS_sys_time.h>

#include <algorithm>

ACE_RCSID(EC_Examples, ECConsumer, "$Id$")

ECConsumer::ECConsumer (EventTypeVector &sub_types, Service_Handler * handler)
  : _worktime(0,0)
  , handler_(handler)
  , sub_types_(sub_types)
{
}

ECConsumer::ECConsumer (EventTypeVector &sub_types,
                        ACE_Time_Value& worktime,
                        Service_Handler *handler)
  : _worktime(worktime)
  , handler_(handler)
  , sub_types_(sub_types)
{
}

ECConsumer::~ECConsumer(void)
{
}

void
ECConsumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"ECConsumer (%P|%t) no events\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG, "ECConsumer (%P|%t) we received event type %d\n",
              events[0].header.type));

  if (this->handler_ != 0)
    {
      ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) calling handle_service_start()\n"));
      this->handler_->handle_service_start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) handle_service_start() DONE\n"));
    }

  if (std::find(this->sub_types_.begin(),this->sub_types_.end(),events[0].header.type) == this->sub_types_.end())
    {
      ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) received unknown type %d; ignoring\n",events[0].header.type));
      return;
    }

  ACE_High_Res_Timer timer;
  ACE_Time_Value elapsed_time(ACE_Time_Value::zero);

  static CORBA::ULong prime_number = 9619899;

  ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) worktime is %isec %iusec\n",
             this->_worktime.sec(),this->_worktime.usec()));

  ACE_Time_Value start_time(ACE_OS::gettimeofday());
  timer.start();
  int j=0;
  while (elapsed_time <= this->_worktime)
    {
      //ACE_DEBUG((LM_DEBUG,"%isec %iusec elapsed\n",elapsed_time.sec(),elapsed_time.usec()));

      ACE::is_prime (prime_number,
                     2,
                     prime_number / 2);

      ++j;
      elapsed_time = ACE_OS::gettimeofday() - start_time;
    }

  //ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) elapsed %isec %iusec\n",elapsed_time.sec(),elapsed_time.usec()));

  timer.stop ();
  timer.elapsed_time (elapsed_time); //total elapsed time

  ACE_DEBUG ((LM_DEBUG, "ECConsumer (%P|%t) request processing for %d done, "
              "elapsed time = %isec %iusec\n",
              events[0].header.type,elapsed_time.sec(),elapsed_time.usec()));
  ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) processing took %d iterations\n",j));
  ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) event had deadline %i\n",
             events[0].header.deadline));

//   ACE_DEBUG ((LM_DEBUG,
//               "Request processing in thread %t done, "
//               "prio = %d, load = %d, elapsed time = %umsec\n",
//               prio, exec_duration, elapsed_time.msec ()));

  //now, trigger the next subtask if any
  ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) triggering next subtasks\n"));
  SupplierVector::iterator siter = this->dependants_.begin();
  for(; siter != this->dependants_.end(); ++siter)
    {
      SupplierVector::value_type supplier = *siter;
      ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) processing dependant %d\n",supplier->get_id()));
      supplier->timeout_occured(ACE_ENV_SINGLE_ARG_PARAMETER);
    }

  ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) finished processing dependants\n"));
  // DEBUG
//   static int t = 0;
//   if (t++ >= 2)
//     this->_worktime.set(0,200000);
  // END DEBUG

  if (this->handler_ != 0)
    {
      ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) calling handle_service_stop()\n"));
      this->handler_->handle_service_stop(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_DEBUG((LM_DEBUG,"ECConsumer (%P|%t) handle_service_stop() DONE\n"));
    }
}

void
ECConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ECConsumer::setWorkTime(ACE_Time_Value& worktime)
{
  this->_worktime.set(worktime.sec(),worktime.usec());
}

void
ECConsumer::rt_info(InfoHandle consumer_rt_info)
{
  rt_info_ = consumer_rt_info;
}

ECConsumer::InfoHandle
ECConsumer::rt_info(void)
{
  return rt_info_;
}

void
ECConsumer::handler(Service_Handler * handler)
{
  this->handler_ = handler;
}

Service_Handler *
ECConsumer::handler(void) const
{
  return this->handler_;
}

void
ECConsumer::pushDependant(ECSupplier *dep)
{
  this->dependants_.push_back(dep);
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
