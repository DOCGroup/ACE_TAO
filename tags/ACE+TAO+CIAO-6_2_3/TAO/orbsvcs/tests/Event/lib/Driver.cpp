// $Id$

#include "Driver.h"
#include "Consumer.h"
#include "Supplier.h"

#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

#include "tao/ORB_Constants.h"

#include "ace/Sched_Params.h"
#include "ace/Arg_Shifter.h"
#include "ace/High_Res_Timer.h"
#include "ace/Stats.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"

#if !defined(EC_DISABLE_OLD_EC)
#include "EC_Scheduler_Info.h"
#endif

#if !defined (__ACE_INLINE__)
#include "Driver.inl"
#endif /* __ACE_INLINE__ */



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
     event_service_name_ (ACE_TEXT("EventService")),
     ec_impl_ (0)
{
  TAO_EC_Default_Factory::init_svcs ();
}

EC_Driver::~EC_Driver (void)
{
}

int
EC_Driver::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // Calibrate the high resolution timer *before* starting the
      // test.
      ACE_High_Res_Timer::calibrate ();

      this->run_init (argc, argv);

      this->execute_test ();

      this->dump_results ();

      this->run_cleanup ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("EC_Driver::run");
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "EC_Driver (%P|%t) non-corba exception raised\n"));
    }
  return 0;
}

void
EC_Driver::run_init (int &argc, ACE_TCHAR* argv[])
{
  this->initialize_orb_and_poa (argc, argv);

  if (this->parse_args (argc, argv))
    throw CORBA::INTERNAL (TAO::VMCID, CORBA::COMPLETED_NO);

  if (this->verbose ())
    this->print_args ();

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

  if (this->move_to_rt_class () == -1)
    throw CORBA::INTERNAL (TAO::VMCID, CORBA::COMPLETED_NO);

  this->initialize_ec_impl ();

  if (this->allocate_consumers () == -1)
    throw CORBA::NO_MEMORY (TAO::VMCID, CORBA::COMPLETED_NO);

  if (this->allocate_suppliers () == -1)
    throw CORBA::NO_MEMORY (TAO::VMCID, CORBA::COMPLETED_NO);

  this->connect_clients ();
}

void
EC_Driver::run_cleanup (void)
{
  this->disconnect_clients ();

  this->shutdown_clients ();

  this->destroy_ec ();

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) channel destroyed\n"));

  this->deactivate_ec ();

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) channel deactivated\n"));

  this->cleanup_tasks ();
  this->cleanup_suppliers ();
  this->cleanup_consumers ();
  this->cleanup_ec ();

  this->root_poa_->destroy (1, 1);
  this->root_poa_ = PortableServer::POA::_nil ();

  this->orb_->destroy ();
  this->orb_ = CORBA::ORB::_nil ();
}

void
EC_Driver::initialize_orb_and_poa (int &argc, ACE_TCHAR* argv[])
{
  this->orb_ =
    CORBA::ORB_init (argc, argv);

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  if (CORBA::is_nil (poa_object.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "EC_Driver (%P|%t) Unable to initialize the POA.\n"));
      return;
    }

  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::POAManager_var poa_manager =
    this->root_poa_->the_POAManager ();

  poa_manager->activate ();
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

              this->pid_file_name_?this->pid_file_name_:ACE_TEXT("nil")
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
EC_Driver::initialize_ec_impl (void)
{
#if !defined(EC_DISABLE_REMOTE_EC)
  if (this->use_remote_ec_ == 1)
    {
      this->obtain_remote_ec ();
      return;
    }
#endif

  this->initialize_new_ec ();
}

#if !defined(EC_DISABLE_REMOTE_EC)
void
EC_Driver::obtain_remote_ec (void)
{
  CosNaming::NamingContext_var naming_context =
    this->get_naming_context ();

  CosNaming::Name channel_name (1);
  channel_name.length (1);
  channel_name[0].id = CORBA::string_dup (this->event_service_name_);

  CORBA::Object_var tmp =
    naming_context->resolve (channel_name);

  this->event_channel_ =
    RtecEventChannelAdmin::EventChannel::_narrow (tmp.in ());
}

