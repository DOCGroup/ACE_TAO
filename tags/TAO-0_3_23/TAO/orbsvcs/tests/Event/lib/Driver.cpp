// $Id$

#include "Driver.h"
#include "Consumer.h"
#include "Supplier.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "ace/Sched_Params.h"
#include "ace/Arg_Shifter.h"
#include "ace/High_Res_Timer.h"

#if !defined(EC_DISABLE_OLD_EC)
#include "EC_Scheduler_Info.h"
#endif

#if !defined (__ACE_INLINE__)
#include "Driver.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(EC_Tests, EC_Driver, "$Id$")

EC_Driver::EC_Driver (void)
  :  n_consumers_ (1),
     consumers_ (0),
     n_suppliers_ (1),
     suppliers_ (0),
     tasks_ (0),
     burst_count_  (100),
     burst_size_   (100),
     payload_size_ (0),
     burst_pause_  (10000),
     consumer_type_start_ (ACE_ES_EVENT_UNDEFINED),
     consumer_type_count_ (1),
     consumer_type_shift_ (2),
     supplier_type_start_ (ACE_ES_EVENT_UNDEFINED),
     supplier_type_count_ (1),
     supplier_type_shift_ (2),
     pid_file_name_ (0),
     verbose_ (0),
     thr_create_flags_ (THR_NEW_LWP|THR_BOUND|THR_SCHED_FIFO),
     use_remote_ec_ (0),
     event_service_name_ ("EventService"),
     use_old_ec_ (0),
#if !defined(TAO_EC_DISABLE_OLD_EC)
     module_factory_ (0),
#endif
     scheduler_impl_ (0),
     ec_impl_ (0)
{
  TAO_EC_Default_Factory::init_svcs ();
}

EC_Driver::~EC_Driver (void)
{
}

int
EC_Driver::run (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Calibrate the high resolution timer *before* starting the
      // test.
      ACE_High_Res_Timer::calibrate ();

      this->run_init (argc, argv, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->execute_test (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->dump_results ();

      this->run_cleanup (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "EC_Driver::run");
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR, "EC_Driver (%P|%t) non-corba exception raised\n"));
    }
  ACE_ENDTRY;
  return 0;
}

void
EC_Driver::run_init (int &argc, char* argv[],
                     CORBA::Environment& ACE_TRY_ENV)
{
  this->initialize_orb_and_poa (argc, argv, ACE_TRY_ENV);
  ACE_CHECK;

  if (this->parse_args (argc, argv))
    ACE_THROW (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                CORBA::COMPLETED_NO));

  if (this->verbose ())
    this->print_args ();

  if (this->pid_file_name_ != 0)
    {
      FILE* pid = ACE_OS::fopen (this->pid_file_name_, "w");
      if (pid != 0)
        {
          ACE_OS::fprintf (pid, "%d\n", ACE_OS::getpid ());
          ACE_OS::fclose (pid);
        }
    }

  if (this->move_to_rt_class () == -1)
    ACE_THROW (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                CORBA::COMPLETED_NO));

  this->initialize_ec_impl (ACE_TRY_ENV);
  ACE_CHECK;

  if (this->allocate_consumers () == -1)
    ACE_THROW (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                 CORBA::COMPLETED_NO));

  if (this->allocate_suppliers () == -1)
    ACE_THROW (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                 CORBA::COMPLETED_NO));

  this->connect_clients (ACE_TRY_ENV);
  ACE_CHECK;
}

void
EC_Driver::run_cleanup (CORBA::Environment &ACE_TRY_ENV)
{
  this->disconnect_clients (ACE_TRY_ENV);
  ACE_CHECK;

  this->destroy_ec (ACE_TRY_ENV);
  ACE_CHECK;

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) channel destroyed\n"));

  this->deactivate_ec (ACE_TRY_ENV);
  ACE_CHECK;

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) channel deactivated\n"));

  this->cleanup_tasks ();
  this->cleanup_suppliers ();
  this->cleanup_consumers ();
  this->cleanup_ec ();
}

void
EC_Driver::initialize_orb_and_poa (int &argc, char* argv[],
                                   CORBA::Environment& ACE_TRY_ENV)
{
  this->orb_ =
    CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");
  if (CORBA::is_nil (poa_object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "EC_Driver (%P|%t) Unable to initialize the POA.\n"));
      return;
    }

  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    this->root_poa_->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK;

  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK;
}

