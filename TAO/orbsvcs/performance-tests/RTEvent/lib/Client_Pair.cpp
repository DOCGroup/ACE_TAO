/**
 * @file Client_Pair.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Client_Pair.h"
#include "Auto_Disconnect.h"

#if !defined(__ACE_INLINE__)
#include "Client_Pair.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO_PERF_RTEC, Client_Pair, "$Id$")

void
Client_Pair::init (CORBA::Long experiment_id,
                   CORBA::Long base_event_type,
                   CORBA::ULong iterations,
                   CORBA::Long workload_in_usecs,
                   ACE_UINT32 gsf,
                   PortableServer::POA_ptr supplier_poa,
                   PortableServer::POA_ptr consumer_poa)
{
  this->supplier_ = new Supplier (experiment_id,
                                  base_event_type,
                                  supplier_poa);
  this->consumer_ = new Consumer (experiment_id,
                                  base_event_type + 1,
                                  iterations,
                                  workload_in_usecs,
                                  gsf,
                                  consumer_poa);
}

void
Client_Pair::connect (RtecEventChannelAdmin::EventChannel_ptr ec
                       ACE_ENV_ARG_DECL)
{
  this->supplier_->connect (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  Auto_Disconnect<Supplier> supplier_disconnect (this->supplier_);

  this->consumer_->connect (ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  Auto_Disconnect<Consumer> consumer_disconnect (this->consumer_);

  consumer_disconnect.release ();
  supplier_disconnect.release ();
}

void
Client_Pair::disconnect (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  Auto_Disconnect<Supplier> supplier_disconnect (this->supplier_);
  Auto_Disconnect<Consumer> consumer_disconnect (this->consumer_);
}
