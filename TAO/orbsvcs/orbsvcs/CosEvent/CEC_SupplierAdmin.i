// $Id$

ACE_INLINE void
TAO_CEC_SupplierAdmin::
    for_each (TAO_EC_Worker<TAO_CEC_ProxyPushConsumer> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  this->push_collection_->for_each (worker, ACE_TRY_ENV);
}

ACE_INLINE void
TAO_CEC_SupplierAdmin::
    for_each (TAO_EC_Worker<TAO_CEC_ProxyPullConsumer> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  this->pull_collection_->for_each (worker, ACE_TRY_ENV);
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Shutdown_Push_Consumer::TAO_CEC_Shutdown_Push_Consumer (void)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Shutdown_Pull_Consumer::TAO_CEC_Shutdown_Pull_Consumer (void)
{
}
