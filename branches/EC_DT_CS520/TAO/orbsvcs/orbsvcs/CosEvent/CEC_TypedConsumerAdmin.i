// $Id$

ACE_INLINE void
TAO_CEC_TypedConsumerAdmin::
    for_each (TAO_ESF_Worker<TAO_CEC_ProxyPushSupplier> *worker
              ACE_ENV_ARG_DECL)
{
  this->typed_push_admin_.for_each (worker ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE
TAO_CEC_Propagate_Typed_Event::TAO_CEC_Propagate_Typed_Event (const TAO_CEC_TypedEvent& typed_event,
                                                              TAO_CEC_TypedEventChannel* typed_event_channel)
  :  typed_event_ (typed_event),
     typed_event_channel_ (typed_event_channel)
{
}
