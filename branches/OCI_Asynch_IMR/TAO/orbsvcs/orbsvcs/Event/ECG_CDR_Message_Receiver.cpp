// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Event/ECG_CDR_Message_Receiver.h"
#include "orbsvcs/Event/ECG_CDR_Message_Sender.h"

#include "tao/Exception.h"

#include "ace/SOCK_Dgram.h"
#include "ace/ACE.h"
#include "ace/OS_NS_string.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_CDR_Message_Receiver.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ECG_CDR_Processor::~TAO_ECG_CDR_Processor (void)
{
}
// ****************************************************************

TAO_ECG_UDP_Request_Entry::~TAO_ECG_UDP_Request_Entry (void)
{
  if (this->own_received_fragments_)
    {
      this->own_received_fragments_ = 0;
      delete[] this->received_fragments_;
    }
}

TAO_ECG_UDP_Request_Entry::
TAO_ECG_UDP_Request_Entry (CORBA::Boolean byte_order,
                           CORBA::ULong request_id,
                           CORBA::ULong request_size,
                           CORBA::ULong fragment_count)
  : byte_order_ (byte_order)
  , request_id_ (request_id)
  , request_size_ (request_size)
  , fragment_count_ (fragment_count)
{
  ACE_CDR::grow (&this->payload_, this->request_size_);
  this->payload_.wr_ptr (request_size_);

  this->received_fragments_ = this->default_received_fragments_;
  this->own_received_fragments_ = 0;
  const int bits_per_ulong = sizeof(CORBA::ULong) * CHAR_BIT;
  this->received_fragments_size_ =
    this->fragment_count_ / bits_per_ulong + 1;
  if (this->received_fragments_size_ > ECG_DEFAULT_FRAGMENT_BUFSIZ)
    {
      ACE_NEW (this->received_fragments_,
               CORBA::ULong[this->received_fragments_size_]);
      this->own_received_fragments_ = 1;
    }

  for (CORBA::ULong i = 0; i < this->received_fragments_size_; ++i)
    this->received_fragments_[i] = 0;
  CORBA::ULong idx = this->fragment_count_ / bits_per_ulong;
  CORBA::ULong bit = this->fragment_count_ % bits_per_ulong;
  this->received_fragments_[idx] = (0xFFFFFFFF << bit);
}

int
TAO_ECG_UDP_Request_Entry::validate_fragment (CORBA::Boolean byte_order,
                                              CORBA::ULong request_size,
                                              CORBA::ULong fragment_size,
                                              CORBA::ULong fragment_offset,
                                              CORBA::ULong /* fragment_id */,
                                              CORBA::ULong fragment_count) const
{
  if (byte_order != this->byte_order_
      || request_size != this->request_size_
      || fragment_count != this->fragment_count_)
    return 0;

  if (fragment_offset >= request_size
      || fragment_offset + fragment_size > request_size)
    return 0;

  return 1;
}

int
TAO_ECG_UDP_Request_Entry::test_received (CORBA::ULong fragment_id) const
{
  // Assume out-of-range fragments as received, so they are dropped...
  if (fragment_id > this->fragment_count_)
    return 1;

  const int bits_per_ulong = sizeof(CORBA::ULong) * CHAR_BIT;
  CORBA::ULong idx = fragment_id / bits_per_ulong;
  CORBA::ULong bit = fragment_id % bits_per_ulong;
  return ACE_BIT_ENABLED (this->received_fragments_[idx], 1<<bit);
}

void
TAO_ECG_UDP_Request_Entry::mark_received (CORBA::ULong fragment_id)
{
  // Assume out-of-range fragments as received, so they are dropped...
  if (fragment_id > this->fragment_count_)
    return;

  const int bits_per_ulong = sizeof(CORBA::ULong) * CHAR_BIT;
  CORBA::ULong idx = fragment_id / bits_per_ulong;
  CORBA::ULong bit = fragment_id % bits_per_ulong;
  ACE_SET_BITS (this->received_fragments_[idx], 1<<bit);
}

int
TAO_ECG_UDP_Request_Entry::complete (void) const
{
  for (CORBA::ULong i = 0;
       i < this->received_fragments_size_;
       ++i)
    {
      if (this->received_fragments_[i] != 0xFFFFFFFF)
        return 0;
    }
  return 1;
}

char*
TAO_ECG_UDP_Request_Entry::fragment_buffer (CORBA::ULong fragment_offset)
{
  return this->payload_.rd_ptr () + fragment_offset;
}
// ****************************************************************

