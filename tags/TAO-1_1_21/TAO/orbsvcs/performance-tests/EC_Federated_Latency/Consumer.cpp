/**
 * @file Consumer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Consumer.h"
#include "ECFL_Configuration.h"
#include "orbsvcs/Event_Service_Constants.h"

ACE_RCSID(EC_Federated_Latency, Consumer, "$Id$")

ECFL_Consumer::
     ECFL_Consumer (CORBA::Long experiment_id,
                    CORBA::ULong iterations)
  : experiment_id_ (experiment_id)
  , samples_ (iterations)
{
}

void
ECFL_Consumer::connect (RtecEventChannelAdmin::EventChannel_ptr ec,
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
  h1.type   = ECFL_RESPONSE_EVENT_TYPE;
  h1.source = this->experiment_id_;

  this->proxy_supplier_->connect_push_consumer (consumer.in (),
                                                consumer_qos,
                                                ACE_TRY_ENV);
  ACE_CHECK;
}

void
ECFL_Consumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
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

const Control::Samples &
ECFL_Consumer::samples (void) const
{
  return this->samples_;
}

void
ECFL_Consumer::push (const RtecEventComm::EventSet &events,
                     CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // ACE_DEBUG ((LM_DEBUG, "Consumer::push (%P|%t)\n"));
  ACE_hrtime_t now = ACE_OS::gethrtime ();

  ACE_hrtime_t creation;
  ORBSVCS_Time::TimeT_to_hrtime (creation,
                                 events[0].header.creation_time);

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  CORBA::ULong l = this->samples_.length ();
  this->samples_.length (l + 1);
  this->samples_[l] = now - creation;
}

void
ECFL_Consumer::disconnect_push_consumer (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->proxy_supplier_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}
