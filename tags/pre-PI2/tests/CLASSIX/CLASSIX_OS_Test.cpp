/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    CLASSIX_OS_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <ACE_CLASSIX_OS> class.
//
// = AUTHOR
//    Wei Chiang
// 
// ============================================================================

#include "tests/test_config.h"
#include "ace/CLASSIX/CLASSIX_OS.h"


int
main (int, char *[])
{
  ACE_START_TEST ("CLASSIX_OS_Test");

  ACE_DEBUG((LM_INFO, "Empty Message\n"));
  ACE_CLASSIX_Msg msg1;
  msg1.dump();

  char buf[] ="This is a test message";
  int size = sizeof (buf);
  ACE_DEBUG((LM_INFO, "%s(size = %d)\n", buf, size));
  ACE_CLASSIX_Msg msg2(buf, size);
  msg2.dump();


  ACE_END_TEST;
  return 0;
}

