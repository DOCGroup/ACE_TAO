//
// $Id$
//

#include "ace/Sched_Params.h"
#include "ace/Get_Opt.h"
#include "tao/corba.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Scheduler_Factory.h"

// This program dumps the results of one scheduling in a C++ file.



int main (int argc, char *argv[])
{
  TAO_TRY
    {
      // Initialize ORB.
      CORBA::ORB_ptr orb = 
	CORBA::ORB_init (argc, argv, "dump_schedule", TAO_TRY_ENV);
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

      ACE_Scheduler_Factory::use_config (naming_context.ptr ());

      RtecScheduler::RT_Info_Set* infos;
      ACE_Scheduler_Factory::server ()->compute_scheduling
	(ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
					 ACE_SCOPE_THREAD),
	 ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
					 ACE_SCOPE_THREAD),
	 infos, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_Scheduler_Factory::dump_schedule (*infos,
					    "Scheduler_Runtime.cpp");
      delete infos;
    }
  TAO_CATCH (CORBA::SystemException, sys_ex)
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;

  return 0;
}
