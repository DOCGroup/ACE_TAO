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
//     class ACE_Dirent.  It is used to ensure that the emulation code
//     works properly on platforms that don't support this capability
//     natively.  As the emulation code is not compiled in other
//     platforms, this test also ensures that there is no impact to
//     platforms that natively support directory scanning operations.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>,
//    Zvika Ashani <zvika@aspectusvi.com>,
//    Rich Newman <RNewman@directv.com>, and
//    Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Dirent.h"
#include "ace/Dirent_Selector.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_unistd.h"
#include "ace/SString.h"

#if defined (ACE_HAS_TCHAR_DIRENT)
#  define TEST_ENTRY ACE_TEXT ("run_test.lst")
#else
#  define TEST_ENTRY "run_test.lst"
#endif /* ACE_HAS_TCHAR_DIRENT */

// Directory to scan - we need to figure it out based on environment.
static ACE_TString TestDir;
static const int RECURSION_INDENT = 3;

// Number of entries in the directory.
static int entrycount = 0;

extern "C" {

static int
selector (const ACE_DIRENT *d)
{
  return ACE_OS::strcmp (d->d_name, TEST_ENTRY) == 0;
}

static int
comparator (const ACE_DIRENT **d1, const ACE_DIRENT **d2)
{
  return ACE_OS::alphasort (d1, d2);
}

} /* extern "C" */

static int
dirent_selector_test (void)
{
  int n;
  int error = 0;
  const ACE_TCHAR *test_dir = TestDir.c_str ();
  ACE_Dirent_Selector sds;

  // Pass in functions that'll specify the selection criteria.
  int status = sds.open (test_dir, selector, comparator);
  if (status == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%s, %p\n"),
                       test_dir,
                       ACE_TEXT ("open")),
                      -1);

  // We should only have located ourselves!
  if (sds.length () != 1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("selected %d entries in %s, should be 1\n"),
                  sds.length (),
                  test_dir));
      error = 1;
    }

  for (n = 0; n < sds.length (); ++n)
    {
#if defined (ACE_HAS_TCHAR_DIRENT)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Sorted: %d: %s\n"),
                  n,
                  sds[n]->d_name));
#else
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Sorted: %d: %C\n"),
                  n,
                  sds[n]->d_name));
#endif
    }

  status = sds.close ();
  if (status == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("after selecting, %p\n"),
                  ACE_TEXT ("close")));
      error = 1;
    }

  ACE_Dirent_Selector ds;

  // Don't specify any selection criteria.
  status = ds.open (test_dir);
  if (status == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%s w/o selection criteria; %p\n"),
                  test_dir,
                  ACE_TEXT ("open")));
      error = 1;
    }

  // We counted the entries earlier by hand; should be the same here.
  if (entrycount != ds.length ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Counted %d entries in %s but selector says %d\n"),
                  entrycount,
                  test_dir,
                  ds.length ()));
      error = 1;
    }

  for (n = 0; n < ds.length (); ++n)
    {
#if defined (ACE_HAS_TCHAR_DIRENT)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Entry %d: %s\n"),
                  n,
                  ds[n]->d_name));
#else
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Entry %d: %C\n"),
                  n,
                  ds[n]->d_name));
#endif
    }

  if (ds.close () == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("w/o selection criteria; %p\n"),
                  ACE_TEXT ("close")));
      error = 1;
    }

  return error ? -1 : 0;
}

static int
dirent_test (void)
{
  ACE_Dirent dir;

   if (dir.open (TestDir.c_str ()) == -1)
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("open of dir %s failed\n"), TestDir.c_str()), -1);

  for (ACE_DIRENT *directory = 0;
       (directory = dir.read ()) != 0;
       entrycount++)
    {
#if defined (ACE_HAS_TCHAR_DIRENT)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Entry %d: %s\n"),
                  entrycount,
                  directory->d_name));
#else
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Entry %d: %C\n"),
                  entrycount,
                  directory->d_name));
#endif
    }

  switch (entrycount)
    {
    case 0:
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("readdir failed to read anything\n")), -1);
      /* NOTREACHED */
    case 1:
      ACE_ERROR_RETURN
        ((LM_ERROR,
          ACE_TEXT ("readdir failed, only matched directory name\n")),
         -1);
      /* NOTREACHED */
    default:
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("readdir succeeded, read %d entries\n"),
                  entrycount));
    }
  return 0;
}

static int
dirent_count (const ACE_TCHAR *dir_path,
              int &dir_count,
              int &file_count,
              int recursion_level)
{
#if !defined (ACE_LACKS_CHDIR)
  if (ACE_OS::chdir (dir_path) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("chdir: %p\n"),
                       dir_path),
                      -1);
#else
  ACE_UNUSED_ARG (dir_path);
