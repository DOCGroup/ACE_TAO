// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"
// Listing 0 code/ch10
#include "ace/Process_Manager.h"

static const int NCHILDREN = 2;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc > 1)     // Running as a child.
    {
      ACE_OS::sleep (10);
    }
  else             // Running as a parent.
    {
      // Get the processwide process manager.
      ACE_Process_Manager* pm = ACE_Process_Manager::instance ();

      // Specify the options for the new processes
      // to be spawned.
      ACE_Process_Options options;
      options.command_line (ACE_TEXT ("%s a"), argv[0]);

      // Spawn two child processes.
      pid_t pids[NCHILDREN];
      pm->spawn_n (NCHILDREN, options, pids);

      // Destroy the first child.
      pm->terminate (pids[0]);

      // Wait for the child we just terminated.
      ACE_exitcode status;
      pm->wait (pids[0], &status);

      // Get the results of the termination.

#if !defined(ACE_WIN32)
      if (WIFSIGNALED (status) != 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%d died because of a signal ")
                    ACE_TEXT ("of type %d\n"),
                    pids[0], WTERMSIG (status)));
#else
      ACE_DEBUG
        ((LM_DEBUG,
          ACE_TEXT ("The process terminated with exit code %d\n"),
          status));
#endif /*ACE_WIN32*/

      // Wait for all (only one left) of the
      // children to exit.
      pm->wait (0);
    }

  return 0;
}
// Listing 0
