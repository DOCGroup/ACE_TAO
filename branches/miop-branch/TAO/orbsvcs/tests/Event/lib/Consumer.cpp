// $Id$

#include "Consumer.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/debug.h"

ACE_RCSID(EC_Tests, EC_Consumer, "$Id$")

EC_Consumer::EC_Consumer (EC_Driver *driver,
                          void *cookie)
  : driver_ (driver),
    cookie_ (cookie),
    push_count_ (0),
    shutdown_event_type_ (ACE_ES_EVENT_SHUTDOWN),
    is_active_ (0)
{
}

void
EC_Consumer::connect (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    const RtecEventChannelAdmin::ConsumerQOS& qos,
    int shutdown_event_type
    TAO_ENV_ARG_DECL)
{
  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->connect (qos, shutdown_event_type TAO_ENV_ARG_PARAMETER);
}

void
EC_Consumer::connect (
    const RtecEventChannelAdmin::ConsumerQOS& qos,
    int shutdown_event_type
    TAO_ENV_ARG_DECL)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return; // @@ Throw?

  this->shutdown_event_type_ = shutdown_event_type;

  if (CORBA::is_nil (this->myself_.in ()))
    {
      this->myself_ = this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  this->is_active_ = 1;

  this->supplier_proxy_->connect_push_consumer (this->myself_.in (),
                                                qos
                                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
EC_Consumer::connected (void) const
{
  return !CORBA::is_nil (this->supplier_proxy_.in ());
}

void
EC_Consumer::disconnect (TAO_ENV_SINGLE_ARG_DECL)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
EC_Consumer::shutdown (TAO_ENV_SINGLE_ARG_DECL)
{
  if (!this->is_active_)
    return;

  // Deactivate the servant
  PortableServer::POA_var poa =
    this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->myself_ = RtecEventComm::PushConsumer::_nil ();
  this->is_active_ = 0;
}

void
EC_Consumer::dump_results (const char* name,
                           ACE_UINT32 gsf)
{
  this->throughput_.dump_results (name, gsf);
}

void
EC_Consumer::accumulate (ACE_Throughput_Stats& throughput) const
{
  throughput.accumulate (this->throughput_);
}

void
EC_Consumer::push (const RtecEventComm::EventSet& events
                   TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->driver_->consumer_push (this->cookie_, events TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "EC_Consumer (%P|%t) no events\n"));
      return;
    }

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  if (this->push_count_ == 0)
    this->throughput_start_ = ACE_OS::gethrtime ();

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
      this->throughput_.sample (now - this->throughput_start_,
                                now - creation);

      if (e.header.type == this->shutdown_event_type_)
        this->driver_->consumer_shutdown (this->cookie_ TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
EC_Consumer::disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->driver_->consumer_disconnect (this->cookie_ TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