void
EC_Driver::print_args (void) const
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
              "  pid file name = <%s>\n",

              this->n_consumers_,
              this->n_suppliers_,
              this->burst_count_,
              this->burst_size_,
              this->payload_size_,
              this->burst_pause_,

              this->consumer_type_start_,
              this->consumer_type_count_,
              this->consumer_type_shift_,
              this->supplier_type_start_,
              this->supplier_type_count_,
              this->supplier_type_shift_,

              this->pid_file_name_?this->pid_file_name_:"nil"
              ) );
}

int
EC_Driver::move_to_rt_class (void)
{
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
                      "EC_Driver (%P|%t): user is not superuser, "
                      "so remain in time-sharing class\n"));
          this->thr_create_flags_ = THR_NEW_LWP;
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "EC_Driver (%P|%t): sched_params failed\n"));
    }

  if (ACE_OS::thr_setprio (priority) == -1)
    {
      ACE_ERROR ((LM_ERROR, "EC_Driver (%P|%t) main thr_setprio failed,"
                  "no real-time features\n"));
    }
  return 0;
}

void
EC_Driver::initialize_ec_impl (CORBA::Environment& ACE_TRY_ENV)
{
#if !defined(EC_DISABLE_REMOTE_EC)
  if (this->use_remote_ec_ == 1)
    {
      this->obtain_remote_ec (ACE_TRY_ENV);
      return;
    }
#endif

#if !defined(EC_DISABLE_OLD_EC)
  if (this->use_old_ec_ == 1)
    {
      this->initialize_old_ec (ACE_TRY_ENV);
      return;
    }
#endif

  this->initialize_new_ec (ACE_TRY_ENV);
}

#if !defined(EC_DISABLE_REMOTE_EC)
void
EC_Driver::obtain_remote_ec (CORBA::Environment& ACE_TRY_ENV)
{
  CosNaming::NamingContext_var naming_context =
    this->get_naming_context (ACE_TRY_ENV);
  ACE_CHECK;

  CosNaming::Name channel_name (1);
  channel_name.length (1);
  channel_name[0].id = CORBA::string_dup (this->event_service_name_);

  CORBA::Object_var tmp =
    naming_context->resolve (channel_name, ACE_TRY_ENV);
  ACE_CHECK;

  this->event_channel_ =
    RtecEventChannelAdmin::EventChannel::_narrow (tmp, ACE_TRY_ENV);
  ACE_CHECK;
}

CosNaming::NamingContext_ptr
EC_Driver::get_naming_context (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService", ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR ((LM_ERROR,
                "EC_Driver (%P|%t) Unable to obtain the "
                "Naming Service.\n"));

  return CosNaming::NamingContext::_narrow (naming_obj.in (), ACE_TRY_ENV);
}
#endif

