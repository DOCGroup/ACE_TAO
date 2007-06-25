// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_CEC_DynamicImplementationServer::TAO_CEC_DynamicImplementationServer
        (PortableServer::POA_ptr poa,
         TAO_CEC_TypedProxyPushConsumer *typed_pp_consumer,
         TAO_CEC_TypedEventChannel *typed_event_channel)
  :  poa_ (PortableServer::POA::_duplicate (poa)),
     typed_pp_consumer_ (typed_pp_consumer),
     typed_event_channel_ (typed_event_channel),
     repository_id_ (CORBA::string_dup (typed_event_channel->supported_interface () ))
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
