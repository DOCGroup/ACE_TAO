// $Id$

ACE_INLINE void
TAO_CEC_TypedSupplierAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_TypedProxyPushConsumer> *worker
              ACE_ENV_ARG_DECL)
{
  this->typed_push_admin_.for_each (worker ACE_ENV_ARG_PARAMETER);
}
