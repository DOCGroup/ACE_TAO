// This is a simple test that sends logging records to the Client
// Logging Daemon running on the localhost.  This daemon then forwards
// them to the Server Logging Daemon.  If there is no Server Logging
// Daemon, the logging records will be written to stderr.

#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Record.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const * const prog_name = argv[0];
  int const iterations = argc < 2 ? 10 : ACE_OS::atoi (argv[1]);
  ACE_TCHAR const * const logger_key = argc < 3 ? ACE_DEFAULT_LOGGER_KEY : argv[2];
  int const verbose = argc < 4 ? 0 : ACE_Log_Msg::VERBOSE;

  ACE_OS::srand (static_cast<u_int> (ACE_OS::time (nullptr)));

  if (ACE_LOG_MSG->open (prog_name, ACE_Log_Msg::LOGGER, logger_key) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Cannot open logger, using STDERR\n"));

      if (ACE_LOG_MSG->open (prog_name, ACE_Log_Msg::STDERR | verbose) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot open logger\n"), -1);
    }

  ACE_DEBUG ((LM_STARTUP, "starting up the test\n"));

  for (int i = 0; i < iterations; i++)
    {
      int priority = ACE_OS::rand () % static_cast<int> (LM_MAX);
      ACE_POW (priority);
      ACE_Log_Priority const log_priority = static_cast<ACE_Log_Priority> (priority);
      ACE_DEBUG ((log_priority,
                  "random message %s (%d)...\n",
                  ACE_Log_Record::priority_name (log_priority),
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
  ACE_LOG_MSG->open (nullptr, ACE_Log_Msg::STDERR, 0);
#endif /* ACE_WIN32 */

  return 0;
}
