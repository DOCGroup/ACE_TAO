// $Id$

#ifndef ACE_CHANNEL_CLIENTS_T_C
#define ACE_CHANNEL_CLIENTS_T_C

#include "orbsvcs/Channel_Clients_T.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/Channel_Clients_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(orbsvcs, Channel_Clients_T, "$Id$")

template<class TARGET> void
ACE_PushConsumer_Adapter<TARGET>::push (const RtecEventComm::EventSet& events,
					CORBA::Environment &_env)
{
  target_->push (events, _env);
}

template<class TARGET> void
ACE_PushConsumer_Adapter<TARGET>::disconnect_push_consumer (CORBA::Environment &_env)
{
  target_->disconnect_push_consumer (_env);
}

template<class TARGET> void
ACE_PushSupplier_Adapter<TARGET>::disconnect_push_supplier (CORBA::Environment &_env)
{
  target_->disconnect_push_supplier (_env);
}

#endif /* ACE_CHANNEL_CLIENTS_T_C */
