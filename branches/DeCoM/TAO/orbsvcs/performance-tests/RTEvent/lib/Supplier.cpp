/**
 * @file Supplier.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Supplier.h"
#include "Implicit_Deactivator.h"

Supplier::Supplier (CORBA::Long experiment_id,
                    CORBA::Long event_type,
                    CORBA::Long event_range,
                    PortableServer::POA_ptr poa)
  : experiment_id_ (experiment_id)
  , event_type_ (event_type)
  , event_range_ (event_range)
  , default_POA_ (PortableServer::POA::_duplicate (poa))
{
}

void
Supplier::connect (RtecEventChannelAdmin::EventChannel_ptr ec)
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
  supplier_qos.publications.length (this->event_range_);
  for (CORBA::Long i = 0; i != this->event_range_; ++i)
    {
      RtecEventComm::EventHeader& sh =
        supplier_qos.publications[i].event.header;
      sh.type   = this->event_type_ + 2 * i;
      sh.source = this->experiment_id_;
    }

  this->proxy_consumer_->connect_push_supplier (supplier.in (),
                                                supplier_qos);
}

void
Supplier::disconnect (void)
{
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (CORBA::is_nil (this->proxy_consumer_.in ()))
      return;
    proxy = this->proxy_consumer_._retn ();
  }

  Implicit_Deactivator deactivator (this);

  try
    {
      proxy->disconnect_push_consumer ();
    }
  catch (const CORBA::Exception&){}
}

void
Supplier::push (const RtecEventComm::EventSet &events)
{
  // ACE_DEBUG ((LM_DEBUG, "Supplier pushing (%d,%d)\n",
  // events[0].header.type, events[0].header.source));
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (CORBA::is_nil (this->proxy_consumer_.in ()))
      return;
    proxy = this->proxy_consumer_;
  }

  proxy->push (events);
}

void
Supplier::disconnect_push_supplier (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->proxy_consumer_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

PortableServer::POA_ptr
Supplier::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}
