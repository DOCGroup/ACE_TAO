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

#define APPLICATION \
__TEXT ("..") ACE_DIRECTORY_SEPARATOR_STR \
__TEXT ("netsvcs") ACE_DIRECTORY_SEPARATOR_STR \
__TEXT ("servers") ACE_DIRECTORY_SEPARATOR_STR \
__TEXT ("main") ACE_PLATFORM_EXE_SUFFIX \
__TEXT (" -f ") ACE_PLATFORM

int
main (int, char *[])
{
  ACE_START_TEST ("Time_Service_Test");

  // Make sure that the backing store is not there. We need to make
  // sure because this test kills the Time Clerk and on some platforms
  // the Clerk is not allowed to do a graceful shutdown. By cleaning
  // the backing store here, we are sure that we get a fresh start and
  // no garbage data from a possible aborted run
  ACE_OS::unlink (ACE_DEFAULT_BACKING_STORE);

  ACE_Process_Options server_options;
  server_options.command_line (APPLICATION __TEXT ("server.conf"));
  ACE_Process server;

  if (server.spawn (server_options) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%n %p.\n", "Server fork failed"), 0);
  else
    ACE_DEBUG ((LM_DEBUG, "Server forked with pid = %d.\n", server.getpid ()));

  ACE_OS::sleep (3);

  ACE_Process_Options clerk_options;
  clerk_options.command_line (APPLICATION __TEXT ("clerk.conf"));
  ACE_Process clerk;

  if (clerk.spawn (clerk_options) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p.\n", "Clerk fork failed"), 0);
  else
    ACE_DEBUG ((LM_DEBUG, "Server forked with pid = %d.\n", clerk.getpid ()));

  ACE_DEBUG ((LM_DEBUG, "Sleeping...\n"));
  ACE_OS::sleep (10);

  if (clerk.kill () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Kill failed for clerk.\n"), -1);

  if (server.kill () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Kill failed for server.\n"), -1);

  // Since we kill the clerk process, on Win32 it may not do a
  // graceful shutdown and the backing store file is left behind.
  if (clerk.wait () != 0)
    ACE_OS::unlink (ACE_DEFAULT_BACKING_STORE);

  ACE_END_TEST;
  return 0;
}
