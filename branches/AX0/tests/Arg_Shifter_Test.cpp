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

ACE_RCSID(tests, Arg_Shifter_Test, "$Id$")

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Arg_Shifter_Test"));

  const int test_argc_size = 5;
  int argl (test_argc_size);
  const ACE_TCHAR *args[test_argc_size] = {
    ACE_TEXT ("-known"),
    ACE_TEXT ("-huh"),
    ACE_TEXT ("-arg"),
    ACE_TEXT ("-what"),
    ACE_TEXT ("arg")
  };

  ACE_Arg_Shifter shifter (argl, args);

  if (!shifter.is_anything_left ())
    ACE_ERROR ((LM_ERROR, "is_anything_left() returned 0 at start.\n"));

  ACE_END_TEST;
  return 0;
}
