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
//    David L. Levine
//
// ============================================================================

#include "test_config.h"
#include "ace/Log_Msg.h"
#include "ace/Basic_Types.h"

static
u_int
check (const char *message, u_int i, u_int j)
{
  if (i == j)
    {
      ACE_DEBUG ((LM_DEBUG, message, j, "\n"));
      return 0;
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "assertion failed \"%s\": %u != %u\n",
                  message, i, j));
      return 1;
    }
}


int
main (int, char *[])
{
  ACE_START_TEST ("Basic_Types_Test");

  ACE_DEBUG ((LM_DEBUG, "This is ACE Version %u.%u.%u\n\n",
              ACE::major_version (),
              ACE::minor_version(),
              ACE::beta_version()));

  u_int errors = 0;

  errors += check ("ACE_SIZEOF_CHAR: %u%s",
                   sizeof (char), ACE_SIZEOF_CHAR);
#if defined (VXWORKS) && defined (ghs)
  errors += check ("ACE_SIZEOF_WCHAR: %u%s",
                   sizeof (char), ACE_SIZEOF_WCHAR);
#else
  errors += check ("ACE_SIZEOF_WCHAR: %u%s",
                   sizeof (wchar_t), ACE_SIZEOF_WCHAR);
#endif /* VXWORKS && ghs */
  errors += check ("ACE_SIZEOF_SHORT: %u%s",
                   sizeof (short), ACE_SIZEOF_SHORT);
  errors += check ("ACE_SIZEOF_INT: %u%s",
                   sizeof (int), ACE_SIZEOF_INT);
  errors += check ("ACE_SIZEOF_LONG: %u%s",
                   sizeof (long), ACE_SIZEOF_LONG);
  errors += check ("ACE_SIZEOF_LONG_LONG: %u%s",
#if defined (ACE_WIN32)
                   sizeof (unsigned __int64),
#else /* ! ACE_WIN32 */
                   sizeof (long long),
#endif /* ! ACE_WIN32 */
                   ACE_SIZEOF_LONG_LONG);
  errors += check ("ACE_SIZEOF_VOID_P: %u%s",
                   sizeof (void *), ACE_SIZEOF_VOID_P);
  errors += check ("ACE_SIZEOF_FLOAT: %u%s",
                   sizeof (float), ACE_SIZEOF_FLOAT);
  errors += check ("ACE_SIZEOF_DOUBLE: %u%s",
                   sizeof (double), ACE_SIZEOF_DOUBLE);
  errors += check ("ACE_SIZEOF_LONG_DOUBLE: %u%s",
                   sizeof (long double), ACE_SIZEOF_LONG_DOUBLE);

  errors += check ("sizeof (ACE_INT16) is %u%s", sizeof (ACE_INT16), 2);
  errors += check ("sizeof (ACE_UINT16) is %u%s", sizeof (ACE_INT16), 2);
  errors += check ("sizeof (ACE_INT32) is %u%s", sizeof (ACE_INT32), 4);
  errors += check ("sizeof (ACE_UINT32) is %u%s", sizeof (ACE_INT32), 4);
  errors += check ("sizeof (ACE_UINT64) is %u%s", sizeof (ACE_UINT64), 8);

#if defined (ACE_LITTLE_ENDIAN)
  ACE_DEBUG ((LM_DEBUG, "\nlittle endian\n"));
#elif defined (ACE_BIG_ENDIAN)
  ACE_DEBUG ((LM_DEBUG, "\nbig endian\n"));
#else
  ACE_ERROR ((LM_ERROR, "assertion failed: no ACE_*_ENDIAN definition!\n"));
#endif /* ACE_LITTLE_ENDIAN */

  ACE_DEBUG ((LM_DEBUG, "OS page size: %u\n", ACE_OS::getpagesize ()));

#if defined (_SC_PAGESIZE)
  ACE_DEBUG ((LM_DEBUG, "sysconf page size: %d\n",
              (int) sysconf (_SC_PAGESIZE)));
#endif /* _SC_PAGESIZE */
#if defined (_SC_CLK_TCK)
  ACE_DEBUG ((LM_DEBUG, "clock ticks/sec = %d\n",
              (int) sysconf (_SC_CLK_TCK)));
#endif /* _SC_CLK_TCK */

  ACE_END_TEST;
  return errors == 0  ?  0  :  1;
}
