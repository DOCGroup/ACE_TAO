/**
 * @file Consumer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Consumer.h"
#include "ECFS_Configuration.h"
#include "orbsvcs/Event_Service_Constants.h"

ACE_RCSID(EC_Federated_Scalability, Consumer, "$Id")

ECFS_Consumer::
     ECFS_Consumer (CORBA::Long experiment_id,
                    CORBA::ULong iterations)
  : experiment_id_ (experiment_id)
  , samples_ (iterations)
{
}

void
ECFS_Consumer::connect (RtecEventChannelAdmin::EventChannel_ptr ec
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
  h1.type   = ECFS_RESPONSE_EVENT_TYPE;
  h1.source = this->experiment_id_;

  this->proxy_supplier_->connect_push_consumer (consumer.in (),
                                                consumer_qos
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
ECFS_Consumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
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

const Control::Samples &
ECFS_Consumer::samples (void) const
{
  return this->samples_;
}

void
ECFS_Consumer::push (const RtecEventComm::EventSet &events
                     ACE_ENV_ARG_DECL_NOT_USED)
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
ECFS_Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->proxy_supplier_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}
