// $Id$

ACE_INLINE void
TAO_CEC_SupplierAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_ProxyPushConsumer> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  this->push_admin_.for_each (worker, ACE_TRY_ENV);
}

ACE_INLINE void
TAO_CEC_SupplierAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_ProxyPullConsumer> *worker,
              CORBA::Environment &ACE_TRY_ENV)
{
  this->pull_admin_.for_each (worker, ACE_TRY_ENV);
}
