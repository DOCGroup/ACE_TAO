// $Id$

#include "ECT_Throughput.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

#include "tao/Timeprobe.h"
#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  ECT_Throughput driver;
  return driver.run (argc, argv);
}

// ****************************************************************

ECT_Throughput::ECT_Throughput (void)
  : n_consumers_ (1),
    n_suppliers_ (1),
    burst_count_ (10),
    burst_size_ (100),
    event_size_ (128),
    burst_pause_ (100),
    consumer_type_start_ (ACE_ES_EVENT_UNDEFINED),
    consumer_type_count_ (1),
    consumer_type_shift_ (0),
    supplier_type_start_ (ACE_ES_EVENT_UNDEFINED),
    supplier_type_count_ (1),
    supplier_type_shift_ (0),
    pid_file_name_ (0),
    active_count_ (0),
    ec_concurrency_hwm_ (1),
    thr_create_flags_ (THR_NEW_LWP|THR_BOUND|THR_SCHED_FIFO)
{
}

ECT_Throughput::~ECT_Throughput (void)
{
}

int
ECT_Throughput::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // Calibrate the high resolution timer *before* starting the
      // test.
      ACE_High_Res_Timer::calibrate ();

      this->orb_ =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (this->parse_args (argc, argv))
        return 1;

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Execution parameters:\n"
                      "  consumers = <%d>\n"
                      "  suppliers = <%d>\n"
                      "  burst count = <%d>\n"
                      "  burst size = <%d>\n"
                      "  event size = <%d>\n"
                      "  burst pause = <%d>\n"
                      "  consumer type start = <%d>\n"
                      "  consumer type count = <%d>\n"
                      "  consumer type shift = <%d>\n"
                      "  supplier type start = <%d>\n"
                      "  supplier type count = <%d>\n"
                      "  supplier type shift = <%d>\n"
                      "  pid file name = <%s>\n"
                      "  concurrency HWM = <%d>\n",

                      this->n_consumers_,
                      this->n_suppliers_,
                      this->burst_count_,
                      this->burst_size_,
                      this->event_size_,
                      this->burst_pause_,
                      this->consumer_type_start_,
                      this->consumer_type_count_,
                      this->consumer_type_shift_,
                      this->supplier_type_start_,
                      this->supplier_type_count_,
                      this->supplier_type_shift_,

                      this->pid_file_name_?this->pid_file_name_:ACE_TEXT("nil"),
                      this->ec_concurrency_hwm_
                      ) );
        }

      if (this->pid_file_name_ != 0)
        {
          FILE* pid = ACE_OS::fopen (this->pid_file_name_, "w");
          if (pid != 0)
            {
              ACE_OS::fprintf (pid, "%ld\n",
                               static_cast<long> (ACE_OS::getpid ()));
              ACE_OS::fclose (pid);
            }
        }

      int priority =
        (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
         + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
      priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                  priority);
      // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

      if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                                  priority,
                                                  ACE_SCOPE_PROCESS)) != 0)
        {
          if (ACE_OS::last_error () == EPERM)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%s: user is not superuser, "
                          "so remain in time-sharing class\n", argv[0]));
              this->thr_create_flags_ = THR_NEW_LWP;
            }
          else
            ACE_ERROR ((LM_ERROR,
                        "%s: ACE_OS::sched_params failed\n", argv[0]));
        }

      if (ACE_OS::thr_setprio (priority) == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) main thr_setprio failed,"
                      "no real-time features\n"));
        }

#if 1
      ACE_Config_Scheduler scheduler_impl;
#else
#include "ECT_Scheduler_Info.h"
      ACE_Runtime_Scheduler scheduler_impl (
        runtime_configs_size,
        runtime_configs,
        runtime_infos_size,
        runtime_infos);
#endif
      RtecScheduler::Scheduler_var scheduler =
        scheduler_impl._this ();

#if 0
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      // This is the name we (potentially) register the Scheduling
      // Service in the Naming Service.
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");

      CORBA::String_var str =
        this->orb_->object_to_string (scheduler.in ());
      ACE_DEBUG ((LM_DEBUG, "The (local) scheduler IOR is <%s>\n",
                  str.in ()));

      // Register the servant with the Naming Context....
      naming_context->rebind (schedule_name, scheduler.in ());

      ACE_Scheduler_Factory::use_config (naming_context.in ());
