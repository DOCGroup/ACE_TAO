// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Dirent_Test.cpp
//
// = DESCRIPTION
//     This is a test of the opendir and readdir emulation provided by the
//     class ACE_OS_Dirent.  It is used to ensure that the emulation code
//     works properly on platforms that don't support this capability
//     natively.  As the emulation code is not compiled in other
//     platforms, this test also ensures that there is no impact to
//     platforms that natively support directory scanning operations.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>,
//    Rich Newman <RNewman@directv.com>, and
//    Douglas C. Schmidt <schmidt@uci.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS.h"
#include "ace/OS_String.h"
#include "ace/Dirent.h"
#include "ace/Dirent_Selector.h"

ACE_RCSID(tests, Dirent_Test, "$Id$")

#if defined (VXWORKS) || defined(CHORUS)
#define TEST_DIR "."
#define TEST_ENTRY ".."
#else
#define TEST_DIR "../tests"
#define TEST_ENTRY "Dirent_Test.cpp"
#endif /* VXWORKS || CHORUS */

// Number of entries in the directory.
static int entrycount = 0;

static int
selector (const dirent *d)
{
  return ACE_OS_String::strcmp (d->d_name, ACE_TEXT (TEST_ENTRY)) == 0;
}

static int
comparator (const dirent **d1, const dirent **d2)
{
  return ACE_OS_String::strcmp ((*d1)->d_name, (*d2)->d_name);
}

static int
dirent_selector_test (void)
{
  int status;
  int n;

  ACE_Dirent_Selector sds;

  // Pass in functions that'll specify the selection criteria.
  status = sds.open (ACE_TEXT (TEST_DIR), selector, comparator);
  ACE_ASSERT (status != -1);

  // We should only have located ourselves!
  ACE_ASSERT (sds.length () == 1);

  for (n = 0; n < sds.length (); ++n)
    ACE_DEBUG ((LM_DEBUG, 
                "Sorted: %d: %s\n",
                n,
                sds[n]->d_name));

  status = sds.close ();
  ACE_ASSERT (status != -1);

  ACE_Dirent_Selector ds;

  // Don't specify any selection criteria.
  status = ds.open (ACE_TEXT (TEST_DIR));
  ACE_ASSERT (status != -1);

  ACE_ASSERT (entrycount == ds.length ());

  for (n = 0; n < ds.length (); ++n)
    ACE_DEBUG ((LM_DEBUG, 
                "Entry %d: %s\n",
                n,
                ds[n]->d_name));

  status = ds.close ();
  ACE_ASSERT (status != -1);

  return status;
}

static int
dirent_test (void)
{
  ACE_Dirent dir (ACE_TEXT (TEST_DIR));

  for (dirent *directory;
       (directory = dir.read ()) != 0;
       entrycount++)
    ACE_DEBUG ((LM_DEBUG,
                "Entry %d: %s\n",
                entrycount,
                directory->d_name));

  switch (entrycount) 
    {
    case 0: 
      ACE_ERROR_RETURN ((LM_ERROR,
                         "readdir failed to read anything\n"),
                        -1);
      /* NOTREACHED */
    case 1:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "readdir failed, only matched directory name\n"),
                        -1);
      /* NOTREACHED */
    default:
      ACE_DEBUG ((LM_DEBUG,
                  "readdir succeeded, read %d entries\n",
                  entrycount));
    }
  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Dirent_Test"));

  int status = dirent_test ();

  if (status != -1)
    status = dirent_selector_test ();

  ACE_END_TEST;
  return status;
}
