#include "Locator_Request_Reply.h"
#include "ace/Assert.h"
#include <cstring>
#include <vector>

int ACE_TMAIN (int , ACE_TCHAR *[])
{
  ACE_URL_Locator_Request original, derived;

  ACE_URL_Property_Seq ouch1(3);
  ouch1[0].name ("name 1");
  ouch1[0].value ("value 1");
  ouch1[1].name ("name 2");
  ouch1[1].value ("value 2");
  ouch1[2].name ("name 3");
  ouch1[2].value ("value 3");

  original.url_query (1, ouch1, 3);

  original.dump ();
  derived.dump ();

  size_t const encoded_size = original.encode ();
  std::vector<char> unaligned_buffer (encoded_size + 1);
  std::memcpy (unaligned_buffer.data () + 1, original.buffer (), encoded_size);

  ACE_ASSERT (derived.decode (unaligned_buffer.data () + 1) == encoded_size);
  ACE_ASSERT (derived.seq () != nullptr);
  ACE_ASSERT (derived.seq ()->size () == ouch1.size ());
  for (size_t i = 0; i < ouch1.size (); ++i)
    ACE_ASSERT ((*derived.seq ())[i] == ouch1[i]);

  derived.dump ();

  return 0;
}
