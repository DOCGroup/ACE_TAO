// $Id$

ACE_INLINE void
TAO_CEC_SupplierAdmin::
    for_each (TAO_EC_Worker<TAO_CEC_ProxyPushConsumer> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_->for_each (worker, ACE_TRY_ENV);
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Connect_Supplier::
    TAO_CEC_Connect_Supplier (TAO_CEC_ProxyPushSupplier *supplier)
      :  supplier_ (supplier)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Reconnect_Supplier::
    TAO_CEC_Reconnect_Supplier (TAO_CEC_ProxyPushSupplier *supplier)
      :  supplier_ (supplier)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Disconnect_Supplier::
    TAO_CEC_Disconnect_Supplier (TAO_CEC_ProxyPushSupplier *supplier)
      :  supplier_ (supplier)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Shutdown_Consumer::TAO_CEC_Shutdown_Consumer (void)
{
}
