// $Id$

#include "orbsvcs/Event/EC_TPC_ProxySupplier.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_TPC_ProxySupplier.inl"
#endif /* __ACE_INLINE__ */



#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_TPC_Dispatching.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

extern unsigned long TAO_EC_TPC_debug_level;

TAO_EC_TPC_ProxyPushSupplier::~TAO_EC_TPC_ProxyPushSupplier (void)
{

}

void
TAO_EC_TPC_ProxyPushSupplier:: disconnect_push_supplier (void)
{
  if (TAO_EC_TPC_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "EC (%P|%t): enter EC_TPC_ProxySupplier::disconnect_push_supplier (%@): refcount=%u,consumer=%@\n",
                this,
                this->refcount_,
                this->consumer_.in()));

  if (this->is_connected_i ())
    {
      this->tpc_dispatching ()->remove_consumer (this->consumer_.in());
    }
  BASECLASS::disconnect_push_supplier ();

  if (TAO_EC_TPC_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "EC (%P|%t): leave EC_TPC_ProxySupplier::disconnect_push_supplier (%@)\n", this));
}

TAO_EC_TPC_Dispatching*
TAO_EC_TPC_ProxyPushSupplier::tpc_dispatching ()
{
  TAO_EC_Dispatching* dispatcher = this->event_channel_->dispatching ();

  return dynamic_cast <TAO_EC_TPC_Dispatching*> (dispatcher);
}

void
TAO_EC_TPC_ProxyPushSupplier::connect_push_consumer (
      RtecEventComm::PushConsumer_ptr push_consumer,
      const RtecEventChannelAdmin::ConsumerQOS& qos)
{
  BASECLASS::connect_push_consumer (push_consumer, qos);

  if (TAO_EC_TPC_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "EC (%P|%t): EC_ProxySupplier(%@): refcount=%u,consumer=%@\n",
              this, this->refcount_, this->consumer_.in()));

  TAO_EC_TPC_Dispatching* tpcdispatcher = this->tpc_dispatching ();

  // the new dispatching task gets automatically created
  tpcdispatcher->add_consumer (push_consumer);
}

TAO_END_VERSIONED_NAMESPACE_DECL
