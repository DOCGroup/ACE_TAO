// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "tao/corba.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Event/Module_Factory.h"
#include "orbsvcs/Event/Event_Channel.h"

ACE_RCSID(Event_Service, Event_Service, "$Id$")

const char* service_name = "EventService";
// The name we use to register with the Naming Service.

int global_scheduler = 0;
// If 0 we instantiante a local Scheduling Service and register it
// with the Naming Service.
// Otherwise we just resolve the Scheduling Service using the Naming
// Service (i.e. we assume there is a global scheduling service running.

int reactive = 0;
// If 1 we use the reactive EC.

int
parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "n:s:r");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          service_name = get_opt.optarg;
          break;

	case 's':
	  // It could be just a flag (i.e. no "global" or "local"
	  // argument, but this is consistent with the EC_Multiple
	  // test and also allows for a runtime scheduling service.

          if (ACE_OS::strcasecmp (get_opt.optarg, "global") == 0)
            {
              global_scheduler = 1;
            }
          else if (ACE_OS::strcasecmp (get_opt.optarg, "local") == 0)
            {
              global_scheduler = 0;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Unknown scheduling type <%s> "
                          "defaulting to local\n",
                          get_opt.optarg));
              global_scheduler = 0;
            }
          break;

        case 'r':
          reactive = 1;
          break;
	  
        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "-n service_name "
                      "-s <global|local> "
                      "-r "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}


int main (int argc, char *argv[])
{
  TAO_TRY
    {
      // Initialize ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (parse_args (argc, argv) == -1)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
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

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      auto_ptr<POA_RtecScheduler::Scheduler> scheduler_impl;
      RtecScheduler::Scheduler_var scheduler;

      // This is the name we (potentially) register the Scheduling
      // Service in the Naming Service.
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");

      if (global_scheduler == 0)
        {
          scheduler_impl =
            auto_ptr<POA_RtecScheduler::Scheduler>(new ACE_Config_Scheduler);
          if (scheduler_impl.get () == 0)
	    return 1;
          scheduler = scheduler_impl->_this (TAO_TRY_ENV);
          TAO_CHECK_ENV;

	  CORBA::String_var str =
	    orb->object_to_string (scheduler.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  ACE_DEBUG ((LM_DEBUG, "The (local) scheduler IOR is <%s>\n",
		      str.in ()));

	  // Register the servant with the Naming Context....
	  naming_context->bind (schedule_name, scheduler.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;
        }

      ACE_Scheduler_Factory::use_config (naming_context.in ());

      // We pick the right module factory according to the command
      // line options
      TAO_Default_Module_Factory default_module_factory;
      TAO_Reactive_Module_Factory reactive_module_factory;

      TAO_Module_Factory* module_factory = &default_module_factory;
      if (reactive)
        module_factory = &reactive_module_factory;

      // Register Event_Service with Naming Service.
      ACE_EventChannel ec_impl (1,
                                ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                module_factory);

      RtecEventChannelAdmin::EventChannel_var ec =
        ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
        orb->object_to_string (ec.in (), TAO_TRY_ENV);

      ACE_DEBUG ((LM_DEBUG,
		  "The EC IOR is <%s>\n", str.in ()));

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup (service_name);
      naming_context->bind (channel_name, ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "%s; running event service\n", __FILE__));
      if (orb->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);

      naming_context->unbind (channel_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (global_scheduler == 0)
	{
	  naming_context->unbind (schedule_name, TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	}

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("EC");
    }
  TAO_ENDTRY;


  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Ptr<POA_RtecScheduler::Scheduler>;
template class auto_ptr<POA_RtecScheduler::Scheduler>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Ptr<POA_RtecScheduler::Scheduler>
#pragma instantiate auto_ptr<POA_RtecScheduler::Scheduler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
