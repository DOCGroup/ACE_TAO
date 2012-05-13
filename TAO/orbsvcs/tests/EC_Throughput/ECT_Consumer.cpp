// $Id$

#include "ECT_Consumer.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/Timeprobe.h"
#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_unistd.h"

Test_Consumer::Test_Consumer (ECT_Driver *driver,
                              void *cookie,
                              int n_suppliers,
                              int stall_length)
  : driver_ (driver),
    cookie_ (cookie),
    n_suppliers_ (n_suppliers),
    recv_count_ (0),
    shutdown_count_ (0),
    stall_length_(stall_length)
{
}

void
Test_Consumer::connect (RtecScheduler::Scheduler_ptr scheduler,
                        const char* name,
                        int type_start,
                        int type_count,
                        RtecEventChannelAdmin::EventChannel_ptr ec)
{
  RtecScheduler::handle_t rt_info =
    scheduler->create (name);

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
                  RtecScheduler::OPERATION);

  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group ();
  qos.insert_type (ACE_ES_EVENT_SHUTDOWN, rt_info);
  for (int i = 0; i != type_count; ++i)
    {
      qos.insert_type (type_start + i, rt_info);
    }

  // = Connect as a consumer.
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers ();

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier ();

  RtecEventComm::PushConsumer_var objref = this->_this ();

  this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                qos.get_ConsumerQOS ());
}

void
Test_Consumer::disconnect (void)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier ();

  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();

  // Deactivate the servant
  PortableServer::POA_var poa =
    this->_default_POA ();
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);
  poa->deactivate_object (id.in ());
}

void
Test_Consumer::dump_results (const ACE_TCHAR* name,
                             ACE_Basic_Stats::scale_factor_type gsf)
{
  this->throughput_.dump_results (name, gsf);
}

void
Test_Consumer::accumulate (ACE_Throughput_Stats& stats) const
{
  stats.accumulate (this->throughput_);
}

void
Test_Consumer::push (const RtecEventComm::EventSet& events)
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ECT_Consumer (%P|%t) no events\n"));
      return;
    }

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  // We start the timer as soon as we receive the first event...
  if (this->recv_count_ == 0)
    {
      this->first_event_ = ACE_OS::gethrtime ();
      ACE_DEBUG ((LM_DEBUG,
                "ECT_Consumer (%P|%t) stalling for %d seconds\n", this->stall_length_));
      ACE_OS::sleep(this->stall_length_);
      ACE_DEBUG ((LM_DEBUG, "ECT_Consumer (%P|%t) finished stalling\n"));
    }


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
              this->driver_->shutdown_consumer (this->cookie_);
            }
        }
      else
        {
          ACE_hrtime_t creation;
          ORBSVCS_Time::TimeT_to_hrtime (creation,
                                         e.header.creation_time);

          const ACE_hrtime_t now = ACE_OS::gethrtime ();
          this->throughput_.sample (now - this->first_event_,
                                    now - creation);
        }
    }
}

void
Test_Consumer::disconnect_push_consumer (void)
{
}
