// $Id$

#include "ace/config-all.h"
#if defined (VXWORKS)
# undef ACE_MAIN
# define ACE_MAIN server
#endif /* VXWORKS */

#include "Cubit_Server.h"
#include "tao/Timeprobe.h"
#include "ace/Sched_Params.h"

#include "tao/Strategies/advanced_resource.h"


ACE_RCSID(IDL_Cubit, server, "$Id$")

// This runs the server test.

int
main (int argc, char *argv[])
{
  int priority = ACE_THR_PRI_FIFO_DEF;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                              priority);

  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  Cubit_Server cubit_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Cubit: server\n\n"));
  ACE_TRY_NEW_ENV
    {
      int ret = cubit_server.init (argc, argv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ret == -1)
        return -1;

      cubit_server.run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_OS::sleep (5);
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }
  ACE_ENDTRY;
  ACE_TIMEPROBE_PRINT;

  return 0;
}
