//
// $Id$
//

#include "tao/corba.h"

#include "orbsvcs/CosNamingC.h"
#include "Config_Scheduler.h"

int main (int argc, char *argv[])
{
  TAO_TRY
    {
      // Initialize ORB.
      CORBA::ORB_var orb = 
	CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

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
			   " (%P|%t) Unable to initialize the POA.\n"),
			  1);

      CosNaming::NamingContext_var naming_context = 
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Create an Scheduling service servant...
      ACE_Config_Scheduler scheduler_impl;
      TAO_CHECK_ENV;

      RtecScheduler::Scheduler_var scheduler = 
	scheduler_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
	orb->object_to_string (scheduler.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "The scheduler IOR is <%s>\n", str.in ()));

      // Register the servant with the Naming Context....
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");
      naming_context->bind (schedule_name, scheduler, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "running scheduling service\n"));
      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("schedule_service");
    }
  TAO_ENDTRY;

  return 0;
}
