// This is a simple test that sends logging records to the Client
// $Id$

// Logging Daemon running on the localhost.  This daemon then forwards
// them to the Server Logging Daemon.  If there is no Server Logging
// Daemon, the logging records will be written to stderr.

#include "ace/Log_Msg.h"

int 
main (int argc, char *argv[])
{
  char *prog_name  = argv[0];
  int  iterations  = argc < 2 ? 10 : ACE_OS::atoi (argv[1]);
  char *logger_key = argc < 3 ? ACE_DEFAULT_RENDEZVOUS : argv[2];

  ACE_OS::srand ((u_int) ACE_OS::time (0));

  ACE_LOG_MSG->open (prog_name, ACE_Log_Msg::LOGGER, logger_key);

  ACE_DEBUG ((LM_STARTUP, "starting up the test\n"));

  for (int i = 0; i < iterations; i++)
    {
      int priority = ACE_OS::rand () % int (LM_MAX);
      ACE_POW (priority);
      ACE_DEBUG ((ACE_Log_Priority (priority), 
		  "random message %d...\n", 
		  priority));
    }

  ACE_DEBUG ((LM_SHUTDOWN, "closing down the test\n"));
  return 0;
}
