/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Group_Test.cpp
//
// = DESCRIPTION
//     This is a test of the <IPP_CLASSIX_Group_*> class.
//
// = AUTHOR
//    Wei Chiang
// 
// ============================================================================

#include "ace/CLASSIX/CLASSIX_Addr.h"
#include "ace/CLASSIX/CLASSIX_Group_Stamp.h"
#include "ace/CLASSIX/CLASSIX_Group_Dynamic.h"

#include "tests/test_config.h"

int
main (int, char *[])
{
  ACE_START_TEST ("Group_Test");

  int stamp = 20;
  /* ================================================================== */
  ACE_DEBUG((LM_INFO, "-----------group target-----------\n"));

  ACE_DEBUG((LM_INFO, "constrctor(Stamp)..................\n"));
  ACE_CLASSIX_Group_Stamp t0(100);
  t0.dump();

  /* ================================================================== */
  ACE_DEBUG((LM_INFO, "-----------group target-----------\n"));

  ACE_DEBUG((LM_INFO, "constrctor(Stamp)..................\n"));
  ACE_CLASSIX_Group_Stamp t1(stamp);
  t1.dump();

  /* ================================================================== */
  ACE_DEBUG((LM_INFO, "empty constrctor(Stamp) & set_group()\n"));
  ACE_CLASSIX_Group_Stamp t2;
  if (t2 == t1)
      ACE_DEBUG((LM_ERROR, "t2 == t1, should be !=\n"));
  t2.set_addr(&stamp);
  t2.dump();
  if (t2 != t1)
      ACE_DEBUG((LM_ERROR, "????t2 != t1, should be ==\n"));
      

  /* ================================================================== */
  ACE_DEBUG((LM_INFO, "Constrctor(Dynamic) & set_group()\n"));
  ACE_CLASSIX_Group_Dynamic t3;
  t3.dump();

  ACE_CLASSIX_Group_Stamp t4;
  t4.set_addr(t2.get_addr(), t2.get_size()); 
  t4.dump();
  if (t4 == t2)
      ACE_DEBUG((LM_ERROR, "????t4 == t2, should be !=\n"));
  t4.set_addr(&stamp); 
  t4.dump();
  if (t4 != t2)
      ACE_DEBUG((LM_ERROR, "????t4 != t2, should be ==\n"));

  ACE_CLASSIX_Group t5(t3.get_addr(), t3.get_size());
  t5.dump();
  if (t5 != t3)
      ACE_DEBUG((LM_ERROR, "????t5 != t3, should be ==\n"));
      

  if (t2 != t1)
      ACE_DEBUG((LM_ERROR, "????t2 != t1, should be ==\n"));
      
  /* ================================================================== */
  ACE_END_TEST;
  return 0;
}

