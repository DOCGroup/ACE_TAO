// $Id$

#include "Notify_Service.h"

#include "orbsvcs/Daemon_Utilities.h"

#include "orbsvcs/Notify/Service.h"
#include "orbsvcs/Notify/EventChannelFactory.h"

#include "tao/debug.h"
#include "tao/IORTable/IORTable.h"
#include "tao/ORB_Core.h"

#if defined (TAO_HAS_NOTIFICATION_MONITOR_CONTROL) && defined (TAO_AS_STATIC_LIBS)
// for static builds, be sure the library is included
#include "orbsvcs/orbsvcs/Notify/MonitorControlExt/MC_Notify_Service.h"
#include "orbsvcs/orbsvcs/Notify/MonitorControl/MonitorManager.h"
#endif

#if defined (TAO_EXPLICIT_NEGOTIATE_CODESETS)
#include "tao/Codeset/Codeset.h"
#endif /* TAO_EXPLICIT_NEGOTIATE_CODESETS */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
#include "tao/Messaging/Messaging.h"
#endif /* TAO_HAS_CORBA_MESSAGING != 0 */

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Synch.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Logging_Strategy.h"

TAO_Notify_Service_Driver::TAO_Notify_Service_Driver (void)
: notify_service_ (0)
, bootstrap_ (false)
, use_name_svc_ (true)
, ior_output_file_name_ (0)
, notify_factory_name_ (NOTIFY_KEY)
, register_event_channel_ (false)
, nthreads_ (1)
, separate_dispatching_orb_ (false)
, timeout_ (0)
, logging_worker_(this)
, shutdown_orb_ (true)
, shutdown_dispatching_orb_ (true)
{
}

TAO_Notify_Service_Driver::~TAO_Notify_Service_Driver (void)
{
}

int
TAO_Notify_Service_Driver::init_ORB (int& argc, ACE_TCHAR *argv [])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  ACE_LOG_MSG->open
    (argv[0], ACE_Log_Msg::SYSLOG, ACE_TEXT ("TAO Notify"));

  this->apply_timeout (this->orb_.in ());

  CORBA::Object_var object =
    this->orb_->resolve_initial_references("RootPOA");

  if (CORBA::is_nil (object.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the RootPOA.\n"),
                      -1);

  this->poa_ =
    PortableServer::POA::_narrow (object.in ());

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager ();

  poa_manager->activate ();

  return 0;
}

int
TAO_Notify_Service_Driver::init_dispatching_ORB (int& argc, ACE_TCHAR *argv [])
{
  this->dispatching_orb_ = CORBA::ORB_init (argc, argv, "dispatcher");

  this->apply_timeout (this->dispatching_orb_.in ());

  return 0;
}

void
TAO_Notify_Service_Driver::apply_timeout (CORBA::ORB_ptr orb)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  if (this->timeout_ != 0)
    {
      // convert from msec to "TimeT"
      CORBA::Any timeout;
      TimeBase::TimeT value = 10000 * this->timeout_;
      timeout <<= value;

      CORBA::Object_var object =
        orb->resolve_initial_references ("ORBPolicyManager");
      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in ());
      if (CORBA::is_nil (policy_manager.in ()))
        throw CORBA::INTERNAL ();

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] = orb->create_policy (
                              Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                              timeout);
      policy_manager->set_policy_overrides (policy_list, CORBA::SET_OVERRIDE);
      policy_list[0]->destroy ();
  }
#else
  ACE_UNUSED_ARG (orb);
#endif /* TAO_HAS_CORBA_MESSAGING != 0 */
}