int
TAO_ECG_CDR_Message_Receiver::Requests::init (size_t size,
                                              size_t min_purge_count)
{
  // Already initialized.
  if (this->fragmented_requests_)
    return -1;

  ACE_NEW_RETURN (this->fragmented_requests_,
                  TAO_ECG_UDP_Request_Entry*[size],
                  -1);

  this->size_ = size;
  this->id_range_low_ = 0;
  this->id_range_high_ = size - 1;
  this->min_purge_count_ = min_purge_count;

  for (size_t i = 0; i < size; ++i)
    {
      this->fragmented_requests_[i] = 0;
    }

  return 0;
}

TAO_ECG_CDR_Message_Receiver::Requests::~Requests (void)
{
  for (size_t i = 0; i < this->size_; ++i)
    {
      TAO_ECG_UDP_Request_Entry* request =
        this->fragmented_requests_[i];

      if (request != &TAO_ECG_CDR_Message_Receiver::Request_Completed_)
        delete request;
    }

  delete [] this->fragmented_requests_;

  this->fragmented_requests_ = 0;
  this->size_ = 0;
  this->id_range_low_ = 0;
  this->id_range_high_ = 0;
}

TAO_ECG_UDP_Request_Entry **
TAO_ECG_CDR_Message_Receiver::Requests::get_request (CORBA::ULong request_id)
{
  if (request_id < this->id_range_low_)
    // <request_id> is below the current range.
    {
      return 0;
    }

  if (request_id > this->id_range_high_)
    // <request_id> is above the current range - need to shift the range
    // to include it.
    {
      CORBA::ULong new_slots_needed = request_id - this->id_range_high_;

      if (new_slots_needed < this->min_purge_count_)
        new_slots_needed = this->min_purge_count_;

      if (new_slots_needed > this->size_)
        // Shifting the range by more than the size of array.
        {
          this->purge_requests (this->id_range_low_, this->id_range_high_);
          this->id_range_high_ = request_id;
          this->id_range_low_ = request_id - this->size_ + 1;
        }
      else
        {
          this->purge_requests (this->id_range_low_,
                                this->id_range_low_ + new_slots_needed - 1);
          this->id_range_high_ += new_slots_needed;
          this->id_range_low_ += new_slots_needed;
        }
    }

  // Return array location for <request_id>.
  int index = request_id % this->size_;
  return this->fragmented_requests_ + index;
}


void
TAO_ECG_CDR_Message_Receiver::Requests::purge_requests (
                                            CORBA::ULong purge_first,
                                            CORBA::ULong purge_last)
{
  for (CORBA::ULong i = purge_first; i <= purge_last; ++i)
    {
      size_t index = i % this->size_;
      if (this->fragmented_requests_[index]
          != &TAO_ECG_CDR_Message_Receiver::Request_Completed_)
        {
          delete this->fragmented_requests_[index];
        }
      this->fragmented_requests_[index] = 0;
    }
}

// ****************************************************************

TAO_ECG_UDP_Request_Entry
TAO_ECG_CDR_Message_Receiver::Request_Completed_ (0, 0, 0, 0);

