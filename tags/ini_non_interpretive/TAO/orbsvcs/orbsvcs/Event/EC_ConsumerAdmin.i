// $Id$

ACE_INLINE void
TAO_EC_ConsumerAdmin::
    for_each (TAO_EC_Worker<TAO_EC_ProxyPushSupplier> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_->for_each (worker, ACE_TRY_ENV);
}

// ****************************************************************

ACE_INLINE
TAO_EC_Connect_Consumer::
    TAO_EC_Connect_Consumer (TAO_EC_ProxyPushConsumer *consumer)
      :  consumer_ (consumer)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Reconnect_Consumer::
    TAO_EC_Reconnect_Consumer (TAO_EC_ProxyPushConsumer *consumer)
      :  consumer_ (consumer)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Disconnect_Consumer::
    TAO_EC_Disconnect_Consumer (TAO_EC_ProxyPushConsumer *consumer)
      :  consumer_ (consumer)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Shutdown_Supplier::TAO_EC_Shutdown_Supplier (void)
{
}