CosNaming::NamingContext_ptr
EC_Driver::get_naming_context (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService");

  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR ((LM_ERROR,
                "EC_Driver (%P|%t) Unable to obtain the "
                "Naming Service.\n"));

  return CosNaming::NamingContext::_narrow (naming_obj.in ());
}
#endif

void
EC_Driver::initialize_new_ec (void)
{
  TAO_EC_Event_Channel_Attributes attr (this->root_poa_.in (),
                                        this->root_poa_.in ());

  this->modify_attributes (attr);

  TAO_EC_Event_Channel *ec =
    new TAO_EC_Event_Channel (attr);
  this->ec_impl_ = ec;

  ec->activate ();

  this->event_channel_ =
    this->ec_impl_->_this ();
}

void
EC_Driver::deactivate_ec (void)
{
#if !defined(EC_DISABLE_REMOTE_EC)
  if (this->use_remote_ec_ == 1)
    return;
#endif

  {
    // Deactivate the EC
    PortableServer::POA_var poa =
      this->ec_impl_->_default_POA ();
    PortableServer::ObjectId_var id =
      poa->servant_to_id (this->ec_impl_);
    poa->deactivate_object (id.in ());
  }

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) EC deactivated\n"));

}

void
EC_Driver::destroy_ec (void)
{
  this->event_channel_->destroy ();
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
EC_Driver::connect_clients (void)
{
  this->connect_consumers ();

  this->connect_suppliers ();
}

void
EC_Driver::disconnect_clients (void)
{
  this->disconnect_suppliers ();

  this->disconnect_consumers ();
}

void
EC_Driver::shutdown_clients (void)
{
  this->shutdown_suppliers ();

  this->shutdown_consumers ();
}

void
EC_Driver::connect_consumers (void)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->event_channel_->for_consumers ();

  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->connect_consumer (consumer_admin.in (), i);
    }
  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) connected consumer(s)\n"));
}

void
EC_Driver::connect_consumer (
    RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
    int i)
{
  RtecEventChannelAdmin::ConsumerQOS qos;
  int shutdown_event_type;
  this->build_consumer_qos (i, qos, shutdown_event_type);

  this->consumers_[i]->connect (consumer_admin,
                                qos,
                                shutdown_event_type);
}

void
EC_Driver::build_consumer_qos (
  int i,
  RtecEventChannelAdmin::ConsumerQOS& qos,
  int& shutdown_event_type)
{
  RtecBase::handle_t rt_info = 0;

  int type_start =
    this->consumer_type_start_
    + i * this->consumer_type_shift_;

  shutdown_event_type = type_start + this->consumer_type_count_;

  ACE_ConsumerQOS_Factory qos_factory;
  qos_factory.start_disjunction_group (1 + this->consumer_type_count_);
  qos_factory.insert_type (shutdown_event_type, rt_info);

  for (int j = 0; j != this->consumer_type_count_; ++j)
    qos_factory.insert_type (type_start + j, rt_info);

  qos = qos_factory.get_ConsumerQOS ();
}

void
EC_Driver::connect_suppliers (void)
{
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->event_channel_->for_suppliers ();

  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->connect_supplier (supplier_admin.in (), i);
    }

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) connected supplier(s)\n"));
}

void
EC_Driver::connect_supplier (
    RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
    int i)
{
  RtecEventChannelAdmin::SupplierQOS qos;
  int shutdown_event_type;
  this->build_supplier_qos (i, qos, shutdown_event_type);

  this->suppliers_[i]->connect (supplier_admin,
                                qos,
                                shutdown_event_type);
}

void
EC_Driver::build_supplier_qos (
      int i,
      RtecEventChannelAdmin::SupplierQOS& qos,
      int& shutdown_event_type)
{
  int type_start = this->supplier_type_start_ + i*this->supplier_type_shift_;
  int supplier_id = i + 1;
  shutdown_event_type = type_start + this->supplier_type_count_;

  RtecBase::handle_t rt_info = 0;

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
EC_Driver::execute_test (void)
{
  if (this->allocate_tasks () == -1)
    return;

  this->activate_tasks ();

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
  if (this->tasks_ != 0)
    return 0;

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
EC_Driver::activate_tasks (void)
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;

  if (ACE_BIT_DISABLED (this->thr_create_flags_, THR_SCHED_FIFO))
    {
      priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_OTHER);
    }

  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      if (this->tasks_[i]->activate (this->thr_create_flags_,
                                     1, 0, priority) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "EC_Driver (%P|%t) Cannot activate thread "
                      "for supplier %d\n%p\n",
                      i, "EC_Driver - OS error is:"));
        }
    }
}

