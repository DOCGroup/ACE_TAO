/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    CLASSIX_SOCK_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <IPP_CLASSIX_Addr> class.
//
// = AUTHOR
//    Wei Chiang
// 
// ============================================================================

#include "tests/test_config.h"
#include "wrapper/CLASSIX_Addr.h"


int
main (int, char *[])
{
  ACE_START_TEST ("SOCK_Test");

  IPP_CLASSIX_Addr a1;
  ACE_DEBUG((LM_INFO, "Addr a1:"));
  a1.dump();

  KnUniqueId k2;
  ::portCreate(K_MYACTOR, &k2);
  int p2 = ::portLi(&k2);
  IPP_CLASSIX_Addr a2(k2);
  ACE_DEBUG((LM_INFO, "Addr a2(ipc port id = %d)\n", p2));
  a2.dump();

  KnUniqueId k3;
  ::portCreate(K_MYACTOR, &k3);
  int p3 = ::portLi(&k3);
  IPP_CLASSIX_Addr a3(p3);
  ACE_DEBUG((LM_INFO, "Addr a3(ipc port id = %d)\n", p3));
  a3.dump();

  IPP_CLASSIX_Addr a4(a1);
  if (a4 == a1)
      ACE_DEBUG((LM_INFO, "OK: Addr a4 == a1\n"));
  else
      ACE_ERROR((LM_ERROR, "Error: Addr a4 != a1\n"));

  if (a4 != a2)
      ACE_DEBUG((LM_INFO, "OK: Addr a4 != a2\n"));
  else
      ACE_ERROR((LM_ERROR, "Error: Addr a4 == a2\n"));


  ACE_END_TEST;
  return 0;
}

