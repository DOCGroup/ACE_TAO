// $Id$

#include "EC_TPC_ProxySupplier.h"

#if ! defined (__ACE_INLINE__)
#include "EC_TPC_ProxySupplier.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_TPC_ProxySupplier, "$Id$")

#include "EC_Event_Channel_Base.h"
#include "EC_TPC_Dispatching.h"

extern unsigned long EC_TPC_debug_level;

TAO_EC_TPC_ProxyPushSupplier::~TAO_EC_TPC_ProxyPushSupplier (void)
{

}

void
TAO_EC_TPC_ProxyPushSupplier:: disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (EC_TPC_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "EC (%P|%t): enter EC_TPC_ProxySupplier::disconnect_push_supplier (%@): refcount=%u,consumer=%@\n", this, this->refcount_, this->consumer_.in()));

  if (this->is_connected_i ())
    this->tpc_dispatching ()->remove_consumer (this->consumer_.in()
                                               ACE_ENV_ARG_PARAMETER);
  BASECLASS::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (EC_TPC_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "EC (%P|%t): leave EC_TPC_ProxySupplier::disconnect_push_supplier (%@)\n", this));
}

TAO_EC_TPC_Dispatching*
TAO_EC_TPC_ProxyPushSupplier::tpc_dispatching ()
{
  TAO_EC_Dispatching* dispatcher = this->event_channel_->dispatching ();
  TAO_EC_TPC_Dispatching* tpcdispatcher =
    ACE_dynamic_cast (TAO_EC_TPC_Dispatching*, dispatcher);
  return tpcdispatcher;  
}

void
TAO_EC_TPC_ProxyPushSupplier::connect_push_consumer (
      RtecEventComm::PushConsumer_ptr push_consumer,
      const RtecEventChannelAdmin::ConsumerQOS& qos
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannelAdmin::AlreadyConnected,
                     RtecEventChannelAdmin::TypeError))
{
  BASECLASS::connect_push_consumer (push_consumer, qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (EC_TPC_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "EC (%P|%t): EC_ProxySupplier(%@): refcount=%u,consumer=%@\n",
              this, this->refcount_, this->consumer_.in()));

  TAO_EC_TPC_Dispatching* tpcdispatcher = this->tpc_dispatching ();

  // the new dispatching task gets automatically created
  tpcdispatcher->add_consumer (push_consumer ACE_ENV_ARG_PARAMETER);
}

