/**
 * @file Peer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Peer.h"
#include "Servant_var.h"
#include "Consumer.h"
#include "Supplier.h"
#include "Loopback.h"
#include "ECFS_Configuration.h"

#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Gateway.h"

#include "ace/High_Res_Timer.h"

ACE_RCSID(EC_Federated_Scalability, Peer, "$Id$")

ECFS_Peer::ECFS_Peer (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

ECFS_Peer::~ECFS_Peer (void)
{
}

void
ECFS_Peer::init (PortableServer::POA_ptr root_poa,
                 CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_Event_Channel_Attributes attr (root_poa, root_poa);
  Servant_var<TAO_EC_Event_Channel> ec_impl (
         new TAO_EC_Event_Channel (attr)
         );

  ec_impl->activate (ACE_TRY_ENV);
  ACE_CHECK;

  this->event_channel_ =
    ec_impl->_this (ACE_TRY_ENV);
  ACE_CHECK;
}

RtecEventChannelAdmin::EventChannel_ptr
ECFS_Peer::channel (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RtecEventChannelAdmin::EventChannel::_duplicate (this->event_channel_.in ());
}

void
ECFS_Peer::connect (RtecEventChannelAdmin::EventChannel_ptr remote_ec,
                    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) Connecting....\n"));
  Servant_var<TAO_EC_Gateway_IIOP> gateway (new TAO_EC_Gateway_IIOP);
  gateway->init (remote_ec,
                 this->event_channel_.in (),
                 ACE_TRY_ENV);
  ACE_CHECK;

  RtecEventChannelAdmin::Observer_var observer =
    gateway->_this (ACE_TRY_ENV);
  ACE_CHECK;

  RtecEventChannelAdmin::Observer_Handle h =
    this->event_channel_->append_observer (observer.in (),
                                           ACE_TRY_ENV);
  ACE_CHECK;

  gateway->observer_handle (h);
}

Control::Loopback_ptr
ECFS_Peer::setup_loopback (CORBA::Long experiment_id,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Servant_var<ECFS_Loopback> loopback (new ECFS_Loopback);

  loopback->init (experiment_id,
                  this->event_channel_.in (),
                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (Control::Loopback::_nil ());

  return loopback->_this (ACE_TRY_ENV);
}

Control::Samples *
ECFS_Peer::run_experiment (CORBA::Long consumer_count,
                           CORBA::Long experiment_id,
                           CORBA::Long iterations,
                           CORBA::Long_out gsf,
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Servant_var<ECFS_Consumer> *consumer;
  ACE_NEW_THROW_EX (consumer,
                    Servant_var<ECFS_Consumer>[consumer_count],
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);
  for (int i = 0; i != consumer_count; ++i)
    {
      consumer[i] =
        Servant_var<ECFS_Consumer> (new ECFS_Consumer (experiment_id,
                                                       iterations));
      consumer[i]->connect (this->event_channel_.in (), ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  Servant_var<ECFS_Supplier> supplier (
          new ECFS_Supplier (experiment_id)
          );

  supplier->connect (this->event_channel_.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // ACE_DEBUG ((LM_DEBUG, "Connected consumer & supplier\n"));

  RtecEventComm::EventSet event (1);
  event.length (1);
  event[0].header.type   = ECFS_START_EVENT_TYPE;
  event[0].header.source = experiment_id;
  event[0].header.ttl    = 1;

  for (int i = 0; i != iterations; ++i)
    {
      ACE_hrtime_t creation = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                     creation);
      // push one event...
      supplier->push (event, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  supplier->disconnect (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  Control::Samples_var samples (new Control::Samples (iterations));
  samples->length (iterations);
  for (int j = 0; j != iterations; ++j)
    samples[j] = 0;

  for (int i = 0; i != consumer_count; ++i)
    {
      for (int j = 0; j != iterations; ++j)
        {
          CORBA::ULongLong sample =
            consumer[i]->samples ()[j];
          if (samples[j] < sample)
            samples[j] = sample;
        }
      consumer[i]->disconnect (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
  ACE_High_Res_Timer::calibrate ();
  gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

  return samples._retn ();
}

void
ECFS_Peer::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->event_channel_->destroy (ACE_TRY_ENV);
  ACE_CHECK;

  this->orb_->shutdown (0, ACE_TRY_ENV);
  ACE_CHECK;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class Servant_var<TAO_EC_Event_Channel>;
template class Servant_var<TAO_EC_Gateway_IIOP>;
template class Servant_var<ECFS_Consumer>;
template class Servant_var<ECFS_Supplier>;
template class Servant_var<ECFS_Loopback>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate Servant_var<TAO_EC_Event_Channel>
#pragma instantiate Servant_var<TAO_EC_Gateway_IIOP>
#pragma instantiate Servant_var<ECFS_Consumer>
#pragma instantiate Servant_var<ECFS_Supplier>
#pragma instantiate Servant_var<ECFS_Loopback>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
