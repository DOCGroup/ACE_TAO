/**
 * @file Loopback_Consumer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Loopback_Consumer.h"
#include "orbsvcs/Event_Service_Constants.h"

ACE_RCSID(PERF_RTEC, Loopback_Consumer, "$Id$")

Loopback_Consumer::
Loopback_Consumer (CORBA::Long experiment_id,
                   CORBA::Long event_type,
                   Loopback_Supplier *supplier)
  : experiment_id_ (experiment_id)
  , event_type_ (event_type)
  , supplier_ (Servant_var<Loopback_Supplier>::duplicate (supplier))
{
}

void
Loopback_Consumer::connect (RtecEventChannelAdmin::EventChannel_ptr ec
                            ACE_ENV_ARG_DECL)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (!CORBA::is_nil (this->proxy_supplier_.in ()))
      return;

    this->proxy_supplier_ =
      consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }

  RtecEventComm::PushConsumer_var consumer =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
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
  h1.source = this->experiment_id_;
  h1.type   = this->event_type_;

  this->proxy_supplier_->connect_push_consumer (consumer.in (),
                                                consumer_qos
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Loopback_Consumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
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
      proxy->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY {} ACE_ENDTRY;

  PortableServer::POA_var poa = this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var id = poa->servant_to_id (this
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Loopback_Consumer::push (const RtecEventComm::EventSet &events
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // ACE_DEBUG ((LM_DEBUG, "Loopback_Consumer::push (%P|%t)\n"));
  this->supplier_->push (events ACE_ENV_ARG_PARAMETER);
}

void
Loopback_Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->proxy_supplier_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}
