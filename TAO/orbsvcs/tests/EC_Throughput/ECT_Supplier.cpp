// $Id$

#include "ECT_Supplier.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/Timeprobe.h"
#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"
#include "ace/ACE.h"
#include "ace/OS_NS_unistd.h"

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
                        RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->burst_count_ = burst_count;
  this->burst_size_ = burst_size;
  this->event_size_ = event_size;
  this->burst_pause_ = burst_pause;
  this->type_start_ = type_start;
  this->type_count_ = type_count;

  RtecScheduler::handle_t rt_info =
    scheduler->create (name);

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
                  RtecScheduler::OPERATION);

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
    ec->for_suppliers ();

  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer ();

  RtecEventComm::PushSupplier_var objref =
    this->supplier_._this ();

  this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                qos.get_SupplierQOS ());
}

void
Test_Supplier::disconnect (void)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  try
    {
      this->consumer_proxy_->disconnect_push_consumer ();
    }
  catch (const CORBA::Exception&)
    {
      // The consumer may be gone already, so we
      // will ignore this exception
    }

  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();

  // Deactivate the servant
  PortableServer::POA_var poa =
    this->supplier_._default_POA ();
  PortableServer::ObjectId_var id =
    poa->servant_to_id (&this->supplier_);
  poa->deactivate_object (id.in ());
}

int
Test_Supplier::svc ()
{
  try
    {
      // Initialize a time value to pace the test
      ACE_Time_Value tv (0, this->burst_pause_);

      // Pre-allocate a message to send
      ACE_Message_Block mb (this->event_size_);
      mb.wr_ptr (this->event_size_);

      RtecEventComm::EventSet event (1);
      event.length (1);
      event[0].header.source = this->supplier_id ();
      event[0].header.ttl = 1;

      ACE_hrtime_t t = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time, t);

      // We use replace to minimize the copies, this should result
      // in just one memory allocation;
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      event[0].data.payload.replace (this->event_size_,
                                     &mb);
#else
      // If the replace method is not available, we will need
      // to do the copy manually.  First, set the octet sequence length.
      event[0].data.payload.length (this->event_size_);

      // Now copy over each byte.
      char* base = mb.data_block ()->base ();
      for(CORBA::ULong i = 0; i < (CORBA::ULong)this->event_size_; i++)
        {
          event[0].data.payload[i] = base[i];
        }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

      ACE_hrtime_t test_start = ACE_OS::gethrtime ();

      for (int i = 0; i < this->burst_count_; ++i)
        {
          for (int j = 0; j < this->burst_size_; ++j)
            {
              event[0].header.type =
                this->type_start_ + j % this->type_count_;

              ACE_hrtime_t request_start = ACE_OS::gethrtime ();
              ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                             request_start);
              // ACE_DEBUG ((LM_DEBUG, "(%t) supplier push event\n"));
              this->consumer_proxy ()->push (event);

              ACE_hrtime_t end = ACE_OS::gethrtime ();
              this->throughput_.sample (end - test_start,
                                        end - request_start);
            }

          if (TAO_debug_level > 0
              && i % 100 == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "ECT_Supplier (%P|%t): %d bursts sent\n",
                          i));
            }

          ACE_OS::sleep (tv);
        }

      // Send one event shutdown from each supplier
      event[0].header.type = ACE_ES_EVENT_SHUTDOWN;
      ACE_hrtime_t request_start = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                     request_start);
      this->consumer_proxy ()->push(event);
      ACE_hrtime_t end = ACE_OS::gethrtime ();
      this->throughput_.sample (end - test_start,
                                end - request_start);
    }
  catch (const CORBA::SystemException& sys_ex)
    {
      sys_ex._tao_print_exception ("SYS_EX");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("NON SYS EX");
    }

  ACE_DEBUG ((LM_DEBUG,
              "Supplier %4.4x completed\n",
              this->supplier_id_));
  return 0;
}

void
Test_Supplier::disconnect_push_supplier (void)
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
Test_Supplier::dump_results (const ACE_TCHAR* name,
                             ACE_UINT32 gsf)
{
  this->throughput_.dump_results (name, gsf);
}

void
Test_Supplier::accumulate (ACE_Throughput_Stats& stats) const
{
  stats.accumulate (this->throughput_);
}
