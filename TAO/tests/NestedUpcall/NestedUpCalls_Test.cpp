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
//    the client. The client and the server
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

int
main (int, char *[])
{
  ACE_START_TEST ("NestedUpCalls_Test:");

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

  ACE_Process_Options client_options;
  client_options.command_line ("./client -ORBport 0");
  ACE_Process client;

  if (client.spawn (client_options) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "%p.\n",
                       "Client spawn failed"),
                      0);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Client forked with pid = %d.\n",
                client.getpid ()));

  if (server.wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) wait on server failed.\n"),
                      -1);

  if (client.wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t)wait on client failed.\n"),
                      -1);

  ACE_END_TEST;
  return 0;
}
