// $Id$

#include "ace/config-all.h"

#if defined (ACE_VXWORKS) && !defined (__RTP__)
# undef ACE_MAIN
# define ACE_MAIN client
#endif /* ACE_VXWORKS && !__RTP__ */

#include "Cubit_Client.h"
#include "tao/Timeprobe.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"

#include "tao/Strategies/advanced_resource.h"

// This function runs the client test.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              ACE_THR_PRI_FIFO_DEF,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "client (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "client (%P|%t): sched_params failed\n"));
    }

  Cubit_Client cubit_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n (%P|%t)\tIDL_Cubit: client\n\n"));

  if (cubit_client.init (argc, argv) == -1)
    return 1;

  int const retval = cubit_client.run ();

  ACE_TIMEPROBE_PRINT;

  return retval;
}
