// $Id$

#include "orbsvcs/Event_Utilities.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/Event_Utilities.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, Event_Utilities, "$Id$")

ACE_ConsumerQOS_Factory::
    ACE_ConsumerQOS_Factory (TAO_EC_Event_Initializer initializer)
  : designator_set_ (0),
    event_initializer_ (initializer)
{
  qos_.is_gateway = 0;
}

ACE_ConsumerQOS_Factory::~ACE_ConsumerQOS_Factory (void)
{
}

int
ACE_ConsumerQOS_Factory::start_conjunction_group (void)
{
  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 1);
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (qos_.dependencies[l].event);
  qos_.dependencies[l].event.header.type = ACE_ES_CONJUNCTION_DESIGNATOR;
  qos_.dependencies[l].rt_info = 0;
  this->designator_set_ = 1;
  return 0;
}

int
ACE_ConsumerQOS_Factory::start_disjunction_group (void)
{
  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 1);
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (qos_.dependencies[l].event);
  qos_.dependencies[l].event.header.type = ACE_ES_DISJUNCTION_DESIGNATOR;
  qos_.dependencies[l].rt_info = 0;
  this->designator_set_ = 1;
  return 0;
}

int
ACE_ConsumerQOS_Factory::start_negation (void)
{
  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 1);
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (qos_.dependencies[l].event);
  qos_.dependencies[l].event.header.type = ACE_ES_NEGATION_DESIGNATOR;
  qos_.dependencies[l].rt_info = 0;
  this->designator_set_ = 1;
  return 0;
}

int
ACE_ConsumerQOS_Factory::start_bitmask (CORBA::ULong source_mask,
                                        CORBA::ULong type_mask)
{
  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 2);
  if (this->event_initializer_ != 0)
    {
      (*this->event_initializer_) (qos_.dependencies[l].event);
      (*this->event_initializer_) (qos_.dependencies[l + 1].event);
    }
  qos_.dependencies[l].event.header.type = ACE_ES_BITMASK_DESIGNATOR;
  qos_.dependencies[l].rt_info = 0;

  qos_.dependencies[l+1].event.header.source = source_mask;
  qos_.dependencies[l+1].event.header.type = type_mask;
  qos_.dependencies[l+1].rt_info = 0;

  this->designator_set_ = 1;
  return 0;
}

int
ACE_ConsumerQOS_Factory::insert (const RtecEventChannelAdmin::Dependency &subscribe)
{
  RtecScheduler::RT_Info dummy;
  // Make sure that a designator is first.
  if (designator_set_ == 0)
    {
      int l = qos_.dependencies.length ();
      qos_.dependencies.length (l + 1);
      if (this->event_initializer_ != 0)
        (*this->event_initializer_) (qos_.dependencies[l].event);
      qos_.dependencies[l].rt_info = 0;
      qos_.dependencies[l].event.header.type = ACE_ES_GLOBAL_DESIGNATOR;

      this->designator_set_ = 1;
    }

  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 1);
  qos_.dependencies[l] = subscribe;
  return 0;
}

void event_debug (const char* p,
                  const RtecEventComm::Event& event)
{
  int l = ACE_OS::strlen (p);
  ACE_DEBUG ((LM_DEBUG,
              "%*.*s - event.source: %d\n"
              "%*.*s   event.type: %d\n",
              l, l, p, event.header.source,
              l, l, p, event.header.type));
}

void
ACE_ConsumerQOS_Factory::debug (const RtecEventChannelAdmin::ConsumerQOS& qos)
{
  ACE_DEBUG ((LM_DEBUG, "ConsumerQOS { \n"));
  ACE_DEBUG ((LM_DEBUG, "  is_gateway: %d\n", qos.is_gateway));

  for (u_int i = 0; i < qos.dependencies.length (); ++i)
    {
      char buf[128];
      ACE_OS::sprintf (buf, " dep[%d]", i);
      event_debug (buf, qos.dependencies[i].event);
      ACE_DEBUG ((LM_DEBUG, "%s  rt_info: %d\n",
                  buf, qos.dependencies[i].rt_info));
    }
  ACE_DEBUG ((LM_DEBUG, "}\n"));
}

// ************************************************************

ACE_SupplierQOS_Factory::
    ACE_SupplierQOS_Factory (TAO_EC_Event_Initializer initializer)
  : event_initializer_ (initializer)
{
  qos_.is_gateway = 0;
}

int
ACE_SupplierQOS_Factory::insert (RtecEventComm::EventSourceID sid,
                                 RtecEventComm::EventType type,
                                 RtecScheduler::handle_t rt_info,
                                 u_int ncalls)
{
  int l = qos_.publications.length ();
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (qos_.publications[l].event);
  qos_.publications.length (l + 1);
  qos_.publications[l].event.header.source = sid;
  qos_.publications[l].event.header.type = type;
  // TODO: IDL union qos_.publications[l].event.data_.lval (0);
  qos_.publications[l].dependency_info.rt_info = rt_info;
  qos_.publications[l].dependency_info.number_of_calls = ncalls;
  return 0;
}

void ACE_SupplierQOS_Factory::debug (const RtecEventChannelAdmin::SupplierQOS& qos)
{
  ACE_DEBUG ((LM_DEBUG, "SupplierQOS { \n"));
  ACE_DEBUG ((LM_DEBUG, "  is_gateway: %d\n", qos.is_gateway));

  for (u_int i = 0; i < qos.publications.length (); ++i)
    {
      char buf[128];
      ACE_OS::sprintf (buf, " publications[%d]", i);
      event_debug (buf, qos.publications[i].event);
      ACE_DEBUG ((LM_DEBUG,
                  "%s   dependency_info.rt_info: %d\n"
                  "%s   dependency_info.number_of_calls: %d\n",
                  buf, qos.publications[i].dependency_info.rt_info,
                  buf, qos.publications[i].dependency_info.number_of_calls));
        }
  ACE_DEBUG ((LM_DEBUG, "}\n"));

}
