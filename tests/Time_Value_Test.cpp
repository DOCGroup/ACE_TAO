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

// Note, for this test the config.h file *must* come first!
#include "ace/config.h"

// Force test of ACE_U_LongLong class on all platforms except
// ACE_WIN32 and with ACE_HAS_64BIT_LONGS.  ACE_U_LongLong isn't used
// on those platforms.  (And the ACE_U_LongLong declaration is in an
// inconvenient place on ace/OS.h to play this game on ACE_WIN32.)
#if defined (ACE_HAS_HI_RES_TIMER) || \
   (defined (ACE_HAS_LONGLONG_T) && !defined (ACE_HAS_64BIT_LONGS))
# if defined (ACE_HAS_HI_RES_TIMER)
#   undef ACE_HAS_HI_RES_TIMER
# endif /* ACE_HAS_HI_RES_TIMER */

# if defined (ACE_HAS_LONGLONG_T)
#   undef ACE_HAS_LONGLONG_T
# endif /* ACE_HAS_LONGLONG_T */

  // Force inlining, in case ACE_U_LongLong member function
  // definitions are not in libACE.
# define __ACE_INLINE__
# if defined (ACE_NO_INLINE)
#   undef ACE_NO_INLINE
# endif /* ACE_NO_INLINE */
#endif /* ACE_HAS_HI_RES_TIMER || (ACE_HAS_LONGLONG_T && !ACE_HAS_64BIT_LONGS) */

#include "test_config.h"
#include "ace/ACE.h"

#if !defined (ACE_WIN32)  &&  !defined (ACE_HAS_64BIT_LONGS)

static
u_long
check_ace_u_longlong (const char *const name,
                      const ACE_U_LongLong ull,
                      const u_long hi,
                      const u_long lo)
{
  if (ull.hi () == hi  &&  ull.lo () == lo)
    {
      return 0;
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "%s; hi: %x, should be %x; "
                            "lo: %x, should be %x.\n",
                  name, ull.hi (), hi, ull.lo (), lo));
      return 1;
    }
}

static
u_long
test_ace_u_longlong ()
{
  u_long errors = 0;

  ACE_U_LongLong ull1 (0x21,1);
  errors += check_ace_u_longlong ("ull1", ull1, 1, 0x21);

  ACE_U_LongLong ull2 (0x20,2);
  errors += check_ace_u_longlong ("ull2", ull2, 2, 0x20);

  ull2 -= ull1;
  errors += check_ace_u_longlong ("ull2", ull2, 0, 0xfffffffful);

  ull2 += ull1;
  errors += check_ace_u_longlong ("ull2", ull2, 2, 0x20);

  ACE_U_LongLong ull3 = ull1 + ull1;
  errors += check_ace_u_longlong ("ull3", ull3, 2, 0x42);

  ACE_U_LongLong ull4 = ACE_U_LongLong (0x1111, 0) -
                        ACE_U_LongLong (0x1112, 0);
  errors += check_ace_u_longlong ("ull4", ull4, 0xfffffffful, 0xfffffffful);

  ACE_U_LongLong ull5 = ACE_U_LongLong (0x1111, 1) -
                        ACE_U_LongLong (0x1112, 0);
  errors += check_ace_u_longlong ("ull5", ull5, 0, 0xfffffffful);

  ACE_U_LongLong ull6 = ull2 + ACE_U_LongLong (0, 1);
  errors += check_ace_u_longlong ("ull6", ull6, 3, 0x20);

  ull6 += ACE_U_LongLong (0xffffffff, 0xfff0);
  errors += check_ace_u_longlong ("ull6", ull6, 0xfff4, 0x1f);

  return errors;
}
#endif /* ! ACE_WIN32 && ! ACE_HAS_64BIT_LONGS */


int
main (int, char *[])
{
  int ret = 0;

  ACE_START_TEST ("Time_Value_Test");

  ACE_DEBUG ((LM_DEBUG, "This is ACE Version %u.%u.%u\n",
              ACE::major_version (),
              ACE::minor_version(),
              ACE::beta_version()));

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

#if !defined (ACE_WIN32)  &&  !defined (ACE_HAS_64BIT_LONGS)
  if (test_ace_u_longlong () != 0) ++ret;
#endif /* ! ACE_WIN32 && ! ACE_HAS_64BIT_LONGS */

  ACE_END_TEST;
  return ret;
}
