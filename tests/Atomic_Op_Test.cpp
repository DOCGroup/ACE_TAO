// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Atomic_Op_Test.cpp
//
// = DESCRIPTION
//
//      This is a simple test of the Atomic Operations Class in ACE.
//      On platforms like Win32, ACE uses template specialization to
//      use native implementations provided by the OS to accelarate
//      these operations.
//
// = AUTHOR
//    Irfan Pyarali 
//
// ============================================================================

#include "ace/Synch.h"
#include "tests/test_config.h"

int
main ()
{
  ACE_START_TEST ("Atomic_Op_Test");

  ACE_Atomic_Op <ACE_Thread_Mutex, long> foo (5L);
  ACE_ASSERT (foo == 5L);

  ++foo;
  ACE_ASSERT (foo == 6L);

  --foo;
  ACE_ASSERT (foo == 5L);

  foo += 10;
  ACE_ASSERT (foo == 15L);

  foo -= 10;
  ACE_ASSERT (foo == 5L);

  foo = 5L;
  ACE_ASSERT (foo == 5L);

  ACE_END_TEST;
  return 0;
}
