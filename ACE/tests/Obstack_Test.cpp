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

#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/Obstack.h"
#include "ace/Log_Msg.h"




int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Obstack_Test"));

  int errors = 0;
  // For this test, the length of the ACE_Obstack must be larger than
  // both of these strings, but less than their sum.
  const ACE_TCHAR str1[] = ACE_TEXT ("Mary had a little lamb.");
  const ACE_TCHAR str2[] = ACE_TEXT ("It's fleece was white as snow; but....");
  ACE_Obstack_T<ACE_TCHAR> stack (sizeof (str1) + 1);

  for (size_t i = 0; i < ACE_OS::strlen (str1); i++)
    stack.grow_fast (str1[i]);

  ACE_TCHAR *str = stack.freeze ();

  if (str == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("freeze failed!\n")));
      ++errors;
    }
  else if (ACE_OS::strcmp (str, str1) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Lost first string; expected: %s, have: %s\n"),
                  str1, str));
      ++errors;
    }

  for (size_t j = 0; j < ACE_OS::strlen (str2); ++j)
    stack.grow (str2[j]);

  ACE_TCHAR* temp = stack.freeze();

  if (temp == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("freeze failed!\n")));
      ++errors;
    }
  else if (ACE_OS::strcmp (temp, str2) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Lost second string; expected: %s, have: %s\n"),
                  str2, temp));
    ++errors;
    }

  for (size_t k = 0; k < ACE_OS::strlen (str1); ++k)
    stack.grow (str1[k]);

  ACE_TCHAR* tmp = stack.freeze ();
  if (tmp == 0)
    {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("freeze failed!\n")));
    ++errors;
  }
  else if (ACE_OS::strcmp (tmp, str1) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Lost third string; expected: %s, have: %s\n"),
                str1, tmp));
    ++errors;
  }

  stack.unwind (temp);

  for (size_t l = 0; l < ACE_OS::strlen (str2); ++l)
    stack.grow (str2[l]);

  temp = stack.freeze();

  if (temp == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("freeze failed!\n")));
      ++errors;
    }
  else if (ACE_OS::strcmp (temp, str2) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Lost fourth string; expected: %s, have: %s\n"),
                str2, temp));
    ++errors;
  }

  if (!errors)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Test ok\n")));

  ACE_END_TEST;
  return errors == 0  ?  0  :  1;
}
