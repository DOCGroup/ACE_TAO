// $Id$

#include "Event_Service.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Argv_Type_Converter.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Event/Module_Factory.h"
#include "orbsvcs/Event/Event_Channel.h"

#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

ACE_RCSID(Event_Service, Event_Service, "$Id$")

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  Event_Service event_service;
  return event_service.run (argc, argv);
}

// ****************************************************************

Event_Service::Event_Service (void)
  : module_factory_ (0),
    sched_impl_ (0),
    ec_impl_ (0),
    scheduler_type_ (SCHED_NONE),
    event_service_type_ (ES_NEW)
{
}

Event_Service::~Event_Service (void)
{
  delete this->ec_impl_;
  this->ec_impl_ = 0;
  delete this->sched_impl_;
  this->sched_impl_ = 0;
  delete this->module_factory_;
  this->module_factory_ = 0;
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

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA"
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::Scheduler_var scheduler;
      // This is the name we (potentially) register the Scheduling
      // Service in the Naming Service.
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");


      //the old EC always needs a scheduler. If none is
      //specified, we default to a local scheduler
      if (this->scheduler_type_ == SCHED_LOCAL ||
          (this->scheduler_type_ == SCHED_NONE && 
           this->event_service_type_ != ES_NEW))
        {
          //Create a local scheduler instance
          ACE_NEW_RETURN (this->sched_impl_,
                          ACE_Config_Scheduler,
                          1);

          scheduler = this->sched_impl_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Register the servant with the Naming Context....
          naming_context->rebind (schedule_name, scheduler.in ()
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (this->scheduler_type_ == SCHED_GLOBAL)
        {
          //Get reference to a scheduler from naming service
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

      switch (this->event_service_type_)
        {
        case ES_NEW:
          {
            TAO_EC_Event_Channel_Attributes attr (root_poa.in (),
                                                  root_poa.in ());

            if (this->scheduler_type_ != SCHED_NONE)
              {
                attr.scheduler = scheduler.in ();
              }

            TAO_EC_Event_Channel* ec;
            ACE_NEW_RETURN (ec,
                            TAO_EC_Event_Channel (attr),
                            1);
            this->ec_impl_ = ec;
            ec->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
          break;

        case ES_OLD_REACTIVE:
          {
            ACE_NEW_RETURN (this->module_factory_,
                            TAO_Reactive_Module_Factory,
                            1);
            ACE_NEW_RETURN (this->ec_impl_,
                            ACE_EventChannel (scheduler.in (),
                                              1,
                                              ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                              this->module_factory_),
                            1);
          }
          break;
        case ES_OLD_MT:
          {
            ACE_NEW_RETURN (this->module_factory_,
                            TAO_Default_Module_Factory,
                            1);

            ACE_NEW_RETURN (this->ec_impl_,
                            ACE_EventChannel (scheduler.in (),
                                              1,
                                              ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                              this->module_factory_),
                            1);
          }

          break;
        }

      // Notice that we activate *this* object with the POA, but we
      // forward all the requests to the underlying EC
      // implementation.
      RtecEventChannelAdmin::EventChannel_var ec =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        this->orb_->object_to_string (ec.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp(this->ior_file_name_.c_str(), "") != 0)
        {
          FILE *output_file= 
            ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR(this->ior_file_name_.c_str()), 
                           ACE_LIB_TEXT("w"));
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
                           ACE_LIB_TEXT("w"));
          if (pidf != 0)
            {
              ACE_OS::fprintf (pidf,
                               "%ld\n",
                               ACE_static_cast (long, ACE_OS::getpid ()));
              ACE_OS::fclose (pidf);
            }
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT("The EC IOR is <%s>\n"), ACE_TEXT_CHAR_TO_TCHAR(str.in ())));

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup (this->service_name_.c_str());
      naming_context->rebind (channel_name, ec.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT("%s; running event service\n"),
                  ACE_TEXT_CHAR_TO_TCHAR(__FILE__)));
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      naming_context->unbind (channel_name ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (scheduler.in ()))
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

  ACE_Get_Opt get_opt (argc, argv, ACE_LIB_TEXT("n:o:p:s:t:"));
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

        case 's':
          // It could be just a flag (i.e. no "global" or "local"
          // argument, but this is consistent with the EC_Multiple
          // test and also allows for a runtime scheduling service.

          if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_LIB_TEXT("global")) == 0)
            {
              this->scheduler_type_ = SCHED_GLOBAL;
            }
          else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_LIB_TEXT("local")) == 0)
            {
              this->scheduler_type_ = SCHED_LOCAL;
            }
          else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_LIB_TEXT("none")) == 0)
            {
              this->scheduler_type_ = SCHED_NONE;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_LIB_TEXT("Unknown scheduling type <%s> ")
                          ACE_LIB_TEXT("defaulting to none\n"),
                          get_opt.opt_arg ()));
              this->scheduler_type_ = SCHED_NONE;
            }
          break;

        case 't':
          if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_LIB_TEXT("NEW")) == 0)
            {
              this->event_service_type_ = ES_NEW;
            }
          else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_LIB_TEXT("OLD_REACTIVE")) == 0)
            {
              this->event_service_type_ = ES_OLD_REACTIVE;
            }
          else if (ACE_OS::strcasecmp (get_opt.opt_arg (), ACE_LIB_TEXT("OLD_MT")) == 0)
            {
              this->event_service_type_ = ES_OLD_MT;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_LIB_TEXT("Unknown event service type <%s> ")
                          ACE_LIB_TEXT("defaulting to NEW\n"),
                          get_opt.opt_arg ()));
              this->event_service_type_ = ES_NEW;
            }
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT("Usage: %s ")
                      ACE_LIB_TEXT("-n service_name ")
                      ACE_LIB_TEXT("-o ior_file_name ")
                      ACE_LIB_TEXT("-p pid_file_name ")
                      ACE_LIB_TEXT("-s <global|local|none> ")
                      ACE_LIB_TEXT("-t <new|old_reactive|old_mt> ")
                      ACE_LIB_TEXT("\n"),
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Ptr<POA_RtecScheduler::Scheduler>;
template class auto_ptr<POA_RtecScheduler::Scheduler>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Ptr<POA_RtecScheduler::Scheduler>
#pragma instantiate auto_ptr<POA_RtecScheduler::Scheduler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
