// $Id$

#include "orbsvcs/Event/EC_Kokyu_Filter.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Kokyu_Filter_Builder.h"
#include "orbsvcs/Event/EC_Type_Filter.h"
#include "orbsvcs/Event/EC_Conjunction_Filter.h"
#include "orbsvcs/Event/EC_Disjunction_Filter.h"
#include "orbsvcs/Event/EC_Timeout_Filter.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "ace/OS_NS_stdio.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Kokyu_Filter_Builder.inl"
#endif /* __ACE_INLINE__ */

const char* designator (long dsgn)
{
  switch(dsgn)
    {
    case ACE_ES_GLOBAL_DESIGNATOR: return "GLOBAL";
    case ACE_ES_CONJUNCTION_DESIGNATOR: return "CONJ";
    case ACE_ES_DISJUNCTION_DESIGNATOR: return "DISJ";
    case ACE_ES_NEGATION_DESIGNATOR: return "NEG";
    case ACE_ES_LOGICAL_AND_DESIGNATOR: return "LOG_AND";
    case ACE_ES_BITMASK_DESIGNATOR: return "BITMASK";
    case ACE_ES_MASKED_TYPE_DESIGNATOR: return "MASKED_TYPE";
    case ACE_ES_NULL_DESIGNATOR: return "NULL";
    }

  return "---";
}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Kokyu_Filter_Builder::~TAO_EC_Kokyu_Filter_Builder (void)
{
}

TAO_EC_Filter*
TAO_EC_Kokyu_Filter_Builder::build (
    TAO_EC_ProxyPushSupplier *supplier,
    RtecEventChannelAdmin::ConsumerQOS& qos) const
{
  CORBA::ULong i=0,found=0;
  CORBA::ULong pos = 0;
  CORBA::Long npos = -1;
  int establish_final_consumer_dependency=0;

  CORBA::Object_var tmp =
    this->event_channel_->scheduler ();

  RtecScheduler::Scheduler_var scheduler =
    RtecScheduler::Scheduler::_narrow (tmp.in ());

#ifdef EC_KOKYU_LOGGING
  for (i=0; i<qos.dependencies.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "consumerqos[%d] event.header.type = %s,"
                  "rt_info = %d\n",
                  i,
                  designator (qos.dependencies[i].event.header.type),
                  qos.dependencies[i].rt_info));
    }
#endif

  //find the first entry which is not a designator. We are going to
  //assume that this entry will have the rt_info of the connecting
  //consumer (ProxyPushSupplier), which is passed into this function.
  for (i=0; !found && i<qos.dependencies.length (); ++i)
    {
      switch (qos.dependencies[i].event.header.type)
        {
        case ACE_ES_CONJUNCTION_DESIGNATOR:
        case ACE_ES_DISJUNCTION_DESIGNATOR:
        case ACE_ES_NEGATION_DESIGNATOR:
        case ACE_ES_LOGICAL_AND_DESIGNATOR:
        case ACE_ES_BITMASK_DESIGNATOR:
        case ACE_ES_MASKED_TYPE_DESIGNATOR:
        case ACE_ES_NULL_DESIGNATOR:
          establish_final_consumer_dependency = 1;
          continue;

        case ACE_ES_GLOBAL_DESIGNATOR:
        case ACE_ES_EVENT_TIMEOUT:
        case ACE_ES_EVENT_INTERVAL_TIMEOUT:
        case ACE_ES_EVENT_DEADLINE_TIMEOUT:
          continue;

        default:
          npos = i;
          found = 1;
          break;
        }
    }

  ACE_CString final_consumer_rep_name;
  RtecScheduler::handle_t h_final_consumer_rt_info = 0;
  RtecScheduler::handle_t h_final_consumer_rep_rt_info = 0;

#ifdef EC_KOKYU_LOGGING
  ACE_DEBUG ((LM_DEBUG, "consumer rt_info found in consumerqos[%d]\n", npos));
