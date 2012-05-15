// $Id$

#include "Supplier.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Utilities.h"

#include "tao/debug.h"
#include "ace/OS_NS_unistd.h"

EC_Supplier::EC_Supplier (EC_Driver *driver,
                          void* cookie)
  :  driver_ (driver),
     cookie_ (cookie),
     push_count_ (0),
     burst_count_ (0),
     burst_size_ (0),
     payload_size_ (0),
     burst_pause_ (0),
     shutdown_event_type_ (0),
     is_active_ (0)
{
}

void
EC_Supplier::send_event (int event_number)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  // Create the event...

  RtecEventComm::EventSet event (1);
  event.length (1);

  event[0].header.ttl = 1;

  ACE_hrtime_t t = ACE_OS::gethrtime ();
  ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time, t);

  // We use replace to minimize the copies, this should result
  // in just one memory allocation:
  event[0].data.payload.length (this->payload_size_);

  this->event_type (event_number, event[0]);

  this->send_event (event);
}

void
EC_Supplier::send_event (const RtecEventComm::EventSet& event)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->push_count_ == 0)
    this->throughput_start_ = ACE_OS::gethrtime ();

  this->push_count_ += event.length ();

  if (TAO_debug_level > 0
      && this->push_count_ % 100 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "EC_Consumer (%P|%t): %d events received\n",
                  this->push_count_));
    }

  ACE_hrtime_t start = ACE_OS::gethrtime ();

  this->consumer_proxy_->push (event);

  ACE_hrtime_t end = ACE_OS::gethrtime ();
  this->throughput_.sample (end - this->throughput_start_,
                            end - start);
}

void
EC_Supplier::event_type (int event_number,
                         RtecEventComm::Event &event)
{
  CORBA::ULong l = this->qos_.publications.length ();

  if (l == 0)
    {
      event.header.source = 0;
      event.header.type = this->shutdown_event_type_;
    }
  else
    {
      int i = event_number % l;
      int type = this->qos_.publications[i].event.header.type;
      if (type == this->shutdown_event_type_)
        i = 0;

      RtecEventComm::EventHeader& header =
        this->qos_.publications[i].event.header;

      event.header.source = header.source;
      event.header.type = header.type;
    }
}

void
EC_Supplier::connect (RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
                      const RtecEventChannelAdmin::SupplierQOS& qos,
                      int shutdown_event_type)
{
  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer ();

  this->connect (qos, shutdown_event_type);
}

void
EC_Supplier::connect (const RtecEventChannelAdmin::SupplierQOS& qos,
                      int shutdown_event_type)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return; // @@ Throw?

  this->qos_ = qos;
  this->shutdown_event_type_ = shutdown_event_type;

  if (CORBA::is_nil (this->myself_.in ()))
    {
      this->myself_ = this->_this ();
    }
  this->is_active_ = 1;

  this->consumer_proxy_->connect_push_supplier (this->myself_.in (),
                                                qos);
}

void
EC_Supplier::disconnect (void)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer ();

  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
EC_Supplier::shutdown (void)
{
  if (!this->is_active_)
    return;

  // Deactivate the servant
  PortableServer::POA_var poa =
    this->_default_POA ();
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);
  poa->deactivate_object (id.in ());
  this->is_active_ = 0;
  this->myself_ = RtecEventComm::PushSupplier::_nil ();
}

void
EC_Supplier::disconnect_push_supplier (void)
{
  this->driver_->supplier_disconnect (this->cookie_);
  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
EC_Supplier::dump_results (
  const ACE_TCHAR* name,
  ACE_High_Res_Timer::global_scale_factor_type gsf)
{
  this->throughput_.dump_results (name, gsf);
}

void
EC_Supplier::accumulate (ACE_Throughput_Stats& stats) const
{
  stats.accumulate (this->throughput_);
}

// ****************************************************************

EC_Supplier_Task::EC_Supplier_Task (EC_Supplier* supplier,
                                    EC_Driver* driver,
                                    void* cookie,
                                    int burst_count,
                                    int burst_size,
                                    int burst_pause,
                                    int payload_size,
                                    int shutdown_event_type,
                                    ACE_Thread_Manager* thr_mgr)
  :  ACE_Task_Base (thr_mgr),
     supplier_ (supplier),
     driver_ (driver),
     cookie_ (cookie),
     burst_count_ (burst_count),
     burst_size_ (burst_size),
     burst_pause_ (burst_pause),
     payload_size_ (payload_size),
     shutdown_event_type_ (shutdown_event_type)
{
}

int
EC_Supplier_Task::svc (void)
{

  // Initialize a time value to pace the test
  ACE_Time_Value tv (0, this->burst_pause_);

  RtecEventComm::EventSet event (1);
  event.length (1);

  event[0].header.ttl = 1;

  ACE_hrtime_t t = ACE_OS::gethrtime ();
  ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time, t);

  // We use replace to minimize the copies, this should result
  // in just one memory allocation;
  event[0].data.payload.length (this->payload_size_);

  for (int i = 0; i < this->burst_count_; ++i)
    {
      for (int j = 0; j < this->burst_size_; ++j)
        {
          try
            {
              this->supplier_->event_type (j, event[0]);

              ACE_hrtime_t now = ACE_OS::gethrtime ();
              ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                             now);
              // ACE_DEBUG ((LM_DEBUG, "(%t) supplier push event\n"));

              this->supplier_->send_event (event);

            }
          catch (const CORBA::SystemException& sys_ex)
            {
              sys_ex._tao_print_exception ("SYS_EX");
            }
          catch (const CORBA::Exception& ex)
            {
              ex._tao_print_exception ("SYS_EX");
            }
        }
      ACE_OS::sleep (tv);
    }

  try
    {
      // Send one event shutdown from each supplier
      event[0].header.type = this->shutdown_event_type_;
      ACE_hrtime_t now = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                     now);
      this->supplier_->send_event (event);
    }
  catch (const CORBA::SystemException& sys_ex)
    {
      sys_ex._tao_print_exception ("SYS_EX");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("SYS_EX");
    }

  ACE_DEBUG ((LM_DEBUG,
              "Supplier task finished\n"));
  return 0;
}
