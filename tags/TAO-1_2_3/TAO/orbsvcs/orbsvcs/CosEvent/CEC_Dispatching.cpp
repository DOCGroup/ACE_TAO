// $Id$

#include "CEC_Dispatching.h"
#include "CEC_ProxyPushSupplier.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_Dispatching.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_Dispatching, "$Id$")

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
                                    const CORBA::Any& event
                                    ACE_ENV_ARG_DECL)
{
  proxy->reactive_push_to_consumer (event ACE_ENV_ARG_PARAMETER);
}

void
TAO_CEC_Reactive_Dispatching::push_nocopy (TAO_CEC_ProxyPushSupplier* proxy,
                                           CORBA::Any& event
                                           ACE_ENV_ARG_DECL)
{
  proxy->reactive_push_to_consumer (event ACE_ENV_ARG_PARAMETER);
}
