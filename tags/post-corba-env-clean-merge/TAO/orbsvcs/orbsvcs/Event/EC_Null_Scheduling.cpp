// $Id$

#include "EC_Null_Scheduling.h"
#include "EC_QOS_Info.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Null_Scheduling.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Null_Scheduling, "$Id$")

void
TAO_EC_Null_Scheduling::add_proxy_supplier_dependencies (
      TAO_EC_ProxyPushSupplier *,
      TAO_EC_ProxyPushConsumer *
      TAO_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_Null_Scheduling::init_event_qos (
    const RtecEventComm::EventHeader&,
    TAO_EC_ProxyPushConsumer *,
    TAO_EC_QOS_Info&
    TAO_ENV_ARG_DECL_NOT_USED)
{
}
