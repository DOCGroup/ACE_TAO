// $Id$

#include "EC_Sched_Filter.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "EC_Sched_Filter_Builder.h"
#include "EC_Type_Filter.h"
#include "EC_Conjunction_Filter.h"
#include "EC_Disjunction_Filter.h"
#include "EC_Timeout_Filter.h"
#include "EC_Event_Channel.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Sched_Filter_Builder.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Sched_Filter_Builder, "$Id$")

TAO_EC_Sched_Filter_Builder::~TAO_EC_Sched_Filter_Builder (void)
{
}

TAO_EC_Filter*
TAO_EC_Sched_Filter_Builder::build (
    TAO_EC_ProxyPushSupplier *supplier,
    RtecEventChannelAdmin::ConsumerQOS& qos,
    CORBA::Environment &ACE_TRY_ENV) const
{
  CORBA::ULong pos = 0;
  RtecScheduler::Scheduler_var scheduler =
    this->event_channel_->scheduler ();
  return this->recursive_build (supplier, qos, pos,
                                scheduler.in (), 0,
                                ACE_TRY_ENV);
}

TAO_EC_Filter*
TAO_EC_Sched_Filter_Builder:: recursive_build (
    TAO_EC_ProxyPushSupplier *supplier,
    RtecEventChannelAdmin::ConsumerQOS& qos,
    CORBA::ULong& pos,
    RtecScheduler::Scheduler_ptr scheduler,
    const char* base_name,
    CORBA::Environment& ACE_TRY_ENV) const
{
  const RtecEventComm::Event& e = qos.dependencies[pos].event;
  ACE_CString name;
  RtecScheduler::handle_t body_info = qos.dependencies[pos].rt_info;
  if (base_name == 0)
    {
      RtecScheduler::RT_Info_var info =
        scheduler->get (body_info, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      name = info->entry_point.in ();
    }
  else
    name = base_name;

  if (e.header.type == ACE_ES_CONJUNCTION_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
      for (CORBA::ULong i = 0; i != n; ++i)
        {
          ACE_CString child_name = name;
          char buf[16];
          ACE_OS::sprintf (buf, "/%04.4x", i);
          child_name += buf;
          children[i] = this->recursive_build (supplier, qos, pos,
                                               scheduler,
                                               child_name.c_str (),
                                               ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
          pos++;
        }
      return new TAO_EC_Sched_Filter (name.c_str (),
                                      scheduler,
                                      new TAO_EC_Conjunction_Filter(children,
                                                                    n),
                                      body_info,
                                      RtecScheduler::CONJUNCTION);
    }

  else if (e.header.type == ACE_ES_DISJUNCTION_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
      for (CORBA::ULong i = 0; i != n; ++i)
        {
          ACE_CString child_name = name;
          char buf[16];
          ACE_OS::sprintf (buf, "/%04.4x", i);
          child_name += buf;

          children[i] = this->recursive_build (supplier, qos, pos,
                                               scheduler,
                                               child_name.c_str (),
                                               ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);
          pos++;
        }
      return new TAO_EC_Sched_Filter (name.c_str (),
                                      scheduler,
                                      new TAO_EC_Disjunction_Filter (children,
                                                                     n),
                                      body_info,
                                      RtecScheduler::DISJUNCTION);
    }

  else if (e.header.type == ACE_ES_EVENT_TIMEOUT
           || e.header.type == ACE_ES_EVENT_INTERVAL_TIMEOUT
           || e.header.type == ACE_ES_EVENT_DEADLINE_TIMEOUT)
    {
      pos++;
      TAO_EC_QOS_Info qos_info;

      qos_info.rt_info =
        scheduler->create (name.c_str (), ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      // Provide dummy values the scheduler will compute them based on the
      // dependencies and the fact that this is a DISJUNCTION.
      scheduler->set (qos_info.rt_info,
                      RtecScheduler::VERY_LOW_CRITICALITY,
                      0, // worst_cast_execution_time
                      0, // typical_cast_execution_time
                      0, // cached_cast_execution_time
                      e.header.creation_time, // period
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      0, // quantum
                      0, // threads
                      RtecScheduler::OPERATION,
                      ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      return new TAO_EC_Timeout_Filter (this->event_channel_,
                                        supplier,
                                        qos_info,
                                        e.header.type,
                                        e.header.creation_time);
    }

  return new TAO_EC_Sched_Filter (name.c_str (),
                                  scheduler,
                                  new TAO_EC_Type_Filter (e.header),
                                  body_info,
                                  RtecScheduler::OPERATION);
}

CORBA::ULong
TAO_EC_Sched_Filter_Builder::
    count_children (RtecEventChannelAdmin::ConsumerQOS& qos,
                    CORBA::ULong pos) const
{
  CORBA::ULong l = qos.dependencies.length ();
  CORBA::ULong i;
  for (i = pos; i != l; ++i)
    {
      const RtecEventComm::Event& e = qos.dependencies[i].event;
      if (e.header.type == ACE_ES_CONJUNCTION_DESIGNATOR
          || e.header.type == ACE_ES_DISJUNCTION_DESIGNATOR)
        break;
    }
  return i - 1;
}
