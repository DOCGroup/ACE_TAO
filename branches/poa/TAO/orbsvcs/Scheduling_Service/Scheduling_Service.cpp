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
      CORBA::ORB_ptr orb = 
	CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::POA_ptr poa =
	orb->POA_init(argc, argv, "POA");
      if (poa == 0)
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     " (%P|%t) Unable to initialize the POA.\n"),
			    1);
	}

      CORBA::Object_ptr objref =
	orb->resolve_initial_references ("NameService");
      TAO_CHECK_ENV;

      CosNaming::NamingContext_var naming_context = 
        CosNaming::NamingContext::_narrow (objref, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Create an Scheduling service servant...
      RtecScheduler::Scheduler_ptr scheduler = new ACE_Config_Scheduler;
      // CORBA::Object::_duplicate(scheduler);
      TAO_CHECK_ENV;

      CORBA::String str =
	orb->object_to_string (scheduler, TAO_TRY_ENV);
      ACE_OS::puts ((char *) str);

      // Register the servant with the Naming Context....
      CosNaming::Name schedule_name (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");
      schedule_name.length (1);
      naming_context->bind (schedule_name, scheduler, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "running scheduling service\n"));
      if (orb->run () == -1)
	{
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);
	}

      CORBA::release (scheduler);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("schedule_service");
    }
  TAO_ENDTRY;

  return 0;
}
