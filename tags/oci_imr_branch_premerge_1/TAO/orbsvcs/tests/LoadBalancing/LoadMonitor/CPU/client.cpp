// $Id$

#include "orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"
#include "ace/Get_Opt.h"


ACE_RCSID (CPU,
	   client,
	   "$Id$")


const char * location = "MyLocation";

const int MAX_RETRIES = 10;
const CosLoadBalancing::LoadId LOAD_ID = CosLoadBalancing::CPU;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "l:");

  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'l':
        location = get_opts.opt_arg ();
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s -l <ior>\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

void
check_loads (const CosLoadBalancing::LoadList & loads
             ACE_ENV_ARG_DECL)
{
  if (loads.length () != 1)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Load list length is not equal to one.\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }

  if (loads[0].id != LOAD_ID
      || loads[0].value < 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Returned load is not a CPU load.\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb =
	CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain a reference to the CodecFactory.
      CORBA::Object_var obj =
	orb->resolve_initial_references ("LoadManager"
					 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosLoadBalancing::LoadManager_var lm =
	CosLoadBalancing::LoadManager::_narrow (obj.in ()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosLoadBalancing::Location the_location (1);
      the_location.length (1);

      the_location[0].id = CORBA::string_dup (location);

      // Attempt to get loads from the LoadManager.
      ACE_DEBUG ((LM_INFO,
                  "\n"
                  "Retrieving loads from LoadManager ..."));

      CosLoadBalancing::LoadList_var loads;

      CORBA::Boolean retrieved_load = 0;

      // Try a few times until a load is capable of being retrieved.
      for (int i = 0; i < MAX_RETRIES && retrieved_load == 0; ++i)
        {
          ACE_TRY_EX (foo)
            {
              loads = lm->get_loads (the_location
                                     ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK_EX (foo);

              retrieved_load = 1;
            }
          ACE_CATCHANY
            {
              ACE_DEBUG ((LM_DEBUG, ".")); // Show some progress.

              // Give some time for loads to be reported to the
              // LoadManager.
              ACE_OS::sleep (2);
            }
          ACE_ENDTRY;
          ACE_TRY_CHECK;
        }

      if (retrieved_load == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\nERROR: Unable to retrieve loads "
                           "from LoadManager.\n"),
                          -1);
      else
        ACE_DEBUG ((LM_INFO,
                    " DONE\n"));

      ::check_loads (loads.in ()
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Attempt to get loads directly from the LoadMonitor.
      CosLoadBalancing::LoadMonitor_var monitor =
        lm->get_load_monitor (the_location
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosLoadBalancing::Location_var cpu_mon_location =
        monitor->the_location (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (cpu_mon_location.in () != the_location)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Mismatched CPU load monitor location\n"));

          ACE_TRY_THROW (CORBA::INTERNAL ());
        }

      ACE_DEBUG ((LM_INFO,
                  "Retrieving loads directly from LoadMonitor ..."));

      loads = monitor->loads (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_INFO,
                  " DONE\n"));

      ::check_loads (loads.in ()
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
			   "CPU Load Monitor test:");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_INFO, "CPU Load Monitor test passed.\n\n"));

  return 0;
}
