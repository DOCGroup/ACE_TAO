/**
 * @file Loopback_Pair.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Loopback_Pair.h"
#include "Auto_Disconnect.h"

#if !defined(__ACE_INLINE__)
#include "Loopback_Pair.inl"
#endif /* __ACE_INLINE__ */

void
Loopback_Pair::init (CORBA::Long experiment_id,
                     CORBA::Long base_event_type,
                     PortableServer::POA_ptr supplier_poa,
                     PortableServer::POA_ptr consumer_poa)
{
  this->loopback_supplier_ = new Loopback_Supplier (experiment_id,
                                                    base_event_type + 1,
                                                    supplier_poa);
  this->loopback_consumer_ = new Loopback_Consumer (experiment_id,
                                                    base_event_type,
                                                    loopback_supplier_.in (),
                                                    consumer_poa);
}

void
Loopback_Pair::connect (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->loopback_supplier_->connect (ec);
  Auto_Disconnect<Loopback_Supplier> loopback_supplier_disconnect (this->loopback_supplier_);

  this->loopback_consumer_->connect (ec);
  Auto_Disconnect<Loopback_Consumer> loopback_consumer_disconnect (this->loopback_consumer_);

  loopback_consumer_disconnect.release ();
  loopback_supplier_disconnect.release ();
}

void
Loopback_Pair::disconnect (void)
{
  Auto_Disconnect<Loopback_Supplier> loopback_supplier_disconnect (this->loopback_supplier_);
  Auto_Disconnect<Loopback_Consumer> loopback_consumer_disconnect (this->loopback_consumer_);
}
