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
#include "ECFL_Configuration.h"

#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Gateway.h"

#include "ace/High_Res_Timer.h"

ACE_RCSID (EC_Federated_Latency,
           Peer,
           "$Id$")

ECFL_Peer::ECFL_Peer (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

ECFL_Peer::~ECFL_Peer (void)
{
}

void
ECFL_Peer::init (PortableServer::POA_ptr root_poa
                 TAO_ENV_ARG_DECL)
{
  TAO_EC_Event_Channel_Attributes attr (root_poa, root_poa);
  Servant_var<TAO_EC_Event_Channel> ec_impl (
         new TAO_EC_Event_Channel (attr)
         );

  ec_impl->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->event_channel_ =
    ec_impl->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

RtecEventChannelAdmin::EventChannel_ptr
ECFL_Peer::channel (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return RtecEventChannelAdmin::EventChannel::_duplicate (this->event_channel_.in ());
}

void
ECFL_Peer::connect (RtecEventChannelAdmin::EventChannel_ptr remote_ec
                    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) Connecting....\n"));
  Servant_var<TAO_EC_Gateway_IIOP> gateway (new TAO_EC_Gateway_IIOP);
  gateway->init (remote_ec,
                 this->event_channel_.in ()
                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::Observer_var observer =
    gateway->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::Observer_Handle h =
    this->event_channel_->append_observer (observer.in ()
                                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  gateway->observer_handle (h);
}

Control::Loopback_ptr
ECFL_Peer::setup_loopback (CORBA::Long experiment_id
                           TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Servant_var<ECFL_Loopback> loopback (new ECFL_Loopback);

  loopback->init (experiment_id,
                  this->event_channel_.in ()
                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Control::Loopback::_nil ());

  return loopback->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
}

Control::Samples *
ECFL_Peer::run_experiment (CORBA::Long experiment_id,
                           CORBA::Long iterations,
                           CORBA::Long_out gsf
                           TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Servant_var<ECFL_Consumer> consumer (
          new ECFL_Consumer (experiment_id,
                             iterations)
          );

  consumer->connect (this->event_channel_.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  Servant_var<ECFL_Supplier> supplier (
          new ECFL_Supplier (experiment_id)
          );

  supplier->connect (this->event_channel_.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // ACE_DEBUG ((LM_DEBUG, "Connected consumer & supplier\n"));

  RtecEventComm::EventSet event (1);
  event.length (1);
  event[0].header.type   = ECFL_START_EVENT_TYPE;
  event[0].header.source = experiment_id;
  event[0].header.ttl    = 1;

  for (int i = 0; i != iterations; ++i)
    {
      ACE_hrtime_t creation = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                     creation);
      // push one event...
      supplier->push (event TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  supplier->disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  consumer->disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
  ACE_High_Res_Timer::calibrate ();
  gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

  return new Control::Samples (consumer->samples ());
}

void
ECFL_Peer::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->event_channel_->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class Servant_var<TAO_EC_Event_Channel>;
template class Servant_var<TAO_EC_Gateway_IIOP>;
template class Servant_var<ECFL_Consumer>;
template class Servant_var<ECFL_Supplier>;
template class Servant_var<ECFL_Loopback>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate Servant_var<TAO_EC_Event_Channel>
#pragma instantiate Servant_var<TAO_EC_Gateway_IIOP>
#pragma instantiate Servant_var<ECFL_Consumer>
#pragma instantiate Servant_var<ECFL_Supplier>
#pragma instantiate Servant_var<ECFL_Loopback>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
