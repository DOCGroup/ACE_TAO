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
#include "wrapper/CLASSIX_SAP.h"

class testSap : public ACE_CLASSIX_SAP
{
public:
    testSap(): ACE_CLASSIX_SAP() {}
    testSap(const ACE_Addr& theAddr) : ACE_CLASSIX_SAP(theAddr) {}

    ~testSap() {ACE_DEBUG((LM_DEBUG, "~testSap()\n"));}
};

int
main (int, char *[])
{
  ACE_START_TEST ("SOCK_Test");

  /* ================================================================== */
  ACE_DEBUG((LM_INFO, "... test virtual destructor...\n"));
  testSap *n1 = new testSap();
  ACE_CLASSIX_SAP *n2 = n1;
  delete n2;

  /* ================================================================== */
  ACE_DEBUG((LM_INFO, "... ACE_CLASSIX_Addr...\n"));
  ACE_CLASSIX_Addr a1;
  a1.set();
  ACE_DEBUG((LM_INFO, "Addr a1:"));
  a1.dump();

  KnUniqueId k2;
  ::portCreate(K_MYACTOR, &k2);
  int p2 = ::portLi(&k2);
  ACE_CLASSIX_Addr a2(k2);
  ACE_DEBUG((LM_INFO, "Addr a2(ipc port id = %d)\n", p2));
  a2.dump();

  KnUniqueId k3;
  ::portCreate(K_MYACTOR, &k3);
  int p3 = ::portLi(&k3);
  ACE_CLASSIX_Addr a3(p3);
  ACE_DEBUG((LM_INFO, "Addr a3(ipc port id = %d)\n", p3));
  a3.dump();

  ACE_CLASSIX_Addr a4(a1);
  if (a4 == a1)
      ACE_DEBUG((LM_INFO, "OK: Addr a4 == a1\n"));
  else
      ACE_ERROR((LM_ERROR, "Error: Addr a4 != a1\n"));

  if (a4 != a2)
      ACE_DEBUG((LM_INFO, "OK: Addr a4 != a2\n"));
  else
      ACE_ERROR((LM_ERROR, "Error: Addr a4 == a2\n"));

  /* ================================================================== */

  ACE_DEBUG((LM_INFO, "\n... ACE_CLASSIX...\n"));
  testSap sap1;
  sap1.set_local_addr(a1);
  ACE_CLASSIX_Addr b1; 
  sap1.get_local_addr(b1);
  if (b1 != a1)
      ACE_ERROR((LM_ERROR, "Error: sap address"));
  sap1.dump();


  testSap sap2(a2);
  ACE_CLASSIX_Addr b2; 
  sap2.get_local_addr(b2);
  if (b2 != a2)
      ACE_ERROR((LM_ERROR, "Error: sap address"));
  sap2.dump();


  ACE_END_TEST;
  return 0;
}

