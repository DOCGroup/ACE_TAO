// $Id$

#include "ECT_Throughput.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "orbsvcs/Event/Module_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

#include "tao/Timeprobe.h"
#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"

ACE_RCSID(EC_Throughput, ECT_Throughput, "$Id$")

int
main (int argc, char *argv [])
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
    reactive_ec_ (0),
    new_ec_ (1),
    ec_concurrency_hwm_ (1),
    thr_create_flags_ (THR_NEW_LWP|THR_BOUND|THR_SCHED_FIFO)
{
}

ECT_Throughput::~ECT_Throughput (void)
{
}

int
ECT_Throughput::run (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Calibrate the high resolution timer *before* starting the
      // test.
      ACE_High_Res_Timer::calibrate ();

      this->orb_ =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA"
                                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
                      "  reactive EC = <%d>\n"
                      "  new EC = <%d>\n"
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

                      this->pid_file_name_?this->pid_file_name_:"nil",
                      this->reactive_ec_,
                      this->new_ec_,
                      this->ec_concurrency_hwm_
                      ) );
        }

      if (this->pid_file_name_ != 0)
        {
          FILE* pid = ACE_OS::fopen (this->pid_file_name_, "w");
          if (pid != 0)
            {
              ACE_OS::fprintf (pid, "%ld\n",
                               ACE_static_cast (long, ACE_OS::getpid ()));
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
        scheduler_impl._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

#if 0
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService"
                                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // This is the name we (potentially) register the Scheduling
      // Service in the Naming Service.
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");

      CORBA::String_var str =
        this->orb_->object_to_string (scheduler.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "The (local) scheduler IOR is <%s>\n",
                  str.in ()));

      // Register the servant with the Naming Context....
      naming_context->rebind (schedule_name, scheduler.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Scheduler_Factory::use_config (naming_context.in ());
#endif /* 0 */

      // The factories must be destroyed *after* the EC, hence the
      // auto_ptr declarations must go first....
      auto_ptr<TAO_Module_Factory> module_factory;

      auto_ptr<POA_RtecEventChannelAdmin::EventChannel> ec_impl;
      if (this->new_ec_ == 0)
        {

          if (this->reactive_ec_ == 1)
            {
              auto_ptr<TAO_Module_Factory> auto_module_factory (new TAO_Reactive_Module_Factory);
              module_factory = auto_module_factory;
            }
          else
            {
              auto_ptr<TAO_Module_Factory> auto_module_factory (new TAO_Default_Module_Factory);
              module_factory = auto_module_factory;
            }

          // Create the EC
          auto_ptr<POA_RtecEventChannelAdmin::EventChannel> auto_ec_impl
            (new ACE_EventChannel (scheduler.in (),
                                   1,
                                   ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                   module_factory.get ()));
          ec_impl = auto_ec_impl;
        }
      else
        {
          TAO_EC_Event_Channel_Attributes attr (root_poa.in (),
                                                root_poa.in ());
          attr.busy_hwm = this->ec_concurrency_hwm_;
          attr.max_write_delay = this->ec_concurrency_hwm_;

          TAO_EC_Event_Channel *ec =
            new TAO_EC_Event_Channel (attr);

          ec->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          auto_ptr<POA_RtecEventChannelAdmin::EventChannel> auto_ec_impl (ec);
          ec_impl = auto_ec_impl;
        }

      RtecEventChannelAdmin::EventChannel_var channel =
        ec_impl->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->connect_consumers (scheduler.in (),
                               channel.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "connected consumer(s)\n"));

      this->connect_suppliers (scheduler.in (),
                               channel.in ()
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "connected supplier(s)\n"));

      this->activate_suppliers (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "suppliers are active\n"));

      // Wait for the supplier threads...
      if (ACE_Thread_Manager::instance ()->wait () == -1)
        {
          ACE_ERROR ((LM_ERROR, "Thread_Manager wait failed\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "suppliers finished\n"));

      this->dump_results ();

      this->disconnect_consumers (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "consumers disconnected\n"));

      this->disconnect_suppliers (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "suppliers disconnected\n"));

      channel->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "channel destroyed\n"));

      {
        // Deactivate the EC
        PortableServer::POA_var poa =
          ec_impl->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
        PortableServer::ObjectId_var id =
          poa->servant_to_id (ec_impl.get () TAO_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        ACE_DEBUG ((LM_DEBUG, "EC deactivated\n"));
      }

      {
        // Deactivate the Scheduler
        PortableServer::POA_var poa =
          scheduler_impl._default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
        PortableServer::ObjectId_var id =
          poa->servant_to_id (&scheduler_impl TAO_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
        poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        ACE_DEBUG ((LM_DEBUG, "scheduler deactivated\n"));
      }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "ECT_Throughput::run");
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR, "non-corba exception raised\n"));
    }
  ACE_ENDTRY;

  return 0;
}

