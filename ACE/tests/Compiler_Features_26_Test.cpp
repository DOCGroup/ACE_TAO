// $Id$

/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

#include <memory>

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_26_Test"));

  int retval = 0;
  std::shared_ptr<int> a,b,c,d;

  a = std::make_shared<int> (10);
  b = std::make_shared<int> (10);
  c = b;

  if (!(a!=b) || (a==b))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Problem using a!=b\n")));
      ++retval;
    }
  if (!(b==c) || (b!=c))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Problem using b==c\n")));
      ++retval;
    }
  if ((c==d) || !(d!=c))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Problem using b==c\n")));
      ++retval;
    }
  if ((a==nullptr) || !(a!=nullptr))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Problem using a==nullptr\n")));
      ++retval;
    }
  if ((b==nullptr) || !(b!=nullptr))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Problem using b==nullptr\n")));
      ++retval;
    }
  if ((c==nullptr) || !(c!=nullptr))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Problem using c==nullptr\n")));
      ++retval;
    }
  if ((d!=nullptr) || !(d==nullptr))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Problem using d!=nullptr\n")));
      ++retval;
    }

  if (retval == 0)
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Compiler Feature 26 Test does compile and run.\n")));
    }

  ACE_END_TEST;

  return retval;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_25_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
