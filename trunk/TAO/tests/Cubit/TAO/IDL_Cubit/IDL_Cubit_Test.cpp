// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Cubit/TAO/IDL_Cubit
// 
// = FILENAME
//    IDL_Cubit_Test.cpp
//
// = DESCRIPTION
//    This example tests the IDL Cubit server and client
//    components. The test forks and execs two processes to run both
//    the client and the time server. The clerk and the server
//    communicate for a short duration after which the main process
//    kills both the processes. No command line arguments are needed
//    to run the test.
//
// = AUTHOR
//    Nagarajan Surendran (naga@cs.wustl.edu)
// 
// ============================================================================

#include "tests/test_config.h"
#include "ace/Process.h"

int
main (int, char *[])
{
  ACE_START_TEST ("IDL_Cubit_Test:");

  ACE_Process_Options server_options;
  server_options.command_line ("./server -ORBport 0 -ORBobjrefstyle url");
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

  // create a client that will shutdown the server after the tests.
  ACE_Process_Options client_options;
  client_options.command_line ("./client -ORBport 0 -ORBobjrefstyle url -x");
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

  if (server.wait () < 0)
    ACE_DEBUG ((LM_DEBUG,"(%P|%t) Wait on server failed\n"));
  if (client.wait () < 0)
    ACE_DEBUG ((LM_DEBUG,"(%P|%t) Wait on client failed\n"));

  ACE_END_TEST;
  return 0;
}