#endif /* 0 */

      auto_ptr<POA_RtecEventChannelAdmin::EventChannel> ec_impl;

      TAO_EC_Event_Channel_Attributes attr (root_poa.in (),
                                            root_poa.in ());

      TAO_EC_Event_Channel *ec =
        new TAO_EC_Event_Channel (attr);

      ec->activate ();

      auto_ptr<POA_RtecEventChannelAdmin::EventChannel> auto_ec_impl (ec);
      ec_impl = auto_ec_impl;

      RtecEventChannelAdmin::EventChannel_var channel =
        ec_impl->_this ();

      this->connect_consumers (scheduler.in (),
                               channel.in ());

      ACE_DEBUG ((LM_DEBUG, "connected consumer(s)\n"));

      this->connect_suppliers (scheduler.in (),
                               channel.in ());

      ACE_DEBUG ((LM_DEBUG, "connected supplier(s)\n"));

      this->activate_suppliers ();

      ACE_DEBUG ((LM_DEBUG, "suppliers are active\n"));

      // Wait for the supplier threads...
      if (ACE_Thread_Manager::instance ()->wait () == -1)
        {
          ACE_ERROR ((LM_ERROR, "Thread_Manager wait failed\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "suppliers finished\n"));

      this->dump_results ();

      this->disconnect_consumers ();

      ACE_DEBUG ((LM_DEBUG, "consumers disconnected\n"));

      this->disconnect_suppliers ();

      ACE_DEBUG ((LM_DEBUG, "suppliers disconnected\n"));

      channel->destroy ();

      ACE_DEBUG ((LM_DEBUG, "channel destroyed\n"));

      {
        // Deactivate the EC
        PortableServer::POA_var poa =
          ec_impl->_default_POA ();
        PortableServer::ObjectId_var id =
          poa->servant_to_id (ec_impl.get ());
        poa->deactivate_object (id.in ());

        ACE_DEBUG ((LM_DEBUG, "EC deactivated\n"));
      }

      {
        // Deactivate the Scheduler
        PortableServer::POA_var poa =
          scheduler_impl._default_POA ();
        PortableServer::ObjectId_var id =
          poa->servant_to_id (&scheduler_impl);
        poa->deactivate_object (id.in ());

        ACE_DEBUG ((LM_DEBUG, "scheduler deactivated\n"));
      }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ECT_Throughput::run");
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "non-corba exception raised\n"));
    }

  return 0;
}

void
ECT_Throughput::shutdown_consumer (void*)
{
  // int ID =
  //   (reinterpret_cast<Test_Consumer**> (consumer_cookie)
  //    - this->consumers_);
  //
  // ACE_DEBUG ((LM_DEBUG, "(%t) events received by consumer %d\n", ID));

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->active_count_--;
  if (this->active_count_ <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) shutting down the ORB\n"));
      // Not needed: this->orb_->shutdown (0);
    }
}

void
ECT_Throughput::connect_consumers
     (RtecScheduler::Scheduler_ptr scheduler,
      RtecEventChannelAdmin::EventChannel_ptr channel)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
    this->active_count_ = this->n_consumers_;
  }
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "consumer_%02d", i);

      ACE_NEW (this->consumers_[i],
               Test_Consumer (this,
                              this->consumers_ + i,
                              this->n_suppliers_));

      int start = this->consumer_type_start_
        + i * this->consumer_type_shift_;

      this->consumers_[i]->connect (scheduler,
                                    buf,
                                    start,
                                    this->consumer_type_count_,
                                    channel);
    }
}

void
ECT_Throughput::connect_suppliers
     (RtecScheduler::Scheduler_ptr scheduler,
      RtecEventChannelAdmin::EventChannel_ptr channel)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "supplier_%02d", i);

      ACE_NEW (this->suppliers_[i], Test_Supplier (this));

      int start = this->supplier_type_start_ + i*this->supplier_type_shift_;
      this->suppliers_[i]->connect (scheduler,
                                    buf,
                                    this->burst_count_,
                                    this->burst_size_,
                                    this->event_size_,
                                    this->burst_pause_,
                                    start,
                                    this->supplier_type_count_,
                                    channel);
    }
}

void
ECT_Throughput::activate_suppliers (void)
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;

  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      if (this->suppliers_[i]->activate (this->thr_create_flags_,
                                         1, 0, priority) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate thread for supplier %d\n",
                      i));
        }
    }
}

