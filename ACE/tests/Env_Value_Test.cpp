// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//      This is a simple test to make sure the Env_Value template is
//      working correctly.
//
// = AUTHOR
//    Chris Cleeland <cleeland@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/Process.h"
#include "ace/Env_Value_T.h"

#define TEST_THIS(type, varname, defval, expval) \
do { \
  ACE_Env_Value<type> val (varname, (defval)); \
  if (val != (expval)) { \
    ACE_ERROR ((LM_ERROR, \
               ACE_TEXT ("val %d does not match expected value of %d\n"), \
               (int) (type) val, (int) (expval))); \
  } \
} while (0)

int
run_main (int, ACE_TCHAR* [])
{
#ifdef ACE_LACKS_PUTENV
    ACE_START_TEST (ACE_TEXT ("Env_Value_Test"));
    ACE_END_TEST;
#else
  // Only Win32 can set wide-char environment strings. So, for all
  // others, use char string literals regardless of ACE_USES_WCHAR.
#  if defined (ACE_WIN32)
  ACE_OS::putenv (ACE_TEXT ("TEST_VALUE_POSITIVE=10.2"));
  ACE_OS::putenv (ACE_TEXT ("TEST_VALUE_NEGATIVE=-10.2"));
#  else
  ACE_OS::putenv ("TEST_VALUE_POSITIVE=10.2");
  ACE_OS::putenv ("TEST_VALUE_NEGATIVE=-10.2");
#  endif /* ACE_WIN32 */
    {
    ACE_START_TEST (ACE_TEXT ("Env_Value_Test"));

      TEST_THIS (int, ACE_TEXT ("TEST_VALUE_POSITIVE"), 4, 10);
      TEST_THIS (double, ACE_TEXT ("TEST_VALUE_POSITIVE"), -1.0, 10.2);
      TEST_THIS (long, ACE_TEXT ("TEST_VALUE_POSITIVE"), 0, 10);
      TEST_THIS (unsigned long, ACE_TEXT ("TEST_VALUE_POSITIVE"), 0, 10);
      TEST_THIS (short, ACE_TEXT ("TEST_VALUE_POSITIVE"), 0, 10);
      TEST_THIS (unsigned short, ACE_TEXT ("TEST_VALUE_POSITIVE"), 0, 10);

      TEST_THIS (int, ACE_TEXT ("TEST_VALUE_NEGATIVE"), 4, -10);
      TEST_THIS (double, ACE_TEXT ("TEST_VALUE_NEGATIVE"), -1.0, -10.2);
      TEST_THIS (long, ACE_TEXT ("TEST_VALUE_NEGATIVE"), 0, -10L);
      TEST_THIS (unsigned long, ACE_TEXT ("TEST_VALUE_NEGATIVE"), 0, (unsigned long) -10);
      TEST_THIS (short, ACE_TEXT ("TEST_VALUE_NEGATIVE"), 0, -10);
      TEST_THIS (unsigned short, ACE_TEXT ("TEST_VALUE_NEGATIVE"), 0, (unsigned short) -10);

      const ACE_TCHAR *defstr = ACE_TEXT ("Sarah Cleeland is Two!");
      ACE_Env_Value<const ACE_TCHAR *> sval (ACE_TEXT ("This_Shouldnt_Be_Set_Hopefully"),
                                  defstr);
      if (ACE_OS::strcmp (sval, defstr) != 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Mismatch: %s should be %s\n"),
                    (const ACE_TCHAR *)sval, defstr));
      ACE_END_TEST;
    }
#endif // ACE_LACKS_PUTENV
  return 0;
}

