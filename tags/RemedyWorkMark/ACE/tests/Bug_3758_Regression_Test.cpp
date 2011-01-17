// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This simple test exercises ACE_OS::fileno.
//
// = AUTHOR
//    Olli Savia <ops@iki.fi>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"



#undef THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
#define THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL(X) \
  ((X)                                                          \
   ? static_cast<void>(0)                                       \
   : ACE_VERSIONED_NAMESPACE_NAME::__ace_assert(__FILE__, __LINE__, ACE_TEXT_CHAR_TO_TCHAR (#X)))

// Test ACE_OS::fileno()
int
fileno_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing fileno method\n")));

  int test_status = 0;

  ACE_HANDLE fn;

  fn = ACE_OS::fileno (stdin);
  if (fn == ACE_INVALID_HANDLE)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("fileno(stdin) returned ACE_INVALID_HANDLE.\n")));
      test_status = -1;
    }
  else if (fn != ACE_STDIN)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("stdin test failed.\n")));
      test_status = -1;
    }

  fn = ACE_OS::fileno (stdout);
  if (fn == ACE_INVALID_HANDLE)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("fileno(stdout) returned ACE_INVALID_HANDLE.\n")));
      test_status = -1;
    }
  else
#if defined (ACE_WIN32)
  // Check if stdout is not associated with an output stream.
  // This is not an error.
  if (fn == reinterpret_cast<ACE_HANDLE>(-2))
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("fileno(stdout) returned -2.\n")));
    }
  else
#endif
  if (fn != ACE_STDOUT)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("stdout test failed.\n")));
      test_status = -1;
    }

  fn = ACE_OS::fileno (stderr);
  if (fn == ACE_INVALID_HANDLE)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("fileno(stderr) returned ACE_INVALID_HANDLE.\n")));
      test_status = -1;
    }
  else
#if defined (ACE_WIN32)
  // Check if stderr is not associated with an output stream.
  // This is not an error.
  if (fn == reinterpret_cast<ACE_HANDLE>(-2))
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("fileno(stderr) returned -2.\n")));
    }
  else
#endif
  if (fn != ACE_STDERR)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("stderr test failed.\n")));
      test_status = -1;
    }

  return test_status;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3758_Regression_Test"));

  int status = 0;
  int result;

  if ((result = fileno_test ()) != 0)
    status = -1;

  ACE_END_TEST;
  return status;
}
#undef THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
