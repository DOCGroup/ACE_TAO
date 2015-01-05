// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This test exercises ACE_Service_Config SEGV during shutdown when
//    Modules get double deleted. Once by Stream parent,
//    once by ACE_Service_Repository
//
// = AUTHOR
//    Chad Beaulac <chad@objectivesolutions.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"



int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3334_Regression_Test"));

  ACE_TCHAR *_argv[3] = { argv[0],
                          const_cast<ACE_TCHAR*> (ACE_TEXT ("-f")),
                          const_cast<ACE_TCHAR*>
                            (ACE_TEXT ("Bug_3334_Regression_Test.conf")) };
  int status = 0;
  if ((status = ACE_Service_Config::open (3,
                                          _argv,
                                          ACE_DEFAULT_LOGGER_KEY,
                                          true,
                                          true /*ignore def svc.conf*/)) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("open"),
                1));

  ACE_Service_Config::fini_svcs ();

  ACE_END_TEST;
  return status;
}
