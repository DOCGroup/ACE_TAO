/**
 * @file Client_Group.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Client_Group.h"
#include "Auto_Disconnect.h"

#if !defined(__ACE_INLINE__)
#include "Client_Group.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO_PERF_RTEC, Client_Group, "$Id$")

void
Client_Group::init (CORBA::Long experiment_id,
                    CORBA::Long base_event_type,
                    CORBA::ULong iterations,
                    CORBA::Long workload_in_usecs,
                    ACE_UINT32 gsf,
                    PortableServer::POA_ptr supplier_poa,
                    PortableServer::POA_ptr consumer_poa)
{
  this->client_pair_.init (experiment_id,
                           base_event_type,
                           iterations,
                           workload_in_usecs,
                           gsf,
                           supplier_poa,
                           consumer_poa);
  this->loopback_pair_.init (experiment_id, base_event_type,
                             supplier_poa, consumer_poa);
}

void
Client_Group::connect (RtecEventChannelAdmin::EventChannel_ptr ec
                       ACE_ENV_ARG_DECL)
{
  this->client_pair_.connect (ec ACE_ENV_ARG_PARAMETER);
  Auto_Disconnect<Client_Pair> client_pair_disconnect (&this->client_pair_);

  this->loopback_pair_.connect (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  Auto_Disconnect<Loopback_Pair> loopback_pair_disconnect (&this->loopback_pair_);

  loopback_pair_disconnect.release ();
  client_pair_disconnect.release ();
}

void
Client_Group::disconnect (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  Auto_Disconnect<Client_Pair> client_pair_disconnect (&this->client_pair_);
  Auto_Disconnect<Loopback_Pair> loopback_pair_disconnect (&this->loopback_pair_);
}
