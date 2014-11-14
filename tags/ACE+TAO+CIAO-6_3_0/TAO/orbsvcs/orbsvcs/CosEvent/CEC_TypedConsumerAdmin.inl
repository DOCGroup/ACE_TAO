// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
TAO_CEC_TypedConsumerAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_ProxyPushSupplier> *worker)
{
  this->typed_push_admin_.for_each (worker);
}

ACE_INLINE
TAO_CEC_Propagate_Typed_Event::TAO_CEC_Propagate_Typed_Event (
  const TAO_CEC_TypedEvent& typed_event,
  TAO_CEC_TypedEventChannel* )
  :  typed_event_ (typed_event)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