#endif /* !ACE_LACKS_CHDIR */

  ACE_Dirent dir;
   if (dir.open (ACE_TEXT (".")) == -1)
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("open of dir . failed\n")), -1);

  // Since the dir struct d_name type changes depending on the setting
  // of ACE_LACKS_STRUCT_DIR, copy each name into a neutral format
  // array to work on it.
  size_t const maxnamlen = MAXNAMLEN;
  ACE_TCHAR tname[maxnamlen + 1];

  int entry_count = 0;

  for (ACE_DIRENT *directory; (directory = dir.read ()) != 0;)
    {
      // Skip the ".." and "." files.
      if (ACE::isdotdir(directory->d_name) == true)
        continue;
      ++entry_count;

#if defined (ACE_HAS_TCHAR_DIRENT)
      ACE_OS::strncpy (tname, directory->d_name, maxnamlen);
#else
      ACE_OS::strncpy (tname,
                       ACE_TEXT_CHAR_TO_TCHAR (directory->d_name),
                       maxnamlen);
#endif /* ACE_LACKS_STRUCT_DIR */

      int local_file_count = 0;
      int local_dir_count = 0;
      ACE_stat stat_buf;
      if (ACE_OS::lstat (directory->d_name, &stat_buf) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           tname),
                          -1);

      switch (stat_buf.st_mode & S_IFMT)
        {
        case S_IFREG: // Either a regular file or an executable.
          ++file_count;
          break;

        case S_IFLNK: // Either a file or directory link, so let's find out.
          if (ACE_OS::stat (directory->d_name, &stat_buf) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               tname),
                              -1);

          if ((stat_buf.st_mode & S_IFMT) == S_IFDIR)
            // Don't recurse through symbolic directory links!
            ++dir_count;
          else
            ++file_count;
          break;

        case S_IFDIR:
          ACE_DEBUG ((LM_DEBUG, "%*sentering subdirectory %s\n",
                      recursion_level * RECURSION_INDENT,
                      ACE_TEXT (""),
                      tname));
          if (dirent_count (tname,
                            local_dir_count,
                            local_file_count,
                            recursion_level + 1) != -1)
            {
              ACE_DEBUG
                ((LM_DEBUG,
                  ACE_TEXT ("%*ssubdirectory %s has %d files and %d subdirectories.\n"),
                  recursion_level * RECURSION_INDENT,
                  ACE_TEXT (""),
                  tname,
                  local_file_count,
                  local_dir_count));
              ++dir_count;

#if !defined (ACE_LACKS_CHDIR)
# if (defined (ACE_VXWORKS) && (ACE_VXWORKS < 0x600))
              // Move back to parent directory.
              if (ACE_OS::chdir (full_path) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("chdir: %p\n"),
                                   full_path),
                                  -1);
# else
              // Move back up a level.
              if (ACE_OS::chdir (ACE_TEXT ("..")) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("chdir: %p\n"),
                                   dir_path),
                                  -1);
# endif
#endif /* !ACE_LACKS_CHDIR */
            }
          break;

        default: // Must be some other type of file (PIPE/FIFO/device)
          ++file_count;
          break;
        }
    }

  return entry_count;
}

static int
dirent_recurse_test (void)
{
  int total_dirs = 0;
  int total_files = 0;
  const ACE_TCHAR *test_dir = TestDir.c_str ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Starting directory recursion test for %s\n"),
              test_dir));

  if (dirent_count (test_dir,
                    total_dirs,
                    total_files,
                    1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Directory recursion test failed for %s\n"),
                       test_dir),
                      -1);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Directory recursion test succeeded for %s, read %d files %d dirs\n"),
              test_dir,
              total_files,
              total_dirs));
  return 0;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Dirent_Test"));

  // First, find out where to run most of the scans. If a platform has a
  // compiled-in setting in TEST_DIR, honor that. Else, look for:
  //   $top_srcdir/tests: The ACE_wrappers dir for autoconf builds
  //   $ACE_ROOT/tests: The ACE_wrappers dir for most other builds
  //   ../test: Last-chance try to hit the right place
#if defined (TEST_DIR)
  TestDir = TEST_DIR;
#else
  const char *root = ACE_OS::getenv ("top_srcdir");
  if (root == 0)
    root = ACE_OS::getenv ("ACE_ROOT");
  if (root != 0)
    {
      TestDir = ACE_TEXT_CHAR_TO_TCHAR (root);
      TestDir += ACE_DIRECTORY_SEPARATOR_STR;
      TestDir += ACE_TEXT ("tests");
    }
  else
    {
      TestDir = ACE_TEXT ("../tests");
    }
#endif /* TEST_DIR */

  int status = 0;

  if (-1 == dirent_test ())
    status = -1;

  if (-1 == dirent_selector_test ())
    status = -1;

  if (-1 == dirent_recurse_test ())
    status = -1;

  ACE_END_TEST;
  return status;
}
