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

void
Client_Group::init (CORBA::Long experiment_id,
                    CORBA::Long base_event_type,
                    CORBA::ULong iterations,
                    CORBA::Long workload_in_usecs,
                    ACE_High_Res_Timer::global_scale_factor_type gsf,
                    PortableServer::POA_ptr supplier_poa,
                    PortableServer::POA_ptr consumer_poa)
{
  this->init (experiment_id, base_event_type, 1,
              iterations, workload_in_usecs, gsf,
              supplier_poa, consumer_poa);
}

void
Client_Group::init (CORBA::Long experiment_id,
                    CORBA::Long base_event_type,
                    CORBA::Long event_type_range,
                    CORBA::ULong iterations,
                    CORBA::Long workload_in_usecs,
                    ACE_High_Res_Timer::global_scale_factor_type gsf,
                    PortableServer::POA_ptr supplier_poa,
                    PortableServer::POA_ptr consumer_poa)
{
  this->client_pair_.init (experiment_id,
                           base_event_type,
                           event_type_range,
                           iterations,
                           workload_in_usecs,
                           gsf,
                           supplier_poa,
                           consumer_poa);
  this->loopback_pair_.init (experiment_id, base_event_type,
                             supplier_poa, consumer_poa);
}

void
Client_Group::connect (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->client_pair_.connect (ec);
  Auto_Disconnect<Client_Pair> client_pair_disconnect (&this->client_pair_);

  this->loopback_pair_.connect (ec);
  Auto_Disconnect<Loopback_Pair> loopback_pair_disconnect (&this->loopback_pair_);

  loopback_pair_disconnect.release ();
  client_pair_disconnect.release ();
}

void
Client_Group::disconnect (void)
{
  Auto_Disconnect<Client_Pair> client_pair_disconnect (&this->client_pair_);
  Auto_Disconnect<Loopback_Pair> loopback_pair_disconnect (&this->loopback_pair_);
}
