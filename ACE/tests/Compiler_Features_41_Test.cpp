/**
 * This program checks if the compiler/RTL does have correct support
 * for structured exception handling using __try/__finally
 */

#include "test_config.h"

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_41_Test"));
  int result = 0;

#if defined (ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS)
  ACE_DEBUG ((LM_DEBUG,("Testing __try/__finally\n")));

  ACE_SEH_TRY
    {
      ACE_DEBUG ((LM_DEBUG,("In SEH_TRY\n")));

      char *cause_exception {};
      char a = *cause_exception;
      // If we get here without an exception, SEH isn't working
      ACE_ERROR ((LM_ERROR, "No exception was raised from null pointer dereference\n"));
      result = -1;
    }
  ACE_SEH_FINALLY
    {
      ACE_DEBUG ((LM_DEBUG,("In SEH_FINALLY\n")));
      // If we're here due to an exception, that's the expected behavior - test passes
    }

  ACE_DEBUG ((LM_DEBUG,("After SEH_FINALLY\n")));

#else
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Platform lacks ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS\n")));
#endif /* ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS */

  ACE_END_TEST;

  return result;
}

