// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"

#include "tao/Timeprobe.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "orbsvcs/Event/Module_Factory.h"
#include "ECT_Throughput.h"

ACE_RCSID(EC_Throughput, ECT_Throughput, "$Id$")

int
main (int argc, char *argv [])
{
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
    event_a_ (ACE_ES_EVENT_UNDEFINED),
    event_b_ (ACE_ES_EVENT_UNDEFINED + 1),
    pid_file_name_ (0),
    active_count_ (0),
    reactive_ec_ (0)
{
}

ECT_Throughput::~ECT_Throughput (void)
{
}

int
ECT_Throughput::run (int argc, char* argv[])
{
  TAO_TRY
    {
      this->orb_ =
        CORBA::ORB_init (argc, argv, "", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->parse_args (argc, argv))
        return 1;

      ACE_DEBUG ((LM_DEBUG,
                  "Execution parameters:\n"
                  "  consumers = <%d>\n"
                  "  suppliers = <%d>\n"
                  "  burst count = <%d>\n"
                  "  burst size = <%d>\n"
                  "  event size = <%d>\n"
                  "  burst size = <%d>\n"
                  "  supplier Event A = <%d>\n"
                  "  supplier Event B = <%d>\n"
                  "  pid file name = <%s>\n"
                  "  remote EC = <%d>\n",

                  this->n_consumers_,
                  this->n_suppliers_,
                  this->burst_count_,
                  this->burst_size_,
                  this->event_size_,
                  this->burst_pause_,
                  this->event_a_,
                  this->event_b_,

                  this->pid_file_name_?this->pid_file_name_:"nil",
                  this->reactive_ec_) );

      if (this->pid_file_name_ != 0)
        {
          FILE* pid = ACE_OS::fopen (this->pid_file_name_, "w");
          if (pid != 0)
            {
              ACE_OS::fprintf (pid, "%d\n", ACE_OS::getpid ());
              ACE_OS::fclose (pid);
            }
        }

      int min_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_FIFO);
        // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

      if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                                  min_priority,
                                                  ACE_SCOPE_PROCESS)) != 0)
        {
          if (ACE_OS::last_error () == EPERM)
            ACE_DEBUG ((LM_DEBUG,
                        "%s: user is not superuser, "
                        "so remain in time-sharing class\n", argv[0]));
          else
            ACE_ERROR ((LM_ERROR,
                        "%s: ACE_OS::sched_params failed\n", argv[0]));
        }

      if (ACE_OS::thr_setprio (min_priority) == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) main thr_setprio failed,"
                      "no real-time features\n"));
        }

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);
      
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // This is the name we (potentially) register the Scheduling
      // Service in the Naming Service.
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");

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
        scheduler_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;


      CORBA::String_var str =
        this->orb_->object_to_string (scheduler.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG, "The (local) scheduler IOR is <%s>\n",
                  str.in ()));

      // Register the servant with the Naming Context....
      naming_context->bind (schedule_name, scheduler.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_Scheduler_Factory::use_config (naming_context.in ());

      // We pick the right module factory according to the command
      // line options
      TAO_Default_Module_Factory default_module_factory;
      TAO_Reactive_Module_Factory reactive_module_factory;

      TAO_Module_Factory* module_factory = &default_module_factory;
      if (this->reactive_ec_)
        module_factory = &reactive_module_factory;

      // Register Event_Service with Naming Service.
      ACE_EventChannel ec_impl (1,
                                ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                module_factory);
      RtecEventChannelAdmin::EventChannel_var channel =
        ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->connect_consumers (channel.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "connected consumer(s)\n"));

      this->connect_suppliers (channel.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "connected supplier(s)\n"));

      this->activate_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "suppliers are active\n"));

      ACE_DEBUG ((LM_DEBUG, "running the event loop\n"));
      if (this->orb_->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);
      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      naming_context->unbind (schedule_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Wait for the supplier threads...
      if (ACE_Thread_Manager::instance ()->wait () == -1)
        {
          ACE_ERROR ((LM_ERROR, "Thread_Manager wait failed\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "suppliers finished\n"));

      this->dump_results ();

      {
        // Deactivate the EC
        PortableServer::POA_var poa = 
          ec_impl._default_POA (TAO_TRY_ENV);
        TAO_CHECK_ENV;
        PortableServer::ObjectId_var id =
          poa->servant_to_id (&ec_impl, TAO_TRY_ENV);
        TAO_CHECK_ENV;
        poa->deactivate_object (id.in (), TAO_TRY_ENV);
        TAO_CHECK_ENV;
      }

      {
        // Deactivate the Scheduler
        PortableServer::POA_var poa = 
          scheduler_impl._default_POA (TAO_TRY_ENV);
        TAO_CHECK_ENV;
        PortableServer::ObjectId_var id =
          poa->servant_to_id (&scheduler_impl, TAO_TRY_ENV);
        TAO_CHECK_ENV;
        poa->deactivate_object (id.in (), TAO_TRY_ENV);
        TAO_CHECK_ENV;
      }

      this->disconnect_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->disconnect_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      channel->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::Exception, ex)
    {
      TAO_TRY_ENV.print_exception ("ECT_Throughput::run");
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "non-corba exception raised\n"));
    }
  TAO_ENDTRY;
  return 0;
}

