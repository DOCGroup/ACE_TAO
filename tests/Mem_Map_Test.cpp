// ============================================================================
// $Id$

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
//    Prashant Jain
// 
// ============================================================================

#include "ace/Log_Msg.h"
#include "ace/Mem_Map.h"
#include "test_config.h"

static int size = 10;
static int num_lines = 15;

static void
reverse_file (ACE_HANDLE file_handle,
	      char *array, 
	      int size)
{
  int count = 0;
  size--;

  if (array[size] == '\0')
    array[size] = '\n';

  while (--size >= 0)
    {
      if (array[size] == '\n')
	{
	  ACE_OS::write (file_handle, array + size + 1, count);
	  ACE_OS::write (file_handle, "\n", 1);
	  count = 0;
	}
      else
	count++;
    }
  ACE_OS::write (file_handle, array, count+1);
}

int
create_test_file ()
{
  ACE_HANDLE file_handle;
  char *mybuf = new char[size+1];
  char c = 'a';
  char d = c;

  if ((file_handle = ACE_OS::open (ACE_DEFAULT_TEST_FILE, 
				   O_RDWR | O_CREAT | O_TRUNC, 
				   0666)) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Open failed\n"), -1);
  
  for (int j = 0; j < num_lines; j++)
    {
      for (int i = 0; i < size; i++)
	{
	  mybuf[i] = c;
	  c++;
	}
      mybuf[size] = '\0';
      c = ++d;
      if (ACE_OS::write (file_handle, mybuf, size) != size)
	ACE_ERROR_RETURN ((LM_ERROR, "write to file failed\n"), -1);

      if (ACE_OS::write (file_handle, "\n", 1) != 1)
	ACE_ERROR_RETURN ((LM_ERROR, "write to file failed\n"), -1);
    }

  ACE_OS::close (file_handle);
  return 0;
}

int
main (int, char **argv)
{
  ACE_START_TEST ("Mem_Map_Test.cpp");

  ACE_LOG_MSG->open (argv[0]);

  ACE_HANDLE temp_file_handle;

  // First create a test file to work on
  if (create_test_file () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Create test file failed\n"), -1);

  ACE_Mem_Map mmap;
  
  // First memory map the test file
  if (mmap.map (ACE_DEFAULT_TEST_FILE) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n%a", "mmap"), -1);

  // Now create a temporary file for intermediate processing
  if ((temp_file_handle = ACE_OS::open (ACE_TEMP_FILE_NAME, 
					O_RDWR | O_TRUNC | O_CREAT,
					0666)) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Open failed\n"), -1);

  // Reverse the original file and write the output to the temporary
  // file 
  reverse_file (temp_file_handle, 
		(char *) mmap.addr (), 
		mmap.size ());

  ACE_OS::close (temp_file_handle);

  ACE_Mem_Map temp_mmap;
  
  // Now memory map the temporary file
  if (temp_mmap.map (ACE_TEMP_FILE_NAME) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n%a", "mmap"), -1);

  char temp_file_name[BUFSIZ];

  // Create another temporary file that would hold the output of
  // reversing the first temporary file
  ACE_OS::sprintf (temp_file_name, "%s%s", ACE_TEMP_FILE_NAME, "2");
  if ((temp_file_handle = ACE_OS::open (temp_file_name, 
					O_RDWR | O_TRUNC | O_CREAT,
					0666)) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Open failed\n"), -1);
  
  // Now reverse the temporary file and write everything to the second
  // temporary file
  reverse_file (temp_file_handle, 
		(char *) temp_mmap.addr (), 
		temp_mmap.size ());

  ACE_OS::close (temp_file_handle);

  // Memory map the second temporary file
  ACE_Mem_Map temp_mmap2;
  if (temp_mmap2.map (temp_file_name) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n%a", "mmap"), -1);

  // Now do a memcmp -- the orig file and the second temporary file
  // should be identical.
  ACE_ASSERT (ACE_OS::memcmp (temp_mmap2.addr (), 
			      mmap.addr (),
			      mmap.size ()) == 0);

  ACE_END_TEST;
  return 0;
}