#if !defined(EC_DISABLE_OLD_EC)
int
EC_Driver::initialize_old_ec (CORBA::Environment& ACE_TRY_ENV)
{
  CosNaming::NamingContext_var naming_context =
    this->get_naming_context (ACE_TRY_ENV);
  ACE_CHECK;

  // This is the name we (potentially) register the Scheduling
  // Service in the Naming Service.
  CosNaming::Name schedule_name (1);
  schedule_name.length (1);
  schedule_name[0].id = CORBA::string_dup ("ScheduleService");

  if (this->use_runtime_scheduler_ == 1)
    ACE_NEW (this->scheduler_impl_,
             ACE_Runtime_Scheduler (runtime_configs_size,
                                    runtime_configs,
                                    runtime_infos_size,
                                    runtime_infos));
  else
    ACE_NEW (this->scheduler_impl_,
             ACE_Config_Scheduler ());


  this->scheduler_ =
    this->scheduler_impl_._this (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var str =
    this->orb_->object_to_string (scheduler.in (), ACE_TRY_ENV);
  ACE_CHECK;
  ACE_DEBUG ((LM_DEBUG,
              "EC_Driver (%P|%t) The (local) scheduler IOR is <%s>\n",
              str.in ()));


  if (this->reactive_ec_ == 1)
    ACE_NEW_RETURN (this->module_factory_,
                    TAO_Reactive_Module_Factory,
                    -1);
  else
    ACE_NEW_RETURN (this->module_factory_,
                    TAO_Default_Module_Factory,
                    -1);

  ACE_NEW_RETURN (this->ec_impl_,
                  ACE_EventChannel (scheduler.in (), 1,
                                    ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                    this->module_factory_),
                  -1);

  this->event_channel_ =
    this->ec_impl_->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
}
#endif /* */

void
EC_Driver::initialize_new_ec (CORBA::Environment& ACE_TRY_ENV)
{
  TAO_EC_Event_Channel_Attributes attr (this->root_poa_.in (),
                                        this->root_poa_.in ());
  this->modify_attributes (attr);

  TAO_EC_Event_Channel *ec =
    new TAO_EC_Event_Channel (attr);
  this->ec_impl_ = ec;

  ec->activate (ACE_TRY_ENV);
  ACE_CHECK;

  this->event_channel_ =
    this->ec_impl_->_this (ACE_TRY_ENV);
  ACE_CHECK;
}

void
EC_Driver::deactivate_ec (CORBA::Environment& ACE_TRY_ENV)
{
#if !defined(EC_DISABLE_REMOTE_EC)
  if (this->use_remote_ec_ == 1)
    return;
#endif

  {
    // Deactivate the EC
    PortableServer::POA_var poa =
      this->ec_impl_->_default_POA (ACE_TRY_ENV);
    ACE_CHECK;
    PortableServer::ObjectId_var id =
      poa->servant_to_id (this->ec_impl_, ACE_TRY_ENV);
    ACE_CHECK;
    poa->deactivate_object (id.in (), ACE_TRY_ENV);
    ACE_CHECK;
  }

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) EC deactivated\n"));

#if !defined(EC_DISABLE_OLD_EC)
  if (this->use_old_ec_ == 1)
    {
      // Deactivate the Scheduler
      PortableServer::POA_var poa =
        scheduler_impl._default_POA (ACE_TRY_ENV);
      ACE_CHECK;
      PortableServer::ObjectId_var id =
        poa->servant_to_id (&scheduler_impl, ACE_TRY_ENV);
      ACE_CHECK;
      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_CHECK;

      if (this->verbose ())
        ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) scheduler deactivated\n"));
    }
#endif
}

void
EC_Driver::destroy_ec (CORBA::Environment& ACE_TRY_ENV)
{
  this->event_channel_->destroy (ACE_TRY_ENV);
  ACE_CHECK;
}

int
EC_Driver::allocate_consumers (void)
{
  ACE_NEW_RETURN (this->consumers_,
                  EC_Consumer*[this->n_consumers_],
                  -1);
  for (int i = 0; i < this->n_consumers_; ++i)
    this->consumers_[i] =
      this->allocate_consumer (i);
  return 0;
}

int
EC_Driver::allocate_suppliers (void)
{
  ACE_NEW_RETURN (this->suppliers_,
                  EC_Supplier*[this->n_suppliers_],
                  -1);
  for (int i = 0; i < this->n_suppliers_; ++i)
    this->suppliers_[i] =
      this->allocate_supplier (i);
  return 0;
}

EC_Consumer*
EC_Driver::allocate_consumer (int i)
{
  return new EC_Consumer (this,
                          this->consumers_ + i);
}

EC_Supplier*
EC_Driver::allocate_supplier (int i)
{
  return new EC_Supplier (this,
                          this->suppliers_ + i);
}

void
EC_Driver::connect_clients (CORBA::Environment &ACE_TRY_ENV)
{
  this->connect_consumers (ACE_TRY_ENV);
  ACE_CHECK;

  this->connect_suppliers (ACE_TRY_ENV);
  ACE_CHECK;
}

void
EC_Driver::disconnect_clients (CORBA::Environment &ACE_TRY_ENV)
{
  this->disconnect_suppliers (ACE_TRY_ENV);
  ACE_CHECK;

  this->disconnect_consumers (ACE_TRY_ENV);
  ACE_CHECK;
}

void
EC_Driver::connect_consumers (CORBA::Environment &ACE_TRY_ENV)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->event_channel_->for_consumers (ACE_TRY_ENV);
  ACE_CHECK;

  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->connect_consumer (consumer_admin.in (), i, ACE_TRY_ENV);
      ACE_CHECK;
    }
  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) connected consumer(s)\n"));
}

