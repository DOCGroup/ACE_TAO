/* -*- C++ -*- */

// $Id$

#if !defined (ACE_LOCATOR_REQUEST_REPLY_C)
#define ACE_LOCATOR_REQUEST_REPLY_C

#define ACE_BUILD_DLL
#include "Locator_Request_Reply.h"

#if !defined (__ACE_INLINE__)
#include "Locator_Request_Reply.i"
#endif

#include "URL_Properties.h"
#include "URL_Array_Helper.h"
#include "URL_Locator.h"

int
ACE_URL_Locator_Request::url_query (const int how,
				    const ACE_URL_Property_Seq &pseq,
				    const int how_many)
{
  ACE_TRACE ("ACE_URL_Locator_Request::url_query");

  if (how >= ACE_URL_Locator::INVALID_SELECTION)
    return -1;
  ACE_NEW_RETURN (this->seq1_, ACE_URL_Property_Seq (pseq), -1);
  this->how_ = how;
  this->how_many_ = how_many;
  this->code_ = ACE_URL_Locator::QUERY;
  return 0;    
}

int
ACE_URL_Locator_Request::export_offer (const ACE_URL_Offer &offer)
{
  ACE_TRACE ("ACE_URL_Locator_Request::export_offer");
  
  ACE_NEW_RETURN (this->offer_, ACE_URL_Offer (offer), -1);
  this->code_ = ACE_URL_Locator::EXPORT;
  return 0;
}
  
int
ACE_URL_Locator_Request::withdraw_offer (const ACE_WString &offer_id)
{
  ACE_TRACE ("ACE_URL_Locator_Request::withdraw_offer");

  this->id_ = offer_id;
  this->code_ = ACE_URL_Locator::WITHDRAW;
  return 0;
}

int
ACE_URL_Locator_Request::describe_offer (const ACE_WString &offer_id)
{
  ACE_TRACE ("ACE_URL_Locator_Request::describe_offer");
  
  this->id_ = offer_id;
  this->code_ = ACE_URL_Locator::DESCRIBE;
  return 0;
}

int
ACE_URL_Locator_Request::modify_offer (const ACE_WString &offer_id,
				       const ACE_WString *url,
				       const ACE_URL_Property_Seq &del,
				       const ACE_URL_Property_Seq &modify)
{
  ACE_TRACE ("ACE_URL_Locator_Request::modify_offer");

  ACE_NEW_RETURN (this->seq1_, ACE_URL_Property_Seq (del), -1);
  ACE_NEW_RETURN (this->seq2_, ACE_URL_Property_Seq (modify), -1);

  if (url != 0)
    this->url_ = *url;

  this->id_ = offer_id;
  this->code_ = ACE_URL_Locator::MODIFY;
  return 0;
}

#define ENCODE_UINT32(ADDR,LEN,V) \
   * (ACE_UINT32 *) (ADDR+LEN) = htonl (V); \
   LEN += sizeof (ACE_UINT32);

#define DECODE_UINT32(ADDR,LEN,V) \
   V = ntohl (* (ACE_UINT32 *) (ADDR+LEN)); \
   LEN += sizeof (ACE_UINT32);

size_t
ACE_URL_Locator_Request::encode (void)
{
  ACE_TRACE ("ACE_URL_Locator_Request::encode");

  size_t buf_size = this->bsize ();
  size_t total_length = 0;

  ACE_NEW_RETURN (this->buffer_, char [buf_size], 0);

  ENCODE_UINT32 (this->buffer_, total_length, buf_size);
  // Encode buffer size.

  ENCODE_UINT32 (this->buffer_, total_length, this->code_);
  // Encode Op code.

  ENCODE_UINT32 (this->buffer_, total_length, this->how_);
  // Encode selection criteria.
  
  ENCODE_UINT32 (this->buffer_, total_length, this->how_many_);
  // Encode number of offers interested.

  ENCODE_UINT32 (this->buffer_, total_length, this->valid_ptr_);
  // Encode valide pointer flag.

  if (this->seq1_ != 0)
    total_length += ace_array_encode (this->buffer_ + total_length, *this->seq1_);
  if (this->seq2_ != 0)
    total_length += ace_array_encode (this->buffer_ + total_length, *this->seq2_);
  if (this->offer_ != 0)
    total_length += this->offer_->encode (this->buffer_ + total_length);

  total_length += ACE_WString_Helper::encode (this->buffer_ + total_length,
					      &this->id_);
  total_length += ACE_WString_Helper::encode (this->buffer_ + total_length,
					      &this->url_);

  ACE_ASSERT (total_length == buf_size);
  return total_length;
}

