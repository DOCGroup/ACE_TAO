// $Id$

#include "tao/StringSeqC.h"

#if defined (__BORLANDC__) && (__BORLANDC__ >= 0x660) && (__BORLANDC__ <= 0x680)
#  pragma option push -w-8011
#endif

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  char const *str = "Some string";

  CORBA::StringSeq seq;
  seq.length (100);
  for (CORBA::ULong i = 0; i < seq.length (); ++i)
    {
      seq[i] = str;
    }

  // Save a pointer to the whole buffer.
  char const * const *wholebuf = seq.get_buffer ();

  // This call should reinitialize the the 100th element
  // (the fact that the shrunk elements are reinitialized is TAO
  // specific but we test for it).
  seq.length (99);
  // No reallocation should happen for the buffer.
  ACE_TEST_ASSERT (seq.get_buffer () == wholebuf);
  // And set the length to the same value
  seq.length (99);
  ACE_TEST_ASSERT (seq.get_buffer () == wholebuf);
  // We cannot be sure that the pointer to the reinitialized 100th
  // element is different from the old one since memory manager can
  // return the same pointer that we've just released but it must
  // not be 0 and it must be an empty string.
  ACE_TEST_ASSERT (wholebuf[99] != 0);
  ACE_TEST_ASSERT (ACE_OS::strcmp (wholebuf[99], "") == 0);

  // Extend the sequence to the original size.
  seq.length (100);
  // No reallocation should happen for the buffer.
  ACE_TEST_ASSERT (seq.get_buffer () == wholebuf);
  // And now we can test absolutely legally that the 100th
  // element was reinitialized as CORBA spec requires.
  ACE_TEST_ASSERT (seq[99].in () != 0);
  ACE_TEST_ASSERT (ACE_OS::strcmp (seq[99].in (), "") == 0);
  seq.length (101);
  // Reallocation should happen for the buffer.
  ACE_TEST_ASSERT (seq.get_buffer () != wholebuf);
  ACE_TEST_ASSERT (seq[100].in () != 0);
  ACE_TEST_ASSERT (ACE_OS::strcmp (seq[100].in (), "") == 0);

  return 0;
}

#if defined (__BORLANDC__) && (__BORLANDC__ >= 0x660) && (__BORLANDC__ <= 0x680)
# pragma option pop
#endif
