// $Id$

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Log_Macros.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/Event_Utilities.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ConsumerQOS_Factory::
    ACE_ConsumerQOS_Factory (TAO_EC_Event_Initializer initializer,
                             CORBA::ULong qos_max_len)
  : designator_set_ (0),
    event_initializer_ (initializer)
{
  qos_.is_gateway = 0;

  // Allocate the space requested by the application....
  qos_.dependencies.length (qos_max_len);

  // ... now reset the length, we do not want to use any elements in
  // the sequence that have not been initialized....
  qos_.dependencies.length (0);
}

ACE_ConsumerQOS_Factory::~ACE_ConsumerQOS_Factory (void)
{
}

int
ACE_ConsumerQOS_Factory::start_conjunction_group (int nchildren)
{
  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 1);
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (qos_.dependencies[l].event);
  qos_.dependencies[l].event.header.type = ACE_ES_CONJUNCTION_DESIGNATOR;
  qos_.dependencies[l].event.header.source = nchildren;
  qos_.dependencies[l].rt_info = 0;
  this->designator_set_ = 1;
  return 0;
}

int
ACE_ConsumerQOS_Factory::start_disjunction_group (int nchildren)
{
  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 1);
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (qos_.dependencies[l].event);
  qos_.dependencies[l].event.header.type = ACE_ES_DISJUNCTION_DESIGNATOR;
  qos_.dependencies[l].event.header.source = nchildren;
  qos_.dependencies[l].rt_info = 0;
  this->designator_set_ = 1;
  return 0;
}

int
ACE_ConsumerQOS_Factory::start_logical_and_group (int nchildren)
{
  int l = qos_.dependencies.length ();
  qos_.dependencies.length (l + 1);
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (qos_.dependencies[l].event);
  qos_.dependencies[l].event.header.type = ACE_ES_LOGICAL_AND_DESIGNATOR;
  qos_.dependencies[l].event.header.source = nchildren;
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
  size_t l = ACE_OS::strlen (p);
  ORBSVCS_DEBUG ((LM_DEBUG,
              "%*.*s - event.source: %d (0x%x)\n"
              "%*.*s   event.type: %d (0x%x)\n",
              l, l, p, event.header.source, event.header.source,
              l, l, p, event.header.type, event.header.type));
}

void
ACE_ConsumerQOS_Factory::debug (const RtecEventChannelAdmin::ConsumerQOS& qos)
{
  ORBSVCS_DEBUG ((LM_DEBUG, "ConsumerQOS {\n"));
  ORBSVCS_DEBUG ((LM_DEBUG, "  is_gateway: %d\n", qos.is_gateway));

  for (u_int i = 0; i < qos.dependencies.length (); ++i)
    {
      char buf[128];
      ACE_OS::sprintf (buf, " dep[%d]", i);
      event_debug (buf, qos.dependencies[i].event);
      ORBSVCS_DEBUG ((LM_DEBUG, "%s  rt_info: %d\n",
                  buf, qos.dependencies[i].rt_info));
    }
  ORBSVCS_DEBUG ((LM_DEBUG, "}\n"));
}

// ************************************************************
ACE_SupplierQOS_Factory::
    ACE_SupplierQOS_Factory (TAO_EC_Event_Initializer initializer,
                             CORBA::ULong qos_max_len)
  : event_initializer_ (initializer)
{
  qos_.is_gateway = 0;

  // Allocate the space requested by the application....
  qos_.publications.length (qos_max_len);

  // ... now reset the length, we do not want to use any elements in
  // the sequence that have not been initialized....
  qos_.publications.length (0);
}

int
ACE_SupplierQOS_Factory::insert (RtecEventComm::EventSourceID sid,
                                 RtecEventComm::EventType type,
                                 RtecBase::handle_t rt_info,
                                 u_int ncalls)
{
  CORBA::ULong l = this->qos_.publications.length ();

  // @@ TODO We may want to consider more efficient growing
  // strategies here, for example, duplicating the size of the
  // buffer, or growing in fixed sized chunks...

  // This needs to accurately reflect the used length, and should always be
  // set
  this->qos_.publications.length (l + 1);

  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (qos_.publications[l].event);
  this->qos_.publications[l].event.header.source = sid;
  this->qos_.publications[l].event.header.type = type;
  // TODO: IDL union this->qos_.publications[l].event.data_.lval (0);
  this->qos_.publications[l].dependency_info.rt_info = rt_info;
  this->qos_.publications[l].dependency_info.number_of_calls = ncalls;
  return 0;
}

void ACE_SupplierQOS_Factory::debug (const RtecEventChannelAdmin::SupplierQOS& qos)
{
  ORBSVCS_DEBUG ((LM_DEBUG, "SupplierQOS {\n"));
  ORBSVCS_DEBUG ((LM_DEBUG, "  is_gateway: %d\n", qos.is_gateway));

  for (u_int i = 0; i < qos.publications.length (); ++i)
    {
      char buf[128] = { 0 };
      ACE_OS::sprintf (buf, " publications[%d]", i);
      event_debug (buf, qos.publications[i].event);
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "%s   dependency_info.rt_info: %d\n"
                  "%s   dependency_info.number_of_calls: %d\n",
                  buf, qos.publications[i].dependency_info.rt_info,
                  buf, qos.publications[i].dependency_info.number_of_calls));
        }
  ORBSVCS_DEBUG ((LM_DEBUG, "}\n"));

}

TAO_END_VERSIONED_NAMESPACE_DECL
