// $Id$

#include "EC_Group_Scheduling.h"
#include "EC_QOS_Info.h"
#include "EC_Supplier_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Group_Scheduling.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Group_Scheduling, "$Id$")

void
TAO_EC_Group_Scheduling::add_proxy_supplier_dependencies (
      TAO_EC_ProxyPushSupplier *,
      TAO_EC_ProxyPushConsumer *
      TAO_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_Group_Scheduling::schedule_event (const RtecEventComm::EventSet &event,
                                        TAO_EC_ProxyPushConsumer *,
                                        TAO_EC_Supplier_Filter *filter
                                        TAO_ENV_ARG_DECL)
{
  TAO_EC_QOS_Info event_info;
  // @@ yuck...
  filter->push_scheduled_event (ACE_const_cast (RtecEventComm::EventSet&,
                                                event),
                                event_info
                                TAO_ENV_ARG_PARAMETER);
}