void
ECT_Throughput::shutdown_consumer (void*
                                   TAO_ENV_ARG_DECL_NOT_USED)
{
  // int ID =
  //   (ACE_reinterpret_cast(Test_Consumer**,consumer_cookie)
  //    - this->consumers_);
  //
  // ACE_DEBUG ((LM_DEBUG, "(%t) events received by consumer %d\n", ID));

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->active_count_--;
  if (this->active_count_ <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) shutting down the ORB\n"));
      // Not needed: this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
    }
}

void
ECT_Throughput::connect_consumers
     (RtecScheduler::Scheduler_ptr scheduler,
      RtecEventChannelAdmin::EventChannel_ptr channel
      TAO_ENV_ARG_DECL)
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
                                    channel
                                    TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
ECT_Throughput::connect_suppliers
     (RtecScheduler::Scheduler_ptr scheduler,
      RtecEventChannelAdmin::EventChannel_ptr channel
      TAO_ENV_ARG_DECL)
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
                                    channel
                                    TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
ECT_Throughput::activate_suppliers (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
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
ECT_Throughput::disconnect_suppliers (TAO_ENV_SINGLE_ARG_DECL)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
ECT_Throughput::disconnect_consumers (TAO_ENV_SINGLE_ARG_DECL)
{
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->consumers_[i]->disconnect (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
ECT_Throughput::dump_results (void)
{
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();

  ACE_Throughput_Stats consumers;
  for (int j = 0; j < this->n_consumers_; ++j)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "consumer_%02d", j);

      this->consumers_[j]->dump_results (buf, gsf);
      this->consumers_[j]->accumulate (consumers);
    }
  consumers.dump_results ("ECT_Consumer/totals", gsf);

  ACE_Throughput_Stats suppliers;
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "supplier_%02d", i);

      this->suppliers_[i]->dump_results (buf, gsf);
      this->suppliers_[i]->accumulate (suppliers);
    }
  suppliers.dump_results ("ECT_Supplier/totals", gsf);
}

int
ECT_Throughput::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "rdc:s:u:n:t:b:h:l:p:m:w:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'r':
          this->new_ec_ = 0;
          this->reactive_ec_ = 1;
          break;

        case 'm':
          if (ACE_OS::strcasecmp (get_opt.opt_arg (), "rt") == 0)
            {
              this->new_ec_ = 0;
              this->reactive_ec_ = 0;
            }
          else if (ACE_OS::strcasecmp (get_opt.opt_arg (), "st") == 0)
            {
              this->new_ec_ = 0;
              this->reactive_ec_ = 1;
            }
          else if (ACE_OS::strcasecmp (get_opt.opt_arg (), "new") == 0)
            {
              this->new_ec_ = 1;
              this->reactive_ec_ = 1;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Unknown mode <%s> "
                          "default is rt\n",
                          get_opt.opt_arg ()));
              this->new_ec_ = 0;
              this->reactive_ec_ = 0;
            }
          break;

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
                char* arg = ACE_OS::strtok_r (get_opt.opt_arg (), ",", &aux);

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
                char* arg = ACE_OS::strtok_r (get_opt.opt_arg (), ",", &aux);

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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Auto_Basic_Ptr<POA_RtecScheduler::Scheduler>;
template class auto_ptr<POA_RtecScheduler::Scheduler>;
template class ACE_Auto_Basic_Ptr<POA_RtecEventChannelAdmin::EventChannel>;
template class auto_ptr<POA_RtecEventChannelAdmin::EventChannel>;
template class ACE_Auto_Basic_Ptr<TAO_Module_Factory>;
template class auto_ptr<TAO_Module_Factory>;
template class ACE_Auto_Basic_Ptr<TAO_EC_Factory>;
template class auto_ptr<TAO_EC_Factory>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Auto_Basic_Ptr<POA_RtecScheduler::Scheduler>
#pragma instantiate auto_ptr<POA_RtecScheduler::Scheduler>
#pragma instantiate ACE_Auto_Basic_Ptr<POA_RtecEventChannelAdmin::EventChannel>
#pragma instantiate auto_ptr<POA_RtecEventChannelAdmin::EventChannel>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Module_Factory>
#pragma instantiate auto_ptr<TAO_Module_Factory>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_EC_Factory>
#pragma instantiate auto_ptr<TAO_EC_Factory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
