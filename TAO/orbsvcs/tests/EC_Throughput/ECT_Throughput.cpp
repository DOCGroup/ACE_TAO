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
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"
#include <memory>

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  ECT_Throughput driver;
  return driver.run (argc, argv);
}

// ****************************************************************

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

                      this->options_.n_consumers,
                      this->options_.n_suppliers,
                      this->options_.burst_count,
                      this->options_.burst_size,
                      this->options_.event_size,
                      this->options_.burst_pause,
                      this->options_.consumer_type_start,
                      this->options_.consumer_type_count,
                      this->options_.consumer_type_shift,
                      this->options_.supplier_type_start,
                      this->options_.supplier_type_count,
                      this->options_.supplier_type_shift,

                      this->options_.pid_file_name != nullptr
                        ? this->options_.pid_file_name
                        : ACE_TEXT ("nil"),
                      this->options_.ec_concurrency_hwm));
        }

      if (this->options_.pid_file_name != nullptr)
        {
          FILE* pid = ACE_OS::fopen (this->options_.pid_file_name, "w");
          if (pid != nullptr)
            {
              ACE_OS::fprintf (pid, "%ld\n",
                               static_cast<long> (ACE_OS::getpid ()));
              ACE_OS::fclose (pid);
            }
        }

      int priority =
        (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
         + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
      priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO, priority);

      // Enable FIFO scheduling
      if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                                  priority,
                                                  ACE_SCOPE_PROCESS)) != 0)
        {
          if (ACE_OS::last_error () == EPERM)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "%s: user is not superuser, "
                          "so remain in time-sharing class\n", argv[0]));
              this->options_.thr_create_flags = THR_NEW_LWP;
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

      TAO_EC_Event_Channel_Attributes attr (root_poa.in (),
                                            root_poa.in ());

      std::unique_ptr<TAO_EC_Event_Channel> ec_impl =
        std::make_unique<TAO_EC_Event_Channel> (attr);

      ec_impl->activate ();

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
  catch (CORBA::Exception const& ex)
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
ECT_Throughput::shutdown_consumer ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->active_count_--;
  if (this->active_count_ <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) shutting down the ORB\n"));
      // Not needed: this->orb_->shutdown (false);
    }
}

void
ECT_Throughput::connect_consumers
     (RtecScheduler::Scheduler_ptr scheduler,
      RtecEventChannelAdmin::EventChannel_ptr channel)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
    this->active_count_ = this->options_.n_consumers;
  }
  for (int i {}; i < this->options_.n_consumers; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "consumer_%02d", i);

      this->consumers_[i] = std::make_unique<Test_Consumer> (
        *this,
        this->options_.n_suppliers);

      int const start = this->options_.consumer_type_start
        + i * this->options_.consumer_type_shift;

      this->consumers_[i]->connect (scheduler,
                                    buf,
                                    start,
                                    this->options_.consumer_type_count,
                                    channel);
    }
}

void
ECT_Throughput::connect_suppliers
     (RtecScheduler::Scheduler_ptr scheduler,
      RtecEventChannelAdmin::EventChannel_ptr channel)
{
  for (int i {}; i < this->options_.n_suppliers; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "supplier_%02d", i);

      this->suppliers_[i] = std::make_unique<Test_Supplier> ();

      int const start = this->options_.supplier_type_start
        + i * this->options_.supplier_type_shift;
      this->suppliers_[i]->connect (scheduler,
                                    buf,
                                    this->options_.burst_count,
                                    this->options_.burst_size,
                                    this->options_.event_size,
                                    this->options_.burst_pause,
                                    start,
                                    this->options_.supplier_type_count,
                                    channel);
    }
}

void
ECT_Throughput::activate_suppliers ()
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;

  for (int i {}; i < this->options_.n_suppliers; ++i)
    {
      if (this->suppliers_[i]->activate (this->options_.thr_create_flags,
                                         1, 0, priority) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate thread for supplier %d\n",
                      i));
        }
    }
}

void
ECT_Throughput::disconnect_suppliers ()
{
  for (int i {}; i < this->options_.n_suppliers; ++i)
    {
      this->suppliers_[i]->disconnect ();
      this->suppliers_[i].reset ();
    }
}

