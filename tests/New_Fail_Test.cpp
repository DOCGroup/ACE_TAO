// $Id$
//
// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    New_Fail_Test.cpp
//
// = DESCRIPTION
//    Checks to be sure that a failed ACE_NEW[_RETURN] doesn't end up throwing
//    an exception up to the caller.
//
//    Note that this test doesn't get a real attempt on platforms which:
//      1. Are known to throw exceptions when 'new' runs out of resources,
//      2. Are built with exceptions disabled.
//    In these cases, the test puts a message in the log noting that a failed
//    new will throw an exception, and trust that the user accepts that risk.
//
// = AUTHOR
//    Steve Huston
//
// ============================================================================

#include "test_config.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"
#include "ace/CORBA_macros.h"

ACE_RCSID(tests, New_Fail_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */


// This test allocates all of the heap memory, forcing 'new' to fail because
// of a lack of memory.  The ACE_NEW macros should prevent an exception from
// being thrown past the ACE_NEW.  If this test doesn't wipe out on an alloc
// exception, it passes.
//
// If it doesn't ever fail an allocation, there's a warning that something is
// wrong.  The allocated memory is always freed to avoid masking a leak
// somewhere else in the test.
static const int BIG_BLOCK = 1024*1024;       // 1MB
static const int MAX_ALLOCS_IN_TEST = 4096;   // about 4GB max in the test


static void try_ace_new (char **p)
{
  ACE_NEW (*p, char[BIG_BLOCK]);
  return;
}


static char * try_ace_new_return (void)
{
  char *p;
  ACE_NEW_RETURN (p, char[BIG_BLOCK], 0);
  return p;
}


int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("New_Fail_Test"));
  int status = 0;

  // Some platforms are known to throw an exception on a failed 'new', but
  // are customarily built without exception support to improve performance.
  // These platforms are noted, and the test passes.
  // For new ports, it is wise to let this test run.  Depending on intended
  // conditions, exceptions can be disabled when the port is complete.
#if (defined (__SUNPRO_CC) || defined (__GNUG__)) && \
    !defined (ACE_HAS_EXCEPTIONS)
  ACE_DEBUG ((LM_NOTICE, "Out-of-memory will throw an unhandled exception\n"));
  ACE_DEBUG ((LM_NOTICE, "Rebuild with exceptions=1 to prevent this, but it may impair performance.\n"));

  // Use the static function addresses, to prevent warnings about the
  // functions not being used.
  if (&try_ace_new) /* null */;
  if (&try_ace_new_return) /* null */;
#else

  char *blocks[MAX_ALLOCS_IN_TEST];
  int i;

#if defined (ACE_HAS_EXCEPTIONS)
  try
    {
#endif /* ACE_HAS_EXCEPTIONS */
      // First part: test ACE_NEW
      for (i = 0; i < MAX_ALLOCS_IN_TEST; i++)
        {
          try_ace_new (&blocks[i]);
          if (blocks[i] == 0)
            break;
        }
      if (i == MAX_ALLOCS_IN_TEST)
        {
          ACE_ERROR((LM_WARNING,
                     "Test didn't exhaust all available memory\n"));
          --i;    // Back up to valid pointer for deleting
        }
      else
        {
          ACE_ASSERT (blocks[i] == 0);
          ACE_ASSERT (errno == ENOMEM);
          ACE_DEBUG((LM_DEBUG, "ACE_NEW failed properly at block %d\n", i));
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
          ACE_ERROR((LM_WARNING,
                     "Test didn't exhaust all available memory\n"));
          --i;    // Back up to valid pointer
        }
      else
        {
          ACE_ASSERT (blocks[i] == 0);
          ACE_ASSERT (errno == ENOMEM);
          ACE_DEBUG ((LM_DEBUG, "ACE_NEW_RETURN failed properly at block %d\n",
                      i));
        }
      while (i >= 0)
        delete [] blocks[i--];

#if defined (ACE_HAS_EXCEPTIONS)
    }

  catch (...)
    {
      ACE_ERROR ((LM_ERROR,
        "Caught exception during test; "
        "ACE_bad_alloc not defined correctly, or\n"));
      ACE_ERROR ((LM_ERROR,
        "ACE_NEW_THROWS_EXCEPTIONS is not #defined (and should be).\n"));
      status = 1;        // Mark test failure
    }
#endif /* ACE_HAS_EXCEPTIONS */
#endif /* __SUNPRO_CC && !ACE_HAS_EXCEPTIONS */

  ACE_END_TEST;
  return status;
}