void
EC_Driver::connect_consumer (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    int i,
    CORBA::Environment &ACE_TRY_ENV)
{
  RtecEventChannelAdmin::ConsumerQOS qos;
  int shutdown_event_type;
  this->build_consumer_qos (i, qos, shutdown_event_type, ACE_TRY_ENV);
  ACE_CHECK;

  this->consumers_[i]->connect (consumer_admin,
                                qos,
                                shutdown_event_type,
                                ACE_TRY_ENV);
}

void
EC_Driver::build_consumer_qos (
  int i,
  RtecEventChannelAdmin::ConsumerQOS& qos,
  int& shutdown_event_type,
  CORBA::Environment&)
{
  RtecScheduler::handle_t rt_info = 0;

  int type_start =
    this->consumer_type_start_
    + i * this->consumer_type_shift_;

  shutdown_event_type = type_start + this->consumer_type_count_;

  ACE_ConsumerQOS_Factory qos_factory;
  qos_factory.start_disjunction_group ();
  qos_factory.insert_type (shutdown_event_type, rt_info);

  for (int j = 0; j != this->consumer_type_count_; ++j)
    qos_factory.insert_type (type_start + j, rt_info);

  qos = qos_factory.get_ConsumerQOS ();
}

void
EC_Driver::connect_suppliers (CORBA::Environment &ACE_TRY_ENV)
{
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->event_channel_->for_suppliers (ACE_TRY_ENV);
  ACE_CHECK;

  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->connect_supplier (supplier_admin.in (), i, ACE_TRY_ENV);
      ACE_CHECK;
    }

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) connected supplier(s)\n"));
}

void
EC_Driver::connect_supplier (
    RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
    int i,
    CORBA::Environment &ACE_TRY_ENV)
{
  RtecEventChannelAdmin::SupplierQOS qos;
  int shutdown_event_type;
  this->build_supplier_qos (i, qos, shutdown_event_type, ACE_TRY_ENV);
  ACE_CHECK;

  this->suppliers_[i]->connect (supplier_admin,
                                qos,
                                shutdown_event_type,
                                ACE_TRY_ENV);
}

void
EC_Driver::build_supplier_qos (
      int i,
      RtecEventChannelAdmin::SupplierQOS& qos,
      int& shutdown_event_type,
      CORBA::Environment&)
{
  int type_start = this->supplier_type_start_ + i*this->supplier_type_shift_;
  int supplier_id = i + 1;
  shutdown_event_type = type_start + this->supplier_type_count_;

  RtecScheduler::handle_t rt_info = 0;

  ACE_SupplierQOS_Factory qos_factory;
  for (int j = 0; j != this->supplier_type_count_; ++j)
    qos_factory.insert (supplier_id,
                        type_start + j,
                        rt_info, 1);

  qos_factory.insert (supplier_id,
                      shutdown_event_type,
                      rt_info, 1);

  qos = qos_factory.get_SupplierQOS ();
}

void
EC_Driver::execute_test (CORBA::Environment &ACE_TRY_ENV)
{
  if (this->allocate_tasks () == -1)
    return;

  this->activate_tasks (ACE_TRY_ENV);
  ACE_CHECK;

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) suppliers are active\n"));

  // Wait for the supplier threads...
  if (ACE_Thread_Manager::instance ()->wait () == -1)
    {
      ACE_ERROR ((LM_ERROR, "EC_Driver (%P|%t) Thread_Manager wait failed\n"));
    }

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) suppliers finished\n"));
}

int
EC_Driver::allocate_tasks (void)
{
  ACE_NEW_RETURN (this->tasks_,
                  ACE_Task_Base*[this->n_suppliers_],
                  -1);

  for (int i = 0; i < this->n_suppliers_; ++i)
    this->tasks_[i] =
      this->allocate_task (i);
  return 0;
}

ACE_Task_Base*
EC_Driver::allocate_task (int i)
{
  int start = this->supplier_type_start_ + i*this->supplier_type_shift_;
  return new EC_Supplier_Task (this->suppliers_[i],
                               this,
                               this->suppliers_ + i,
                               this->burst_count_,
                               this->burst_size_,
                               this->burst_pause_,
                               this->payload_size_,
                               start + this->supplier_type_count_);
}

void
EC_Driver::activate_tasks (CORBA::Environment &)
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;

  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->tasks_[i] = this->allocate_task (i);

      if (this->tasks_[i]->activate (this->thr_create_flags_,
                                     1, 0, priority) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate thread for supplier %d\n",
                      i));
        }
    }
}

