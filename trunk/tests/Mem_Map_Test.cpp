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

ACE_RCSID(tests, Mem_Map_Test, "$Id$")

static const int LINE_LENGTH = 10;
static const int NUM_LINES = 15;

static void
reverse_file (ACE_HANDLE file_handle,
              char *array,
              int size)
{
  int count = 0;
  // LynxOS 3.0.0/PowerPC needs the volatile qualifier, with -O2
  // optimization enabled and without ACE_HAS_INLINE.
  volatile int i = size;
  --i;

  if (array[i] == '\0')
    array[i] = '\n';

  while (--i >= 0)
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
  char *c = ACE_ALPHABET;
  char *d = c;

  ACE_HANDLE file_handle = ACE_OS::open (filename,
                                         O_RDWR | O_CREAT | O_TRUNC,
                                         0666);

  if (file_handle  == ACE_INVALID_HANDLE)
    {
      delete [] mybuf;
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Open failed\n")), -1);
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
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("write to file failed: %p (%d)\n"),
                             errno, errno), -1);
        }

      if (ACE_OS::write (file_handle, ACE_TEXT ("\n"), 1) != 1)
        {
          delete [] mybuf;
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("write to file failed\n")), -1);
        }
    }

  ACE_OS::close (file_handle);

  delete [] mybuf;
  return 0;
}

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Mem_Map_Test"));

  // = Initialize the temporary variable names

  ACE_TCHAR test_file[MAXPATHLEN + 1]; 
  ACE_TCHAR temp_file1[MAXPATHLEN + 1]; 
  ACE_TCHAR temp_file2[MAXPATHLEN + 1]; 

  // Get the temporary directory
  // - 18 is for the filenames, ace_mem_map_temp_1 is the longest
  if (ACE::get_temp_dir (test_file, MAXPATHLEN - 18) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Temporary path too long\n")), -1);

  // Copy the temp directory to the other variables
  ACE_OS::strcpy (temp_file1, test_file);
  ACE_OS::strcpy (temp_file2, test_file);

  // Add the filenames to the end
  ACE_OS::strcat (test_file, ACE_TEXT ("ace_mem_map_test"));
  ACE_OS::strcat (temp_file1, ACE_TEXT ("ace_mem_map_temp_1"));
  ACE_OS::strcat (temp_file2, ACE_TEXT ("ace_mem_map_temp_2"));


  // First create a test file to work on
  if (create_test_file (test_file, LINE_LENGTH, NUM_LINES) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Create test file failed\n")), -1);

  ACE_Mem_Map mmap;

  // First memory map the test file
  if (mmap.map (test_file) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%n: %p\n%a"), ACE_TEXT ("mmap")), -1);

  // Now create a temporary file for intermediate processing
  ACE_HANDLE temp_file_handle = ACE_OS::open (temp_file1,
                                              O_RDWR | O_TRUNC | O_CREAT,
                                              0666);

  if (temp_file_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Open failed\n")), -1);

  // Reverse the original file and write the output to the temporary
  // file.
  reverse_file (temp_file_handle,
                (char *) mmap.addr (),
                mmap.size ());

  ACE_OS::close (temp_file_handle);

  ACE_Mem_Map temp_mmap;

  // Now memory map the temporary file
  if (temp_mmap.map (temp_file1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%n: %p\n%a"), ACE_TEXT ("mmap")), -1);

  if ((temp_file_handle = ACE_OS::open (temp_file2,
                                        O_RDWR | O_TRUNC | O_CREAT,
                                        0666)) == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Open failed\n")), -1);

  // Now reverse the temporary file and write everything to the second
  // temporary file.
  reverse_file (temp_file_handle,
                (char *) temp_mmap.addr (),
                temp_mmap.size ());

  ACE_OS::close (temp_file_handle);

  // Memory map the second temporary file
  ACE_Mem_Map temp_mmap2;

  if (temp_mmap2.map (temp_file2) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%n: %p\n%a"), ACE_TEXT ("mmap")), -1);

  // Now do a memcmp -- the orig file and the second temporary file
  // should be identical.
  ACE_ASSERT (ACE_OS::memcmp (temp_mmap2.addr (),
                              mmap.addr (),
                              mmap.size ()) == 0);

  // Delete the test file
  mmap.remove ();

  // Delete ACE_TEMP_TEST_FILE
  temp_mmap.remove ();

  // Delete ACE_TEMP_TEST_FILE_2
  temp_mmap2.remove ();

  ACE_END_TEST;
  return 0;
}