int
TAO_ECG_CDR_Message_Receiver::handle_input (
                                 ACE_SOCK_Dgram& dgram,
                                 TAO_ECG_CDR_Processor *cdr_processor)
{
  char nonaligned_header[TAO_ECG_CDR_Message_Sender::ECG_HEADER_SIZE
                        + ACE_CDR::MAX_ALIGNMENT];
  char *header_buf = ACE_ptr_align_binary (nonaligned_header,
                                           ACE_CDR::MAX_ALIGNMENT);
  char nonaligned_data[ACE_MAX_DGRAM_SIZE + ACE_CDR::MAX_ALIGNMENT];
  char *data_buf = ACE_ptr_align_binary (nonaligned_data,
                                         ACE_CDR::MAX_ALIGNMENT);

  // Read the message from dgram.

  const int iovcnt = 2;
  iovec iov[iovcnt];
  iov[0].iov_base = header_buf;
  iov[0].iov_len  = TAO_ECG_CDR_Message_Sender::ECG_HEADER_SIZE;
  iov[1].iov_base = data_buf;
  iov[1].iov_len  = ACE_MAX_DGRAM_SIZE;

  ACE_INET_Addr from;
  ssize_t n = dgram.recv (iov, iovcnt, from);

  if (n == -1)
    {
      if (errno == EWOULDBLOCK)
        return 0;

      ORBSVCS_ERROR_RETURN ((LM_ERROR, "Error reading mcast fragment (%m).\n"),
                        -1);
    }

  if (n == 0)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR, "Trying to read mcast fragment: "
                                  "read 0 bytes from socket.\n"),
                        0);
    }

  if (n < TAO_ECG_CDR_Message_Sender::ECG_HEADER_SIZE)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR, "Trying to read mcast fragment: "
                                   "# of bytes read < mcast header size.\n"),
                         -1);
    }

  u_int crc = 0;

  if (this->check_crc_)
    {
      iov[1].iov_len = n - iov[0].iov_len;
      iov[0].iov_len -= 4;  // don't include crc

      crc = ACE::crc32 (iov, 2);
    }
  // Check whether the message is a loopback message.
  if (this->ignore_from_.get () != 0
      && this->ignore_from_->is_loopback (from))
    {
      return 0;
    }

  // Decode and validate mcast header.
  Mcast_Header header;
  if (header.read (header_buf, n, this->check_crc_) == -1)
    return -1;

  if ( this->check_crc_ && header.crc != crc)
    {
      static unsigned int err_count = 0;
      ORBSVCS_ERROR ((LM_ERROR,
                  "******************************\n"));

      ORBSVCS_ERROR ((LM_ERROR,
                  "ERROR DETECTED\n"));

      if (crc == 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Sending process may not have computed CRC\n"));
        }
      else
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      " NETWORK CRC CHECKSUM FAILED\n"));
        }

      ORBSVCS_ERROR ((LM_ERROR,
                  "Message was received from [%s:%s:%d]\n",
                  from.get_host_name (),
                  from.get_host_addr (),
                  from.get_port_number()));

      ORBSVCS_ERROR ((LM_ERROR,
                  "Num errors = %d\n",
                  ++err_count));
      ORBSVCS_ERROR ((LM_ERROR,
                  "This is a bad thing. Attempting to ignore ..\n"));

      return 0;
    }

  // Process received data.
  if (header.fragment_count == 1)
    {
      // Update <request_map_> to mark this request as completed. (Not
      // needed if we don't care about duplicates.)
      int const result = this->mark_received (from, header.request_id);
      if (result != 1)
        return result;

      TAO_InputCDR cdr (data_buf, header.request_size, header.byte_order);
      if (cdr_processor->decode (cdr) == -1)
        return -1;
      else
        return 1;
    }

  return this->process_fragment (from, header, data_buf, cdr_processor);
}

int
TAO_ECG_CDR_Message_Receiver::mark_received (const ACE_INET_Addr &from,
                                             CORBA::ULong request_id)
{
  // ACE_GUARD_RETURN (ACE_Lock, guard, *this->lock_, -1);

  Request_Map::ENTRY * entry = this->get_source_entry (from);
  if (!entry)
    return -1;

  TAO_ECG_UDP_Request_Entry ** request =
    entry->int_id_->get_request (request_id);

  if (request == 0)
    {
      ORBSVCS_DEBUG ((LM_WARNING, "Received mcast request with sequence"
                              "below currently expected range.\n"));
      return 0;
    }
  if (*request == &Request_Completed_)
    {
      ORBSVCS_DEBUG ((LM_INFO, "Received duplicate mcast fragment. "
                           "(Request already complete).\n"));
      return 0;
    }
  if (*request != 0)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR, "Inconsistent fragments for "
                                   "mcast request.\n"),
                         -1);
    }

  *request = &Request_Completed_;
  return 1;
}

int
TAO_ECG_CDR_Message_Receiver::process_fragment (
                                   const ACE_INET_Addr &from,
                                   const Mcast_Header &header,
                                   char * data_buf,
                                   TAO_ECG_CDR_Processor *cdr_processor)
{
  // ACE_GUARD_RETURN (ACE_Lock, guard, *this->lock_, -1);

  Request_Map::ENTRY * source_entry = this->get_source_entry (from);
  if (!source_entry)
    return -1;

  TAO_ECG_UDP_Request_Entry ** request =
    source_entry->int_id_->get_request (header.request_id);

  if (request == 0)
    {
      ORBSVCS_DEBUG ((LM_WARNING, "Received mcast request with sequence "
                              "below currently expected range.\n"));
      return 0;
    }
  if (*request == &Request_Completed_)
    {
      ORBSVCS_DEBUG ((LM_INFO, "Received duplicate mcast fragment. "
                           "(Request already complete).\n"));
      return 0;
    }
  if (*request == 0)
    // Entry for this request has not yet been allocated.
    {
      ACE_NEW_RETURN (*request,
                      TAO_ECG_UDP_Request_Entry (header.byte_order,
                                                 header.request_id,
                                                 header.request_size,
                                                 header.fragment_count),
                      -1);
    }

  // Validate the fragment.
  if ((*request)->validate_fragment (header.byte_order,
                                     header.request_size,
                                     header.fragment_size,
                                     header.fragment_offset,
                                     header.fragment_id,
                                     header.fragment_count) == 0)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         "Received invalid mcast fragment.\n"),
                        -1);
    }

  // Check whether this fragment was already received.
  if ((*request)->test_received (header.fragment_id) == 1)
    {
      ORBSVCS_DEBUG ((LM_INFO, "Received duplicate mcast fragment.\n"));
      return 0;
    }

  // Add the fragment to the request entry.
  (*request)->mark_received (header.fragment_id);
  ACE_OS::memcpy ((*request)->fragment_buffer (header.fragment_offset),
                  data_buf,
                  header.fragment_size);

  // The request is not yet complete.
  if (!(*request)->complete ())
    {
      return 0;
    }

  // The request is complete - decode it.
  TAO_InputCDR cdr ((*request)->fragment_buffer (0),
                    header.request_size,
                    header.byte_order);

  if (cdr_processor->decode (cdr) == -1)
    return -1;

  delete *request;
  *request = &Request_Completed_;
  return 1;
}

