// $Id$

ACE_INLINE void
TAO_CEC_ConsumerAdmin::
    for_each (TAO_EC_Worker<TAO_CEC_ProxyPushSupplier> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_->for_each (worker, ACE_TRY_ENV);
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Connect_Consumer::
    TAO_CEC_Connect_Consumer (TAO_CEC_ProxyPushConsumer *consumer)
      :  consumer_ (consumer)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Reconnect_Consumer::
    TAO_CEC_Reconnect_Consumer (TAO_CEC_ProxyPushConsumer *consumer)
      :  consumer_ (consumer)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Disconnect_Consumer::
    TAO_CEC_Disconnect_Consumer (TAO_CEC_ProxyPushConsumer *consumer)
      :  consumer_ (consumer)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Shutdown_Supplier::TAO_CEC_Shutdown_Supplier (void)
{
}
