#include "tao/StringSeqC.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  char const *str = "Some string";
  constexpr CORBA::ULong initial_length = 100;
  constexpr CORBA::ULong shortened_length = initial_length - 1;

  CORBA::StringSeq seq;
  seq.length (initial_length);
  for (CORBA::ULong i = 0; i < seq.length (); ++i)
    {
      seq[i] = str;
    }

  // Save a pointer to the whole buffer.
  char const * const *wholebuf = seq.get_buffer ();

  // This call should reinitialize the 100th element
  // (the fact that the shrunk elements are reinitialized is TAO
  // specific but we test for it).
  seq.length (shortened_length);
  // No reallocation should happen for the buffer.
  ACE_TEST_ASSERT (seq.get_buffer () == wholebuf);
  // And set the length to the same value
  seq.length (shortened_length);
  ACE_TEST_ASSERT (seq.get_buffer () == wholebuf);
  // We cannot be sure that the pointer to the reinitialized 100th
  // element is different from the old one since memory manager can
  // return the same pointer that we've just released but it must
  // not be 0 and it must be an empty string.
  ACE_TEST_ASSERT (wholebuf[shortened_length] != nullptr);
  ACE_TEST_ASSERT (ACE_OS::strcmp (wholebuf[shortened_length], "") == 0);

  // Extend the sequence to the original size.
  seq.length (initial_length);
  // No reallocation should happen for the buffer.
  ACE_TEST_ASSERT (seq.get_buffer () == wholebuf);
  // And now we can test absolutely legally that the 100th
  // element was reinitialized as CORBA spec requires.
  ACE_TEST_ASSERT (seq[shortened_length].in () != nullptr);
  ACE_TEST_ASSERT (ACE_OS::strcmp (seq[shortened_length].in (), "") == 0);
  seq.length (initial_length + 1);
  // Reallocation should happen for the buffer.
  ACE_TEST_ASSERT (seq.get_buffer () != wholebuf);
  ACE_TEST_ASSERT (seq[initial_length].in () != nullptr);
  ACE_TEST_ASSERT (ACE_OS::strcmp (seq[initial_length].in (), "") == 0);

  return 0;
}
