// $Id$

/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered when testing C++11 features
 */

#include "test_config.h"

#if defined (ACE_HAS_CPP11)

enum class OPERATIONS
{
  ADD_OBJECT_POLICY,
  ADD_CURRENT_POLICY,
  LAST_OPERATION
};

/// Using this to iterate over the OPERATIONS enum
OPERATIONS operator++(OPERATIONS& x) { return x = static_cast<OPERATIONS>(static_cast<uint16_t>(x) + 1); }
OPERATIONS operator*(OPERATIONS c) {return c;}
OPERATIONS begin(OPERATIONS ) {return OPERATIONS::ADD_OBJECT_POLICY;}
OPERATIONS end(OPERATIONS )   {return OPERATIONS::LAST_OPERATION;}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_28_Test"));

  int16_t count = 0;
  for (OPERATIONS op : OPERATIONS ())
  {
    if (op == OPERATIONS::ADD_OBJECT_POLICY && count != 0)
      ACE_ERROR ((LM_ERROR, "Wrong order when using range based for loop\n"));
    if (op == OPERATIONS::ADD_CURRENT_POLICY && count != 1)
      ACE_ERROR ((LM_ERROR, "Wrong order when using range based for loop\n"));
    if (op == OPERATIONS::LAST_OPERATION && count != 2)
      ACE_ERROR ((LM_ERROR, "Wrong order when using range based for loop\n"));
    ++count;
  }

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_28_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("No C++11 support enabled\n")));

  ACE_END_TEST;
  return 0;
}

#endif