int
TAO_Notify_Service_Driver::init (int argc, ACE_TCHAR *argv[])
{
  // Check if -ORBDaemon is specified and if so, daemonize at this moment,
  // -ORBDaemon in the ORB core is faulty, see bugzilla 3335
  TAO_Daemon_Utility::check_for_daemon (argc, argv);

  if (this->parse_args(argc, argv) != 0)
    return -1;

  // initalize the ORB.
  if (this->init_ORB (argc, argv) != 0)
    return -1;

  this->notify_service_ = TAO_Notify_Service::load_default ();

  if (this->notify_service_ == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Service not found. Check service ")
                      ACE_TEXT ("configurator file.\n")));
        }
      return -1;
    }

  if (this->separate_dispatching_orb_)
    {
      if (this->init_dispatching_ORB (argc, argv) != 0)
        {
          return -1;
        }

      this->notify_service_->init_service2 (this->orb_.in (), this->dispatching_orb_.in());
    }
  else
    {
      this->notify_service_->init_service (this->orb_.in ());
    }

  this->logging_worker_.start();

  if (this->nthreads_ > 0) // we have chosen to run in a thread pool.
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Running %d ORB threads\n"),
                      this->nthreads_));
        }
      worker_.orb (this->orb_.in ());

      // Task activation flags.
      long const flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

      int const priority =
        ACE_Sched_Params::priority_min (ACE_Utils::truncate_cast<ACE_Sched_Params::Policy> (this->orb_->orb_core ()->orb_params ()->sched_policy ()),
                                        ACE_Utils::truncate_cast<int> (this->orb_->orb_core ()->orb_params ()->scope_policy ()));

      if (worker_.activate (flags,
                            this->nthreads_, 0, priority) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot activate client threads\n"), -1);
        }
    }

  // Check first if the naming service
  if (this->use_name_svc_)
    {
      // Resolve the naming service.
      int const ns_ret = this->resolve_naming_service ();

      if (ns_ret != 0)
        return -1;
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("\nStarting up the Notification Service...\n")));
    }

  // Activate the factory
  this->notify_factory_ =
    notify_service_->create (this->poa_.in (),
                             this->notify_factory_name_.c_str ());

  ACE_ASSERT (!CORBA::is_nil (this->notify_factory_.in ()));

  if (this->bootstrap_) // Enable corbaloc usage
    {
      CORBA::Object_var table_object =
        this->orb_->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in ());

      if (CORBA::is_nil (adapter.in ()))
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Nil IORTable. corbaloc support not enabled.\n"));
            }
        }
      else
        {
          CORBA::String_var ior =
            this->orb_->object_to_string (this->notify_factory_.in ());
          adapter->bind (this->notify_factory_name_.c_str (), ior.in ());
        }
    }

  // Register with the Name service, if asked
  if (this->use_name_svc_ && !CORBA::is_nil (this->naming_.in ()))
    {
      CosNaming::Name_var name =
        this->naming_->to_name (this->notify_factory_name_.c_str ());

      this->naming_->rebind (name.in (),
                             this->notify_factory_.in ());

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Registered with the naming service as: %C\n",
                      this->notify_factory_name_.c_str()));
        }

      if (this->register_event_channel_)
        {
          // If we don't have any name configured, default to the standard name
          if (this->notify_channel_name_.is_empty ())
            {
              notify_channel_name_.insert (NOTIFY_CHANNEL_NAME);
            }

          for (ACE_Unbounded_Set<ACE_CString>::const_iterator ci (
                this->notify_channel_name_); !ci.done(); ci++)
            {
              // create an event channel
              CosNotifyChannelAdmin::ChannelID id;

              CosNotification::QoSProperties initial_qos;
              CosNotification::AdminProperties initial_admin;

              CosNotifyChannelAdmin::EventChannel_var ec;
              TAO_Notify_EventChannelFactory* factory_impl =
                dynamic_cast<TAO_Notify_EventChannelFactory*> (
                  this->notify_factory_->_servant ());
              if (factory_impl == 0)
                {
                  ec = this->notify_factory_->create_channel (initial_qos,
                                                              initial_admin,
                                                              id);
                }
              else
                {
                  ec = factory_impl->create_named_channel (
                                       initial_qos, initial_admin,id,
                                       (*ci).c_str ());
                }

              name = this->naming_->to_name ((*ci).c_str ());

              this->naming_->rebind (name.in (), ec.in ());

              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Registered an Event Channel with the naming "
                              "service as: %C\n",
                              (*ci).c_str()));
                }
            }
        }
    }

  // Write IOR to a file, if asked.
  // Note: do this last to ensure that we're up and running before the file is written
  CORBA::String_var str =
    this->orb_->object_to_string (this->notify_factory_.in ());

  if (this->ior_output_file_name_)
    {
       FILE* ior_output_file = ACE_OS::fopen (ior_output_file_name_,
                                              ACE_TEXT("w"));
       if (ior_output_file == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             this->ior_output_file_name_, "Notify_Service"),
                             -1);
        }

      ACE_OS::fprintf (ior_output_file, "%s", str.in ());
      ACE_OS::fclose (ior_output_file);
    }
  else if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("The Notification Event Channel Factory IOR is <%C>\n"),
                  str.in ()));
    }

  return 0;
}

