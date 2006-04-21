// $Id$

#include "NodeApplication_Core.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"
#include "ciao/CIAO_common.h"


void print_arg (int argc, char *argv[])
{
  for (int i = 0; i < argc; ++i)
    ACE_DEBUG ((LM_DEBUG, "NodeApplication ARGV[%d] = %s\n", i, argv [i]));
}

int
main (int argc, char *argv[])
{
   int priority = 20;
   // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_RR,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (errno == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG, "*** Starting NodeApplication\n"));

      CIAO::NodeApplication_Options nodeapp_options;

      if (CIAO::debug_level () > 9)
        print_arg (argc, argv);

      if (nodeapp_options.parse_args (argc, argv) != 0)
        return -1;

      CIAO::NodeApplication_Core nodeapp_core (nodeapp_options);

      if (nodeapp_core.startup (argc, argv) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to start up the NodeApplication_Core\n"), -1);

      // Run the main event loop for the ORB.
      nodeapp_core.run_orb ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "NodeApp::main\t\n");
      return 1;
    }
  ACE_ENDTRY;

  if (CIAO::debug_level () > 1)
    ACE_DEBUG ((LM_DEBUG, "CIAO (%P|%t) - This NodeApplication is destroyed!\n"));

  return 0;
}
