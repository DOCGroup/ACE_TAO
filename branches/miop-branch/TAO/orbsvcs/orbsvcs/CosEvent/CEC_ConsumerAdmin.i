// $Id$

ACE_INLINE void
TAO_CEC_ConsumerAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_ProxyPushSupplier> *worker
              TAO_ENV_ARG_DECL)
{
  this->push_admin_.for_each (worker TAO_ENV_ARG_PARAMETER);
}

ACE_INLINE void
TAO_CEC_ConsumerAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_ProxyPullSupplier> *worker
              TAO_ENV_ARG_DECL)
{
  this->pull_admin_.for_each (worker TAO_ENV_ARG_PARAMETER);
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