void
EC_Driver::disconnect_suppliers (void)
{
  if (this->suppliers_ == 0)
    return;
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->disconnect ();
    }
  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) suppliers disconnected\n"));
}

void
EC_Driver::disconnect_consumers (void)
{
  if (this->consumers_ == 0)
    return;
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->consumers_[i]->disconnect ();
    }
  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) consumers disconnected\n"));
}

void
EC_Driver::shutdown_suppliers (void)
{
  if (this->suppliers_ == 0)
    return;
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->shutdown ();
    }
  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) suppliers deactivated\n"));
}

void
EC_Driver::shutdown_consumers (void)
{
  if (this->consumers_ == 0)
    return;
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->consumers_[i]->shutdown ();
    }
  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "EC_Driver (%P|%t) consumers deactivated\n"));
}

void
EC_Driver::dump_results (void)
{
  ACE_Throughput_Stats throughput;
  ACE_High_Res_Timer::global_scale_factor_type gsf =
    ACE_High_Res_Timer::global_scale_factor ();
  ACE_TCHAR buf[BUFSIZ];
  for (int j = 0; j < this->n_consumers_; ++j)
    {
      ACE_OS::sprintf (buf, ACE_TEXT("Consumer [%02d]"), j);

      this->consumers_[j]->dump_results (buf, gsf);
      this->consumers_[j]->accumulate (throughput);
    }
  ACE_DEBUG ((LM_DEBUG, "\n"));

  ACE_Throughput_Stats suppliers;
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      ACE_OS::sprintf (buf, ACE_TEXT("Supplier [%02d]"), i);

      this->suppliers_[i]->dump_results (buf, gsf);
      this->suppliers_[i]->accumulate (suppliers);
    }

  ACE_DEBUG ((LM_DEBUG, "\nTotals:\n"));
  throughput.dump_results (ACE_TEXT("EC_Consumer/totals"), gsf);

  ACE_DEBUG ((LM_DEBUG, "\n"));
  suppliers.dump_results (ACE_TEXT("EC_Supplier/totals"), gsf);
}

int
EC_Driver::parse_args (int &argc, ACE_TCHAR *argv [])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, ACE_TEXT("-verbose")) == 0)
        {
          arg_shifter.consume_arg ();
          this->verbose_ = 1;
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-remote")) == 0)
        {
          arg_shifter.consume_arg ();

          this->use_remote_ec_ = 1;
          if (arg_shifter.is_parameter_next ())
            {
              this->event_service_name_ = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-suppliers")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->n_suppliers_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-consumers")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->n_consumers_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-burstcount")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->burst_count_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-burstsize")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->burst_size_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-payloadsize")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->payload_size_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-burstpause")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->burst_pause_ = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-consumer_tstart")) == 0)
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

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-consumer_tcount")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->consumer_type_count_ =
                ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-consumer_tshift")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->consumer_type_shift_ =
                ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-supplier_tstart")) == 0)
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

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-supplier_tcount")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->supplier_type_count_ =
                ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-supplier_tshift")) == 0)
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
EC_Driver::modify_attributes (TAO_EC_Event_Channel_Attributes& attr)
{
   ACE_UNUSED_ARG(attr);

   // This method can be overruled by derived tests to set the event channel
   // attributes
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
#if !defined(EC_DISABLE_OLD_EC)
  delete this->module_factory_;
#endif
}

int
EC_Driver::decode_consumer_cookie (void* cookie) const
{
  return static_cast<EC_Consumer**> (cookie) - this->consumers_;
}

int
EC_Driver::decode_supplier_cookie (void* cookie) const
{
  return static_cast<EC_Supplier**> (cookie) - this->suppliers_;
}

void
EC_Driver::consumer_push (void*,
                          const RtecEventComm::EventSet&)
{
}

void
EC_Driver::consumer_shutdown (void*)
{
}

void
EC_Driver::consumer_disconnect (void*)
{
}

void
EC_Driver::supplier_disconnect (void*)
{
}
