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
#include "ace/OS.h"

ACE_RCSID(tests, Basic_Types_Test, "$Id$")

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
  // This is pointless - since ACE_SIZEOF_WCHAR is defined to sizeof (wchar_t)
  // but oh well...
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

// Crays don't have 16-bit quantities, so don't even test for 16-bit values
#if !defined(_UNICOS)
  errors += check (ACE_TEXT ("sizeof (ACE_INT16) is %u%s"),
                   sizeof (ACE_INT16), 2);
  errors += check (ACE_TEXT ("sizeof (ACE_UINT16) is %u%s"),
                   sizeof (ACE_INT16), 2);
#else /* ! _UNICOS */
  errors += check (ACE_TEXT ("sizeof (ACE_INT16) is %u%s"),
                   sizeof (ACE_INT16), 8);
  errors += check (ACE_TEXT ("sizeof (ACE_UINT16) is %u%s"),
                   sizeof (ACE_INT16), 8);
#endif /* ! _UNICOS */

// MPP Crays do have 32-bit quantities (short), though vector Crays don't
#if !defined(_UNICOS) || defined(_CRAYMPP)
  errors += check (ACE_TEXT ("sizeof (ACE_INT32) is %u%s"),
                   sizeof (ACE_INT32), 4);
  errors += check (ACE_TEXT ("sizeof (ACE_UINT32) is %u%s"),
                   sizeof (ACE_INT32), 4);
#else /* ! _UNICOS */
  errors += check (ACE_TEXT ("sizeof (ACE_INT32) is %u%s"),
                   sizeof (ACE_INT32), 8);
  errors += check (ACE_TEXT ("sizeof (ACE_UINT32) is %u%s"),
                   sizeof (ACE_INT32), 8);
#endif /* ! _UNICOS */
  errors += check (ACE_TEXT ("sizeof (ACE_UINT64) is %u%s"),
                   sizeof (ACE_UINT64), 8);

  // ACE assumes sizeof (ptr_arith_t) == sizeof (void*)
  if (sizeof (ptr_arith_t) == sizeof (void *))
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("sizeof (ptr_arith_t) == sizeof (void*)\n")));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("sizeof (ptr_arith_t) != sizeof (void*)\n")));

  // ACE assumes sizeof (ptr_arith_t) >= sizeof (a_function_pointer)
  if (sizeof (ptr_arith_t) >= sizeof (a_function_pointer))
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("sizeof (ptr_arith_t) >= sizeof (a_function_pointer)\n")));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("sizeof (ptr_arith_t) < sizeof (a_function_pointer)\n")));

#if defined (ACE_LITTLE_ENDIAN)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("little endian\n")));
#elif defined (ACE_BIG_ENDIAN)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("big endian\n")));
#else
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("assertion failed: no ACE_*_ENDIAN definition!\n")));
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