#endif

  if (npos >= 0 && establish_final_consumer_dependency == 1)
    {
      //Hopefully this will have the final consumer's rt_info
      h_final_consumer_rt_info = qos.dependencies[npos].rt_info;

#ifdef EC_KOKYU_LOGGING
      ACE_DEBUG ((LM_DEBUG, "about to get rt_info =  %d\n",
                  h_final_consumer_rep_rt_info));
#endif

      RtecScheduler::RT_Info_var final_consumer_rt_info =
        scheduler->get ( h_final_consumer_rt_info);

      final_consumer_rep_name = final_consumer_rt_info->entry_point.in ();
      final_consumer_rep_name += "#rep";

#ifdef EC_KOKYU_LOGGING
      ACE_DEBUG ((LM_DEBUG, "about to create consumer rep %s\n",
                  final_consumer_rep_name.c_str ()));
#endif

      //create an rt_info corresponding to this rep.
      h_final_consumer_rep_rt_info =
        scheduler->create (final_consumer_rep_name.c_str ());
#ifdef EC_KOKYU_LOGGING
      ACE_DEBUG ((LM_DEBUG, "consumer rep created\n"));
#endif

    }

  //We are passing the final consumer as the parent. The final
  //consumer is the one which is connecting to the ProxyPushSupplier
  //passed in to this function.

  TAO_EC_Filter* filter =
    this->recursive_build (supplier, qos, pos,
                           scheduler.in (),
                           h_final_consumer_rep_rt_info  //parent_info
                           );

#ifdef EC_KOKYU_LOGGING
  ACE_DEBUG ((LM_DEBUG,
              "Filter_Builder::Verifying whether root filter"
              " dependency can be established\n"));
#endif

  if (npos >= 0 && establish_final_consumer_dependency == 1)
    {
#ifdef EC_KOKYU_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "Filter_Builder::root filter dependency "
                  "can be established\n"));
#endif
      TAO_EC_Kokyu_Filter* kokyu_filter =
        dynamic_cast<TAO_EC_Kokyu_Filter*> (filter);

      //add the dependency between the root in the filter hierarchy and
      //the final consumer
      TAO_EC_QOS_Info qos_info;
      kokyu_filter->get_qos_info (qos_info);

      scheduler->add_dependency (h_final_consumer_rt_info,
                                 qos_info.rt_info,
                                 1,
                                 RtecBase::ONE_WAY_CALL);
    }
  return filter;
}

TAO_EC_Filter*
TAO_EC_Kokyu_Filter_Builder::recursive_build (
    TAO_EC_ProxyPushSupplier *supplier,
    RtecEventChannelAdmin::ConsumerQOS& qos,
    CORBA::ULong& pos,
    RtecScheduler::Scheduler_ptr scheduler,
    RtecScheduler::handle_t parent_info) const
{
  const RtecEventComm::Event& e = qos.dependencies[pos].event;

#ifdef EC_KOKYU_LOGGING
  ACE_DEBUG ((LM_DEBUG, "Filter_Builder::In recursive build\n"));
#endif

  if (e.header.type == ACE_ES_CONJUNCTION_DESIGNATOR)
    {
#ifdef EC_KOKYU_LOGGING
      ACE_DEBUG ((LM_DEBUG, "Filter_Builder::Conjuction designator\n"));
#endif
      CORBA::ULong npos = pos;
      ACE_CString name;
      this->recursive_name (qos, npos,
                            scheduler, name);

      pos++; // Consume the designator

      CORBA::ULong n = this->count_children (qos, pos);

      RtecBase::handle_t conj_rt_info = parent_info;

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
      for (CORBA::ULong i = 0; i != n; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos,
                                               scheduler,
                                               conj_rt_info);
        }

      TAO_EC_Kokyu_Filter *filter;
      ACE_NEW_RETURN (filter,
                      TAO_EC_Kokyu_Filter (name.c_str (),
                                           conj_rt_info,
                                           scheduler,
                                           new TAO_EC_Conjunction_Filter(children,
                                                                         n),
                                           conj_rt_info,
                                           conj_rt_info,
                                           RtecScheduler::CONJUNCTION),
                      0);
      TAO_EC_QOS_Info qos_info;
      filter->get_qos_info (qos_info);
      // @@
      return filter;
    }

  else if (e.header.type == ACE_ES_DISJUNCTION_DESIGNATOR)
    {
#ifdef EC_KOKYU_LOGGING
      ACE_DEBUG ((LM_DEBUG, "Filter_Builder::Disjunction designator\n"));
#endif
      CORBA::ULong npos = pos;
      ACE_CString name;
      this->recursive_name (qos, npos,
                            scheduler, name);

      pos++; // Consume the designator

      RtecBase::handle_t disj_rt_info = parent_info;

      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
      for (CORBA::ULong i = 0; i != n; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos,
                                               scheduler,
                                               disj_rt_info);
        }
      TAO_EC_Kokyu_Filter *filter;
      ACE_NEW_RETURN (filter,
                      TAO_EC_Kokyu_Filter (name.c_str (),
                                           disj_rt_info,
                                           scheduler,
                                           new TAO_EC_Disjunction_Filter (children,
                                                                          n),
                                           disj_rt_info,
                                           disj_rt_info,
                                           RtecScheduler::DISJUNCTION),
                      0);

      TAO_EC_QOS_Info qos_info;
      filter->get_qos_info (qos_info);
      // @@
      return filter;
    }
    else if (e.header.type == ACE_ES_EVENT_TIMEOUT
             || e.header.type == ACE_ES_EVENT_INTERVAL_TIMEOUT
             || e.header.type == ACE_ES_EVENT_DEADLINE_TIMEOUT)
      {
#ifdef EC_KOKYU_LOGGING
        ACE_DEBUG ((LM_DEBUG, "Filter_Builder::Timeout designator\n"));
#endif
        // @@ We need a unique name for each timeout, assigned by the
        //    application?
        char buf[64];

        //get the rt_info for the timer consumer
        RtecBase::handle_t h_consumer_rt_info = qos.dependencies[pos].rt_info;

        //build a unique name using the cosumer_rt_info
        ACE_OS::sprintf (buf, "TIMEOUT:%umsec:%d",
                         static_cast<u_int> ((e.header.creation_time / 10000)),
                         h_consumer_rt_info);
        ACE_CString name = buf;

        TAO_EC_QOS_Info qos_info;
        qos_info.rt_info =
          scheduler->create (name.c_str ());

        // Convert the time to the proper units....
        RtecScheduler::Period_t period =
          static_cast<RtecScheduler::Period_t> (e.header.creation_time);

#if 1 //by VS original code replaced with this
        RtecScheduler::RT_Info* consumer_rt_info_ptr;

        consumer_rt_info_ptr = scheduler->get (h_consumer_rt_info);
        scheduler->set (qos_info.rt_info,
                        consumer_rt_info_ptr->criticality,
                        0, // worst_cast_execution_time
                        0, // typical_cast_execution_time
                        0, // cached_cast_execution_time
                        period,
                        consumer_rt_info_ptr->importance,
                        0, // quantum
                        1, // threads
                        RtecScheduler::OPERATION);

        scheduler->add_dependency (qos_info.rt_info,
                                   h_consumer_rt_info,
                                   1,
                                   RtecBase::TWO_WAY_CALL);
#endif //by VS

        pos++;
        return new TAO_EC_Timeout_Filter (this->event_channel_,
                                          supplier,
                                          qos_info,
                                          e.header.type,
                                          e.header.creation_time);
      }

