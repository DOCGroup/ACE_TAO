// $Id$

#include "orbsvcs/Event/EC_Null_Scheduling.h"
#include "orbsvcs/Event/EC_QOS_Info.h"
#include "orbsvcs/Event/EC_Supplier_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Null_Scheduling.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Null_Scheduling, "$Id$")

void
TAO_EC_Null_Scheduling::add_proxy_supplier_dependencies (
      TAO_EC_ProxyPushSupplier *,
      TAO_EC_ProxyPushConsumer *
      ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_Null_Scheduling::schedule_event (const RtecEventComm::EventSet &event,
                                        TAO_EC_ProxyPushConsumer *,
                                        TAO_EC_Supplier_Filter *filter
                                        ACE_ENV_ARG_DECL)
{
  for (CORBA::ULong j = 0; j < event.length (); ++j)
    {
      const RtecEventComm::Event& e = event[j];
      RtecEventComm::Event* buffer =
        const_cast<RtecEventComm::Event*> (&e);
      RtecEventComm::EventSet single_event (1, 1, buffer, 0);

      TAO_EC_QOS_Info event_info;
      filter->push_scheduled_event (single_event, event_info
                                    ACE_ENV_ARG_PARAMETER);
    }
}
