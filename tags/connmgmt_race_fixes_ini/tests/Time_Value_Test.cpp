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
//    Prashant Jain <pjain@cs.wustl.edu> and David Levine <levine@cs.wustl.edu>
//
// ============================================================================

// Note, for this test the config.h file *must* come first!
#include "ace/config-all.h"

ACE_RCSID(tests, Time_Value_Test, "$Id$")

// Force test of ACE_U_LongLong class on Solaris.
#if defined (sun) && !defined (ACE_LACKS_LONGLONG_T)
# include <limits.h>
# undef ULLONG_MAX

# if defined (ACE_HAS_HI_RES_TIMER)
#   undef ACE_HAS_HI_RES_TIMER
# endif /* ACE_HAS_HI_RES_TIMER */

# define ACE_LACKS_LONGLONG_T

  // Force inlining, in case ACE_U_LongLong member function
  // definitions are not in libACE.
# if !defined (__ACE_INLINE__)
#   define __ACE_INLINE__
# endif /* ! __ACE_INLINE__ */
# if defined (ACE_NO_INLINE)
#   undef ACE_NO_INLINE
# endif /* ACE_NO_INLINE */
#endif /* sun && ! ACE_LACKS_LONGLONG_T */

#include "test_config.h"
#include "ace/ACE.h"

#if defined (sun) && !defined (ACE_LACKS_LONGLONG_T)
static
u_long
check_ace_u_longlong (const ACE_TCHAR *const name,
                      const ACE_U_LongLong ull,
                      const u_long hi,
                      const u_long lo)
{
  if (ull.hi () == hi  &&  ull.lo () == lo)
    return 0;
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%s; hi: %x, should be %x; ")
                       ACE_TEXT ("lo: %x, should be %x.\n"),
                       name, ull.hi (), hi, ull.lo (), lo),
                      1);
}