int
TAO_Notify_Service_Driver::resolve_naming_service (void)
{
  try
    {
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");

      // Need to check return value for errors.
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Naming Service.\n"),
                          -1);

      this->naming_ = CosNaming::NamingContextExt::_narrow (naming_obj.in ());
    }
  catch (CORBA::Exception &)
    {
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Naming Service.\n"),
                          -1);
    }
  catch (...)
    {
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Naming Service.\n"),
                          -1);
    }
  return 0;
}

int
TAO_Notify_Service_Driver::run (void)
{
  if (TAO_debug_level > 0 )
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%C: Running the Notification Service\n"),
                __FILE__));

  if (this->nthreads_ > 0)
    {
      this->worker_.wait ();
      return 0;
    }
  else
    {
      this->orb_->run ();
    }

  this->logging_worker_.end();
  return 0;
}

int
TAO_Notify_Service_Driver::fini (void)
{
  /// Release all the _vars as the ORB about to go away.
  CosNotifyChannelAdmin::EventChannelFactory_var factory =
    this->notify_factory_._retn ();
  CORBA::ORB_var orb = this->orb_._retn ();
  CORBA::ORB_var dispatching_orb = this->dispatching_orb_._retn ();
  PortableServer::POA_var poa = this->poa_._retn ();
  CosNaming::NamingContextExt_var naming = this->naming_._retn ();

  // This must be called to ensure that all services shut down
  // correctly.  Depending upon the type of service loaded, it may
  // or may not actually perform any actions.
  this->notify_service_->finalize_service (factory.in ());
  factory = CosNotifyChannelAdmin::EventChannelFactory::_nil ();

  this->notify_service_->fini ();

  // Deactivate.
  if (this->use_name_svc_ && !CORBA::is_nil (naming.in ()))
    {
      // Unbind all event channels from the naming service
      if (this->register_event_channel_)
        {
          for (ACE_Unbounded_Set<ACE_CString>::const_iterator ci (
               this->notify_channel_name_); !ci.done(); ci++)
            {
              CosNaming::Name_var name = naming->to_name ((*ci).c_str ());
              naming->unbind (name.in ());
            }
        }

      // Unbind from the naming service.
      CosNaming::Name_var name =
        naming->to_name (this->notify_factory_name_.c_str ());

      naming->unbind (name.in ());

      naming = CosNaming::NamingContextExt::_nil ();
    }

  if (!CORBA::is_nil (poa.in ()))
    {
      poa->destroy (true, true);
      poa = PortableServer::POA::_nil ();
    }

  if (this->shutdown_dispatching_orb_ && !CORBA::is_nil (dispatching_orb_.in ()))
    {
      dispatching_orb->shutdown ();
    }

  // shutdown the ORB.
  if (this->shutdown_orb_ && !CORBA::is_nil (orb.in ()))
    {
      orb->shutdown ();
    }

  // Make sure all worker threads are gone.
  this->worker_.wait ();
  this->logging_worker_.wait ();

  // Destroy the ORB
  if (this->shutdown_dispatching_orb_ && !CORBA::is_nil (dispatching_orb_.in ()))
    {
      dispatching_orb->destroy ();
    }

  // Destroy the ORB.
  if (this->shutdown_orb_ && !CORBA::is_nil (orb.in ()))
    {
      orb->destroy ();
    }

  dispatching_orb_ = CORBA::ORB::_nil ();

  worker_.orb (CORBA::ORB::_nil ());

  orb = CORBA::ORB::_nil ();

  return 0;
}

