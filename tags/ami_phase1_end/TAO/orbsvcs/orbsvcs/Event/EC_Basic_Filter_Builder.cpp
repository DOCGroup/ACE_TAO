// $Id$

#include "orbsvcs/Event_Service_Constants.h"
#include "EC_Basic_Filter_Builder.h"
#include "EC_Type_Filter.h"
#include "EC_Conjunction_Filter.h"
#include "EC_Disjunction_Filter.h"
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
  const RtecEventComm::Event& e = qos.dependencies[pos].event;
  if (e.header.type == ACE_ES_CONJUNCTION_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
      for (CORBA::ULong i = 0; i != n; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos);
          pos++;
        }
      return new TAO_EC_Conjunction_Filter (children, n);
    }
  else if (e.header.type == ACE_ES_DISJUNCTION_DESIGNATOR)
    {
      pos++; // Consume the designator
      CORBA::ULong n = this->count_children (qos, pos);

      TAO_EC_Filter** children;
      ACE_NEW_RETURN (children, TAO_EC_Filter*[n], 0);
      for (CORBA::ULong i = 0; i != n; ++i)
        {
          children[i] = this->recursive_build (supplier, qos, pos);
          pos++;
        }
      return new TAO_EC_Disjunction_Filter (children, n);
    }
  else if (e.header.type == ACE_ES_EVENT_TIMEOUT
           || e.header.type == ACE_ES_EVENT_INTERVAL_TIMEOUT
           || e.header.type == ACE_ES_EVENT_DEADLINE_TIMEOUT)
    {
      pos++;
      TAO_EC_QOS_Info qos_info;
      return new TAO_EC_Timeout_Filter (this->event_channel_,
                                        supplier,
                                        qos_info,
                                        e.header.type,
                                        e.header.creation_time);
    }
  return new TAO_EC_Type_Filter (e.header);
}

CORBA::ULong
TAO_EC_Basic_Filter_Builder::
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
