
//=============================================================================
/**
 *  @file    New_Fail_Test.cpp
 *
 *  $Id$
 *
 *  Checks to be sure that a failed ACE_NEW[_RETURN | _NORETURN] doesn't end
 *  up throwing an exception up to the caller.
 *
 *  Note that this test doesn't get a real attempt on platforms which:
 *    1. Are known to throw exceptions when 'new' runs out of resources,
 *    2. Are built with exceptions disabled.
 *  In these cases, the test puts a message in the log noting that a failed
 *  new will throw an exception, and trust that the user accepts that risk.
 *
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Log_Msg.h"
#include "ace/OS_Memory.h"
#include "ace/CORBA_macros.h"



#if (!defined (__SUNPRO_CC) && !defined (__GNUG__)) || \
  defined (ACE_HAS_EXCEPTIONS)

#include "ace/Numeric_Limits.h"

// This test allocates all of the heap memory, forcing 'new' to fail
// because of a lack of memory.  The ACE_NEW macros should prevent an
// exception from being thrown past the ACE_NEW.  If this test doesn't
// wipe out on an alloc exception, it passes.
//
// If it doesn't ever fail an allocation, there's a warning that something is
// wrong.  The allocated memory is always freed to avoid masking a leak
// somewhere else in the test.

// Most we can do, by half. Using max alone gets "invalid allocation size"
// messages on stdout on Windows.
static const size_t BIG_BLOCK = ACE_Numeric_Limits<size_t>::max () / 2;

// Shouldn't take many "as much as possible" tries to get a failure.
static const int MAX_ALLOCS_IN_TEST = 4;

static void
try_ace_new (char **p)
{
  ACE_NEW (*p, char[BIG_BLOCK]);
  return;
}

static char *
try_ace_new_return (void)
{
  char *p = 0;
  ACE_NEW_RETURN (p, char[BIG_BLOCK], 0);
  return p;
}

static char *
try_ace_new_noreturn (void)
{
  char *p = 0;
  ACE_NEW_NORETURN (p, char[BIG_BLOCK]);
  return p;
}
#endif /* (!__SUNPRO_CC && !__GNUG__) || ACE_HAS_EXCEPTIONS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("New_Fail_Test"));
  int status = 0;

  // Some platforms are known to throw an exception on a failed 'new',
  // but are customarily built without exception support to improve
  // performance.  These platforms are noted, and the test passes.
  // For new ports, it is wise to let this test run.  Depending on
  // intended conditions, exceptions can be disabled when the port is
  // complete.
#if (defined (__SUNPRO_CC) || defined (__GNUG__)) && \
  !defined (ACE_HAS_EXCEPTIONS)
    ACE_DEBUG ((LM_NOTICE, ACE_TEXT ("Out-of-memory will throw an unhandled exception\n")));
  ACE_DEBUG ((LM_NOTICE, ACE_TEXT ("Rebuild with exceptions=1 to prevent this, but it may impair performance.\n")));

#else

  char *blocks[MAX_ALLOCS_IN_TEST];
  int i;

#  if defined (ACE_HAS_EXCEPTIONS)
  try
    {
#  endif /* ACE_HAS_EXCEPTIONS */
      // First part: test ACE_NEW
      for (i = 0; i < MAX_ALLOCS_IN_TEST; i++)
        {
          try_ace_new (&blocks[i]);
          if (blocks[i] == 0)
            break;
        }
      if (i == MAX_ALLOCS_IN_TEST)
        {
          ACE_ERROR ((LM_WARNING,
                      ACE_TEXT ("Test didn't exhaust available memory\n")));
          // Back up to valid pointer for deleting.
          --i;
        }
      else
        {
          ACE_TEST_ASSERT (blocks[i] == 0);
          if (errno == ENOMEM)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("ACE_NEW failed properly at block %d\n"),
                        i));
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("ACE_NEW failed at block %d, but ")
                        ACE_TEXT ("expected ENOMEM, %p (%d)\n"),
                        i,
                        ACE_TEXT ("got"),
                        ACE_ERRNO_GET));
        }

      // Free the memory to try ACE_NEW_RETURN
      while (i >= 0)
        delete [] blocks[i--];

      // Second part: test ACE_NEW_RETURN
      for (i = 0; i < MAX_ALLOCS_IN_TEST; i++)
        {
          blocks[i] = try_ace_new_return ();
          if (blocks[i] == 0)
            break;
        }

      if (i == MAX_ALLOCS_IN_TEST)
        {
          ACE_ERROR ((LM_WARNING,
                      ACE_TEXT ("Test didn't exhaust available memory\n")));
          // Back up to valid pointer.
          --i;
        }
      else
        {
          ACE_TEST_ASSERT (blocks[i] == 0);
          if (errno == ENOMEM)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("ACE_NEW_RETURN failed properly at block %d\n"),
                        i));
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("ACE_NEW_RETURN failed at block %d, but ")
                        ACE_TEXT ("expected ENOMEM, %p (%d)\n"),
                        i,
                        ACE_TEXT ("got"),
                        ACE_ERRNO_GET));
        }
      while (i >= 0)
        delete [] blocks[i--];

      // Third part: test ACE_NEW_NORETURN
      for (i = 0; i < MAX_ALLOCS_IN_TEST; i++)
        {
          blocks[i] = try_ace_new_noreturn ();
          if (blocks[i] == 0)
            break;
        }

      if (i == MAX_ALLOCS_IN_TEST)
        {
          ACE_ERROR ((LM_WARNING,
                      ACE_TEXT ("Test didn't exhaust available memory\n")));
          // Back up to valid pointer.
          --i;
        }
      else
        {
          ACE_TEST_ASSERT (blocks[i] == 0);
          if (errno == ENOMEM)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("ACE_NEW_NORETURN failed properly at block %d\n"),
                        i));
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("ACE_NEW_NORETURN failed at block %d, but ")
                        ACE_TEXT ("expected ENOMEM, %p (%d)\n"),
                        i,
                        ACE_TEXT ("got"),
                        ACE_ERRNO_GET));
        }
      while (i >= 0)
        delete [] blocks[i--];

#  if defined (ACE_HAS_EXCEPTIONS)
    }

  catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Caught exception during test; ")
                  ACE_TEXT ("ACE_bad_alloc not defined correctly, or\n")));
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ACE_NEW_THROWS_EXCEPTIONS is not #defined ")
                  ACE_TEXT ("(and should be).\n")));
      // Mark test failure
      status = 1;
    }
#  endif /* ACE_HAS_EXCEPTIONS */
#endif /* __SUNPRO_CC && !ACE_HAS_EXCEPTIONS */

  ACE_END_TEST;
  return status;
}
