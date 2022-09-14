/**
 * This program checks if the compiler/RTL does have correct support
 * for structured exception handling
 */

#include "test_config.h"

#if defined (ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS)
int ExFilter(EXCEPTION_POINTERS *ep, DWORD code_arg)
{
  ACE_DEBUG ((LM_INFO,("In SEH Filter\n")));
  ACE_DEBUG ((LM_INFO,("Code param=%d\n"), code_arg));
  ACE_DEBUG ((LM_INFO,("\tep->ExceptionRecord->ExceptionCode =%d\n"), ep->ExceptionRecord->ExceptionCode));
  ACE_DEBUG ((LM_INFO,("\tep->ExceptionRecord->ExceptionAddress =%@\n"), ep->ExceptionRecord->ExceptionAddress));
  if (ep->ExceptionRecord->NumberParameters >= 1)
    ACE_DEBUG ((LM_INFO,("\tep->ExceptionRecord->ExceptionInformation[0]=%@\n"), ep->ExceptionRecord->ExceptionInformation[0]));
  if (ep->ExceptionRecord->NumberParameters == 2)
    ACE_DEBUG ((LM_INFO,("\tep->ExceptionRecord->ExceptionInformation[1]=%@\n"), ep->ExceptionRecord->ExceptionInformation[1]));
  return 1;
}

void test()
{
  volatile int* pInt = 0x0000000;
  *pInt = 20;
}

#endif /* ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_39_Test"));

#if defined (ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS)
  ACE_DEBUG ((LM_DEBUG,("Testing call SEH\n")));

  ACE_SEH_TRY
    {
      test();
    }
  ACE_SEH_EXCEPT (ExFilter(GetExceptionInformation(), GetExceptionCode()))
    {
      ACE_DEBUG ((LM_DEBUG,("In SEH call\n")));
    }

  ACE_DEBUG ((LM_DEBUG,("SEH call worked\n")));

  ACE_DEBUG ((LM_DEBUG,("Testing non-call SEH\n")));

  ACE_SEH_TRY
    {
      volatile int* pInt = 0x0000000;
      *pInt = 20;
    }
  ACE_SEH_EXCEPT (ExFilter(GetExceptionInformation(), GetExceptionCode()))
    {
      ACE_DEBUG ((LM_DEBUG,("In SEH non-call\n")));
    }

  ACE_DEBUG ((LM_DEBUG,("SEH non-call worked\n")));
#else
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Platform lacks ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS\n")));
#endif /* ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS */

  ACE_END_TEST;

  return 0;
}