static
u_long
test_ace_u_longlong (void)
{
  u_long errors = 0;

  ACE_U_LongLong ull1 (0x21,1);
  errors += check_ace_u_longlong (ACE_TEXT ("ull1"), ull1, 1, 0x21);

  ACE_U_LongLong ull2 (0x20,2);
  errors += check_ace_u_longlong (ACE_TEXT ("ull2"), ull2, 2, 0x20);

  ull2 -= ull1;
  errors += check_ace_u_longlong (ACE_TEXT ("ull2"), ull2, 0, 0xfffffffful);

  ull2 += ull1;
  errors += check_ace_u_longlong (ACE_TEXT ("ull2"), ull2, 2, 0x20);

  ACE_U_LongLong ull3 = ull1 + ull1;
  errors += check_ace_u_longlong (ACE_TEXT ("ull3"), ull3, 2, 0x42);

  ACE_U_LongLong ull4 = ACE_U_LongLong (0x1111, 0) -
                        ACE_U_LongLong (0x1112, 0);
  errors += check_ace_u_longlong (ACE_TEXT ("ull4"), ull4, 0xfffffffful, 0xfffffffful);

  ACE_U_LongLong ull5 = ACE_U_LongLong (0x1111, 1) -
                        ACE_U_LongLong (0x1112, 0);
  errors += check_ace_u_longlong (ACE_TEXT ("ull5"), ull5, 0, 0xfffffffful);

  ++ull5;
  errors += check_ace_u_longlong (ACE_TEXT ("ull5"), ull5, 1, 0);

  ACE_U_LongLong ull6 = ull2 + ACE_U_LongLong (0, 1);
  errors += check_ace_u_longlong (ACE_TEXT ("ull6"), ull6, 3, 0x20);

  ull6 += ACE_U_LongLong (0xffffffff, 0xfff0);
  errors += check_ace_u_longlong (ACE_TEXT ("ull6"), ull6, 0xfff4, 0x1f);

  ++ull6;
  errors += check_ace_u_longlong (ACE_TEXT ("ull6"), ull6, 0xfff4, 0x20);

  // The hi part of ull6 will be lost in the following, because
  // the quotient has only 32 bits.
  errors += check_ace_u_longlong (ACE_TEXT ("ull6 / 1"),
                                  (ACE_U_LongLong) (ull6 / 1u),
                                  0, 0x20);

  // There's apparently a small loss in precision in
  // ACE_U_LongLong::operator/.  It calculates
  // ull6 / 0xd0000 as 0x13b013b4 instead of 0x13b04ec4.
  errors += check_ace_u_longlong (ACE_TEXT ("ull6 / 0x10000 / 0xd"),
                                  (ACE_U_LongLong) (ull6 / 0x10000u / 0xd),
                                  0, 0x13b04ec4);

  errors += check_ace_u_longlong (ACE_TEXT ("ull6 % 5"),
                                  (ACE_U_LongLong) (ull6 % 5),
                                  0, 1);

  errors += check_ace_u_longlong (ACE_TEXT ("ull6 % 0x20007"),
                                  (ACE_U_LongLong) (ull6 % 0x20007),
                                  0, 0x3f63);

  ACE_U_LongLong ull7 (12);
  ull7 *= 3125;
  errors += check_ace_u_longlong (ACE_TEXT ("12 * 3125"),
                                  ull7,
                                  0, 37500);

  ull7 *= 100;
  errors += check_ace_u_longlong (ACE_TEXT ("37500 * 100"),
                                  ull7,
                                  0, 3750000);

  errors += check_ace_u_longlong (ACE_TEXT ("3750000 << 16"),
                                  ull7 << 16 ,
                                  0x39, 0x38700000);

  errors += check_ace_u_longlong (ACE_TEXT ("3750000 >> 16"),
                                  ull7 >> 16,
                                  0, 0x39);

  ull7 <<= 32;
  errors += check_ace_u_longlong (ACE_TEXT ("3750000 <<= 32"),
                                  ull7,
                                  3750000, 0);

  ull7 >>= 12;
  errors += check_ace_u_longlong (ACE_TEXT ("3750000 <<= 32 >>= 15"),
                                  ull7,
                                  0x393, 0x87000000);

  ACE_U_LongLong ull8 (0x0f0f, 0xf0f0);
  ACE_U_LongLong ull9 (0xf0f0, 0xf0f0);
  ull8 |= ull9;
  errors += check_ace_u_longlong (ACE_TEXT ("ull8 |= ull9"),
                                  ull8,
                                  0xf0f0, 0xffff);

  ull9.lo (0x5678);
  ull9.hi (0x1234);
  ull8 &= ull9;
  errors += check_ace_u_longlong (ACE_TEXT ("ull8 &= 0x12345678"),
                                  ull9,
                                  0x1234, 0x5678);

  return errors;
}
#endif /* sun && ! ACE_LACKS_LONGLONG_T */


int
main (int, ACE_TCHAR *[])
{
  int ret = 0;

  ACE_START_TEST (ACE_TEXT ("Time_Value_Test"));

  ACE_Time_Value tv1;
  ACE_Time_Value tv2 (2);
  ACE_Time_Value tv3 (100U);
  ACE_Time_Value tv4 (1, 1000000);
  ACE_Time_Value tv5 (2UL);
  ACE_Time_Value tv6 (1, -1000000);
  ACE_Time_Value tv7 (ACE_static_cast (long, 2.0));

  // Beware!  2.5 gets truncated to 2!
  // NOTE:  this is intended to show what happens with
  // ACE_Time_Value (2.5).  Some compilers, such as g++ 2.7.2.3,
  // actually warn about it without the case.
  ACE_Time_Value tv8 (ACE_static_cast (long, 2.5));

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
  ACE_ASSERT (tv7 == tv8); // That's right!  See above . . .

#if defined (sun) && !defined (ACE_LACKS_LONGLONG_T)
  if (test_ace_u_longlong () != 0)
    ++ret;
#endif /* sun && ! ACE_LACKS_LONGLONG_T */

  ACE_END_TEST;
  return ret;
}