#if 1 //added by VS
  else if (e.header.type == ACE_ES_GLOBAL_DESIGNATOR)
    {
      pos++;
      return this->recursive_build (supplier, qos, pos,
                                    scheduler,
                                    parent_info);
    }
  else
    {
#ifdef EC_KOKYU_LOGGING
      ACE_DEBUG ((LM_DEBUG,
                  "Kokyu_Filter_Builder::No designator for this entry. "
                  "Must be a body\n"));
#endif
    }
#endif

  //probably because of a global designator, the parent_info could be 0.
  if (parent_info == 0)
    {
      //In this case, the parent_info is the same as the one supplied
      //in the consumer qos.
      parent_info = qos.dependencies[pos].rt_info;
    }

  RtecScheduler::RT_Info_var info =
    scheduler->get (parent_info);

  ACE_CString name = info->entry_point.in ();

  pos++;
  TAO_EC_Kokyu_Filter *filter;
  ACE_NEW_RETURN (filter,
                  TAO_EC_Kokyu_Filter (name.c_str (),
                                       parent_info,
                                       scheduler,
                                       new TAO_EC_Type_Filter (e.header),
                                       parent_info,
                                       parent_info,
                                       RtecScheduler::OPERATION),
                  0);

  TAO_EC_QOS_Info qos_info;
  filter->get_qos_info (qos_info);
  // @@
  return filter;
}

void
TAO_EC_Kokyu_Filter_Builder:: recursive_name (
    RtecEventChannelAdmin::ConsumerQOS& qos,
    CORBA::ULong& pos,
    RtecScheduler::Scheduler_ptr scheduler,
    ACE_CString& name) const
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
                                child_name);

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
                                child_name);

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
      ACE_OS::sprintf (buf, "TIMEOUT:%umsec",
                       static_cast<u_int> ((e.header.creation_time / 10000)));
      name = buf;

      return;
    }

  RtecScheduler::handle_t body_info = qos.dependencies[pos].rt_info;

  RtecScheduler::RT_Info_var info =
    scheduler->get (body_info);

  name = info->entry_point.in ();
  name += "#rep";

  pos++;
}

CORBA::ULong
TAO_EC_Kokyu_Filter_Builder::
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

TAO_END_VERSIONED_NAMESPACE_DECL
