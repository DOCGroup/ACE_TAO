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
  this->code_ = ACE_URL_Locator_Request::QUERY;
  return 0;    
}

int
ACE_URL_Locator_Request::export_offer (const ACE_URL_Offer &offer)
{
  ACE_TRACE ("ACE_URL_Locator_Request::export_offer");
  
  ACE_NEW_RETURN (this->offer_, ACE_URL_Offer (offer), -1);
  this->code_ = ACE_URL_Locator_Request::EXPORT;
  return 0;
}
  
int
ACE_URL_Locator_Request::withdraw_offer (const ACE_WString &offer_id)
{
  ACE_TRACE ("ACE_URL_Locator_Request::withdraw_offer");

  this->id_ = offer_id;
  this->code_ = ACE_URL_Locator_Request::WITHDRAW;
  return 0;
}

int
ACE_URL_Locator_Request::describe_offer (const ACE_WString &offer_id)
{
  ACE_TRACE ("ACE_URL_Locator_Request::describe_offer");
  
  this->id_ = offer_id;
  this->code_ = ACE_URL_Locator_Request::DESCRIBE;
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
  this->code_ = ACE_URL_Locator_Request::MODIFY;
  return 0;
}

#define ENCODE_UINT32(ADDR,LEN,V) \
   * (ACE_UINT32 *) (ADDR+LEN) = htonl (V); \
   total_length += sizeof (ACE_UINT32);

#define DECODE_UINT32(ADDR,LEN,V) \
   V = ntohl (* (ACE_UINT32 *) (ADDR+LEN)); \
   total_length += sizeof (ACE_UINT32);

size_t
ACE_URL_Locator_Request::encode (void)
{
  ACE_TRACE ("ACE_URL_Locator_Request::encode");

  size_t buf_size = this->bsize ();

  ACE_NEW_RETURN (this->buffer_, char [buf_size], 0);
  ((ACE_UINT32 *) this->buffer_)[0] = htonl (buf_size);
  ((ACE_UINT32 *) this->buffer_)[1] = htonl (this->code_);
  size_t total_length = 2 * sizeof (ACE_UINT32);
  
  switch (this->code_)
    {
    case ACE_URL_Locator_Request::QUERY:
      ENCODE_UINT32 (this->buffer_, total_length, this->how_);
      ENCODE_UINT32 (this->buffer_, total_length, this->how_many_);
      total_length += ace_array_encode (this->buffer_ + total_length, *this->seq1_);
      break;
    case ACE_URL_Locator_Request::EXPORT:
      total_length += this->offer_->encode (this->buffer_ + total_length);
      break;
    case ACE_URL_Locator_Request::WITHDRAW:
    case ACE_URL_Locator_Request::DESCRIBE:
      total_length += ACE_WString_Helper::encode (this->buffer_ + total_length,
						  &this->id_);
      break;
    case ACE_URL_Locator_Request::MODIFY:
      total_length += ACE_WString_Helper::encode (this->buffer_ + total_length,
						  &this->id_);
      total_length += ACE_WString_Helper::encode (this->buffer_ + total_length,
						  &this->url_);
      total_length += ace_array_encode (this->buffer_ + total_length, *this->seq1_);
      total_length += ace_array_encode (this->buffer_ + total_length, *this->seq2_);
      break;
    default:
      // Invalid data encountered. Stop encoding now.
      return 0;
      break;
    }
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

  size_t buf_size = ntohl (((ACE_UINT32 *) this->buffer_)[0]);
  this->code_ = ntohl (((ACE_UINT32 *) this->buffer_)[1]);
  // Get the operation code.
  
  size_t total_length = 2 * sizeof (ACE_UINT32);

  switch (this->code_)
    {
    case ACE_URL_Locator_Request::QUERY:
      DECODE_UINT32 (this->buffer_, total_length, this->how_);
      DECODE_UINT32 (this->buffer_, total_length, this->how_many_);
      delete this->seq1_;
      ACE_NEW_RETURN (this->seq1_, ACE_URL_Property_Seq (1), 0);
      total_length += ace_array_decode (this->buffer_ + total_length, *this->seq1_);
      break;
    case ACE_URL_Locator_Request::EXPORT:
      delete this->offer_;
      ACE_NEW_RETURN (this->offer_, ACE_URL_Offer, 0);
      total_length += this->offer_->decode (this->buffer_);
      break;
    case ACE_URL_Locator_Request::WITHDRAW:
    case ACE_URL_Locator_Request::DESCRIBE:
      total_length = ACE_WString_Helper::decode (this->buffer_ + total_length);
      this->id_ = ACE_WString ((ACE_USHORT16 *) (this->buffer_ + total_length));
      break;
    case ACE_URL_Locator_Request::MODIFY:
      total_length += ACE_WString_Helper::decode (this->buffer_ + total_length);
      this->id_ = ACE_WString ((ACE_USHORT16 *) (this->buffer_ + total_length));
      total_length += ACE_WString_Helper::decode (this->buffer_ + total_length);
      this->url_ = ACE_WString ((ACE_USHORT16 *) (this->buffer_ + total_length));
      ACE_NEW_RETURN (this->seq1_, ACE_URL_Property_Seq (1), 0);
      ACE_NEW_RETURN (this->seq2_, ACE_URL_Property_Seq (1), 0);
      total_length += ace_array_decode (this->buffer_ + total_length, *this->seq1_);
      total_length += ace_array_decode (this->buffer_ + total_length, *this->seq2_);
      break;
    default:
      // Invalid data encountered. Stop encoding now.
      return 0;
      break;
    }
}


