// $Id$

#ifndef ACE_CHANNEL_CLIENTS_T_C
#define ACE_CHANNEL_CLIENTS_T_C

#include "orbsvcs/Channel_Clients_T.h"

ACE_RCSID(orbsvcs, Channel_Clients_T, "$Id$")

#if !defined (__ACE_INLINE__)
#include "orbsvcs/Channel_Clients_T.i"
#endif /* __ACE_INLINE__ */

template<class TARGET> void
ACE_PushConsumer_Adapter<TARGET>::push (const RtecEventComm::EventSet& events,
                                        CORBA::Environment &TAO_IN_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  target_->push (events, TAO_IN_ENV);
}

template<class TARGET> void
ACE_PushConsumer_Adapter<TARGET>::disconnect_push_consumer (
    CORBA::Environment &TAO_IN_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException))
{

  target_->disconnect_push_consumer (TAO_IN_ENV);
}

template<class TARGET> void
ACE_PushSupplier_Adapter<TARGET>::disconnect_push_supplier (
    CORBA::Environment &TAO_IN_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  target_->disconnect_push_supplier (TAO_IN_ENV);
}

#endif /* ACE_CHANNEL_CLIENTS_T_C */
