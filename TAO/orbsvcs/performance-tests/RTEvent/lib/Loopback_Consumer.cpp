/**
 * @file Loopback_Consumer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Loopback_Consumer.h"
#include "Implicit_Deactivator.h"
#include "orbsvcs/Event_Service_Constants.h"

Loopback_Consumer::
Loopback_Consumer (CORBA::Long experiment_id,
                   CORBA::Long event_type,
                   Loopback_Supplier *supplier,
                   PortableServer::POA_ptr poa)
  : experiment_id_ (experiment_id)
  , event_type_ (event_type)
  , supplier_ (TAO::Utils::Servant_Var<Loopback_Supplier>::_duplicate (supplier))
  , default_POA_ (PortableServer::POA::_duplicate (poa))
{
}

void
Loopback_Consumer::connect (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers ();

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (!CORBA::is_nil (this->proxy_supplier_.in ()))
      return;

    this->proxy_supplier_ =
      consumer_admin->obtain_push_supplier ();
  }

  RtecEventComm::PushConsumer_var consumer =
    this->_this ();

  RtecEventChannelAdmin::ConsumerQOS consumer_qos;
  consumer_qos.is_gateway = 0;
  consumer_qos.dependencies.length (2);
  RtecEventComm::EventHeader& h0 =
    consumer_qos.dependencies[0].event.header;
  h0.type   = ACE_ES_DISJUNCTION_DESIGNATOR;
  h0.source = 1;

  RtecEventComm::EventHeader& h1 =
    consumer_qos.dependencies[1].event.header;
  h1.source = this->experiment_id_;
  h1.type   = this->event_type_;

  this->proxy_supplier_->connect_push_consumer (consumer.in (),
                                                consumer_qos);
}

void
Loopback_Consumer::disconnect (void)
{
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (CORBA::is_nil (this->proxy_supplier_.in ()))
      return;
    proxy = this->proxy_supplier_._retn ();
  }

  Implicit_Deactivator deactivator (this);

  try
    {
      proxy->disconnect_push_supplier ();
    }
  catch (const CORBA::Exception&){}
}

void
Loopback_Consumer::push (const RtecEventComm::EventSet &events)
{
  // ACE_DEBUG ((LM_DEBUG, "Loopback_Consumer::push (%P|%t)\n"));
  this->supplier_->push (events);
}

void
Loopback_Consumer::disconnect_push_consumer (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->proxy_supplier_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}

PortableServer::POA_ptr
Loopback_Consumer::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}