void
EC_Driver::disconnect_suppliers (CORBA::Environment &ACE_TRY_ENV)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->disconnect (ACE_TRY_ENV);
      ACE_CHECK;
    }
  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) suppliers disconnected\n"));
}

void
EC_Driver::disconnect_consumers (CORBA::Environment &ACE_TRY_ENV)
{
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->consumers_[i]->disconnect (ACE_TRY_ENV);
      ACE_CHECK;
    }
  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) consumers disconnected\n"));
}

void
EC_Driver::dump_results (void)
{
  EC_Driver::Throughput_Stats throughput;
  EC_Driver::Latency_Stats latency;
  for (int j = 0; j < this->n_consumers_; ++j)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "consumer_%02.2d", j);

      this->consumers_[j]->dump_results (buf);
      this->consumers_[j]->accumulate (throughput, latency);
    }
  ACE_DEBUG ((LM_DEBUG, "\n"));

  EC_Driver::Throughput_Stats suppliers;
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "supplier_%02.2d", i);

      this->suppliers_[i]->dump_results (buf);
      this->suppliers_[i]->accumulate (suppliers);
    }

  ACE_DEBUG ((LM_DEBUG, "\nTotals:\n"));
  throughput.dump_results ("EC_Consumer", "throughput");
  latency.dump_results ("EC_Consumer", "latency");

  ACE_DEBUG ((LM_DEBUG, "\n"));
  suppliers.dump_results ("EC_Supplier", "accumulated");
}

int
EC_Driver::parse_args (int &argc, char *argv [])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, "-verbose") == 0)
        {
          arg_shifter.consume_arg ();
          this->verbose_ = 1;
        }

      else if (ACE_OS::strcmp (arg, "-remote") == 0)
        {
          arg_shifter.consume_arg ();

          this->use_remote_ec_ = 1;
          if (arg_shifter.is_parameter_next ())
            {
              this->event_service_name_ = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-old_reactive") == 0)
        {
          arg_shifter.consume_arg ();
          this->use_old_ec_ = 1;
        }

      else if (ACE_OS::strcmp (arg, "-old_threaded") == 0)
        {
          arg_shifter.consume_arg ();
          this->use_old_ec_ = 1;
        }

      else if (ACE_OS::strcmp (arg, "-suppliers") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->n_suppliers_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-consumers") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->n_consumers_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-burstcount") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->burst_count_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-burstsize") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->burst_size_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-payloadsize") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->payload_size_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-burstpause") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->burst_pause_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-consumer_tstart") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->consumer_type_start_ =
                ACE_ES_EVENT_UNDEFINED
                + ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-consumer_tcount") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->consumer_type_count_ =
                ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-consumer_tshift") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->consumer_type_shift_ =
                ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-supplier_tstart") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->supplier_type_start_ =
                ACE_ES_EVENT_UNDEFINED
                + ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-supplier_tcount") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->supplier_type_count_ =
                ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, "-supplier_tshift") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->supplier_type_shift_ =
                ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else
        {
          arg_shifter.ignore_arg ();
        }

    }
  return 0;
}

void
EC_Driver::print_usage (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "EC_Driver Usage:\n"
              "  -verbose\n"
              "  -remote <ec_name>\n"
              "  -old_reactive\n"
              "  -old_threaded\n"
              "  -suppliers <nsuppliers>\n"
              "  -consumers <nsuppliers>\n"
              "  -burstcount <bursts>\n"
              "  -burstsize <size>\n"
              "  -payloadsize <size>\n"
              "  -burstpause <usecs>\n"
              "  -consumer_tstart <type>\n"
              "  -consumer_tcount <count>\n"
              "  -consumer_tshift <shift>\n"
              "  -supplier_tstart <type>\n"
              "  -supplier_tcount <count>\n"
              "  -supplier_tshift <shift>\n"
              ));
}

void
EC_Driver::modify_attributes (TAO_EC_Event_Channel_Attributes&)
{
}

void
EC_Driver::cleanup_tasks (void)
{
  if (this->tasks_ != 0)
    {
      for (int i = 0; i != this->n_suppliers_; ++i)
        {
          delete this->tasks_[i];
          this->tasks_[i] = 0;
        }
      delete[] this->tasks_;
      this->tasks_ = 0;
    }
}

