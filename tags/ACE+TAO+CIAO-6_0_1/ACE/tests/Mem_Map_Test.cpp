// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Mem_Map_Test.cpp
//
// = DESCRIPTION
//      This test illustrates the use of ACE_Mem_Map to reverse a
//      file. The test first creates a dummy file for testing, then
//      reverses the file and then reverses it again to get back the
//      original file.
//
// = AUTHOR
//    Prashant Jain <pjain@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Mem_Map.h"
#include "ace/ACE.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_Memory.h"



#if !defined (ACE_LACKS_MMAP)

static const char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";
static const int LINE_LENGTH = 10;
static const int NUM_LINES = 15;

static void
reverse_file (ACE_HANDLE file_handle,
              char *array,
              size_t size)
{
  int count = 0;
  // LynxOS 3.0.0/PowerPC needs the volatile qualifier, with -O2
  // optimization enabled and without ACE_HAS_INLINE.
  volatile size_t i = size;
  --i;

  if (array[i] == '\0')
    array[i] = '\n';

  while (i-- > 0)
    {
      if (array[i] == '\n')
        {
          ACE_OS::write (file_handle, array + i + 1, count);
          ACE_OS::write (file_handle, ACE_TEXT ("\n"), 1);
          count = 0;
        }
      else
        count++;
    }
  ACE_OS::write (file_handle, array, count+1);
}

static int
create_test_file (ACE_TCHAR *filename, int line_length, int num_lines)
{
  char *mybuf = 0;

  ACE_NEW_RETURN (mybuf, char[line_length + 1], -1);
  const char *c = ACE_ALPHABET;
  const char *d = c;
#if defined (__QNXNTO__) || (defined (ACE_VXWORKS) && (ACE_VXWORKS <= 0x670))
  // For NTO has to applied to open the file, as Mem_Map can map only shared memory
  ACE_Mem_Map mmap_4_open;
  mmap_4_open.open (filename, O_RDWR | O_CREAT | O_TRUNC, ACE_DEFAULT_FILE_PERMS);
  ACE_HANDLE file_handle  = mmap_4_open.handle();
#else
  ACE_HANDLE file_handle = ACE_OS::open (filename,
                                         O_RDWR | O_CREAT | O_TRUNC,
                                         ACE_DEFAULT_FILE_PERMS);
#endif
  if (file_handle  == ACE_INVALID_HANDLE)
    {
      delete [] mybuf;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Open failed for %s\n"),
                         filename),
                        -1);
    }

  for (int j = 0; j < num_lines; j++)
    {
      for (int i = 0; i < line_length; i++)
        {
          mybuf[i] = *c;
          c++;
        }

      mybuf[line_length] = '\0';

      c = ++d;

      if (ACE_OS::write (file_handle, mybuf, line_length) != line_length)
        {
          delete [] mybuf;
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%p (%d) <%s>\n"),
                             ACE_TEXT ("Write to file failed:"),
                             ACE_ERRNO_GET,
                             filename),
                            -1);
        }

      if (ACE_OS::write (file_handle, ACE_TEXT ("\n"), 1) != 1)
        {
          delete [] mybuf;
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("write to file %s failed\n"),
                             filename),
                            -1);
        }
    }
#if defined (__QNXNTO__) || (defined (ACE_VXWORKS) && (ACE_VXWORKS <= 0x670))
  mmap_4_open.close();
#else
  ACE_OS::close (file_handle);
#endif

  delete [] mybuf;
  return 0;
}

#endif /* !ACE_LACKS_MMAP */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Mem_Map_Test"));

#if !defined (ACE_LACKS_MMAP)

#if defined (__QNXNTO__) || (defined (ACE_VXWORKS) && (ACE_VXWORKS <= 0x670))
  ACE_ERROR ((LM_INFO,
                 ACE_TEXT ("mmap on QNX Neutrino/VxWorks can map only shared memory files\n")));
