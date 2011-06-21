// $Id$

#include "orbsvcs/CosEvent/CEC_Dispatching.h"
#include "orbsvcs/CosEvent/CEC_ProxyPushSupplier.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CEC_Dispatching::~TAO_CEC_Dispatching (void)
{
}

// ****************************************************************

TAO_CEC_Reactive_Dispatching::TAO_CEC_Reactive_Dispatching (void)
{
}

void
TAO_CEC_Reactive_Dispatching::activate (void)
{
}

void
TAO_CEC_Reactive_Dispatching::shutdown (void)
{
}

void
TAO_CEC_Reactive_Dispatching::push (TAO_CEC_ProxyPushSupplier* proxy,
                                    const CORBA::Any& event)
{
  proxy->reactive_push_to_consumer (event);
}

void
TAO_CEC_Reactive_Dispatching::push_nocopy (TAO_CEC_ProxyPushSupplier* proxy,
                                           CORBA::Any& event)
{
  proxy->reactive_push_to_consumer (event);
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_Reactive_Dispatching::invoke (TAO_CEC_ProxyPushSupplier* proxy,
                                      const TAO_CEC_TypedEvent& typed_event)
{
  proxy->reactive_invoke_to_consumer (typed_event);
}

void
TAO_CEC_Reactive_Dispatching::invoke_nocopy (TAO_CEC_ProxyPushSupplier* proxy,
                                             TAO_CEC_TypedEvent& typed_event)
{
  proxy->reactive_invoke_to_consumer (typed_event);
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_END_VERSIONED_NAMESPACE_DECL
