// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls
//
// = FILENAME
//    NestedUpCalls_Test.cpp
//
// = DESCRIPTION
//    This example tests the NestedUpCalls server and client
//    components. The test forks and execs a process to run
//    the client. The clerk and the server
//    communicate for a short duration after which the main process
//    kills both the processes. No command line arguments are needed
//    to run the test.
//
// = AUTHOR
//    Nagarajan Surendran
//
// ============================================================================

#include "tests/test_config.h"
#include "ace/Process.h"

ACE_RCSID(Reactor, NestedUpCalls_Test, "$Id$")

int
main (int, char *[])
{
  ACE_START_TEST ("NestedUpCalls_Test:");

  // Make sure that the backing store is not there. We need to make
  // sure because this test kills the Time Clerk and on some platforms
  // the Clerk is not allowed to do a graceful shutdown. By cleaning
  // the backing store here, we are sure that we get a fresh start and
  // no garbage data from a possible aborted run

  ACE_OS::unlink (ACE_DEFAULT_BACKING_STORE);

  ACE_Process_Options server_options;
  server_options.command_line ("./server -ORBport 0");
  ACE_Process server;

  if (server.spawn (server_options) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "%n %p.\n",
                       "Server fork failed"),
                      0);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Server forked with pid = %d.\n",
                server.getpid ()));

  ACE_OS::sleep (5);

  ACE_Process_Options clerk_options;
  clerk_options.command_line ("./client -ORBport 0");
  ACE_Process clerk;

  if (clerk.spawn (clerk_options) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "%p.\n",
                       "Client spawn failed"),
                      0);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Client forked with pid = %d.\n",
                clerk.getpid ()));

  ACE_DEBUG ((LM_DEBUG, "Sleeping...\n"));
  ACE_OS::sleep (10);

  if (clerk.terminate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Terminate failed for clerk.\n"),
                      -1);

  if (server.terminate () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Terminate failed for server.\n"),
                      -1);

  // Since we kill the clerk process, on Win32 it may not do a
  // graceful shutdown and the backing store file is left behind.
  if (clerk.wait () != 0)
    ACE_OS::unlink (ACE_DEFAULT_BACKING_STORE);

  ACE_END_TEST;
  return 0;
}