TAO_ECG_CDR_Message_Receiver::Request_Map::ENTRY*
TAO_ECG_CDR_Message_Receiver::get_source_entry (const ACE_INET_Addr &from)
{
  // Get the entry for <from> from the <request_map_>.
  Request_Map::ENTRY * entry = 0;

  if (this->request_map_.find (from, entry) == -1)
    {
      // Create an entry if one doesn't exist.
      Requests *requests = 0;
      ACE_NEW_RETURN (requests,
                      Requests,
                      0);
      auto_ptr<Requests> requests_aptr (requests);

      if (requests->init (this->max_requests_, this->min_purge_count_) == -1
          || this->request_map_.bind (from, requests, entry) == -1)
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR, "Unable to create hash map "
                                       "entry for a new request.\n"),
                            0);
        }
      requests_aptr.release ();
    }

  return entry;
}

void
TAO_ECG_CDR_Message_Receiver::shutdown (void)
{
  // ACE_GUARD (ACE_Lock, guard, *this->lock_);

  Request_Map::iterator end = this->request_map_.end ();
  for (Request_Map::iterator i =  this->request_map_.begin ();
       i != end;
       ++i)
    {
      delete (*i).int_id_;
      (*i).int_id_ = 0;
    }

  this->ignore_from_.reset ();
}

// ****************************************************************
int
TAO_ECG_CDR_Message_Receiver::Mcast_Header::read (char *header,
                                                  size_t bytes_received,
                                                  CORBA::Boolean checkcrc)
{
  // Decode.
  this->byte_order = header[0];
  if(this->byte_order != 0 && this->byte_order != 1)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR, "Reading mcast packet header: byte "
                                   "order is neither 0 nor 1, it is %d.\n",
                         this->byte_order),
                        -1);
    }

  TAO_InputCDR header_cdr (header,
                           TAO_ECG_CDR_Message_Sender::ECG_HEADER_SIZE,
                           byte_order);
  CORBA::Boolean unused;
  CORBA::Octet a, b, c;
  if (!header_cdr.read_boolean (unused)
      || !header_cdr.read_octet (a)
      || !header_cdr.read_octet (b)
      || !header_cdr.read_octet (c)
      || a != 'A' || b != 'B' || c != 'C')
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR, "Error reading magic bytes "
                                   "in mcast packet header.\n"),
                        -1);
    }

  if (!header_cdr.read_ulong (this->request_id)
      || !header_cdr.read_ulong (this->request_size)
      || !header_cdr.read_ulong (this->fragment_size)
      || !header_cdr.read_ulong (this->fragment_offset)
      || !header_cdr.read_ulong (this->fragment_id)
      || !header_cdr.read_ulong (this->fragment_count))
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                        "Error decoding mcast packet header.\n"),
                        -1);
    }

  if (checkcrc)
    {
      CORBA::Octet padding[4];
      header_cdr.read_octet_array (padding, 4);

      unsigned char *crcparts = (unsigned char *)(&this->crc);

      for (int cnt=0; cnt != 4; ++cnt)
        {
          crcparts[cnt] = padding[cnt];
        }

      this->crc = ACE_NTOHL (this->crc);
    }

  // Validate.
  size_t const data_bytes_received =
    bytes_received - TAO_ECG_CDR_Message_Sender::ECG_HEADER_SIZE;

  if (this->request_size < this->fragment_size
      || this->fragment_offset >= this->request_size
      || this->fragment_id >= this->fragment_count
      || (this->fragment_count == 1
          && (this->fragment_size != this->request_size
              || this->request_size != data_bytes_received)))
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                        "Invalid mcast fragment: "
                        "inconsistent header fields.\n"),
                        -1);
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
