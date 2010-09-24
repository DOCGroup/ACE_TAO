// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Singleton_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of ACE Singleton
//
// = AUTHOR
//    Johnny Willemsen  <jwillemsen@remedy.nl>
//
// ============================================================================

#include "test_config.h"
#include "ace/Singleton.h"

ACE_RCSID(tests, Singleton_Test, "$Id$")

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Singleton_Test"));

  int *i = ACE_Singleton <int, ACE_SYNCH_RECURSIVE_MUTEX>::instance ();
  *i = 3;
  ACE_Singleton <int, ACE_SYNCH_RECURSIVE_MUTEX>::close ();

  ACE_END_TEST;

  return 0;
}

