/**
 * @file Loopback_Supplier.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Loopback_Supplier.h"
#include "ECFS_Configuration.h"

ACE_RCSID(EC_Federated_Scalability, Loopback_Supplier, "$Id$")

ECFS_Loopback_Supplier::ECFS_Loopback_Supplier (CORBA::Long experiment_id)
  : experiment_id_ (experiment_id)
{
}

void
ECFS_Loopback_Supplier::connect (RtecEventChannelAdmin::EventChannel_ptr ec,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->for_suppliers (ACE_TRY_ENV);
  ACE_CHECK;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (!CORBA::is_nil (this->proxy_consumer_.in ()))
      return;

    this->proxy_consumer_ =
      supplier_admin->obtain_push_consumer (ACE_TRY_ENV);
    ACE_CHECK;
  }

  RtecEventComm::PushSupplier_var supplier =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  RtecEventChannelAdmin::SupplierQOS supplier_qos;
  supplier_qos.is_gateway = 0;
  supplier_qos.publications.length (1);
  RtecEventComm::EventHeader& sh0 =
    supplier_qos.publications[0].event.header;
  sh0.type   = ECFS_RESPONSE_EVENT_TYPE;
  sh0.source = this->experiment_id_;

  this->proxy_consumer_->connect_push_supplier (supplier.in (),
                                                supplier_qos,
                                                ACE_TRY_ENV);
  ACE_CHECK;
}

void
ECFS_Loopback_Supplier::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (CORBA::is_nil (this->proxy_consumer_.in ()))
      return;
    proxy = this->proxy_consumer_._retn ();
  }

  ACE_TRY
    {
      proxy->disconnect_push_consumer (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY {} ACE_ENDTRY;

  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var id = poa->servant_to_id (this,
                                                        ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
ECFS_Loopback_Supplier::push (const RtecEventComm::EventSet &source,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (CORBA::is_nil (this->proxy_consumer_.in ()))
      return;
    proxy = this->proxy_consumer_;
  }

  // ACE_DEBUG ((LM_DEBUG, "Loopback_Supplier::push (%P|%t)\n"));
  RtecEventComm::EventSet events (source);
  for (CORBA::ULong i = 0; i != events.length (); ++i)
    {
      events[i].header.ttl  = 1;
      events[i].header.type = ECFS_RESPONSE_EVENT_TYPE;
      events[i].header.source = this->experiment_id_;
    }

  proxy->push (events, ACE_TRY_ENV);
}

void
ECFS_Loopback_Supplier::disconnect_push_supplier (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->proxy_consumer_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}
