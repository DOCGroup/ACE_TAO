// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Cubit/TAO/DII_Cubit
// 
// = FILENAME
//    DII_Cubit_Test.cpp
//
// = DESCRIPTION
//    This example tests the DII Cubit client using the IDL Cubit
//    server. The test forks and executes two processes to run both
//    the DII cubit client and the IDL cubit server. The client makes DII
//    requests on the server object and shutdowns the server in the end.
//    No command line arguments are needed to run the test.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu)
//    Jeff Parsons (jp4@cs.wustl.edu)
// 
// ============================================================================

#include "tests/test_config.h"
#include "ace/Process.h"

int
main (int, char *[])
{
  ACE_START_TEST ("DII_Cubit_Test:");

  // Create a servant in the IDL_Cubit directory.
  ACE_Process_Options server_options;
  server_options.command_line ("../IDL_Cubit/server -ORBport 0 -ORBobjrefstyle url");
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

  ACE_OS::sleep (2);

  // Create a client that will shut down the server after the tests.
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
