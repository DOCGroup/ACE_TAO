#include "test_config.h"
#include "ace/Malloc_Allocator.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"

#include <cstdio>
#include <cstring>
#include <string>

class Counting_Allocator : public ACE_New_Allocator
{
public:
  void free (void *ptr) override
  {
    ++this->free_count_;
    ACE_New_Allocator::free (ptr);
  }

  size_t free_count_ {};
};

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Read_Buffer_Test"));

  FILE * const file = std::tmpfile ();
  if (file == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("tmpfile failed\n")), -1);
    }

  if (std::fputs ("read buffer\n", file) < 0 ||
      std::fseek (file, 0, SEEK_SET) != 0)
    {
      std::fclose (file);
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to prepare test file\n")), -1);
    }

  Counting_Allocator allocator;
  {
    ACE_Read_Buffer reader (file, true, &allocator);
    {
      ACE_Read_Buffer_Unique_Ptr buffer (reader.read (), {reader.alloc ()});
      if (buffer == nullptr)
        {
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Read_Buffer::read failed\n")), -1);
        }
      ACE_TEST_ASSERT (reader.size () == 12);
      ACE_TEST_ASSERT (std::memcmp (buffer.get (), "read buffer", 11) == 0);
      ACE_TEST_ASSERT (buffer.get ()[11] == '\0');
    }

    ACE_TEST_ASSERT (allocator.free_count_ == 1);
  }

  FILE * const large_file = std::tmpfile ();
  if (large_file == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to create large read test file\n")), -1);
    }
  std::string const input (BUFSIZ * 2 + 1, 'x');
  if (std::fwrite (input.data (), 1, input.size (), large_file) != input.size ()
      || std::fseek (large_file, 0, SEEK_SET) != 0)
    {
      std::fclose (large_file);
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to prepare large read test file\n")), -1);
    }
  {
    ACE_Read_Buffer reader (large_file, true);
    ACE_Read_Buffer_Unique_Ptr buffer (reader.read (), {reader.alloc ()});
    if (buffer == nullptr || reader.size () != input.size ()
        || std::memcmp (buffer.get (), input.data (), input.size ()) != 0
        || buffer.get ()[input.size ()] != '\0')
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Large buffer read failed\n")), -1);
      }
  }

#if !defined (ACE_LACKS_DUP) && !defined (ACE_LACKS_MKSTEMP) && !defined (ACE_LACKS_FDOPEN)
  ACE_TCHAR filename[] = ACE_TEXT ("read_buffer_test_XXXXXX");
  ACE_HANDLE const handle = ACE_OS::mkstemp (filename);
  if (handle == ACE_INVALID_HANDLE)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("mkstemp failed\n")), -1);
    }
  if (ACE_OS::write (handle, "borrowed", 8) != 8)
    {
      ACE_OS::close (handle);
      ACE_OS::unlink (filename);
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to prepare borrowed handle\n")), -1);
    }

  for (int i = 0; i != 100; ++i)
    {
      if (ACE_OS::lseek (handle, 0, SEEK_SET) == -1)
        {
          ACE_OS::close (handle);
          ACE_OS::unlink (filename);
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Borrowed handle was closed\n")), -1);
        }
      ACE_Read_Buffer reader (handle, false);
      ACE_Read_Buffer_Unique_Ptr buffer (reader.read (), {reader.alloc ()});
      if (buffer == nullptr || std::strcmp (buffer.get (), "borrowed") != 0)
        {
          ACE_OS::close (handle);
          ACE_OS::unlink (filename);
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to read borrowed handle\n")), -1);
        }
    }
  int const close_result = ACE_OS::close (handle);
  ACE_OS::unlink (filename);
  if (close_result != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to close borrowed handle\n")), -1);
    }

#endif

  {
    ACE_Read_Buffer invalid_reader (ACE_INVALID_HANDLE, false);
    if (invalid_reader.read () != nullptr)
      {
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Invalid handle was accepted\n")), -1);
      }
  }

  ACE_END_TEST;
  return 0;
}
