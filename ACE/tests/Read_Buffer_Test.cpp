// -*- C++ -*-

#include "test_config.h"
#include "ace/Malloc_Allocator.h"
#include "ace/Read_Buffer.h"

#include <cstdio>
#include <cstring>

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
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("tmpfile failed\n")), -1);

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
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Read_Buffer::read failed\n")), -1);
      ACE_TEST_ASSERT (reader.size () == 12);
      ACE_TEST_ASSERT (std::memcmp (buffer.get (), "read buffer", 11) == 0);
      ACE_TEST_ASSERT (buffer.get ()[11] == '\0');
    }

    ACE_TEST_ASSERT (allocator.free_count_ == 1);
  }

  ACE_END_TEST;
  return 0;
}
