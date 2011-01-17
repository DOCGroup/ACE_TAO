// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program exercises the ACE_Arg_Shifter class.
//
// = AUTHOR
//    Steve Huston  <shuston@riverace.com>
//
// ============================================================================

#include "ace/Arg_Shifter.h"
#include "test_config.h"



int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Arg_Shifter_Test"));
  const int test_argc_size1 = 5;
  int argl (test_argc_size1);
  const ACE_TCHAR *args1[test_argc_size1] = {
    ACE_TEXT ("-known"),
    ACE_TEXT ("-huh"),
    ACE_TEXT ("-arg"),
    ACE_TEXT ("-what"),
    ACE_TEXT ("arg")
  };

  ACE_Arg_Shifter shifter1 (argl, args1);

  if (!shifter1.is_anything_left ())
    ACE_ERROR ((LM_ERROR, "is_anything_left() returned 0 at start.\n"));

  const int test_argc_size2 = 4;
  int arg2 = test_argc_size2;
  const ACE_TCHAR *args2[test_argc_size2] = {
    ACE_TEXT ("-ORBDebugLevel"),
    ACE_TEXT ("-0"),
    ACE_TEXT ("-o"),
    ACE_TEXT ("server.ior")
  };
  bool found_ORBDebug = false;

  ACE_Arg_Shifter arg_shifter2 (arg2, args2);
  while (arg_shifter2.is_anything_left ())
    {
      if (0 == arg_shifter2.cur_arg_strncasecmp (ACE_TEXT ("-ORBDebug")))
        {
          found_ORBDebug = true;
          arg_shifter2.consume_arg ();
        }
      else if (0 == arg_shifter2.cur_arg_strncasecmp (ACE_TEXT ("-ORBDaemon")))
        {
          arg_shifter2.consume_arg ();
        }
      else
        {
          arg_shifter2.ignore_arg ();
        }
    }

  if (found_ORBDebug)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("argshifter 2 test failed\n")), -1);
    }

  ACE_END_TEST;
  return 0;
}
