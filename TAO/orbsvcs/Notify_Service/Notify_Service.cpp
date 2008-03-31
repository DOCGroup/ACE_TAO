// $Id$

#include "Notify_Service.h"

#include "orbsvcs/Notify/Service.h"
#include "orbsvcs/Notify/EventChannelFactory.h"

#include "tao/debug.h"
#include "tao/IORTable/IORTable.h"
#include "tao/ORB_Core.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Synch.h"
#include "ace/Argv_Type_Converter.h"

TAO_Notify_Service_Driver::TAO_Notify_Service_Driver (void)
: notify_service_ (0)
, bootstrap_ (0)
, use_name_svc_ (1)
, ior_output_file_ (0)
, notify_factory_name_ (NOTIFY_KEY)
, notify_channel_name_ (NOTIFY_CHANNEL_NAME)
, register_event_channel_ (0)
, nthreads_ (1)
, separate_dispatching_orb_ (false)
{
  // No-Op.
}

TAO_Notify_Service_Driver::~TAO_Notify_Service_Driver (void)
{
  if (ior_output_file_)
    ACE_OS::fclose(ior_output_file_);
}

int
TAO_Notify_Service_Driver::init_ORB (int& argc, ACE_TCHAR *argv [])
{
  // Copy command line parameter.
  ACE_Argv_Type_Converter command_line(argc, argv);

  this->orb_ = CORBA::ORB_init (command_line.get_argc(),
                                command_line.get_ASCII_argv());

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
  // Copy command line parameter.
  ACE_Argv_Type_Converter command_line(argc, argv);

  this->dispatching_orb_ = CORBA::ORB_init (command_line.get_argc(),
                                command_line.get_ASCII_argv(),
                                "dispatcher");

  return 0;
}

int
TAO_Notify_Service_Driver::init (int argc, ACE_TCHAR *argv[])
{
  if (this->parse_args(argc, argv) != 0)
    return -1;

  // initalize the ORB.
  if (this->init_ORB (argc, argv) != 0)
    return -1;

  this->notify_service_ = TAO_Notify_Service::load_default ();

  if (this->notify_service_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Service not found. Check service configurator file.\n"));
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

  if (this->nthreads_ > 0) // we have chosen to run in a thread pool.
    {
      ACE_DEBUG ((LM_DEBUG, "Running %d ORB threads\n", this->nthreads_));
      worker_.orb (this->orb_.in ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

      int priority =
        ACE_Sched_Params::priority_min (ACE_Utils::truncate_cast<ACE_Sched_Params::Policy> (this->orb_->orb_core ()->orb_params ()->sched_policy ()),
                                        ACE_Utils::truncate_cast<int> (this->orb_->orb_core ()->orb_params ()->scope_policy ()));

      if (worker_.activate (flags,
                            this->nthreads_, 0, priority) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"), -1);
    }

  // Check first if the naming service
  if (this->use_name_svc_)
    {
      // Resolve the naming service.
      int ns_ret = this->resolve_naming_service ();

      if (ns_ret != 0)
        return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\nStarting up the Notification Service...\n"));

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
          ACE_ERROR ((LM_ERROR, "Nil IORTable. corbaloc support not enabled.\n"));
        }
      else
        {
          CORBA::String_var ior =
            this->orb_->object_to_string (this->notify_factory_.in ());
          adapter->bind (this->notify_factory_name_.c_str (), ior.in ());
        }
    }


  // Register with the Name service, if asked
  if (this->use_name_svc_)
    {
      ACE_ASSERT (!CORBA::is_nil (this->naming_.in ()));

      CosNaming::Name_var name =
        this->naming_->to_name (this->notify_factory_name_.c_str ());


      this->naming_->rebind (name.in (),
                             this->notify_factory_.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Registered with the naming service as: %s\n",
                  this->notify_factory_name_.c_str()));

      if (this->register_event_channel_ == 1)
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
                                   this->notify_channel_name_.c_str ());
            }

          name = this->naming_->to_name (this->notify_channel_name_.c_str ());

          this->naming_->rebind (name.in (),
                                 ec.in ());

          ACE_DEBUG ((LM_DEBUG,
                      "Registered an Event Channel with the naming service as: %s\n",
                      this->notify_channel_name_.c_str()));

        }
    }

  // Write IOR to a file, if asked.
  // Note: do this last to ensure that we're up and running before the file is written
  CORBA::String_var str =
    this->orb_->object_to_string (this->notify_factory_.in ());

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_, "%s", str.in ());
      ACE_OS::fclose (this->ior_output_file_);
      this->ior_output_file_ = 0;
    }
  else if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "The Notification Event Channel Factory IOR is <%s>\n",
                  str.in ()));
    }

  return 0;
}

