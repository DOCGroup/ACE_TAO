/**
 * @file Loopback_Consumer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Loopback_Consumer.h"
#include "ECFL_Configuration.h"
#include "orbsvcs/Event_Service_Constants.h"

ACE_RCSID(EC_Federated_Latency, Loopback_Consumer, "$Id$")

ECFL_Loopback_Consumer::
     ECFL_Loopback_Consumer (CORBA::Long experiment_id,
                             ECFL_Loopback_Supplier *supplier)
  : experiment_id_ (experiment_id)
  , supplier_ (supplier)
{
  this->supplier_->_add_ref ();
}

void
ECFL_Loopback_Consumer::connect (RtecEventChannelAdmin::EventChannel_ptr ec,
                       CORBA::Environment &ACE_TRY_ENV)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers (ACE_TRY_ENV);
  ACE_CHECK;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (!CORBA::is_nil (this->proxy_supplier_.in ()))
      return;

    this->proxy_supplier_ =
      consumer_admin->obtain_push_supplier (ACE_TRY_ENV);
    ACE_CHECK;
  }

  RtecEventComm::PushConsumer_var consumer =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  RtecEventChannelAdmin::ConsumerQOS consumer_qos;
  consumer_qos.is_gateway = 0;
  consumer_qos.dependencies.length (2);
  RtecEventComm::EventHeader& h0 =
    consumer_qos.dependencies[0].event.header;
  h0.type   = ACE_ES_DISJUNCTION_DESIGNATOR;
  h0.source = 1;

  RtecEventComm::EventHeader& h1 =
    consumer_qos.dependencies[1].event.header;
  h1.type   = ECFL_START_EVENT_TYPE;
  h1.source = this->experiment_id_;

  this->proxy_supplier_->connect_push_consumer (consumer.in (),
                                                consumer_qos,
                                                ACE_TRY_ENV);
  ACE_CHECK;
}

void
ECFL_Loopback_Consumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (CORBA::is_nil (this->proxy_supplier_.in ()))
      return;
    proxy = this->proxy_supplier_._retn ();
  }

  ACE_TRY
    {
      proxy->disconnect_push_supplier (ACE_TRY_ENV);
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
ECFL_Loopback_Consumer::push (const RtecEventComm::EventSet &events,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // ACE_DEBUG ((LM_DEBUG, "Loopback_Consumer::push (%P|%t)\n"));
  this->supplier_->push (events, ACE_TRY_ENV);
}

void
ECFL_Loopback_Consumer::disconnect_push_consumer (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->proxy_supplier_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}
