// $Id$
//
// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Obtack_Test.cpp
//
// = DESCRIPTION
//    Checks the functionality of ACE_Obstack<T>
//
// = AUTHOR
//    Steve Huston  <shuston@riverace.com>
//
// ============================================================================

#include "ace/OS.h"
#include "ace/Obstack.h"
#include "ace/Log_Msg.h"
#include "test_config.h"

ACE_RCSID(tests, Obstack_Test, "$Id$")


int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Obstack_Test"));

  int errors = 0;
  // For this test, the length of the ACE_Obstack must be larger than
  // both of these strings, but less than their sum.
  const ACE_TCHAR *str1 = ACE_TEXT ("Mary had a little lamb.");
  const ACE_TCHAR *str2 = ACE_TEXT ("It's fleece was white as snow.");
  ACE_Obstack_T<ACE_TCHAR> stack (ACE_OS_String::strlen (str2) + 5);

  // This test checks to see that if a request can't be lengthened,
  // the first part of it doesn't get lost.
  for (size_t i = 0; i < ACE_OS_String::strlen (str1); i++)
    stack.grow_fast (str1[i]);
  if (stack.request (ACE_OS_String::strlen (str2)) == 0) {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Request allowed but it should overflow\n")));
    ++errors;
  }
  ACE_TCHAR *str = stack.freeze ();
  if (str == 0) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("freeze failed!\n")));
    ++errors;
  }
  else if (ACE_OS_String::strcmp (str, str1) != 0) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Lost a string; expected: %s, have: %s\n"),
                str1, str));
    ++errors;
  }
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test ok\n")));

  ACE_END_TEST;
  return errors == 0  ?  0  :  1;
}