void
ECT_Throughput::shutdown_consumer (void*,
                               CORBA::Environment &)
{
  // int ID =
  //   (ACE_reinterpret_cast(Test_Consumer**,consumer_cookie)
  //    - this->consumers_);
  //
  // ACE_DEBUG ((LM_DEBUG, "(%t) events received by consumer %d\n", ID));

  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
  this->active_count_--;
  if (this->active_count_ <= 0)
    this->orb_->shutdown ();
}

void
ECT_Throughput::connect_consumers (RtecEventChannelAdmin::EventChannel_ptr channel,
                           CORBA::Environment &_env)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
    this->active_count_ = this->n_consumers_;
  }
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "consumer_%02.2d", i);

      ACE_NEW (this->consumers_[i],
               Test_Consumer (this,
                              this->consumers_ + i,
                              this->n_suppliers_));

      this->consumers_[i]->connect (buf,
                                    this->event_a_,
                                    this->event_b_,
                                    channel,
                                    _env);
      if (_env.exception () != 0) return;
    }
}

void
ECT_Throughput::connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr channel,
                           CORBA::Environment &_env)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "supplier_%02.2d", i);

      ACE_NEW (this->suppliers_[i], Test_Supplier (this));

      this->suppliers_[i]->connect (buf,
                                    this->burst_count_,
                                    this->burst_size_,
                                    this->event_size_,
                                    this->burst_pause_,
                                    this->event_a_,
                                    this->event_b_,
                                    channel,
                                    _env);
      if (_env.exception () != 0) return;
    }
}

void
ECT_Throughput::activate_suppliers (CORBA::Environment &)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->activate ();
    }
}

void
ECT_Throughput::disconnect_suppliers (CORBA::Environment &_env)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->disconnect (_env);
      if (_env.exception () != 0) return;
    }
}

void
ECT_Throughput::disconnect_consumers (CORBA::Environment &_env)
{
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->consumers_[i]->disconnect (_env);
      TAO_CHECK_ENV_RETURN_VOID (_env);
    }
}

void
ECT_Throughput::dump_results (void)
{
  for (int j = 0; j < this->n_consumers_; ++j)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "consumer_%02.2d", j);

      this->consumers_[j]->dump_results (buf);
    }
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "supplier_%02.2d", i);

      this->suppliers_[i]->dump_results (buf);
    }
  this->dump_latency_results ("Latency");
}

int
ECT_Throughput::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "dc:s:u:n:t:b:h:p:r");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'r':
          this->reactive_ec_ = 1;
          break;

        case 'c':
          this->n_consumers_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 's':
          this->n_suppliers_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'u':
          this->burst_count_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'n':
          this->burst_size_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'b':
          this->event_size_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 't':
          this->burst_pause_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'h':
          {
            char* aux;
                char* arg = ACE_OS::strtok_r (get_opt.optarg, ",", &aux);

            this->event_a_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->event_b_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
          }
          break;

        case 'p':
          this->pid_file_name_ = get_opt.optarg;
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[ORB options] "
                      "-c <n_consumers> "
                      "-s <n_suppliers> "
                      "-u <burst count> "
                      "-n <burst size> "
                      "-b <event payload size> "
                      "-t <burst pause (usecs)> "
                      "-h <eventa,eventb> "
                      "-p <pid file name> "
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
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "%s: number of consumers or "
                         "suppliers out of range, "
                         "reset to default (%d)\n",
                         argv[0], 1), -1);
    }

  if (this->n_suppliers_ < 0
      || this->n_suppliers_ >= ECT_Throughput::MAX_SUPPLIERS)
    {
      this->n_suppliers_ = 1;
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "%s: number of suppliers out of range, "
                         "reset to default (%d)\n",
                         argv[0], 1), -1);
    }

  if (this->n_suppliers_ == 0 && this->n_consumers_ == 0)
    {
      this->n_suppliers_ = 1;
      this->n_consumers_ = 1;
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "%s: no suppliers or consumers, "
                         "reset to default (%d of each)\n",
                         argv[0], 1), -1);
    }

  return 0;
}
