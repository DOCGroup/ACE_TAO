// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"

#include "tao/Timeprobe.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Time_Utilities.h"
#include "ECT_Consumer.h"

ACE_RCSID(EC_Throughput, ECT_Consumer, "$Id$")

Test_Consumer::Test_Consumer (ECT_Driver *driver,
                              void *cookie,
                              int n_suppliers)
  : driver_ (driver),
    cookie_ (cookie),
    n_suppliers_ (n_suppliers),
    recv_count_ (0),
    shutdown_count_ (0)
{
}

void
Test_Consumer::connect (RtecScheduler::Scheduler_ptr scheduler,
                        const char* name,
                        int type_start,
                        int type_count,
                        RtecEventChannelAdmin::EventChannel_ptr ec,
                        CORBA::Environment& TAO_IN_ENV)
{
  RtecScheduler::handle_t rt_info =
    scheduler->create (name, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  // The worst case execution time is far less than 2
  // milliseconds, but that is a safe estimate....
  ACE_Time_Value tv (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  scheduler->set (rt_info,
                  RtecScheduler::VERY_HIGH_CRITICALITY,
                  time, time, time,
                  0,
                  RtecScheduler::VERY_LOW_IMPORTANCE,
                  time,
                  0,
                  RtecScheduler::OPERATION,
                  TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group ();
  qos.insert_type (ACE_ES_EVENT_SHUTDOWN, rt_info);
  for (int i = 0; i != type_count; ++i)
    {
      qos.insert_type (type_start + i, rt_info);
    }

  // = Connect as a consumer.
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  RtecEventComm::PushConsumer_var objref = this->_this (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                qos.get_ConsumerQOS (),
                                                TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
}

void
Test_Consumer::disconnect (CORBA::Environment &TAO_IN_ENV)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();

  // Deactivate the servant
  PortableServer::POA_var poa =
    this->_default_POA (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
  poa->deactivate_object (id.in (), TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
}

void
Test_Consumer::dump_results (const char* name)
{
  this->throughput_.dump_results ("ECT_Consumers", name);
  this->latency_.dump_results ("ECT_Consumers", name);
}

void
Test_Consumer::accumulate (ECT_Driver::Throughput_Stats& stats) const
{
  stats.accumulate (this->throughput_);
}

void
Test_Consumer::accumulate (ECT_Driver::Latency_Stats& stats) const
{
  stats.accumulate (this->latency_);
}

void
Test_Consumer::push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &TAO_IN_ENV)
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ECT_Consumer (%P|%t) no events\n"));
      return;
    }

  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

  // We start the timer as soon as we receive the first event...
  if (this->recv_count_ == 0)
    this->throughput_.start ();

  this->throughput_.sample ();

  this->recv_count_ += events.length ();

  if (TAO_debug_level > 0
      && this->recv_count_ % 100 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ECT_Consumer (%P|%t): %d events received\n",
                  this->recv_count_));
    }

  // ACE_DEBUG ((LM_DEBUG, "%d event(s)\n", events.length ()));

  for (u_int i = 0; i < events.length (); ++i)
    {
      const RtecEventComm::Event& e = events[i];

      if (e.header.type == ACE_ES_EVENT_SHUTDOWN)
        {
          this->shutdown_count_++;
          if (this->shutdown_count_ >= this->n_suppliers_)
            {
              // We stop the timer as soon as we realize it is time to
              // do so.
              this->throughput_.stop ();
              this->driver_->shutdown_consumer (this->cookie_, TAO_IN_ENV);
            }
        }
      else
        {
          ACE_hrtime_t creation;
          ORBSVCS_Time::TimeT_to_hrtime (creation,
                                         e.header.creation_time);

          const ACE_hrtime_t now = ACE_OS::gethrtime ();
          const ACE_hrtime_t elapsed = now - creation;
          this->latency_.sample (elapsed);

#if 0
          ACE_hrtime_t ec_recv;
          ORBSVCS_Time::TimeT_to_hrtime (ec_recv,
                                         e.header.ec_recv_time);

          ACE_hrtime_t ec_send;
          ORBSVCS_Time::TimeT_to_hrtime (ec_send,
                                         e.header.ec_send_time);

          this->driver_->supplier_to_ec (ec_recv - creation);
          this->driver_->inside_ec (ec_send - ec_recv);
          this->driver_->ec_to_consumer (now - ec_send);
#endif /* 0 */
        }
    }
}

void
Test_Consumer::disconnect_push_consumer (CORBA::Environment &)
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