void
ECT_Throughput::disconnect_consumers ()
{
  for (int i {}; i < this->options_.n_consumers; ++i)
    {
      this->consumers_[i]->disconnect ();
      this->consumers_[i].reset ();
    }
}

void
ECT_Throughput::dump_results ()
{
  ACE_High_Res_Timer::global_scale_factor_type const gsf =
    ACE_High_Res_Timer::global_scale_factor ();

  ACE_Throughput_Stats consumers;
  for (int j {}; j < this->options_.n_consumers; ++j)
    {
      ACE_TCHAR buf[BUFSIZ];
      ACE_OS::sprintf (buf, ACE_TEXT("consumer_%02d"), j);

      this->consumers_[j]->dump_results (buf, gsf);
      this->consumers_[j]->accumulate (consumers);
    }
  consumers.dump_results (ACE_TEXT("ECT_Consumer/totals"), gsf);

  ACE_Throughput_Stats suppliers;
  for (int i {}; i < this->options_.n_suppliers; ++i)
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
          this->options_.n_consumers = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 's':
          this->options_.n_suppliers = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'u':
          this->options_.burst_count = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'n':
          this->options_.burst_size = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'b':
          this->options_.event_size = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 't':
          this->options_.burst_pause = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'h':
          {
            char* aux {};
            char* arg = ACE_OS::strtok_r (
              ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ()), ",", &aux);

            this->options_.consumer_type_start = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
            arg = ACE_OS::strtok_r (nullptr, ",", &aux);
            this->options_.consumer_type_count = ACE_OS::atoi (arg);
            arg = ACE_OS::strtok_r (nullptr, ",", &aux);
            this->options_.consumer_type_shift = ACE_OS::atoi (arg);
          }
          break;

        case 'l':
          {
            char* aux {};
            char* arg = ACE_OS::strtok_r (
              ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ()), ",", &aux);

            this->options_.supplier_type_start = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
            arg = ACE_OS::strtok_r (nullptr, ",", &aux);
            this->options_.supplier_type_count = ACE_OS::atoi (arg);
            arg = ACE_OS::strtok_r (nullptr, ",", &aux);
            this->options_.supplier_type_shift = ACE_OS::atoi (arg);
          }
          break;

        case 'p':
          this->options_.pid_file_name = get_opt.opt_arg ();
          break;

        case 'w':
          this->options_.ec_concurrency_hwm = ACE_OS::atoi (get_opt.opt_arg ());
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

  if (this->options_.burst_count <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: burst count (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->options_.burst_count,
                  100));
      this->options_.burst_count = 100;
    }

  if (this->options_.burst_size <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: burst size (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->options_.burst_size,
                  10));
      this->options_.burst_size = 10;
    }

  if (this->options_.event_size < 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: event size (%d) is out of range, "
                  "reseting to default (%d)\n",
                  argv[0], this->options_.event_size,
                  128));
      this->options_.event_size = 128;
    }

  if (this->options_.n_consumers < 0
      || this->options_.n_consumers >= ECT_Throughput::MAX_CONSUMERS)
    {
      this->options_.n_consumers = 1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: number of consumers or "
                         "suppliers out of range, "
                         "reset to default (%d)\n",
                         argv[0], 1), -1);
    }

  if (this->options_.n_suppliers < 0
      || this->options_.n_suppliers >= ECT_Throughput::MAX_SUPPLIERS)
    {
      this->options_.n_suppliers = 1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: number of suppliers out of range, "
                         "reset to default (%d)\n",
                         argv[0], 1), -1);
    }

  if (this->options_.n_suppliers == 0 && this->options_.n_consumers == 0)
    {
      this->options_.n_suppliers = 1;
      this->options_.n_consumers = 1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: no suppliers or consumers, "
                         "reset to default (%d of each)\n",
                         argv[0], 1), -1);
    }

  if (this->options_.ec_concurrency_hwm <= 0)
    {
      this->options_.ec_concurrency_hwm = 1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: invalid concurrency HWM, "
                         "reset to default (%d)\n",
                         argv[0], 1), -1);
    }

  return 0;
}
