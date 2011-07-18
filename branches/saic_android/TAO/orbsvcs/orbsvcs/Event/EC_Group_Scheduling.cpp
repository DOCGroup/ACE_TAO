// $Id$

#include "orbsvcs/Event/EC_Group_Scheduling.h"
#include "orbsvcs/Event/EC_QOS_Info.h"
#include "orbsvcs/Event/EC_Supplier_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Group_Scheduling.inl"
#endif /* __ACE_INLINE__ */



void
TAO_EC_Group_Scheduling::add_proxy_supplier_dependencies (
      TAO_EC_ProxyPushSupplier *,
      TAO_EC_ProxyPushConsumer *)
{
}

void
TAO_EC_Group_Scheduling::schedule_event (const RtecEventComm::EventSet &event,
                                        TAO_EC_ProxyPushConsumer *,
                                        TAO_EC_Supplier_Filter *filter)
{
  TAO_EC_QOS_Info event_info;
  // @@ yuck...
  filter->push_scheduled_event (const_cast<RtecEventComm::EventSet&> (event),
                                event_info);
}