int
TAO_Notify_Service_Driver::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const ACE_TCHAR* current_arg = 0;
  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Factory"))))
        {
          this->notify_factory_name_.set (ACE_TEXT_ALWAYS_CHAR(current_arg));
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-ShutdownORB")) == 0)
        {
          current_arg = arg_shifter.get_the_parameter
                              (ACE_TEXT("-ShutdownORB"));
          if (current_arg != 0 &&
              (ACE_OS::strcmp(ACE_TEXT ("0"), current_arg) == 0 ||
               ACE_OS::strcmp(ACE_TEXT ("1"), current_arg) == 0))
            {
              this->shutdown_orb_ =
                            static_cast<bool> (ACE_OS::atoi(current_arg));
            }
          if (current_arg != 0)
            arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-ShutdownDispatchingORB")) == 0)
        {
          current_arg = arg_shifter.get_the_parameter
                              (ACE_TEXT("-ShutdownDispatchingORB"));
          if (current_arg != 0 &&
              (ACE_OS::strcmp(ACE_TEXT ("0"), current_arg) == 0 ||
               ACE_OS::strcmp(ACE_TEXT ("1"), current_arg) == 0))
            {
              this->shutdown_dispatching_orb_ =
                            static_cast<bool> (ACE_OS::atoi(current_arg));
            }
          if (current_arg != 0)
            arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-UseSeparateDispatchingORB")) == 0)
        {
          current_arg = arg_shifter.get_the_parameter
                              (ACE_TEXT("-UseSeparateDispatchingORB"));
          if (current_arg != 0 &&
              (ACE_OS::strcmp(ACE_TEXT ("0"), current_arg) == 0 ||
               ACE_OS::strcmp(ACE_TEXT ("1"), current_arg) == 0))
            {
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("Using separate dispatching ORB\n")));
                }
              this->separate_dispatching_orb_ =
                            static_cast<bool> (ACE_OS::atoi(current_arg));
            }
          else
            {
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("WARNING: Unrecognized ")
                              ACE_TEXT ("argument (%s) to ")
                              ACE_TEXT ("-UseSeparateDispatchingORB.\n"),
                              (current_arg == 0 ? ACE_TEXT ("''") : current_arg)));
                }
            }
          if (current_arg != 0)
            arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Boot")) == 0)
        {
          this->bootstrap_ = true;
          // Make it bootstrappable

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-NameSvc")) == 0)
        {
          this->use_name_svc_ = true;
          // Register ref with the name service

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-NoNameSvc")) == 0)
        {
          this->use_name_svc_ = false;
          // Don't Register ref with the name service

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-IORoutput"))))
        {
          this->ior_output_file_name_ = current_arg;
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-ChannelName"))))
        {
          this->notify_channel_name_.insert(ACE_CString(ACE_TEXT_ALWAYS_CHAR(current_arg)));
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Channel")) == 0)
        {
          this->register_event_channel_ = true;
          // Register one EC with naming.

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Notify_TPReactor"))))
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG((LM_DEBUG,
                         ACE_TEXT ("-Notify_TPReactor option is deprecated, ")
                         ACE_TEXT ("use -RunThreads option\n")));
            }

          this->nthreads_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-RunThreads"))))
        {
          this->nthreads_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Timeout"))))
        {
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
          this->timeout_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
#else
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG((LM_DEBUG,
                         ACE_TEXT ("WARNING: CORBA Messaging has been disabled.")
                         ACE_TExT ("The timeout will not be applied.\n")));
            }
#endif /* TAO_HAS_CORBA_MESSAGING != 0 */
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-LoggingInterval"))))
        {
          this->logging_interval_ = ACE_Time_Value (ACE_OS::atoi (current_arg));
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-?")) == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT ("usage: %s -Factory factory_name ")
                     ACE_TEXT ("-Boot -[No]NameSvc ")
                     ACE_TEXT ("-IORoutput file_name ")
                     ACE_TEXT ("-Channel -ChannelName channel_name ")
                     ACE_TEXT ("-RunThreads threads ")
                     ACE_TEXT ("-Timeout <msec>\n")
                     ACE_TEXT ("-ShutdownORB 1|0")
                     ACE_TEXT ("-ShutdownDispatchingORB 1|0")
                     ACE_TEXT ("default: %s -Factory NotifyEventChannelFactory ")
                     ACE_TEXT ("-NameSvc -Channel NotifyEventChannel -ORBRunThreads 1\n"),
                     argv[0], argv[0]));

          arg_shifter.consume_arg ();

          return -1;
        }
      else
        {
          arg_shifter.ignore_arg ();
        }
    }
  return 0;
}

/*****************************************************************/
LoggingWorker::LoggingWorker(TAO_Notify_Service_Driver* ns)
: ns_ (ns),
  started_ (false),
  timer_id_ (-1)
{
}


void
LoggingWorker::start ()
{
  ACE_Logging_Strategy* logging_strategy =
    ACE_Dynamic_Service<ACE_Logging_Strategy>::instance ("Logging_Strategy");
  if (logging_strategy == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) logging_strategy == 0\n")));
    }
  else
    {
      if (this->activate (THR_NEW_LWP | THR_JOINABLE, 1) == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Can not activate the ")
                         ACE_TEXT ("logging event thread\n")));
            }
        }
      else {
        if (this->ns_->logging_interval_ > ACE_Time_Value::zero)
        {
          timer_id_ = this->ns_->orb_->orb_core()->reactor()->
              schedule_timer (logging_strategy, 0, this->ns_->logging_interval_,
                              this->ns_->logging_interval_);
          if (timer_id_ == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT("(%P|%t) Failed to schedule ")
                          ACE_TEXT("logging switch timer\n")));
            }
        }
      }
    }
}

