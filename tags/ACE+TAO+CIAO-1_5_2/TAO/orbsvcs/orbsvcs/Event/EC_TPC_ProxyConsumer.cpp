// $Id$

#include "orbsvcs/Event/EC_TPC_ProxyConsumer.h"

ACE_RCSID(Event, EC_TPC_ProxyConsumer, "$Id$")

#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_TPC_Dispatching.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

extern unsigned long EC_TPC_debug_level;

TAO_EC_TPC_ProxyPushConsumer::TAO_EC_TPC_ProxyPushConsumer (TAO_EC_Event_Channel_Base* ec)
: TAO_EC_Default_ProxyPushConsumer (ec)
{
}

TAO_EC_TPC_Dispatching*
TAO_EC_TPC_ProxyPushConsumer::tpc_dispatching ()
{
  TAO_EC_Dispatching* dispatcher = this->event_channel_->dispatching ();
  TAO_EC_TPC_Dispatching* tpcdispatcher =
    dynamic_cast<TAO_EC_TPC_Dispatching*>(dispatcher);
  return tpcdispatcher;  
}

TAO_EC_TPC_ProxyPushConsumer::~TAO_EC_TPC_ProxyPushConsumer (void)
{
  // @@@ Do I need to call the logic to remove the consumer from the
  // dispatch map in here?  I'm not sure...  But, if I do, then I need
  // to fact that "remove" code out of just the
  // disconnect_push_consumer.
  if (EC_TPC_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "RTEC (%P|%t): inside ~TAO_EC_TPC_ProxyPushConsumer (%x)\n", this));
}

void
TAO_EC_TPC_ProxyPushConsumer::disconnect_push_consumer (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  RtecEventComm::PushConsumer_var emulated_exceptions_suck = 
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->tpc_dispatching ()->remove_consumer (emulated_exceptions_suck.in ()
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  BASECLASS::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_END_VERSIONED_NAMESPACE_DECL
