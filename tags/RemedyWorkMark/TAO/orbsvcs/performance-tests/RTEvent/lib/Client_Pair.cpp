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

void
Client_Pair::init (CORBA::Long experiment_id,
                   CORBA::Long base_event_type,
                   CORBA::ULong iterations,
                   CORBA::Long workload_in_usecs,
                   ACE_UINT32 gsf,
                   PortableServer::POA_ptr supplier_poa,
                   PortableServer::POA_ptr consumer_poa)
{
  const CORBA::Long source_event_range = 1;
  this->init (experiment_id,
              base_event_type,
              source_event_range,
              iterations,
              workload_in_usecs,
              gsf,
              supplier_poa,
              consumer_poa);
}

void
Client_Pair::init (CORBA::Long experiment_id,
                   CORBA::Long base_event_type,
                   CORBA::Long source_event_range,
                   CORBA::ULong iterations,
                   CORBA::Long workload_in_usecs,
                   ACE_UINT32 gsf,
                   PortableServer::POA_ptr supplier_poa,
                   PortableServer::POA_ptr consumer_poa)
{
  this->supplier_ = new Supplier (experiment_id,
                                  base_event_type, source_event_range,
                                  supplier_poa);
  this->consumer_ = new Consumer (experiment_id,
                                  base_event_type + 1,
                                  iterations,
                                  workload_in_usecs,
                                  gsf,
                                  consumer_poa);
}

void
Client_Pair::connect (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->supplier_->connect (ec);
  Auto_Disconnect<Supplier> supplier_disconnect (this->supplier_);

  this->consumer_->connect (ec);
  Auto_Disconnect<Consumer> consumer_disconnect (this->consumer_);

  consumer_disconnect.release ();
  supplier_disconnect.release ();
}

void
Client_Pair::disconnect (void)
{
  Auto_Disconnect<Supplier> supplier_disconnect (this->supplier_);
  Auto_Disconnect<Consumer> consumer_disconnect (this->consumer_);
}
