// $Id$

ACE_INLINE void
TAO_CEC_ConsumerAdmin::
    for_each (TAO_EC_Worker<TAO_CEC_ProxyPushSupplier> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  this->push_collection_->for_each (worker, ACE_TRY_ENV);
}

ACE_INLINE void
TAO_CEC_ConsumerAdmin::
    for_each (TAO_EC_Worker<TAO_CEC_ProxyPullSupplier> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  this->pull_collection_->for_each (worker, ACE_TRY_ENV);
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Shutdown_Push_Supplier::TAO_CEC_Shutdown_Push_Supplier (void)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Shutdown_Pull_Supplier::TAO_CEC_Shutdown_Pull_Supplier (void)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Propagate_Event_Push::TAO_CEC_Propagate_Event_Push (const CORBA::Any& event)
  :  event_ (event)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Propagate_Event_Pull::TAO_CEC_Propagate_Event_Pull (const CORBA::Any& event)
  :  event_ (event)
{
}
