// $Id$

#include "Cubit_Client.h"
#include "tao/Timeprobe.h"
#include "ace/Sched_Params.h"

ACE_RCSID(IDL_Cubit, client, "$Id$")

// This function runs the client test.

int
main (int argc, char **argv)
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
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

  Cubit_Client cubit_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tIDL_Cubit: client\n\n"));

  if (cubit_client.init (argc, argv) == -1)
    return 1;

  int retval = cubit_client.run ();

  ACE_TIMEPROBE_PRINT;

//  ACE_END_TEST;

  return retval;
}
