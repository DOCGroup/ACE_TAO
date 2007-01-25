// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
TAO_CEC_ConsumerAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_ProxyPushSupplier> *worker)
{
  this->push_admin_.for_each (worker);
}

ACE_INLINE void
TAO_CEC_ConsumerAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_ProxyPullSupplier> *worker)
{
  this->pull_admin_.for_each (worker);
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

TAO_END_VERSIONED_NAMESPACE_DECL
