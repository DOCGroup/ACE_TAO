// $Id$

ACE_INLINE void
TAO_CEC_SupplierAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_ProxyPushConsumer> *worker
              ACE_ENV_ARG_DECL)
{
  this->push_admin_.for_each (worker ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE void
TAO_CEC_SupplierAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_ProxyPullConsumer> *worker
              ACE_ENV_ARG_DECL)
{
  this->pull_admin_.for_each (worker ACE_ENV_ARG_PARAMETER);
}