size_t
ACE_URL_Locator_Request::decode (void *buffer)
{
  ACE_TRACE ("ACE_URL_Locator_Request::decode");

  if (buffer == 0)
    return 0;
  // Check if we have a buffer available.
  delete [] this->buffer_;
  this->buffer_ = (char*) buffer;

  size_t buf_size = 0;
  size_t total_length = 0;
  
  DECODE_UINT32 (this->buffer_, total_length, buf_size);
  // Decode length of buffer size first.

  DECODE_UINT32 (this->buffer_, total_length, this->code_);
  // Get the operation code.
  
  DECODE_UINT32 (this->buffer_, total_length, this->how_);
  // Decode selection criteria.
  
  DECODE_UINT32 (this->buffer_, total_length, this->how_many_);
  // Decode number of offers interested.

  DECODE_UINT32 (this->buffer_, total_length, this->valid_ptr_);
  // Decode valide pointer flag.

  if (this->valid_ptr_ | VALID_SEQ1 != 0)
    {
      ACE_NEW_RETURN (this->seq1_, ACE_URL_Property_Seq (1), 0);
      total_length += ace_array_decode (this->buffer_ + total_length, *this->seq1_);
    }
  if (this->valid_ptr_ | VALID_SEQ2 != 0)
    {
      ACE_NEW_RETURN (this->seq2_, ACE_URL_Property_Seq (1), 0);
      total_length += ace_array_decode (this->buffer_ + total_length, *this->seq2_);
    }
  if (this->valid_ptr_ | VALID_OFFER != 0)
    {
      ACE_NEW_RETURN (this->offer_, ACE_URL_Offer, 0);
      total_length += this->offer_->decode (this->buffer_ + total_length);
    }
  
  total_length += ACE_WString_Helper::decode (this->buffer_ + total_length);
  this->id_ = ACE_WString ((ACE_USHORT16 *) (this->buffer_ + total_length));
  total_length += ACE_WString_Helper::decode (this->buffer_ + total_length);
  this->url_ = ACE_WString ((ACE_USHORT16 *) (this->buffer_ + total_length));

  ACE_ASSERT (total_length == buf_size);
  return total_length;
}


size_t
ACE_URL_Locator_Request::bsize (void)
{
  ACE_TRACE ("ACE_URL_Locator_Request::bsize");

  size_t total_length = 5 * sizeof (ACE_UINT32);
  // There are 5 UINT32 variables at the beginning
  // of the buffer.  <buffer size>, <code>, <how>,
  // <how_many>, <valid_ptr>.

  this->valid_ptr_ = 0;
  // Check valid pointers and mark corresponding flag in <valid_prt>.

  if (this->seq1_ != 0)
    {
      this->valid_ptr_ |= VALID_SEQ1;
      total_length += ace_array_bsize (*this->seq1_);
    }
  if (this->seq2_ != 0)
    {
      this->valid_ptr_ |= VALID_SEQ2;
      total_length += ace_array_bsize (*this->seq2_);
    }
  if (this->offer_ != 0)
    {
      this->valid_ptr_ |= VALID_OFFER;
      total_length += this->offer_->bsize ();
    }
  
  total_length += ACE_WString_Helper::bsize (&this->id_);
  total_length += ACE_WString_Helper::bsize (&this->url_);

  return total_length;
}

