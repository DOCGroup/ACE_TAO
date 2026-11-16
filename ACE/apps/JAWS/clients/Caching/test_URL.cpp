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

  size_t const decoded_size = derived.decode (unaligned_buffer.data () + 1);
  ACE_TEST_ASSERT (decoded_size == encoded_size);
  ACE_TEST_ASSERT (derived.seq () != nullptr);
  ACE_TEST_ASSERT (derived.seq ()->size () == ouch1.size ());
  for (size_t i = 0; i < ouch1.size (); ++i)
    ACE_TEST_ASSERT ((*derived.seq ())[i] == ouch1[i]);

  ACE_URL_Offer original_offer (ouch1.size (), "http://example.test/resource");
  original_offer.url_properties (ouch1);
  ACE_URL_Offer derived_offer;
  size_t const offer_size = original_offer.size ();
  std::vector<char> offer_buffer (offer_size + 1);
  size_t const encoded_offer_size = original_offer.encode (offer_buffer.data () + 1);
  ACE_TEST_ASSERT (encoded_offer_size == offer_size);
  size_t const decoded_offer_size = derived_offer.decode (offer_buffer.data () + 1);
  ACE_TEST_ASSERT (decoded_offer_size == encoded_offer_size);
  ACE_TEST_ASSERT (derived_offer == original_offer);

  ACE_URL_Offer_Seq offers (1);
  offers[0] = original_offer;
  ACE_URL_Locator_Reply original_reply, derived_reply;
  ACE_TEST_ASSERT (original_reply.query_reply (0, offers.size (), offers) == 0);
  size_t const encoded_reply_size = original_reply.encode ();
  std::vector<char> reply_buffer (encoded_reply_size + 1);
  std::memcpy (reply_buffer.data () + 1,
               original_reply.buffer (),
               encoded_reply_size);
  size_t const decoded_reply_size = derived_reply.decode (reply_buffer.data () + 1);
  ACE_TEST_ASSERT (decoded_reply_size == encoded_reply_size);
  ACE_TEST_ASSERT (derived_reply.offers () != nullptr);
  ACE_TEST_ASSERT (derived_reply.offers ()->size () == offers.size ());
  ACE_TEST_ASSERT ((*derived_reply.offers ())[0] == original_offer);

  derived.dump ();

  return 0;
}
