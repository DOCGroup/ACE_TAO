// $Id$

#include "Event_Service.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_main.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"

#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "ace/OS_NS_strings.h"

ACE_RCSID (Event_Service,
           Event_Service,
           "$Id$")

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  Event_Service event_service;
  return event_service.run (argc, argv);
}

// ****************************************************************

Event_Service::Event_Service (void)
  : sched_impl_ (0),
    ec_impl_ (0),
    scheduler_type_ (ES_SCHED_NONE),
    use_bidir_giop_ (false),
    bind_to_naming_service_ (true)
{
}

Event_Service::~Event_Service (void)
{
  delete this->ec_impl_;
  this->ec_impl_ = 0;
  delete this->sched_impl_;
  this->sched_impl_ = 0;
}

int
Event_Service::run (int argc, ACE_TCHAR* argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Make a copy of command line parameter.
      ACE_Argv_Type_Converter command(argc, argv);

      // Initialize ORB.
      this->orb_ =
        CORBA::ORB_init (command.get_argc(), command.get_ASCII_argv(), "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->parse_args (command.get_argc(), command.get_TCHAR_argv()) == -1)
        return 1;

      CORBA::Object_var root_poa_object =
        this->orb_->resolve_initial_references("RootPOA"
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (root_poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the root POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (root_poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // When we have a service name or a non local scheduler we must use the
      // naming service.

      bool use_name_service = bind_to_naming_service_ ||
                              this->scheduler_type_ != ES_SCHED_NONE;

      CORBA::Object_var naming_obj;
      RtecScheduler::Scheduler_var scheduler;
      CosNaming::NamingContext_var naming_context;

      if (use_name_service)
        {
          naming_obj=
            this->orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (naming_obj.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                                " (%P|%t) Unable to initialize the Naming Service.\n"),
                              1);

          naming_context =
            CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // This is the name we (potentially) register the Scheduling
      // Service in the Naming Service.
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");

      // The old EC always needs a scheduler. If none is
      // specified, we default to a local scheduler
      if (this->scheduler_type_ == ES_SCHED_LOCAL)
        {
          // Create a local scheduler instance
          ACE_NEW_RETURN (this->sched_impl_,
                          ACE_Config_Scheduler,
                          1);

          scheduler = this->sched_impl_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Register the servant with the Naming Context....
          if (!CORBA::is_nil (naming_context.in ()))
            {
              naming_context->rebind (schedule_name, scheduler.in ()
                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
      else if (this->scheduler_type_ == ES_SCHED_GLOBAL)
        {
          // Get reference to a scheduler from naming service
          CORBA::Object_var tmp =
            naming_context->resolve (schedule_name ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          scheduler = RtecScheduler::Scheduler::_narrow (tmp.in ()
                                                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (scheduler.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                                " (%P|%t) Unable to resolve the Scheduling Service.\n"),
                              1);
        }

      TAO_EC_Event_Channel_Attributes attr (root_poa.in (),
                                            root_poa.in ());

      if (this->scheduler_type_ != ES_SCHED_NONE)
        {
          attr.scheduler = scheduler.in ();
        }

      TAO_EC_Event_Channel* ec_impl = 0;
      ACE_NEW_RETURN (ec_impl,
                      TAO_EC_Event_Channel (attr),
                      1);
      this->ec_impl_ = ec_impl;

      ec_impl->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var ec;

      // If the object_id_ is empty and we don't use BiDIR GIOP, activate the
      // servant under the default POA, else create a new child POA with 
      // the needed policies
      int persistent = ACE_OS::strcmp(this->object_id_.c_str(), "");
      if ((persistent == 0) && (this->use_bidir_giop_ == false))
        {
          // Notice that we activate *this* object with the POA, but we
          // forward all the requests to the underlying EC
          // implementation.
          ec = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          int index = 0;

          // Create child POA
          CORBA::PolicyList policies (3);

          if (persistent != 0)
            {
              policies[index++] =
                root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                                        ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              policies[index++] =
                root_poa->create_lifespan_policy (PortableServer::PERSISTENT
                                                   ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          if (this->use_bidir_giop_ == true)
            {
              CORBA::Any pol;
              pol <<= BiDirPolicy::BOTH;
              policies[index++] =
                this->orb_->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                                           pol
                                            ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          policies.length (index);

          ACE_CString child_poa_name = "childPOA";
          PortableServer::POA_var child_poa =
            root_poa->create_POA (child_poa_name.c_str (),
                                  poa_manager.in (),
                                  policies
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Creation of persistentPOA is over. Destroy the Policy objects.
          for (CORBA::ULong i = 0;
               i < policies.length ();
               ++i)
            {
              policies[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          if (CORBA::is_nil (child_poa.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Unable to initialize the child POA.\n"),
                              1);

          PortableServer::ObjectId_var ec_object_id =
            PortableServer::string_to_ObjectId(object_id_.c_str());

          child_poa->activate_object_with_id(ec_object_id.in(),
                                             this
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::Object_var ec_obj =
            child_poa->id_to_reference(ec_object_id.in()
                                        ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ec =
            RtecEventChannelAdmin::EventChannel::_narrow(ec_obj.in()
                                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      CORBA::String_var str =
         this->orb_->object_to_string (ec.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp(this->ior_file_name_.c_str(), "") != 0)
        {
          FILE *output_file=
            ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(this->ior_file_name_.c_str()),
                           ACE_TEXT("w"));
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               this->ior_file_name_.c_str()),
                              1);
          ACE_OS::fprintf (output_file, "%s", str.in ());
          ACE_OS::fclose (output_file);
        }

      if (ACE_OS::strcmp(this->pid_file_name_.c_str(), "") != 0)
        {
          FILE *pidf =
            ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(this->pid_file_name_.c_str()),
                           ACE_TEXT("w"));
          if (pidf != 0)
            {
              ACE_OS::fprintf (pidf,
                               "%ld\n",
                               static_cast<long> (ACE_OS::getpid ()));
              ACE_OS::fclose (pidf);
            }
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("The EC IOR is <%s>\n"),
                  ACE_TEXT_CHAR_TO_TCHAR(str.in ())));

      if (bind_to_naming_service_ && !CORBA::is_nil (naming_context.in ()))
        {
          CosNaming::Name channel_name (1);
          channel_name.length (1);
          channel_name[0].id = CORBA::string_dup (this->service_name_.c_str());
          naming_context->rebind (channel_name, ec.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("%s; running event service\n"),
                  ACE_TEXT_CHAR_TO_TCHAR(__FILE__)));

      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (bind_to_naming_service_ && !CORBA::is_nil (naming_context.in ()))
        {
          CosNaming::Name channel_name (1);
          channel_name.length (1);
          channel_name[0].id = CORBA::string_dup (this->service_name_.c_str());
          naming_context->unbind (channel_name ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (!CORBA::is_nil (scheduler.in ()) &&
          !CORBA::is_nil (naming_context.in ()))
        {
          naming_context->unbind (schedule_name ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "EC");
    }
  ACE_ENDTRY;


  return 0;
}

int
Event_Service::parse_args (int argc, ACE_TCHAR* argv [])
{
  // default values...
  this->service_name_ = "EventService";

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("n:o:p:s:q:bx"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name_ = ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ());
          break;

        case 'o':
          this->ior_file_name_ = ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ());
          break;

        case 'p':
          this->pid_file_name_ = ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ());
          break;

        case 'q':
          this->object_id_ = ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ());
          break;

        case 'b':
          this->use_bidir_giop_ = true;
          break;

        case 'x':
          this->bind_to_naming_service_ = false;
          break;

        case 's':
          // It could be just a flag (i.e. no "global" or "local"
          // argument, but this is consistent with the EC_Multiple
          // test and also allows for a runtime scheduling service.

          if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("global")) == 0)
            {
              this->scheduler_type_ = ES_SCHED_GLOBAL;
            }
          else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("local")) == 0)
            {
              this->scheduler_type_ = ES_SCHED_LOCAL;
            }
          else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_TEXT("none")) == 0)
            {
              this->scheduler_type_ = ES_SCHED_NONE;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT("Unknown scheduling type <%s> ")
                          ACE_TEXT("defaulting to none\n"),
                          get_opt.opt_arg ()));
              this->scheduler_type_ = ES_SCHED_NONE;
            }
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("Usage: %s ")
                      ACE_TEXT("-n service_name ")
                      ACE_TEXT("-o ior_file_name ")
                      ACE_TEXT("-p pid_file_name ")
                      ACE_TEXT("-s <global|local|none> ")
                      ACE_TEXT("-q ec_object_id ")
                      ACE_TEXT("-x [disable naming service bind] ")
                      ACE_TEXT("-b [use bidir giop] ")
                      ACE_TEXT("\n"),
                      argv[0]));
          return -1;
        }
    }

  return 0;
}


RtecEventChannelAdmin::ConsumerAdmin_ptr
Event_Service::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->ec_impl_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

RtecEventChannelAdmin::SupplierAdmin_ptr
Event_Service::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->ec_impl_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
Event_Service::destroy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->ec_impl_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->orb_->shutdown ();
}

RtecEventChannelAdmin::Observer_Handle
Event_Service::append_observer (RtecEventChannelAdmin::Observer_ptr observer
                                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
          RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER))
{
  return this->ec_impl_->append_observer (observer ACE_ENV_ARG_PARAMETER);
}

void
Event_Service::remove_observer (RtecEventChannelAdmin::Observer_Handle handle
                                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
          RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER))
{
  this->ec_impl_->remove_observer (handle ACE_ENV_ARG_PARAMETER);
}

