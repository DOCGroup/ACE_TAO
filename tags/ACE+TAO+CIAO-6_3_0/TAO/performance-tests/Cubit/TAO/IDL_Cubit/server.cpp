// $Id$

#include "ace/config-all.h"

#if defined (ACE_VXWORKS) && !defined (__RTP__)
# undef ACE_MAIN
# define ACE_MAIN server
#endif /* ACE_VXWORKS && !__RTP__ */

#include "Cubit_Server.h"
#include "tao/Timeprobe.h"
#include "ace/Sched_Params.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"

#include "tao/Strategies/advanced_resource.h"

// This runs the server test.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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
  try
    {
      if (cubit_server.init (argc, argv)== -1)
        return -1;

      cubit_server.run ();

      ACE_OS::sleep (5);
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }
  ACE_TIMEPROBE_PRINT;

  return 0;
}
