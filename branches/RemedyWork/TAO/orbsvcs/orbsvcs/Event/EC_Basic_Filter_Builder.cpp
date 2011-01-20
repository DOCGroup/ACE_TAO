// $Id$

#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Basic_Filter_Builder.h"
#include "orbsvcs/Event/EC_Type_Filter.h"
#include "orbsvcs/Event/EC_Conjunction_Filter.h"
#include "orbsvcs/Event/EC_Disjunction_Filter.h"
#include "orbsvcs/Event/EC_And_Filter.h"
#include "orbsvcs/Event/EC_Negation_Filter.h"
#include "orbsvcs/Event/EC_Bitmask_Filter.h"
#include "orbsvcs/Event/EC_Masked_Type_Filter.h"
#include "orbsvcs/Event/EC_Timeout_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Basic_Filter_Builder.inl"
#endif /* __ACE_INLINE__ */




TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Basic_Filter_Builder::~TAO_EC_Basic_Filter_Builder (void)
{
}

TAO_EC_Filter*
TAO_EC_Basic_Filter_Builder::build (
    TAO_EC_ProxyPushSupplier *supplier,
    RtecEventChannelAdmin::ConsumerQOS& qos) const
{
  CORBA::ULong pos = 0;
  return this->recursive_build (supplier, qos, pos);
}

TAO_EC_Filter*
TAO_EC_Basic_Filter_Builder:: recursive_build (
    TAO_EC_ProxyPushSupplier *supplier,
    RtecEventChannelAdmin::ConsumerQOS& qos,
    CORBA::ULong& pos) const
{
  CORBA::ULong l = qos.dependencies.length ();
  if (pos == l)
    return 0;

  const RtecEventComm::Event& e = qos.dependencies[pos].event;
  if (e.header.type == ACE_ES_CONJUNCTION_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
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
      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
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
      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
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
        return 0;
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
        return 0;
      CORBA::ULong source_mask = qos.dependencies[pos].event.header.source;
      CORBA::ULong type_mask = qos.dependencies[pos].event.header.type;
      pos++;

      if (pos == qos.dependencies.length ())
        return 0;
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

CORBA::ULong
TAO_EC_Basic_Filter_Builder::
    count_children (RtecEventChannelAdmin::ConsumerQOS& qos,
                    CORBA::ULong pos) const
{
  CORBA::ULong l = qos.dependencies.length ();
  CORBA::ULong i;
  int count = 0;
  for (i = pos; i != l; ++i)
    {
      const RtecEventComm::Event& e = qos.dependencies[i].event;
      if (e.header.type == ACE_ES_CONJUNCTION_DESIGNATOR
          || e.header.type == ACE_ES_DISJUNCTION_DESIGNATOR
          || e.header.type == ACE_ES_LOGICAL_AND_DESIGNATOR)
        // We won't let these be nested by the basic filter builder.
        // Assume these are the end of the group
        break;
      else if (e.header.type == ACE_ES_BITMASK_DESIGNATOR)
        // These take up an extra element
        i++;
      else if (e.header.type == ACE_ES_MASKED_TYPE_DESIGNATOR)
        // These take up two extra elements
        i += 2;
      else if (e.header.type == ACE_ES_NEGATION_DESIGNATOR) {
        // These enclose another filter.
        // Lets try to figure out how many elements the enclosed
        // filter takes up (but don't count it in the group).
        // Only allow basic filter types and bitmasks within
        // a negation (when it is nested within a group).
        // This is isn't perfect, but its about the best we can
        // do without prefixes.
        i++;
        switch (qos.dependencies[i].event.header.type) {
        case ACE_ES_BITMASK_DESIGNATOR:
          i++;
          break;
        case ACE_ES_MASKED_TYPE_DESIGNATOR:
          i += 2;
          break;
        }
      }
      count++;
    }
  return count;
}

TAO_END_VERSIONED_NAMESPACE_DECL
