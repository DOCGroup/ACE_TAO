#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Prefix_Filter_Builder.h"
#include "orbsvcs/Event/EC_Type_Filter.h"
#include "orbsvcs/Event/EC_Conjunction_Filter.h"
#include "orbsvcs/Event/EC_Disjunction_Filter.h"
#include "orbsvcs/Event/EC_And_Filter.h"
#include "orbsvcs/Event/EC_Negation_Filter.h"
#include "orbsvcs/Event/EC_Bitmask_Filter.h"
#include "orbsvcs/Event/EC_Masked_Type_Filter.h"
#include "orbsvcs/Event/EC_Timeout_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Prefix_Filter_Builder.inl"
#endif /* __ACE_INLINE__ */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Prefix_Filter_Builder::~TAO_EC_Prefix_Filter_Builder ()
{
}

TAO_EC_Filter*
TAO_EC_Prefix_Filter_Builder::build (
    TAO_EC_ProxyPushSupplier *supplier,
    RtecEventChannelAdmin::ConsumerQOS& qos) const
{
  CORBA::ULong pos = 0;
  return this->recursive_build (supplier, qos, pos);
}

TAO_EC_Filter*
TAO_EC_Prefix_Filter_Builder:: recursive_build (
    TAO_EC_ProxyPushSupplier *supplier,
    RtecEventChannelAdmin::ConsumerQOS& qos,
    CORBA::ULong& pos) const
{
  CORBA::ULong l = qos.dependencies.length ();
  if (pos == l)
    return nullptr;

  const RtecEventComm::Event& e = qos.dependencies[pos].event;
  if (e.header.type == ACE_ES_CONJUNCTION_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = e.header.source;

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], nullptr);
      CORBA::ULong i = 0;
      for (; i != n; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos);
        }
      return new TAO_EC_Conjunction_Filter (children, n);
    }
  else if (e.header.type == ACE_ES_DISJUNCTION_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = e.header.source;

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], nullptr);
      CORBA::ULong i = 0;
      for (; i != n; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos);
        }
      return new TAO_EC_Disjunction_Filter (children, n);
    }

  else if (e.header.type == ACE_ES_LOGICAL_AND_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = e.header.source;

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], nullptr);
      CORBA::ULong i = 0;
      for (; i != n; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos);
        }
      return new TAO_EC_And_Filter (children, n);
    }

  else if (e.header.type == ACE_ES_NEGATION_DESIGNATOR)
    {
      pos++; // Consume the designator

      TAO_EC_Filter *child =
        this->recursive_build (supplier, qos, pos);
      return new TAO_EC_Negation_Filter (child);
    }
  else if (e.header.type == ACE_ES_BITMASK_DESIGNATOR)
    {
      pos++; // COnsumer the designator

      if (pos == qos.dependencies.length ())
        return nullptr;
      CORBA::ULong source_mask = qos.dependencies[pos].event.header.source;
      CORBA::ULong type_mask = qos.dependencies[pos].event.header.type;
      pos++;

      TAO_EC_Filter *child =
        this->recursive_build (supplier, qos, pos);
      return new TAO_EC_Bitmask_Filter (source_mask,
                                        type_mask,
                                        child);
    }
  else if (e.header.type == ACE_ES_MASKED_TYPE_DESIGNATOR)
    {
      pos++; // Consume the designator

      if (pos == qos.dependencies.length ())
        return nullptr;
      CORBA::ULong source_mask = qos.dependencies[pos].event.header.source;
      CORBA::ULong type_mask = qos.dependencies[pos].event.header.type;
      pos++;

      if (pos == qos.dependencies.length ())
        return nullptr;
      CORBA::ULong source_value = qos.dependencies[pos].event.header.source;
      CORBA::ULong type_value = qos.dependencies[pos].event.header.type;
      pos++;

      return new TAO_EC_Masked_Type_Filter (source_mask,
                                            type_mask,
                                            source_value,
                                            type_value);
    }
  else if (e.header.type == ACE_ES_NULL_DESIGNATOR)
    {
      pos++; // Consume the designator

      return new TAO_EC_Null_Filter ();
    }
  else if (e.header.type == ACE_ES_EVENT_TIMEOUT
           || e.header.type == ACE_ES_EVENT_INTERVAL_TIMEOUT
           || e.header.type == ACE_ES_EVENT_DEADLINE_TIMEOUT)
    {
      pos++; // Consume the designator
      TAO_EC_QOS_Info qos_info;
      return new TAO_EC_Timeout_Filter (this->event_channel_,
                                        supplier,
                                        qos_info,
                                        e.header.type,
                                        e.header.creation_time);
    }
  pos++; // Consume the event
  return new TAO_EC_Type_Filter (e.header);
}

TAO_END_VERSIONED_NAMESPACE_DECL
