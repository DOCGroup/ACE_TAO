// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"

#include "tao/Timeprobe.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Time_Utilities.h"
#include "ECT_Supplier.h"

ACE_RCSID(EC_Throughput, ECT_Supplier, "$Id$")

Test_Supplier::Test_Supplier (ECT_Driver *driver)
  :  driver_ (driver),
     supplier_ (this),
     burst_count_ (0),
     burst_size_ (0),
     event_size_ (0),
     burst_pause_ (0),
     type_start_ (ACE_ES_EVENT_UNDEFINED),
     type_count_ (1)
{
}

void
Test_Supplier::connect (RtecScheduler::Scheduler_ptr scheduler,
                        const char* name,
                        int burst_count,
                        int burst_size,
                        int event_size,
                        int burst_pause,
                        int type_start,
                        int type_count,
                        RtecEventChannelAdmin::EventChannel_ptr ec,
                        CORBA::Environment &TAO_IN_ENV)
{
  this->burst_count_ = burst_count;
  this->burst_size_ = burst_size;
  this->event_size_ = event_size;
  this->burst_pause_ = burst_pause;
  this->type_start_ = type_start;
  this->type_count_ = type_count;

  RtecScheduler::handle_t rt_info =
    scheduler->create (name, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  ACE_Time_Value tv (0, burst_pause);
  RtecScheduler::Period_t rate = tv.usec () * 10;

  // The execution times are set to reasonable values, but
  // actually they are changed on the real execution, i.e. we
  // lie to the scheduler to obtain right priorities; but we
  // don't care if the set is schedulable.
  tv.set (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  scheduler->set (rt_info,
                  RtecScheduler::VERY_HIGH_CRITICALITY,
                  time, time, time,
                  rate,
                  RtecScheduler::VERY_LOW_IMPORTANCE,
                  time,
                  1,
                  RtecScheduler::OPERATION,
                  TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->supplier_id_ = ACE::crc32 (name);
  ACE_DEBUG ((LM_DEBUG, "ID for <%s> is %04.4x\n", name,
              this->supplier_id_));

  ACE_SupplierQOS_Factory qos;
  for (int i = 0; i != type_count; ++i)
    {
      qos.insert (this->supplier_id_,
                  type_start + i,
                  rt_info, 1);
    }
  qos.insert (this->supplier_id_,
              ACE_ES_EVENT_SHUTDOWN,
              rt_info, 1);

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->for_suppliers (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  RtecEventComm::PushSupplier_var objref =
    this->supplier_._this (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                qos.get_SupplierQOS (),
                                                TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
}

void
Test_Supplier::disconnect (CORBA::Environment &TAO_IN_ENV)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();

  // Deactivate the servant
  PortableServer::POA_var poa =
    this->supplier_._default_POA (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
  PortableServer::ObjectId_var id =
    poa->servant_to_id (&this->supplier_, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
  poa->deactivate_object (id.in (), TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

int
Test_Supplier::svc ()
{
  int min_priority =
    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO);
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::thr_setprio (min_priority) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Test_Supplier::svc (%P|%t) "
                  "thr_setprio failed\n"));
    }

  TAO_TRY
    {
      ACE_Time_Value tv (0, this->burst_pause_);

      ACE_Message_Block mb (this->event_size_);
      mb.wr_ptr (this->event_size_);

      RtecEventComm::EventSet event (1);
      event.length (1);
      event[0].header.source = this->supplier_id ();
      event[0].header.ttl = 1;

      ACE_hrtime_t t = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time, t);
      event[0].header.ec_recv_time = ORBSVCS_Time::zero;
      event[0].header.ec_send_time = ORBSVCS_Time::zero;

      event[0].data.x = 0;
      event[0].data.y = 0;

      // We use replace to minimize the copies, this should result
      // in just one memory allocation;
      event[0].data.payload.replace (this->event_size_,
                                     &mb);

      this->timer_.start ();
      for (int i = 0; i < this->burst_count_; ++i)
        {
          for (int j = 0; j < this->burst_size_; ++j)
            {
              event[0].header.type =
                this->type_start_ + j % this->type_count_;

              ACE_hrtime_t now = ACE_OS::gethrtime ();
              ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                             now);
              // ACE_DEBUG ((LM_DEBUG, "(%t) supplier push event\n"));
              this->consumer_proxy ()->push (event, TAO_TRY_ENV);

              TAO_CHECK_ENV;
            }
          ACE_OS::sleep (tv);
        }

      // Send one event shutdown from each supplier
      event[0].header.type = ACE_ES_EVENT_SHUTDOWN;
      this->consumer_proxy ()->push(event, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->timer_.stop ();

    }
  TAO_CATCH (CORBA::SystemException, sys_ex)
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("NON SYS EX");
    }
  TAO_ENDTRY;
  return 0;
}

void
Test_Supplier::disconnect_push_supplier (CORBA::Environment &)
{
}

int Test_Supplier::supplier_id (void) const
{
  return this->supplier_id_;
}

RtecEventChannelAdmin::ProxyPushConsumer_ptr
Test_Supplier::consumer_proxy (void)
{
  return this->consumer_proxy_.in ();
}

void
Test_Supplier::dump_results (const char* name)
{
  ACE_Time_Value tv;
  this->timer_.elapsed_time (tv);

  int event_count = this->burst_count_ * this->burst_size_ + 1;
  double f = 1.0 / (tv.sec () + tv.usec () / 1000000.0);
  double eps = event_count * f;

  ACE_DEBUG ((LM_DEBUG,
              "ECT_Supplier (%s):\n"
              "    Total time: %d.%08.8d (secs.usecs)\n"
              "    Total events: %d\n"
              "    Events per second: %.3f\n",
              name,
              tv.sec (), tv.usec (),
              event_count,
              eps));
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_PushSupplier_Adapter<Test_Supplier>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_PushSupplier_Adapter<Test_Supplier>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