void
EC_Driver::cleanup_suppliers (void)
{
  if (this->suppliers_ != 0)
    {
      for (int i = 0; i != this->n_suppliers_; ++i)
        {
          delete this->suppliers_[i];
          this->suppliers_[i] = 0;
        }
      delete[] this->suppliers_;
      this->suppliers_ = 0;
    }
}

void
EC_Driver::cleanup_consumers (void)
{
  if (this->consumers_ != 0)
    {
      for (int i = 0; i != this->n_consumers_; ++i)
        {
          delete this->consumers_[i];
          this->consumers_[i] = 0;
        }
      delete[] this->consumers_;
      this->consumers_ = 0;
    }
}

void
EC_Driver::cleanup_ec (void)
{
  delete this->ec_impl_;
  delete this->scheduler_impl_;
#if !defined(EC_DISABLE_OLD_EC)
  delete this->module_factory_;
#endif
}

int
EC_Driver::decode_consumer_cookie (void* cookie) const
{
  return ACE_static_cast(EC_Consumer**,cookie) - this->consumers_;
}

int
EC_Driver::decode_supplier_cookie (void* cookie) const
{
  return ACE_static_cast(EC_Supplier**,cookie) - this->suppliers_;
}

void
EC_Driver::consumer_push (void*,
                          const RtecEventComm::EventSet&,
                          CORBA::Environment&)
{
}

void
EC_Driver::consumer_shutdown (void*,
                              CORBA::Environment&)
{
}

void
EC_Driver::consumer_disconnect (void*,
                                CORBA::Environment&)
{
}

void
EC_Driver::supplier_disconnect (void*,
                                CORBA::Environment&)
{
}

// ****************************************************************

void
EC_Driver::Latency_Stats::dump_results (const char *test_name,
                                        const char *sub_test)
{
  if (this->n_ <= 1)
    return;

  ACE_hrtime_t avg = this->sum_ / this->n_;
  ACE_hrtime_t dev =
    this->sum2_ / this->n_ - avg*avg;

  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();

  double min_usec = ACE_CU64_TO_CU32 (this->min_) / gsf;
  double max_usec = ACE_CU64_TO_CU32 (this->max_) / gsf;
  double avg_usec = ACE_CU64_TO_CU32 (avg) / gsf;
  double dev_usec = ACE_CU64_TO_CU32 (dev) / (gsf * gsf);
  ACE_DEBUG ((LM_DEBUG,
              "%s/%s: %.2f/%.2f/%.2f/%.2f (min/avg/max/var^2) [usecs]\n",
              test_name, sub_test,
              min_usec, avg_usec, max_usec, dev_usec));
}

void
EC_Driver::Latency_Stats::accumulate (const Latency_Stats& rhs)
{
  if (rhs.n_ == 0)
    return;

  if (this->n_ == 0)
    {
      *this = rhs;
      return;
    }

  if (this->min_ > rhs.min_)
    this->min_ = rhs.min_;
  if (this->max_ < rhs.max_)
    this->max_ = rhs.max_;

  this->sum_ += rhs.sum_;
  this->sum2_ += rhs.sum2_;
  this->n_ += rhs.n_;
}

// ****************************************************************

void
EC_Driver::Throughput_Stats::accumulate (const Throughput_Stats& rhs)
{
  if (!this->done_)
    {
      this->done_ = 1;
      this->start_ = rhs.start_;
      this->stop_ = rhs.stop_;
      this->n_ = rhs.n_;
      return;
    }

  if (this->start_ > rhs.start_)
    this->start_ = rhs.start_;

  if (this->stop_ < rhs.stop_)
    this->stop_ = rhs.stop_;

  this->n_ += rhs.n_;
}

void
EC_Driver::Throughput_Stats::dump_results (const char *test_name,
                                           const char *subtest)
{
  if (this->n_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s/%s: no events recorded\n",
                  test_name, subtest));
      return;
    }

  if (this->done_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s/%s: incomplete data,"
                  " potentially skewed results\n",
                  test_name, subtest));
    }

  ACE_Time_Value tv;
  ACE_High_Res_Timer::hrtime_to_tv (tv, this->stop_ - this->start_);

  double f = 1.0/(tv.sec () + tv.usec () / 1000000.0);
  double events_per_second = this->n_ * f;

  ACE_DEBUG ((LM_DEBUG,
              "%s/%s: "
              "%d / %d.%06.6d = %.3f events/second\n",
              test_name, subtest,
              this->n_,
              tv.sec (), tv.usec (),
              events_per_second));
}
