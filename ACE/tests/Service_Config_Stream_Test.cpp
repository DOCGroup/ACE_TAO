// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This test exercises ACE_Service_Config assembling a stream.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"

ACE_RCSID(tests, Service_Config_Stream_Test, "$Id$")

int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Service_Config_Stream_Test"));

  ACE_TCHAR *_argv[3] = { argv[0],
                          const_cast<ACE_TCHAR*> (ACE_TEXT ("-f")),
                          const_cast<ACE_TCHAR*>
                            (ACE_TEXT ("Service_Config_Stream_Test.conf")) };
  int status;
  if ((status = ACE_Service_Config::open (3,
                                          _argv,
                                          ACE_DEFAULT_LOGGER_KEY,
                                          true,
                                          true /*ignore def svc.conf*/)) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("open"),
                1));

  ACE_END_TEST;
  return status;
}
