#include "orbsvcs/Event/EC_Reactive_Dispatching.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_EC_Reactive_Dispatching::activate ()
{
}

void
TAO_EC_Reactive_Dispatching::shutdown ()
{
}

void
TAO_EC_Reactive_Dispatching::push (TAO_EC_ProxyPushSupplier* proxy,
                                   RtecEventComm::PushConsumer_ptr consumer,
                                   const RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info&)
{
  proxy->reactive_push_to_consumer (consumer, event);
}

void
TAO_EC_Reactive_Dispatching::push_nocopy (TAO_EC_ProxyPushSupplier* proxy,
                                          RtecEventComm::PushConsumer_ptr consumer,
                                          RtecEventComm::EventSet& event,
                                          TAO_EC_QOS_Info&)
{
  proxy->reactive_push_to_consumer (consumer, event);
}

TAO_END_VERSIONED_NAMESPACE_DECL
