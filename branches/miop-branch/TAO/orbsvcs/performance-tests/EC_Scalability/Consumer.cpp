/**
 * @file Consumer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Consumer.h"
#include "orbsvcs/Event_Service_Constants.h"

ACE_RCSID(EC_Scalability, Consumer, "$Id$")

ECS_Consumer::ECS_Consumer (int iterations)
  : sample_history_ (iterations)
{
}

void
ECS_Consumer::connect (RtecEventChannelAdmin::EventChannel_ptr ec
                       TAO_ENV_ARG_DECL)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (!CORBA::is_nil (this->proxy_supplier_.in ()))
      return;

    this->proxy_supplier_ =
      consumer_admin->obtain_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }

  RtecEventComm::PushConsumer_var consumer =
    this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::ConsumerQOS consumer_qos;
  consumer_qos.dependencies.length (2);
  RtecEventComm::EventHeader& h0 =
    consumer_qos.dependencies[0].event.header;
  h0.type   = ACE_ES_DISJUNCTION_DESIGNATOR;
  h0.source = 1;

  RtecEventComm::EventHeader& h1 =
    consumer_qos.dependencies[1].event.header;
  h1.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
  h1.source = ACE_ES_EVENT_SOURCE_ANY;

  this->proxy_supplier_->connect_push_consumer (consumer.in (),
                                                consumer_qos
                                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
ECS_Consumer::disconnect (TAO_ENV_SINGLE_ARG_DECL)
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
      proxy->disconnect_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY {} ACE_ENDTRY;

  PortableServer::POA_var poa = this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var id = poa->servant_to_id (this
                                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

ACE_Sample_History &
ECS_Consumer::sample_history (void)
{
  return this->sample_history_;
}

void
ECS_Consumer::push (const RtecEventComm::EventSet &events
                    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_hrtime_t now = ACE_OS::gethrtime ();

  ACE_hrtime_t creation;
  ORBSVCS_Time::TimeT_to_hrtime (creation,
                                 events[0].header.creation_time);

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->sample_history_.sample (now - creation);
}

void
ECS_Consumer::disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->proxy_supplier_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}
