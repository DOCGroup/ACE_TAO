// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Time_Value_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of ACE_Time_Value.  No command line arguments
//      are needed to run the test.  It also tests the ACE_U_LongLong class.
//      Because ACE_U_LongLong is used for ACE_hrtime_t on some platforms,
//      this seems like a reasonable place to test it.
//
// = AUTHOR
//    Prashant Jain and David Levine
//
// ============================================================================

#include "ace/config.h"

// Force test of ACE_U_LongLong class.
#undef ACE_HAS_HI_RES_TIMER
#undef ACE_HAS_LONGLONG_T

#include "ace/ACE.h"
#include "test_config.h"

u_long
test_ace_u_longlong ()
{
  u_long errors = 0;

  ACE_U_LongLong ull1 (0x21,1);
  if (! (ull1.hi () == 1  &&  ull1.lo () == 0x21)) ++errors;
  ACE_ASSERT (ull1.hi () == 1  &&  ull1.lo () == 0x21);

  ACE_U_LongLong ull2 (0x20,2);
  if (! (ull2.hi () == 2  &&  ull2.lo () == 0x20)) ++errors;
  ACE_ASSERT (ull2.hi () == 2  &&  ull2.lo () == 0x20);

  ull2 -= ull1;
  if (! (ull2.hi () == 0  &&  ull2.lo () == 0xfffffffful)) ++errors;
  ACE_ASSERT (ull2.hi () == 0  &&  ull2.lo () == 0xfffffffful);

  ull2 += ull1;
  if (! (ull2.hi () == 2  &&  ull2.lo () == 0x20)) ++errors;
  ACE_ASSERT (ull2.hi () == 2  &&  ull2.lo () == 0x20);

  ACE_U_LongLong ull3 = ull1 + ull1;
  if (! (ull3.hi () == 2  &&  ull3.lo () == 0x42)) ++errors;
  ACE_ASSERT (ull3.hi () == 2  &&  ull3.lo () == 0x42);

  ACE_U_LongLong ull4 = ACE_U_LongLong (0x1111, 0) -
                        ACE_U_LongLong (0x1112, 0);
  if (! (ull4.hi () == 0xfffffffful  &&  ull4.lo () == 0xfffffffful)) ++errors;
  ACE_ASSERT (ull4.hi () == 0xfffffffful  &&  ull4.lo () == 0xfffffffful);

  ACE_U_LongLong ull5 = ACE_U_LongLong (0x1111, 1) -
                        ACE_U_LongLong (0x1112, 0);
  if (! (ull5.hi () == 0  &&  ull5.lo () == 0xfffffffful)) ++errors;
  ACE_ASSERT (ull5.hi () == 0  &&  ull5.lo () == 0xfffffffful);

  ACE_U_LongLong ull6 = ull2 + ACE_U_LongLong (0, 1);
  if (! (ull6.hi () == 3  &&  ull6.lo () == 0x20)) ++errors;
  ACE_ASSERT (ull6.hi () == 3  &&  ull6.lo () == 0x20);

  ull6 += ACE_U_LongLong (0xffffffff, 0xfff0);
  if (! (ull6.hi () == 0xfff4  &&  ull6.lo () == 0x1f)) ++errors;
  ACE_ASSERT (ull6.hi () == 0xfff4  &&  ull6.lo () == 0x1f);

  return errors;
}


int
main (int, char *[])
{
  int ret = 0;

  ACE_START_TEST ("Time_Value_Test");

  ACE_Time_Value tv1;
  ACE_Time_Value tv2 (2);
  ACE_Time_Value tv3 (100);
  ACE_Time_Value tv4 (1, 1000000);
  ACE_Time_Value tv5 (2);
  ACE_Time_Value tv6 (1, -1000000);
  ACE_Time_Value tv7 ((long) 2.0);

  ACE_ASSERT (tv1 == ACE_Time_Value (0));
  ACE_ASSERT (tv2 < tv3);
  ACE_ASSERT (tv2 <= tv2);
  ACE_ASSERT (tv2 >= tv4);
  ACE_ASSERT (tv5 >= tv6);
  ACE_ASSERT (tv2 == ACE_Time_Value (1, 1000000));
  ACE_ASSERT (tv5 == tv4);
  ACE_ASSERT (tv2 == tv4);
  ACE_ASSERT (tv1 != tv2);
  ACE_ASSERT (tv6 == tv1);
  ACE_ASSERT (tv5 == tv7);

  if (test_ace_u_longlong () != 0) ++ret;

  ACE_END_TEST;
  return ret;
}