int
TAO_Notify_Service_Driver::resolve_naming_service (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService");

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Naming Service.\n"),
                      -1);

  this->naming_ =
    CosNaming::NamingContextExt::_narrow (naming_obj.in ());

  return 0;
}

int
TAO_Notify_Service_Driver::run (void)
{
  if (TAO_debug_level > 0 )
    ACE_DEBUG ((LM_DEBUG, "%s: Running the Notification Service\n",
                __FILE__));

  if (this->nthreads_ > 0)
    {
      worker_.thr_mgr ()->wait ();
      return 0;
    }

  this->orb_->run ();

  return 0;
}

void
TAO_Notify_Service_Driver::shutdown (void)
{
  /// Release all the _vars as the ORB about to go away.
  CosNotifyChannelAdmin::EventChannelFactory_var factory =
    this->notify_factory_._retn ();
  CORBA::ORB_var orb = this->orb_._retn ();
  PortableServer::POA_var poa = this->poa_._retn ();
  CosNaming::NamingContextExt_var naming = this->naming_._retn ();

  // This must be called to ensure that all services shut down
  // correctly.  Depending upon the type of service loaded, it may
  // or may not actually perform any actions.
  this->notify_service_->finalize_service (factory.in ());

  // Deactivate.
  if (this->use_name_svc_ && !CORBA::is_nil (naming.in ()))
    {
      // Unbind from the naming service.
      CosNaming::Name_var name =
        naming->to_name (this->notify_factory_name_.c_str ());

      naming->unbind (name.in ());
    }

  poa->destroy (true, true);

  // shutdown the ORB.
  if (!CORBA::is_nil (orb.in ()))
    orb->shutdown ();
}

int
TAO_Notify_Service_Driver::parse_args (int &argc, ACE_TCHAR *argv[])
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
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-UseSeparateDispatchingORB")) == 0)
        {
      current_arg = arg_shifter.get_the_parameter
                            (ACE_TEXT("-UseSeparateDispatchingORB"));
      if (current_arg != 0 &&
          (ACE_OS::strcmp(ACE_TEXT ("0"), current_arg) == 0 ||
           ACE_OS::strcmp(ACE_TEXT ("1"), current_arg) == 0))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Using separate dispatching ORB\n")));
          this->separate_dispatching_orb_ =
                        static_cast<bool> (ACE_OS::atoi(current_arg));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("WARNING: Unrecognized ")
                      ACE_TEXT ("argument (%s) to ")
                      ACE_TEXT ("-UseSeparateDispatchingORB.\n"),
                      (current_arg == 0 ? ACE_TEXT ("''") : current_arg)));
        }
      if (current_arg != 0)
        arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Boot")) == 0)
        {
          this->bootstrap_ = 1;
          // Make it bootstrappable

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-NameSvc")) == 0)
        {
          this->use_name_svc_ = 1;
          // Register ref with the name service

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-NoNameSvc")) == 0)
        {
          this->use_name_svc_ = 0;
          // Don't Register ref with the name service

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-IORoutput"))))
        {
          this->ior_output_file_ = ACE_OS::fopen (current_arg, ACE_TEXT("w"));
          if (this->ior_output_file_ == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open %s for writing: %p\n",
                               current_arg, "Notify_Service"), -1);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-ChannelName"))))
        {
          this->notify_channel_name_.set(ACE_TEXT_ALWAYS_CHAR(current_arg));
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Channel")) == 0)
        {
          this->register_event_channel_ = 1;
          // Register one EC with naming.

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Notify_TPReactor"))))
        {
          ACE_DEBUG((LM_DEBUG, "-Notify_TPReactor option is deprecated, use -ORBRunThreads option\n"));

          this->nthreads_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-ORBRunThreads"))))
        {
          this->nthreads_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-?")) == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s -Factory factory_name "
                     "-Boot -[No]NameSvc "
                     "-IORoutput file_name "
                     "-Channel -ChannelName channel_name "
                     "-ORBRunThreads threads\n"
                     "default: %s -Factory NotifyEventChannelFactory "
                     "-NameSvc -Channel NotifyEventChannel -ORBRunThreads 1\n",
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
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TAO (%P|%t) - Failed to get Worker thread priority\n")));
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, "Activated Worker Thread to run the ORB @ priority:%d \n", priority));
#endif

  try
    {
      this->orb_->run ();

    }
  catch (const CORBA::Exception&)
    {
    }
  return 0;
}
