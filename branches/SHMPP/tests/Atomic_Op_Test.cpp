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
//      This is a simple test of the Atomic Operations Class in ACE.
//      On platforms like Win32, ACE uses template specialization to
//      use native implementations provided by the OS to accelarate
//      these operations.
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#include "tests/test_config.h"
#include "ace/Synch.h"

ACE_RCSID(tests, Atomic_Op_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Atomic_Op_Test"));

  ACE_Atomic_Op <ACE_Thread_Mutex, long> foo (5);

  ACE_ASSERT (foo == 5);

  ++foo;
  ACE_ASSERT (foo == 6);

  --foo;
  ACE_ASSERT (foo == 5);

  foo += 10;
  ACE_ASSERT (foo == 15);

  foo -= 10;
  ACE_ASSERT (foo == 5);

  foo = 5L;
  ACE_ASSERT (foo == 5);

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, long>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, long>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Atomic_Op_Test"));

  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS */
