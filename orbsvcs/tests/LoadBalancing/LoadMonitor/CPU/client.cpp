// $Id$

#include "orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *location = ACE_TEXT("MyLocation");

const int MAX_RETRIES = 10;
const CosLoadBalancing::LoadId LOAD_ID = CosLoadBalancing::LoadAverage;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("l:"));

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
check_loads (const CosLoadBalancing::LoadList & loads)
{
  if (loads.length () != 1)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Load list length is not equal to one.\n"));

      throw CORBA::INTERNAL ();
    }

  if (loads[0].id != LOAD_ID
      || loads[0].value < 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Returned load is not a CPU load.\n"));

      throw CORBA::INTERNAL ();
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Obtain a reference to the LoadManager.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("LoadManager");

      CosLoadBalancing::LoadManager_var lm =
        CosLoadBalancing::LoadManager::_narrow (obj.in ());

      CosLoadBalancing::Location the_location (1);
      the_location.length (1);

      the_location[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(location));

      // Attempt to get loads from the LoadManager.
      ACE_DEBUG ((LM_INFO,
                  "\n"
                  "Retrieving loads from LoadManager ..."));

      CosLoadBalancing::LoadList_var loads;

      CORBA::Boolean retrieved_load = 0;

      // Try a few times until a load is capable of being retrieved.
      for (int i = 0; i < MAX_RETRIES && retrieved_load == 0; ++i)
        {
          try
            {
              loads = lm->get_loads (the_location);

              retrieved_load = 1;
            }
          catch (const CORBA::Exception&)
            {
              ACE_DEBUG ((LM_DEBUG, ".")); // Show some progress.

              // Give some time for loads to be reported to the
              // LoadManager.
              ACE_OS::sleep (2);
            }
        }

      if (retrieved_load == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\nERROR: Unable to retrieve loads "
                           "from LoadManager.\n"),
                          -1);
      else
        ACE_DEBUG ((LM_INFO,
                    " DONE\n"));

      ::check_loads (loads.in ());

      // Attempt to get loads directly from the LoadMonitor.
      CosLoadBalancing::LoadMonitor_var monitor =
        lm->get_load_monitor (the_location);

      CosLoadBalancing::Location_var cpu_mon_location =
        monitor->the_location ();

      if (cpu_mon_location.in () != the_location)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: Mismatched CPU load monitor location\n"));

          throw CORBA::INTERNAL ();
        }

      ACE_DEBUG ((LM_INFO,
                  "Retrieving loads directly from LoadMonitor ..."));

      loads = monitor->loads ();

      ACE_DEBUG ((LM_INFO,
                  " DONE\n"));

      ::check_loads (loads.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CPU Load Monitor test:");
      return -1;
    }

  ACE_DEBUG ((LM_INFO, "CPU Load Monitor test passed.\n\n"));

  return 0;
}
