// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    Test bugzilla 3505
//
// = AUTHOR
//    Johnny Willemsen <jwillemsen@remedy.nl>
//
// ============================================================================

#include "ace/Service_Gestalt.h"
#include "test_config.h"



int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3505_Regression_Test"));
  const int test_argc_size2 = 5;
  int arg2 = test_argc_size2;
  const ACE_TCHAR *args2[test_argc_size2] = {
    ACE_TEXT ("\\network\\temp\\ACE\\wince6\\ACE_wrappers\\TAO\\tests\\Hello\\server"),
    ACE_TEXT ("-ORBDebugLevel"),
    ACE_TEXT ("-0"),
    ACE_TEXT ("-o"),
    ACE_TEXT ("\\network\\temp\\ACE\\wince6\\ACE_wrappers/TAO\\tests\\Hello/server.ior")
  };

  // Make sure ACE debug is false
  ACE::debug (false);

  ACE_Service_Gestalt gestalt;
  gestalt.parse_args (arg2, const_cast<ACE_TCHAR**>(args2));

  if (ACE::debug ())
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("ACE debug has been enabled by the service gestalt\n")), -1);
    }

  ACE_END_TEST;
  return 0;
}
