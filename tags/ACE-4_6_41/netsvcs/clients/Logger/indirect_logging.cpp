// $Id$

// This is a simple test that sends logging records to the Client
// Logging Daemon running on the localhost.  This daemon then forwards
// them to the Server Logging Daemon.  If there is no Server Logging
// Daemon, the logging records will be written to stderr.

#include "ace/OS.h"

ACE_RCSID(Logger, indirect_logging, "$Id$")

int 
main (int argc, char *argv[])
{
  const char *prog_name  = argv[0];
  int iterations  = argc < 2 ? 10 : ACE_OS::atoi (argv[1]);
  const char *logger_key = argc < 3 ? ACE_DEFAULT_LOGGER_KEY : argv[2];

  ACE_OS::srand ((u_int) ACE_OS::time (0));

  ACE_LOG_MSG->open (prog_name, ACE_Log_Msg::LOGGER,
		     ACE_WIDE_STRING (logger_key));

  ACE_DEBUG ((LM_STARTUP, "starting up the test\n"));

  for (int i = 0; i < iterations; i++)
    {
      size_t priority = ACE_OS::rand () % int (LM_MAX);
      ACE_POW (priority);
      ACE_DEBUG ((ACE_Log_Priority (priority), 
		  "random message %d...\n", 
		  priority));
    }

  ACE_DEBUG ((LM_SHUTDOWN, "closing down the test\n"));

#if defined (ACE_WIN32)
  // !!Important, Winsock is broken in that if you don't close
  // down the connection before exiting main, you'll lose data.
  // More over, your server might get "Access Violation" from
  // within Winsock functions.

  // Here we close down the connection to Logger by redirecting
  // the logging destination back to stderr.
  ACE_LOG_MSG->open (0, ACE_Log_Msg::STDERR, 0);
#endif /* ACE_WIN32 */

  return 0;
}
