/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    CLASSIX_Addr_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <IPP_CLASSIX_Addr> class.
//
// = AUTHOR
//    Wei Chiang
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_Addr.h"
#include "ace/CLASSIX/CLASSIX_SAP.h"
#include "ace/CLASSIX/CLASSIX_Port_Default.h"

#include "CLASSIX_test_config.h"

class testSap : public ACE_CLASSIX_SAP
{
public:
    testSap(): ACE_CLASSIX_SAP() {}
    testSap(const ACE_Addr& theAddr) : ACE_CLASSIX_SAP(theAddr) {}

    ~testSap() {}
};

int
main (int, char *[])
{
  ACE_START_TEST ("Addr_Test");

  /* ================================================================== */
  ACE_DEBUG((LM_INFO, "------test virtual destructor------\n"));
  testSap *n1 = new testSap();
  ACE_CLASSIX_SAP *n2 = n1;
  delete n2;

  /* ================================================================== */
  ACE_DEBUG((LM_INFO, "--------ACE_CLASSIX_Addr----------\n"));
  ACE_DEBUG((LM_DEBUG, "\n--------------------------------\n"));
  ACE_CLASSIX_Port_Core *k2 = new ACE_CLASSIX_Port_Core();
  int p2 = k2->get_handle();
  ACE_CLASSIX_Port a2(*k2);
  ACE_DEBUG((LM_INFO, "Addr a2(ipc port id = %d)\n", p2));
  a2.dump();

  ACE_CLASSIX_Port *a1 = ACE_CLASSIX_DEFAULT_PORT::instance();
  ACE_DEBUG((LM_INFO, "Default port:"));
  a1->dump();
  a1->set_addr(a2.get_addr(), a2.get_size());
  if (*a1 != *ACE_CLASSIX_DEFAULT_PORT::instance())
      ACE_DEBUG((LM_ERROR, "???? Default port cannot be changed-2 !!!"));

  ACE_DEBUG((LM_DEBUG, "\n--------------------------------\n"));
  ACE_CLASSIX_Port_Core k3;
  ACE_CLASSIX_Port a3(k3);
  ACE_DEBUG((LM_INFO, "Addr a3(ipc port id = %d)\n", k3.get_handle));
  a3.dump();

  ACE_DEBUG((LM_DEBUG, "\n--------------------------------\n"));
  ACE_CLASSIX_Port a4(a1->get_addr(), a1->get_size());
  a4.dump();
  if (a4 == *a1)
      ACE_DEBUG((LM_INFO, "OK: Addr a4 == a1\n"));
  else
      ACE_ERROR((LM_ERROR, "Error: Addr a4 != a1\n"));

  if (a4 != a2)
      ACE_DEBUG((LM_INFO, "OK: Addr a4 != a2\n"));
  else
      ACE_ERROR((LM_ERROR, "Error: Addr a4 == a2\n"));

  /* ================================================================== */

  ACE_DEBUG((LM_INFO, "\n--------ACE_CLASSIX_Sap----------\n"));
  testSap sap1;
  sap1.set_addr(*a1);
  ACE_CLASSIX_Port b1; 
  if (sap1.get_addr(b1) < 0)
      ACE_ERROR((LM_ERROR, "failed to get sap1 address"));
  if (b1 != *a1)
      ACE_ERROR((LM_ERROR, "Error: sap1 address"));
  sap1.dump();


  ACE_DEBUG((LM_DEBUG, "\n--------------------------------\n"));
  testSap sap2(a2);
  ACE_CLASSIX_Port b2; 
  if (sap2.get_addr(b2) < 0)
      ACE_ERROR((LM_ERROR, "failed to get sap1 address"));
  if (b2 != a2)
      ACE_ERROR((LM_ERROR, "Error: sap2 address"));
  b2.dump();
  sap2.dump();


  /* ================================================================== */
  int result = 0;
  int p = k2->get_handle();
  delete k2;
  if ((result = portDelete(K_MYACTOR, p)) == 0)
      ACE_ERROR((LM_ERROR, 
		 "????Error: delete already deleted port"));

  ACE_END_TEST;
  return 0;
}

