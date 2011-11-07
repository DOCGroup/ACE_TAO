// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This simple test exercises and illustrates use of ACE value-added
//    functions.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "test_config.h"

#include "ace/ACE.h"
#include "ace/OS_NS_stdlib.h"

int
log2_test (void)
{
  u_long values[] = {1, 2, 4, 8, 1048576};
  u_long results[] = {0, 1, 2, 3, 20};
  u_long result = 0;
  int error_count = 0;

  for (size_t i = 0 ; i < sizeof (values) / sizeof (u_long) ; i++)
    {
      result = ACE::log2(values [i]);
      if (result != results [i])
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Log2 error: input %d, output %d, expected %d\n"), values [i], result, results [i]));
          error_count++;
        }
    }

  return error_count;
}

int
ace_debug_test (void)
{
  int test_status = 0;

  // Check if ACE::debug() is by default false when no ACE_DEBUG environment variable
  // is there
  const char* debug = ACE_OS::getenv ("ACE_DEBUG");
  if (debug == 0)
    {
      if (ACE::debug())
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ACE::debug() returns true.\n")));
    }
  else
    {
      if (!ACE::debug())
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ACE::debug() returns false.\n")));
    }

  return test_status;
}

// Test ACE::execname to be sure it finds .exe without regard to case.
int
execname_test (void)
{
  int error_count = 0;

  // This test is only interesting on Win32
#if defined (ACE_WIN32)
  const ACE_TCHAR *newname;
  const ACE_TCHAR *prog1 = ACE_TEXT ("myprog.exe");
  const ACE_TCHAR *prog2 = ACE_TEXT ("myprog.EXE");
  const ACE_TCHAR *prog3 = ACE_TEXT ("myprog");

  newname = ACE::execname (prog1);
  if (newname != prog1)   // Didn't find .exe correctly
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Name %s, not %s\n"), newname, prog1));
      delete [] const_cast<ACE_TCHAR *> (newname);
      ++error_count;
    }

  newname = ACE::execname (prog2);
  if (newname != prog2)   // Didn't find .exe correctly
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Name %s, not %s\n"), newname, prog2));
      delete [] const_cast<ACE_TCHAR *> (newname);
      ++error_count;
    }

  newname = ACE::execname (prog3);
  if (newname == prog3)   // Thought the name didn't need .exe
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Says .exe not needed for %s\n"),
                  newname));
      ++error_count;
    }
  else
    delete [] const_cast<ACE_TCHAR *> (newname);
#endif /* ACE_WIN32 */

  return error_count;
}


int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("ACE_Test"));

  int status = 0;
  int result;

  if ((result = execname_test ()) != 0)
    status = result;

  if ((result = log2_test ()) != 0)
    status = result;

  if ((result = ace_debug_test ()) != 0)
      status = result;

  ACE_END_TEST;
  return status;
}
