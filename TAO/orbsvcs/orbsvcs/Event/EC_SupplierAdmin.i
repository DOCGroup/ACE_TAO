// $Id$

ACE_INLINE void
TAO_EC_SupplierAdmin::
    for_each (TAO_EC_Worker<TAO_EC_ProxyPushConsumer> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  this->collection_->for_each (worker, ACE_TRY_ENV);
}

// ****************************************************************

ACE_INLINE
TAO_EC_Connect_Supplier::
    TAO_EC_Connect_Supplier (TAO_EC_ProxyPushSupplier *supplier)
      :  supplier_ (supplier)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Reconnect_Supplier::
    TAO_EC_Reconnect_Supplier (TAO_EC_ProxyPushSupplier *supplier)
      :  supplier_ (supplier)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Disconnect_Supplier::
    TAO_EC_Disconnect_Supplier (TAO_EC_ProxyPushSupplier *supplier)
      :  supplier_ (supplier)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Shutdown_Consumer::TAO_EC_Shutdown_Consumer (void)
{
}
