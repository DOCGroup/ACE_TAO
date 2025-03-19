/**
 * This program checks if the compiler/RTL does have correct support
 * for structured exception handling using __try/__finally
 */

#include "test_config.h"

#if defined (ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS)
void test()
{
  ACE_DEBUG ((LM_DEBUG,("In test\n")));
  volatile int* pInt = 0x0000000;
  *pInt = 20;
}
#endif /* ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_41_Test"));
  int result = 0;

#if defined (ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS)
  bool finally_executed = false;

  ACE_DEBUG ((LM_DEBUG,("Testing __try/__finally\n")));

  ACE_SEH_TRY
    {
      ACE_DEBUG ((LM_DEBUG,("In SEH_TRY\n")));

      // Intentionally cause an access violation exception, use a helper function
      // as non-call SEH isn't supported with clang based windows compilers
      test();

      // If we get here without an exception, SEH isn't working
      ACE_ERROR ((LM_ERROR, "No exception was raised from null pointer dereference\n"));
      result = -1;
    }
  ACE_SEH_FINALLY
    {
      ACE_DEBUG ((LM_DEBUG,("In SEH_FINALLY\n")));
      finally_executed = true;
      // If we're here due to an exception, that's the expected behavior - test passes
    }

  ACE_DEBUG ((LM_DEBUG,("After SEH_FINALLY\n")));

  // On successful SEH handling:
  // 1. The null pointer dereference should raise an exception
  // 2. The FINALLY block should execute
  // 3. Execution should continue after the SEH blocks
  if (finally_executed)
  {
    ACE_DEBUG ((LM_DEBUG, "SEH test passed - FINALLY block was executed\n"));
  }
  else
  {
    ACE_ERROR ((LM_ERROR, "SEH test failed - FINALLY block was not executed\n"));
    result = -1;
  }
#else
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Platform lacks ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS\n")));
#endif /* ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS */

  ACE_END_TEST;

  return result;
}