size_t
ACE_URL_Locator_Request::bsize (void) const
{
  ACE_TRACE ("ACE_URL_Locator_Request::bsize");

  size_t total_length = 2 * sizeof (ACE_UINT32);
  
  switch (this->code_)
    {
    case ACE_URL_Locator_Request::QUERY:
      total_length += 2 * sizeof (ACE_UINT32);
      total_length += ace_array_bsize (*this->seq1_);
      break;
    case ACE_URL_Locator_Request::EXPORT:
      total_length += this->offer_->bsize ();
      break;
    case ACE_URL_Locator_Request::WITHDRAW:
    case ACE_URL_Locator_Request::DESCRIBE:
      total_length += ACE_WString_Helper::bsize (&this->id_);
      break;
    case ACE_URL_Locator_Request::MODIFY:
      total_length += ACE_WString_Helper::bsize (&this->id_);
      total_length += ACE_WString_Helper::bsize (&this->url_);
      total_length += ace_array_bsize (*this->seq1_);
      total_length += ace_array_bsize (*this->seq2_);
      break;
    default:
      // Invalid data encountered. Stop encoding now.
      return 0;
      break;
    }
  return total_length;
}

void
ACE_URL_Locator_Request::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  
  switch (this->code_)
    {
    case ACE_URL_Locator_Request::QUERY:
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
      this->seq1_->dump ();
      break;
    case ACE_URL_Locator_Request::EXPORT:
      ACE_DEBUG ((LM_DEBUG, "Export Request:\n"));
      break;
    case ACE_URL_Locator_Request::WITHDRAW:
      ACE_DEBUG ((LM_DEBUG, "Withdraw Request:\n"));
      break;
    case ACE_URL_Locator_Request::DESCRIBE:
      ACE_DEBUG ((LM_DEBUG, "Describe Request:\n"));
      break;
    case ACE_URL_Locator_Request::MODIFY:
      ACE_DEBUG ((LM_DEBUG, "Modify Request:\n"));
      break;
    default:
      // Invalid data encountered. Stop encoding now.
      ACE_DEBUG ((LM_DEBUG, "Invalid Request.\n"));
      break;
    }
  
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#endif /* ACE_LOCATOR_REQUEST_REPLY_C */
