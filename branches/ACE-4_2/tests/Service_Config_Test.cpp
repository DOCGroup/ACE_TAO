// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Service_Config_Test.cpp
//
// = DESCRIPTION
//      This is a simple test to make sure the Service_Configurator is
//      working correctly. 
//
// = AUTHOR
//    Doug Schmidt
// 
// ============================================================================

#include "ace/Service_Config.h"
#include "test_config.h"

static void
run_test (int argc, char *argv[])
{
  // We need this scope to make sure
  ACE_Service_Config daemon;
  
  daemon.open (argc, argv);

  ACE_Time_Value tv (argc > 1 ? atoi (argv[1]) : 2);

  ACE_ASSERT (daemon.run_reactor_event_loop (tv) == 0);
}

int 
main (int argc, char *argv[])
{
  ACE_START_TEST ("Service_Config_Test");

  run_test (argc, argv);

  ACE_END_TEST;
  return 0;
}
