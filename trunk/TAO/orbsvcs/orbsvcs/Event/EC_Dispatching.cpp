// $Id$

#include "EC_Dispatching.h"
#include "EC_ProxySupplier.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Dispatching.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Dispatching, "$Id$")

TAO_EC_Dispatching::~TAO_EC_Dispatching (void)
{
}

// ****************************************************************

TAO_EC_Reactive_Dispatching::TAO_EC_Reactive_Dispatching (void)
{
}

void
TAO_EC_Reactive_Dispatching::activate (void)
{
}

void
TAO_EC_Reactive_Dispatching::shutdown (void)
{
}

void
TAO_EC_Reactive_Dispatching::push (TAO_EC_ProxyPushSupplier* proxy,
                                   const RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info,
                                   CORBA::Environment& ACE_TRY_ENV)
{
  proxy->push_to_consumer (event, ACE_TRY_ENV);
}

void
TAO_EC_Reactive_Dispatching::push_nocopy (TAO_EC_ProxyPushSupplier* proxy,
                                          RtecEventComm::EventSet& event,
                                          TAO_EC_QOS_Info& qos_info,
                                          CORBA::Environment& ACE_TRY_ENV)
{
  proxy->push_to_consumer (event, ACE_TRY_ENV);
}
