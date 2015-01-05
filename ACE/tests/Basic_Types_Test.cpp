// $Id$
//
// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Basic_Types_Test.cpp
//
// = DESCRIPTION
//    Checks the #defines in ace/Basic_Types.h, and a few other basics.
//
// = AUTHOR
//    David L. Levine <levine@cs.wustl.edu>
//
// ============================================================================

#include "ace/config-all.h"
// Don't use the ACE version accessors in class ACE, so that we can
// support this test cleanly with the OS component, only.
#include "ace/Version.h"

#if defined (ACE_HAS_MINIMAL_ACE_OS)
  // Redefine these macros to allow the test to print out useful info.
# undef ACE_DEBUG
# define ACE_DEBUG(x) ACE_OS::fprintf x
# define LM_DEBUG stdout
# undef ACE_ERROR
# define ACE_ERROR(x) ACE_OS::fprintf x
# define LM_ERROR stderr
# define ACE_START_TEST(x) ACE_OS::printf (x ACE_TEXT ("\n"))
# define ACE_END_TEST
#else  /* ! ACE_HAS_MINIMAL_ACE_OS */
# include "test_config.h"
#endif /* ! ACE_HAS_MINIMAL_ACE_OS */

#include "ace/Basic_Types.h"
#include "ace/OS_NS_unistd.h"

typedef void* (*a_function_pointer) (void*);

static
u_int
check (const ACE_TCHAR *message, u_int i, u_int j)
{
  if (i == j)
    {
      ACE_DEBUG ((LM_DEBUG, message, j, ACE_TEXT ("\n")));
      return 0;
    }
  else
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("assertion failed \"%s\": %u != %u\n"),
                  message, i, j));
      return 1;
    }
}

// If the platform lacks an unsigned long long, define one.
#if defined (ACE_LACKS_LONGLONG_T) || defined (ACE_LACKS_UNSIGNEDLONGLONG_T)
static
u_int
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
u_int
test_ace_u_longlong (void)
{
  u_int errors = 0;

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

#else
static u_int
test_ace_u_longlong (void)
{
  return 0;    /* Platform has native types; no need to test ACE_U_LongLong */
}
#endif /* ACE_LACKS_LONGLONG_T || ACE_LACKS_UNSIGNEDLONGLONG_T */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Basic_Types_Test"));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE_MAJOR_VERSION,
              ACE_MINOR_VERSION,
              ACE_BETA_VERSION));

  u_int errors = 0;

  errors += check (ACE_TEXT ("ACE_SIZEOF_CHAR: %u%s"),
                   sizeof (char), ACE_SIZEOF_CHAR);
#if defined (ACE_HAS_WCHAR)
  errors += check (ACE_TEXT ("ACE_SIZEOF_WCHAR: %u%s"),
                   sizeof (wchar_t), ACE_SIZEOF_WCHAR);
#endif /* ACE_HAS_WCHAR */
  errors += check (ACE_TEXT ("ACE_SIZEOF_SHORT: %u%s"),
                   sizeof (short), ACE_SIZEOF_SHORT);
  errors += check (ACE_TEXT ("ACE_SIZEOF_INT: %u%s"),
                   sizeof (int), ACE_SIZEOF_INT);
  errors += check (ACE_TEXT ("ACE_SIZEOF_LONG: %u%s"),
                   sizeof (long), ACE_SIZEOF_LONG);
  errors += check (ACE_TEXT ("ACE_SIZEOF_LONG_LONG: %u%s"),
#if defined (ACE_LACKS_LONGLONG_T)
                   sizeof (ACE_U_LongLong),
#elif defined (ACE_WIN32)
                   sizeof (unsigned __int64),
#else  /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
                   sizeof (long long),
#endif /* ! ACE_WIN32 && ! ACE_LACKS_LONGLONG_T */
                   ACE_SIZEOF_LONG_LONG);
  errors += check (ACE_TEXT ("sizeof 64-bit literal: %u%s"),
                   sizeof ACE_UINT64_LITERAL (1),
                   8);
  errors += check (ACE_TEXT ("ACE_SIZEOF_VOID_P: %u%s"),
                   sizeof (void *), ACE_SIZEOF_VOID_P);
  errors += check (ACE_TEXT ("ACE_SIZEOF_FLOAT: %u%s"),
                   sizeof (float), ACE_SIZEOF_FLOAT);
  errors += check (ACE_TEXT ("ACE_SIZEOF_DOUBLE: %u%s"),
                   sizeof (double), ACE_SIZEOF_DOUBLE);
  errors += check (ACE_TEXT ("ACE_SIZEOF_LONG_DOUBLE: %u%s"),
                   sizeof (long double), ACE_SIZEOF_LONG_DOUBLE);

  errors += check (ACE_TEXT ("sizeof (ACE_INT16) is %u%s"),
                   sizeof (ACE_INT16), 2);
  errors += check (ACE_TEXT ("sizeof (ACE_UINT16) is %u%s"),
                   sizeof (ACE_INT16), 2);

  errors += check (ACE_TEXT ("sizeof (ACE_INT32) is %u%s"),
                   sizeof (ACE_INT32), 4);
  errors += check (ACE_TEXT ("sizeof (ACE_UINT32) is %u%s"),
                   sizeof (ACE_INT32), 4);
  errors += check (ACE_TEXT ("sizeof (ACE_UINT64) is %u%s"),
                   sizeof (ACE_UINT64), 8);

  errors += test_ace_u_longlong ();

  // ACE assumes sizeof (ptrdiff_t) == sizeof (void*)
  if (sizeof (ptrdiff_t) == sizeof (void *))
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("sizeof (ptrdiff_t) == sizeof (void*)\n")));
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("sizeof (ptrdiff_t) != sizeof (void*)\n")));
      ++errors;
    }

  // ACE assumes sizeof (ptrdiff_t) >= sizeof (a_function_pointer)
  if (sizeof (ptrdiff_t) >= sizeof (a_function_pointer))
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("sizeof (ptrdiff_t) >= sizeof (a_function_pointer)\n")));
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("sizeof (ptrdiff_t) < sizeof (a_function_pointer)\n")));
      ++errors;
    }

  ACE_UINT16 test_val = 123;
#if defined (ACE_LITTLE_ENDIAN)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("little endian\n")));
  if (ACE_HTONS (test_val) == test_val)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Endian test: %d == %d but should be different\n"),
                ACE_HTONS (test_val), test_val));
#elif defined (ACE_BIG_ENDIAN)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("big endian\n")));
  if (ACE_HTONS (test_val) != test_val)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Endian test: %d != %d but should be equal\n"),
                ACE_HTONS (test_val), test_val));
#else
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("assertion failed: no ACE_*_ENDIAN definition!\n")));
  ++errors;
#endif /* ACE_LITTLE_ENDIAN */

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OS page size: %u\n"),
              ACE_OS::getpagesize ()));

#if defined (_SC_PAGESIZE)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("sysconf page size: %d\n"),
              (int) ACE_OS::sysconf (_SC_PAGESIZE)));
#endif /* _SC_PAGESIZE */
#if defined (_SC_CLK_TCK)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("clock ticks/sec = %d\n"),
              (int) ACE_OS::sysconf (_SC_CLK_TCK)));
#endif /* _SC_CLK_TCK */


  ACE_END_TEST;
  return errors == 0  ?  0  :  1;
}