void
ECT_Throughput::disconnect_suppliers (void)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->disconnect ();
    }
}

void
ECT_Throughput::disconnect_consumers (void)
{
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->consumers_[i]->disconnect ();
    }
}

void
ECT_Throughput::dump_results (void)
{
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();

  ACE_Throughput_Stats consumers;
  for (int j = 0; j < this->n_consumers_; ++j)
    {
      ACE_TCHAR buf[BUFSIZ];
      ACE_OS::sprintf (buf, ACE_TEXT("consumer_%02d"), j);

      this->consumers_[j]->dump_results (buf, gsf);
      this->consumers_[j]->accumulate (consumers);
    }
  consumers.dump_results (ACE_TEXT("ECT_Consumer/totals"), gsf);

  ACE_Throughput_Stats suppliers;
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      ACE_TCHAR buf[BUFSIZ];
      ACE_OS::sprintf (buf, ACE_TEXT("supplier_%02d"), i);

      this->suppliers_[i]->dump_results (buf, gsf);
      this->suppliers_[i]->accumulate (suppliers);
    }
  suppliers.dump_results (ACE_TEXT("ECT_Supplier/totals"), gsf);
}

int
ECT_Throughput::parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("dc:s:u:n:t:b:h:l:p:w:"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'c':
          this->n_consumers_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 's':
          this->n_suppliers_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'u':
          this->burst_count_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'n':
          this->burst_size_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'b':
          this->event_size_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 't':
          this->burst_pause_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'h':
          {
            char* aux;
                char* arg = ACE_OS::strtok_r (ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ()), ",", &aux);

            this->consumer_type_start_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->consumer_type_count_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->consumer_type_shift_ = ACE_OS::atoi (arg);
          }
          break;

        case 'l':
          {
            char* aux;
                char* arg = ACE_OS::strtok_r (ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ()), ",", &aux);

            this->supplier_type_start_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->supplier_type_count_ = ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->supplier_type_shift_ = ACE_OS::atoi (arg);
          }
          break;

        case 'p':
          this->pid_file_name_ = get_opt.opt_arg ();
          break;

        case 'w':
          this->ec_concurrency_hwm_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[ORB options] "
                      "-r -d -x "
                      "-c <n_consumers> "
                      "-s <n_suppliers> "
                      "-u <burst count> "
                      "-n <burst size> "
                      "-b <event payload size> "
                      "-t <burst pause (usecs)> "
                      "-h <consumer_start,consumer_count,consumer_shift> "
                      "-l <supplier_start,supplier_count,supplier_shift> "
                      "-p <pid file name> "
                      "-w <concurrency HWM> "
                      "-r "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  if (this->burst_count_ <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: burst count (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->burst_count_,
                  100));
      this->burst_count_ = 100;
    }

  if (this->burst_size_ <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: burst size (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->burst_size_,
                  10));
      this->burst_size_ = 10;
    }

  if (this->event_size_ < 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: event size (%d) is out of range, "
                  "reseting to default (%d)\n",
                  argv[0], this->event_size_,
                  128));
      this->event_size_ = 128;
    }

  if (this->n_consumers_ < 0
      || this->n_consumers_ >= ECT_Throughput::MAX_CONSUMERS)
    {
      this->n_consumers_ = 1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: number of consumers or "
                         "suppliers out of range, "
                         "reset to default (%d)\n",
                         argv[0], 1), -1);
    }

  if (this->n_suppliers_ < 0
      || this->n_suppliers_ >= ECT_Throughput::MAX_SUPPLIERS)
    {
      this->n_suppliers_ = 1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: number of suppliers out of range, "
                         "reset to default (%d)\n",
                         argv[0], 1), -1);
    }

  if (this->n_suppliers_ == 0 && this->n_consumers_ == 0)
    {
      this->n_suppliers_ = 1;
      this->n_consumers_ = 1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: no suppliers or consumers, "
                         "reset to default (%d of each)\n",
                         argv[0], 1), -1);
    }

  if (this->ec_concurrency_hwm_ <= 0)
    {
      this->ec_concurrency_hwm_ = 1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: invalid concurrency HWM, "
                         "reset to default (%d)\n",
                         argv[0], 1), -1);
    }

  return 0;
}
