// $Id$

#include "Consumer.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Time_Utilities.h"

ACE_RCSID(EC_Tests, EC_Consumer, "$Id$")

EC_Consumer::EC_Consumer (EC_Driver *driver,
                          void *cookie)
  : driver_ (driver),
    cookie_ (cookie),
    push_count_ (0),
    shutdown_event_type_ (ACE_ES_EVENT_SHUTDOWN)
{
}

void
EC_Consumer::connect (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    const RtecEventChannelAdmin::ConsumerQOS& qos,
    int shutdown_event_type,
    CORBA::Environment &ACE_TRY_ENV)
{
  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (ACE_TRY_ENV);
  ACE_CHECK;

  this->connect (qos, shutdown_event_type, ACE_TRY_ENV);
}

void
EC_Consumer::connect (
    const RtecEventChannelAdmin::ConsumerQOS& qos,
    int shutdown_event_type,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return; // @@ Throw?

  this->shutdown_event_type_ = shutdown_event_type;

  RtecEventComm::PushConsumer_var objref = this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                qos,
                                                ACE_TRY_ENV);
  ACE_CHECK;
}

int
EC_Consumer::connected (void) const
{
  return !CORBA::is_nil (this->supplier_proxy_.in ());
}

void
EC_Consumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();

  // Deactivate the servant
  PortableServer::POA_var poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  poa->deactivate_object (id.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
EC_Consumer::dump_results (const char* name)
{
  this->throughput_.dump_results ("EC_Consumers", name);
  this->latency_.dump_results ("EC_Consumers", name);
}

void
EC_Consumer::accumulate (EC_Driver::Throughput_Stats& throughput,
                           EC_Driver::Latency_Stats& latency) const
{
  throughput.accumulate (this->throughput_);
  latency.accumulate (this->latency_);
}

void
EC_Consumer::push (const RtecEventComm::EventSet& events,
                   CORBA::Environment &ACE_TRY_ENV)
{
  this->driver_->consumer_push (this->cookie_, events, ACE_TRY_ENV);

  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "EC_Consumer (%P|%t) no events\n"));
      return;
    }

  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
  if (this->push_count_ == 0)
    this->throughput_.start ();

  // We start the timer as soon as we receive the first event...
  this->throughput_.sample ();

  this->push_count_ += events.length ();

  if (TAO_debug_level > 0
      && this->push_count_ % 100 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "EC_Consumer (%P|%t): %d events received\n",
                  this->push_count_));
    }

  for (u_int i = 0; i < events.length (); ++i)
    {
      const RtecEventComm::Event& e = events[i];

      ACE_hrtime_t creation;
      ORBSVCS_Time::TimeT_to_hrtime (creation,
                                     e.header.creation_time);

      const ACE_hrtime_t now = ACE_OS::gethrtime ();
      const ACE_hrtime_t elapsed = now - creation;
      this->latency_.sample (elapsed);

      if (e.header.type == this->shutdown_event_type_)
        this->driver_->consumer_shutdown (this->cookie_, ACE_TRY_ENV);
    }
}

void
EC_Consumer::disconnect_push_consumer (CORBA::Environment &ACE_TRY_ENV)
{
  this->driver_->consumer_disconnect (this->cookie_, ACE_TRY_ENV);
  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
