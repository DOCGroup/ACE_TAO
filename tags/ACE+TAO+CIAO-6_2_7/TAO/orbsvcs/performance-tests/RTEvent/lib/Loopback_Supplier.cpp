/**
 * @file Loopback_Supplier.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Loopback_Supplier.h"
#include "Implicit_Deactivator.h"

Loopback_Supplier::Loopback_Supplier (CORBA::Long experiment_id,
                                      CORBA::Long response_type,
                                      PortableServer::POA_ptr poa)
  : experiment_id_ (experiment_id)
  , response_type_ (response_type)
  , default_POA_ (PortableServer::POA::_duplicate (poa))
  , counter_ (0)
{
}

void
Loopback_Supplier::connect (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->for_suppliers ();

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (!CORBA::is_nil (this->proxy_consumer_.in ()))
      return;

    this->proxy_consumer_ =
      supplier_admin->obtain_push_consumer ();
  }

  RtecEventComm::PushSupplier_var supplier =
    this->_this ();

  RtecEventChannelAdmin::SupplierQOS supplier_qos;
  supplier_qos.is_gateway = 0;
  supplier_qos.publications.length (1);
  RtecEventComm::EventHeader& sh0 =
    supplier_qos.publications[0].event.header;
  sh0.type   = this->response_type_;
  sh0.source = this->experiment_id_;

  this->proxy_consumer_->connect_push_supplier (supplier.in (),
                                                supplier_qos);
}

void
Loopback_Supplier::disconnect (void)
{
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (CORBA::is_nil (this->proxy_consumer_.in ()))
      return;
    proxy = this->proxy_consumer_._retn ();
  }

  Implicit_Deactivator deactivator (this);

  try{
    proxy->disconnect_push_consumer ();
  } catch (const CORBA::Exception&) {
  }
}

void
Loopback_Supplier::push (const RtecEventComm::EventSet &source)
{
  // ACE_DEBUG ((LM_DEBUG, "Loopback_Supplier pushing\n"));
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (CORBA::is_nil (this->proxy_consumer_.in ()))
      return;
    proxy = this->proxy_consumer_;

#if 0
    this->counter_ += source.length ();
    if ((this->counter_ + 1) % 1000 == 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) - Loopback (%d) sending %d messages\n",
                    this->response_type_, this->counter_ + 1));
      }
#endif /* 0 */
  }

  // ACE_DEBUG ((LM_DEBUG, "Loopback_Supplier::push (%P|%t)\n"));
  RtecEventComm::EventSet events (source);
  for (CORBA::ULong i = 0; i != events.length (); ++i)
    {
      events[i].header.ttl    = 1;
      events[i].header.type   = this->response_type_;
      events[i].header.source = this->experiment_id_;
    }

  proxy->push (events);
}

void
Loopback_Supplier::disconnect_push_supplier (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->proxy_consumer_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

PortableServer::POA_ptr
Loopback_Supplier::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}
