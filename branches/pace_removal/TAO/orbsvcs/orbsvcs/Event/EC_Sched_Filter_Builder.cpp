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
    RtecEventChannelAdmin::ConsumerQOS& qos
    ACE_ENV_ARG_DECL) const
{
  CORBA::ULong pos = 0;
  CORBA::Object_var tmp =
    this->event_channel_->scheduler ();

  RtecScheduler::Scheduler_var scheduler =
    RtecScheduler::Scheduler::_narrow (tmp.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // @@ How do we figure out which parent???
  RtecScheduler::handle_t parent_info =
    scheduler->lookup ("Dispatching_Task-250000.us" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->recursive_build (supplier, qos, pos,
                                scheduler.in (),
                                parent_info
                                 ACE_ENV_ARG_PARAMETER);
}

TAO_EC_Filter*
TAO_EC_Sched_Filter_Builder::recursive_build (
    TAO_EC_ProxyPushSupplier *supplier,
    RtecEventChannelAdmin::ConsumerQOS& qos,
    CORBA::ULong& pos,
    RtecScheduler::Scheduler_ptr scheduler,
    RtecScheduler::handle_t parent_info
    ACE_ENV_ARG_DECL) const
{
  const RtecEventComm::Event& e = qos.dependencies[pos].event;

  if (e.header.type == ACE_ES_CONJUNCTION_DESIGNATOR)
    {
      CORBA::ULong npos = pos;
      ACE_CString name;
      this->recursive_name (qos, npos,
                            scheduler, name
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      RtecScheduler::handle_t rt_info =
        scheduler->create (name.c_str () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      pos++; // Consume the designator
      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
      for (CORBA::ULong i = 0; i != n; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos,
                                               scheduler,
                                               rt_info
                                                ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }

      TAO_EC_Sched_Filter *filter;
      ACE_NEW_RETURN (filter,
                      TAO_EC_Sched_Filter (name.c_str (),
                                           rt_info,
                                           scheduler,
                                           new TAO_EC_Conjunction_Filter(children,
                                                                         n),
                                           rt_info,
                                           parent_info,
                                           RtecScheduler::CONJUNCTION),
                      0);
      TAO_EC_QOS_Info qos_info;
      filter->get_qos_info (qos_info ACE_ENV_ARG_PARAMETER);
      // @@
      ACE_CHECK_RETURN (0);
      return filter;
    }

  else if (e.header.type == ACE_ES_DISJUNCTION_DESIGNATOR)
    {
      CORBA::ULong npos = pos;
      ACE_CString name;
      this->recursive_name (qos, npos,
                            scheduler, name
                             ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      RtecScheduler::handle_t rt_info =
        scheduler->create (name.c_str () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      pos++; // Consume the designator
      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
      for (CORBA::ULong i = 0; i != n; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos,
                                               scheduler,
                                               rt_info
                                                ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
      TAO_EC_Sched_Filter *filter;
      ACE_NEW_RETURN (filter,
                      TAO_EC_Sched_Filter (name.c_str (),
                                           rt_info,
                                           scheduler,
                                           new TAO_EC_Disjunction_Filter (children,
                                                                          n),
                                           rt_info,
                                           parent_info,
                                           RtecScheduler::DISJUNCTION),
                      0);

      TAO_EC_QOS_Info qos_info;
      filter->get_qos_info (qos_info ACE_ENV_ARG_PARAMETER);
      // @@
      ACE_CHECK_RETURN (0);
      return filter;
    }

  else if (e.header.type == ACE_ES_EVENT_TIMEOUT
           || e.header.type == ACE_ES_EVENT_INTERVAL_TIMEOUT
           || e.header.type == ACE_ES_EVENT_DEADLINE_TIMEOUT)
    {
      pos++;

      // @@ We need a unique name for each timeout, assigned by the
      //    application?
      char buf[64];
      ACE_OS::sprintf (buf, "TIMEOUT:%u",
                       ACE_static_cast (u_int,
                                        (e.header.creation_time / 10000)));
      ACE_CString name = buf;

      TAO_EC_QOS_Info qos_info;
      qos_info.rt_info =
        scheduler->create (name.c_str () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // Convert the time to the proper units....
      RtecScheduler::Period_t period =
        ACE_static_cast (RtecScheduler::Period_t,
                         e.header.creation_time / 10);

      scheduler->set (qos_info.rt_info,
                      RtecScheduler::VERY_LOW_CRITICALITY,
                      0, // worst_cast_execution_time
                      0, // typical_cast_execution_time
                      0, // cached_cast_execution_time
                      period,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      0, // quantum
                      1, // threads
                      RtecScheduler::OPERATION
                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      scheduler->add_dependency (qos_info.rt_info,
                                 parent_info,
                                 1,
                                 RtecBase::TWO_WAY_CALL
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return new TAO_EC_Timeout_Filter (this->event_channel_,
                                        supplier,
                                        qos_info,
                                        e.header.type,
                                        e.header.creation_time);
    }

  RtecScheduler::handle_t body_info = qos.dependencies[pos].rt_info;

  RtecScheduler::RT_Info_var info =
    scheduler->get (body_info ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_CString name = info->entry_point.in ();
  name += "#rep";

  RtecScheduler::handle_t rt_info =
    scheduler->create (name.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  pos++;
  TAO_EC_Sched_Filter *filter;
  ACE_NEW_RETURN (filter,
                  TAO_EC_Sched_Filter (name.c_str (),
                                       rt_info,
                                       scheduler,
                                       new TAO_EC_Type_Filter (e.header),
                                       body_info,
                                       parent_info,
                                       RtecScheduler::OPERATION),
                  0);

  TAO_EC_QOS_Info qos_info;
  filter->get_qos_info (qos_info ACE_ENV_ARG_PARAMETER);
  // @@
  ACE_CHECK_RETURN (0);
  return filter;
}

void
TAO_EC_Sched_Filter_Builder:: recursive_name (
    RtecEventChannelAdmin::ConsumerQOS& qos,
    CORBA::ULong& pos,
    RtecScheduler::Scheduler_ptr scheduler,
    ACE_CString& name
    ACE_ENV_ARG_DECL) const
{
  const RtecEventComm::Event& e = qos.dependencies[pos].event;

  if (e.header.type == ACE_ES_CONJUNCTION_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = this->count_children (qos, pos);

      for (CORBA::ULong i = 0; i != n; ++i)
        {
          ACE_CString child_name;
          this->recursive_name (qos, pos,
                                scheduler,
                                child_name
                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (i == 0)
            name += "(";
          else
            name += "&&";
          name += child_name;
        }
      name += ")";
      return;
    }

  else if (e.header.type == ACE_ES_DISJUNCTION_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = this->count_children (qos, pos);

      for (CORBA::ULong i = 0; i != n; ++i)
        {
          ACE_CString child_name;

          this->recursive_name (qos, pos,
                                scheduler,
                                child_name
                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (i == 0)
            name += "(";
          else
            name += "||";
          name += child_name;
        }
      name += ")";
      return;
    }

  else if (e.header.type == ACE_ES_EVENT_TIMEOUT
           || e.header.type == ACE_ES_EVENT_INTERVAL_TIMEOUT
           || e.header.type == ACE_ES_EVENT_DEADLINE_TIMEOUT)
    {
      pos++;

      char buf[64];
      ACE_OS::sprintf (buf, "TIMEOUT:%u",
                       ACE_static_cast (u_int,
                                        (e.header.creation_time / 10000)));
      name = buf;

      return;
    }

  RtecScheduler::handle_t body_info = qos.dependencies[pos].rt_info;

  RtecScheduler::RT_Info_var info =
    scheduler->get (body_info ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  name = info->entry_point.in ();
  name += "#rep";

  pos++;
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
