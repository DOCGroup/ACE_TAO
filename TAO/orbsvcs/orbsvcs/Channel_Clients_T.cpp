// $Id$

#ifndef ACE_CHANNEL_CLIENTS_T_C
#define ACE_CHANNEL_CLIENTS_T_C

#include "orbsvcs/Channel_Clients_T.h"

ACE_RCSID(orbsvcs, Channel_Clients_T, "$Id$")

#if !defined (__ACE_INLINE__)
#include "orbsvcs/Channel_Clients_T.i"
#endif /* __ACE_INLINE__ */

template<class TARGET> void
ACE_PushConsumer_Adapter<TARGET>::push (const RtecEventComm::EventSet& events
                                        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  target_->push (events ACE_ENV_ARG_PARAMETER);
}

template<class TARGET> void
ACE_PushConsumer_Adapter<TARGET>::disconnect_push_consumer (
    ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{

  target_->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template<class TARGET> void
ACE_PushSupplier_Adapter<TARGET>::disconnect_push_supplier (
    ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  target_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
}

#endif /* ACE_CHANNEL_CLIENTS_T_C */