void
ACE_URL_Locator_Request::dump (void) const
{
  //ACE_TRACE ("ACE_URL_Locator_Request::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  size_t i;
  
  switch (this->code_)
    {
    case ACE_URL_Locator::QUERY:
      ACE_DEBUG ((LM_DEBUG, "Query Request:\nSelection: "));
      switch (this->how_)
	{
	case ACE_URL_Locator::NONE:
	  ACE_DEBUG ((LM_DEBUG, "NONE.\n"));
	  break;
	case ACE_URL_Locator::SOME:
	  ACE_DEBUG ((LM_DEBUG, "SOME.\n"));
	  break;
	case ACE_URL_Locator::ALL:
	  ACE_DEBUG ((LM_DEBUG, "ALL.\n"));
	  break;
	default:
	  ACE_DEBUG ((LM_DEBUG, "Invalid Selection??\n"));
	  break;
	}
      ACE_DEBUG ((LM_DEBUG, "At most %d reply.\n", this->how_many_));
      for (i = 0; i < this->seq1_->size (); i++)
      (*this->seq1_)[i].dump ();
      break;
    case ACE_URL_Locator::EXPORT:
      ACE_DEBUG ((LM_DEBUG, "Export Request:\n"));
      break;
    case ACE_URL_Locator::WITHDRAW:
      ACE_DEBUG ((LM_DEBUG, "Withdraw Request:\n"));
      break;
    case ACE_URL_Locator::DESCRIBE:
      ACE_DEBUG ((LM_DEBUG, "Describe Request:\n"));
      break;
    case ACE_URL_Locator::MODIFY:
      ACE_DEBUG ((LM_DEBUG, "Modify Request:\n"));
      break;
    default:
      // Invalid data encountered. Stop encoding now.
      ACE_DEBUG ((LM_DEBUG, "Invalid Request.\n"));
      break;
    }
  
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

int
ACE_URL_Locator_Reply::status_reply (u_int op, int result)
{
  ACE_TRACE ("ACE_URL_Locator_Reply::status_reply");

  this->code_ = op;
  this->status_ = result;
  return 0;
}

int
ACE_URL_Locator_Reply::query_reply (int result, size_t num,
	     const ACE_URL_Offer_Seq &offers)
{
  ACE_TRACE ("ACE_URL_Locator_Reply::query_reply");
  
  this->code_ = ACE_URL_Locator::QUERY;
  this->status_ = result;
  ACE_NEW_RETURN (this->offers_, ACE_URL_Offer_Seq (offers), -1);
  return 0;
}

int
ACE_URL_Locator_Reply::describe_reply (int result,
		const ACE_URL_Offer &offer)
{
  ACE_TRACE ("ACE_URL_Locator_Reply::describe_reply");

  this->code_ = ACE_URL_Locator::DESCRIBE;
  this->status_ = result;
  ACE_NEW_RETURN (this->offer_, ACE_URL_Offer (offer), -1);
  return 0;
}

size_t
ACE_URL_Locator_Reply::encode (void)
{
  ACE_TRACE ("ACE_URL_Locator_Reply::encode");

  size_t buf_size = this->bsize ();
  size_t total_length = 0;

  ACE_NEW_RETURN (this->buffer_, char [buf_size], 0);

  ENCODE_UINT32 (this->buffer_, total_length, buf_size);
  // Encode buffer size.

  ENCODE_UINT32 (this->buffer_, total_length, this->code_);
  // Encode Op code.

  ENCODE_UINT32 (this->buffer_, total_length, this->status_);
  // Encode Op result status.

  ENCODE_UINT32 (this->buffer_, total_length, this->num_offers_);
  // Encode number of offers in this->offers_.

  ENCODE_UINT32 (this->buffer_, total_length, this->valid_ptr_);
  // Encode valid pointers mask.

  // Encode request for network communication.  If succeed,
  // returns the size of the buffer, otherwise, return 0.
  return 0;
}
 
size_t
ACE_URL_Locator_Reply::decode (void *buffer)
{
  ACE_TRACE ("ACE_URL_Locator_Reply::decode");

  // Restore from network data.  Returns size of the buffer
  // if succeed, 0 otherwise.
  return 0;
}
 
size_t
ACE_URL_Locator_Reply::bsize (void)
{
  ACE_TRACE ("ACE_URL_Locator_Reply:bsize");

  size_t total_length = 5 * sizeof (ACE_UINT32);
  // size for 5 ACE_UINT32 objects: <buffer size>, <code_>,
  // <status_>, <num_offers_>, and <valid_ptr_>.

  this->valid_ptr_ = 0;
  if (this->offer_ != 0)
    {
      this->valid_ptr_ |= VALID_OFFER;
      total_length += this->offer_->bsize ();
    }
  if (this->offers_ != 0)
    {
      this->valid_ptr_ |= VALID_OFFERS;
      total_length += ace_array_bsize (this->offers_);
    }
  return total_length;
}

void
ACE_URL_Locator_Reply::dump (void) const
{
  //ACE_TRACE ("ACE_URL_Locator_Reply::dump");

}
#endif /* ACE_LOCATOR_REQUEST_REPLY_C */