#endif

  // = Initialize the temporary variable names

  ACE_TCHAR test_file[MAXPATHLEN + 1];
  ACE_TCHAR temp_file1[MAXPATHLEN + 1];
  ACE_TCHAR temp_file2[MAXPATHLEN + 1];

  // Get the temporary directory
  // - 18 is for the filenames, ace_mem_map_temp_1 is the longest
  if (ACE::get_temp_dir (test_file, MAXPATHLEN - 18) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Temporary path too long\n")),
                      -1);

  // Copy the temp directory to the other variables
  ACE_OS::strcpy (temp_file1, test_file);
  ACE_OS::strcpy (temp_file2, test_file);

  // Add the filenames to the end
  ACE_OS::strcat (test_file,
                  ACE_TEXT ("ace_mem_map_test"));
  ACE_OS::strcat (temp_file1,
                  ACE_TEXT ("ace_mem_map_temp_1"));
  ACE_OS::strcat (temp_file2,
                  ACE_TEXT ("ace_mem_map_temp_2"));

  // First create a test file to work on
  if (create_test_file (test_file, LINE_LENGTH, NUM_LINES) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Create test file failed\n")),
                      -1);
  ACE_Mem_Map mmap;

  // First memory map the test file
  if (mmap.map (test_file) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%n: %p %s\n%a"),
                       ACE_TEXT ("mmap"),
                       test_file),
                      -1);

  // Now create a temporary file for intermediate processing
#if defined (__QNXNTO__) || (defined (ACE_VXWORKS) && (ACE_VXWORKS <= 0x670))
  ACE_Mem_Map mmap_4_open;
  mmap_4_open.open(temp_file1,
                   O_RDWR | O_TRUNC | O_CREAT,
                   ACE_DEFAULT_FILE_PERMS);
  ACE_HANDLE temp_file_handle = mmap_4_open.handle();
#else
  ACE_HANDLE temp_file_handle = ACE_OS::open (temp_file1,
                                              O_RDWR | O_TRUNC | O_CREAT,
                                              ACE_DEFAULT_FILE_PERMS);
#endif

  if (temp_file_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Open failed\n")),
                      -1);

  // Reverse the original file and write the output to the temporary
  // file.
  reverse_file (temp_file_handle,
                (char *) mmap.addr (),
                mmap.size ());
#if defined (__QNXNTO__) || (defined (ACE_VXWORKS) && (ACE_VXWORKS <= 0x670))
  mmap_4_open.close();
#else
  ACE_OS::close (temp_file_handle);
#endif

  ACE_Mem_Map temp_mmap;

  // Now memory map the temporary file
  if (temp_mmap.map (temp_file1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%n: %p %s\n%a"),
                       ACE_TEXT ("mmap"),
                       temp_file1),
                      -1);
#if defined (__QNXNTO__) || (defined (ACE_VXWORKS) && (ACE_VXWORKS <= 0x670))
  mmap_4_open.open(temp_file2,
                   O_RDWR | O_TRUNC | O_CREAT,
                   ACE_DEFAULT_FILE_PERMS);
                   temp_file_handle = mmap_4_open.handle();
#else
  temp_file_handle = ACE_OS::open (temp_file2,
                                   O_RDWR | O_TRUNC | O_CREAT,
                                   ACE_DEFAULT_FILE_PERMS);
#endif
  if ( temp_file_handle == ACE_INVALID_HANDLE)

    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Open failed\n")), -1);

  // Now reverse the temporary file and write everything to the second
  // temporary file.
  reverse_file (temp_file_handle,
                (char *) temp_mmap.addr (),
                temp_mmap.size ());
#if defined (__QNXNTO__) || (defined (ACE_VXWORKS) && (ACE_VXWORKS <= 0x670))
  mmap_4_open.close();
#else
  ACE_OS::close (temp_file_handle);
#endif

  // Memory map the second temporary file
  ACE_Mem_Map temp_mmap2;

  if (temp_mmap2.map (temp_file2) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%n: %p %s\n%a"),
                       ACE_TEXT ("mmap"),
                       temp_file2),
                      -1);

  // Now do a memcmp -- the orig file and the second temporary file
  // should be identical.
  ACE_TEST_ASSERT (ACE_OS::memcmp (temp_mmap2.addr (),
                              mmap.addr (),
                              mmap.size ()) == 0);

  // Delete the test file
  mmap.remove ();

  // Delete ACE_TEMP_TEST_FILE
  temp_mmap.remove ();

  // Delete ACE_TEMP_TEST_FILE_2
  temp_mmap2.remove ();

#else /* !ACE_LACKS_MMAP */

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("mmap is not supported on this platform\n")));

#endif /* !ACE_LACKS_MMAP */

  ACE_END_TEST;
  return 0;
}
