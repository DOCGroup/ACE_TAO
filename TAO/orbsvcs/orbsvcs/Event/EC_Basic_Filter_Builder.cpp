// $Id$

#include "orbsvcs/Event_Service_Constants.h"
#include "EC_Basic_Filter_Builder.h"
#include "EC_Type_Filter.h"
#include "EC_Conjunction_Filter.h"
#include "EC_Disjunction_Filter.h"
#include "EC_Negation_Filter.h"
#include "EC_Bitmask_Filter.h"
#include "EC_Masked_Type_Filter.h"
#include "EC_Timeout_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Basic_Filter_Builder.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Basic_Filter_Builder, "$Id$")

TAO_EC_Basic_Filter_Builder::~TAO_EC_Basic_Filter_Builder (void)
{
}

TAO_EC_Filter*
TAO_EC_Basic_Filter_Builder::build (
    TAO_EC_ProxyPushSupplier *supplier,
    RtecEventChannelAdmin::ConsumerQOS& qos,
    CORBA::Environment&) const
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
      for (; i != n && pos != l; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos);
        }
      if (pos == l)
        ++i;
      return new TAO_EC_Conjunction_Filter (children, i);
    }
  else if (e.header.type == ACE_ES_DISJUNCTION_DESIGNATOR
           || e.header.type == ACE_ES_GLOBAL_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
      for (CORBA::ULong i = 0; i != n; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos);
        }
      CORBA::ULong i = 0;
      for (; i != n && pos != l; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos);
        }
      if (pos == l)
        ++i;
      return new TAO_EC_Disjunction_Filter (children, i);
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

  int n = 0;
  for (i = pos; i < l; ++i)
    {
      const RtecEventComm::Event& e = qos.dependencies[i].event;
      if (e.header.type == ACE_ES_CONJUNCTION_DESIGNATOR
          || e.header.type == ACE_ES_DISJUNCTION_DESIGNATOR)
        break;
      // @@ Properly count the number of children, and not the number
      // of nodes below...
      ++n;
    }
  return n;
}