int
LoggingWorker::svc (void)
{
  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t)Running logging reactor \n")));
    }
  started_ = true;
  this->logging_reactor_.run_event_loop();

  return 0;
}

void
LoggingWorker::end ()
{
  if (started_)
  {
    this->logging_reactor_.end_event_loop();
    this->wait ();
  }

  if (timer_id_ != -1)
    {
      this->ns_->orb_->orb_core()->reactor()->
        cancel_timer (timer_id_);
      timer_id_ = -1;
    }
}

Worker::Worker (void)
{
}

void
Worker::orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

int
Worker::svc (void)
{
#if 0
  // ACE_Thread::getprio() fails on systems that do not support thread
  // priorities.  While we could just treat the failure as benign, I'm
  // just disabling it altogether.  It doesn't provide much value, and
  // makes service startup needlessly more verbose.  See bugzilla 2477
  // for details.

  ACE_hthread_t current;
  ACE_Thread::self (current);

  int priority;
  if (ACE_Thread::getprio (current, priority) == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - Failed to get ")
                      ACE_TEXT ("Worker thread priority\n")));
        }
      return -1;
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Activated Worker Thread to run ")
                  ACE_TEXT ("the ORB @ priority:%d\n", priority));
    }
#endif

  try
    {
      if (!CORBA::is_nil (this->orb_.in ()))
        {
          this->orb_->run ();
        }
    }
  catch (const CORBA::Exception&)
    {
    }
  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_Notify_Service_Driver,
                       ACE_TEXT ("TAO_Notify_Service_Driver"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Notify_Service_Driver),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)


ACE_FACTORY_DEFINE (TAO_Notify_Service, TAO_Notify_Service_Driver)


#if defined (TAO_HAS_NOTIFICATION_MONITOR_CONTROL) && defined (TAO_AS_STATIC_LIBS)
ACE_STATIC_SVC_REQUIRE(TAO_MC_Notify_Service)
ACE_STATIC_SVC_REQUIRE(TAO_MonitorAndControl)
#endif


#if defined (TAO_AS_STATIC_LIBS)
ACE_STATIC_SVC_REQUIRE(ACE_Logging_Strategy)
#endif

