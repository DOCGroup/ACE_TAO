// ============================================================================
// @(#)Mutex_Test.cpp	1.1	10/18/96

//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Mutex_Test.cpp
//
// = DESCRIPTION
//      This is a simple test to illustrate the functionality of
//      ACE_Process_Mutex. The test acquires and releases mutexes. No
//      command line arguments are needed to run the test.
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
// 
// ============================================================================

#include "ace/Synch.h"
#include "ace/Log_Msg.h"
#include "test_config.h"

int
main (int argc, char *argv[])
{
  ACE_START_TEST;

  char *name = argc == 1 ? "hello" : argv[1];

  ACE_Process_Mutex pm (name);

  for (int i = 0; i < ACE_MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = trying to acquire\n"));
      ACE_ASSERT (pm.acquire () == 0);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = acquired\n"));

      ACE_OS::sleep (5);

      ACE_ASSERT (pm.release () == 0);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) = released\n"));
    }
  ACE_END_TEST;
  return 0;
}

