// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Time_Service_Test
//
// = DESCRIPTION
//    This example tests the Time Service server and clerk
//    components. The test forks and execs two processes to run both
//    the clerk and the time server. The clerk and the server
//    communicate for a short duration after which the main process
//    kills both the processes. No command line arguments are needed
//    to run the test.
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================

#include "ace/OS.h"

#include "test_config.h"
#include "ace/Process.h"

int
main (int, char *[])
{
  ACE_START_TEST ("Time_Service_Test");

  char app[BUFSIZ];
  char server_conf[BUFSIZ];
  char clerk_conf[BUFSIZ];

  ACE_OS::sprintf (server_conf, "%s", ACE_PLATFORM "server.conf");
  ACE_OS::sprintf (clerk_conf, "%s", ACE_PLATFORM "clerk.conf");

  ACE_OS::sprintf (app, ".." ACE_DIRECTORY_SEPARATOR_STR_A "netsvcs" ACE_DIRECTORY_SEPARATOR_STR_A
		   "servers" ACE_DIRECTORY_SEPARATOR_STR_A "main" ACE_PLATFORM_EXE_SUFFIX);

  char *s_argv[4];
  s_argv[0] = app;
  s_argv[1] = "-f";
  s_argv[2] = server_conf;
  s_argv[3] = 0;

  ACE_Process server;

  if (server.start (s_argv) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%n %p.\n", "Server fork failed"), 0);
  else
    ACE_DEBUG ((LM_DEBUG, "Server forked with pid = %d.\n", server.getpid ()));

  ACE_OS::sleep (3);
  s_argv[2] = clerk_conf;

  ACE_Process clerk;

  if (clerk.start (s_argv) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p.\n", "Clerk fork failed"), 0);
  else
    ACE_DEBUG ((LM_DEBUG, "Server forked with pid = %d.\n", clerk.getpid ()));

  ACE_DEBUG ((LM_DEBUG, "Sleeping...\n"));
  ACE_OS::sleep (10);

  if (clerk.kill () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Kill failed for clerk.\n"), -1);

  if (server.kill () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Kill failed for server.\n"), -1);

  ACE_END_TEST;
  return 0;
}
