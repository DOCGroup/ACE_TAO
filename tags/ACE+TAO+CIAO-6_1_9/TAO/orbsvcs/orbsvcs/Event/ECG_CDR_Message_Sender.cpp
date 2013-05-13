// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Event/ECG_CDR_Message_Sender.h"
#include "tao/CDR.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/ACE.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_CDR_Message_Sender.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_ECG_CDR_Message_Sender::init (
      TAO_ECG_Refcounted_Endpoint endpoint_rptr)
{
  if (endpoint_rptr.get () == 0
      || endpoint_rptr->dgram ().get_handle () == ACE_INVALID_HANDLE)
    {
      ORBSVCS_ERROR ((LM_ERROR, "TAO_ECG_CDR_Message_Sender::init(): "
                            "nil or unitialized endpoint argument.\n"));
      throw CORBA::INTERNAL ();
    }

  this->endpoint_rptr_ = endpoint_rptr;
}

void
TAO_ECG_CDR_Message_Sender::send_message  (const TAO_OutputCDR &cdr,
                                           const ACE_INET_Addr &addr)
{
  if (this->endpoint_rptr_.get () == 0)
    {
      ORBSVCS_ERROR ((LM_ERROR, "Attempt to invoke send_message() "
                            "on non-initialized sender object.\n"));
      throw CORBA::INTERNAL ();
    }

  CORBA::ULong max_fragment_payload = this->mtu () -
    TAO_ECG_CDR_Message_Sender::ECG_HEADER_SIZE;
  // ACE_ASSERT (max_fragment_payload != 0);

#if defined (ACE_HAS_BROKEN_DGRAM_SENDV)
  const int TAO_WRITEV_MAX = ACE_IOV_MAX - 1;
#else
  const int TAO_WRITEV_MAX = ACE_IOV_MAX;
#endif /* ACE_HAS_BROKEN_DGRAM_SENDV */
  iovec iov[TAO_WRITEV_MAX];

  CORBA::ULong total_length;
  CORBA::ULong fragment_count =
    this->compute_fragment_count (cdr.begin (),
                                  cdr.end (),
                                  TAO_WRITEV_MAX,
                                  max_fragment_payload,
                                  total_length);

  CORBA::ULong request_id = this->endpoint_rptr_->next_request_id ();

  // Reserve the first iovec for the header...
  int iovcnt = 1;
  CORBA::ULong fragment_id = 0;
  CORBA::ULong fragment_offset = 0;
  CORBA::ULong fragment_size = 0;
  for (const ACE_Message_Block* b = cdr.begin ();
       b != cdr.end ();
       b = b->cont ())
    {
      CORBA::ULong l = b->length ();

      char* rd_ptr = b->rd_ptr ();

      iov[iovcnt].iov_base = rd_ptr;
      iov[iovcnt].iov_len  = l;
      fragment_size += l;
      ++iovcnt;
      while (fragment_size > max_fragment_payload)
        {
          // This fragment is full, we have to send it...

          // First adjust the last iov entry:
          CORBA::ULong last_mb_length =
            max_fragment_payload - (fragment_size - l);
          iov[iovcnt - 1].iov_len = last_mb_length;

          this->send_fragment (addr,
                               request_id,
                               total_length,
                               max_fragment_payload,
                               fragment_offset,
                               fragment_id,
                               fragment_count,
                               iov,
                               iovcnt);
          ++fragment_id;
          fragment_offset += max_fragment_payload;

          // Reset, but don't forget that the last Message_Block
              // may need to be sent in multiple fragments..
          l -= last_mb_length;
          rd_ptr += last_mb_length;
          iov[1].iov_base = rd_ptr;
          iov[1].iov_len = l;
          fragment_size = l;
          iovcnt = 2;
        }
      if (fragment_size == max_fragment_payload)
        {
          // We filled a fragment, but this time it was filled
          // exactly, the treatment is a little different from the
          // loop above...
          this->send_fragment (addr,
                               request_id,
                               total_length,
                               max_fragment_payload,
                               fragment_offset,
                               fragment_id,
                               fragment_count,
                               iov,
                               iovcnt);
          ++fragment_id;
          fragment_offset += max_fragment_payload;

          iovcnt = 1;
          fragment_size = 0;
        }
      if (iovcnt == TAO_WRITEV_MAX)
        {
          // Now we ran out of space in the iovec, we must send a
          // fragment to work around that....
          this->send_fragment (addr,
                               request_id,
                               total_length,
                               fragment_size,
                               fragment_offset,
                               fragment_id,
                               fragment_count,
                               iov,
                               iovcnt);
          ++fragment_id;
          fragment_offset += fragment_size;

          iovcnt = 1;
          fragment_size = 0;
        }
    }
  // There is something left in the iovvec that we must send
  // also...
  if (iovcnt != 1)
    {
      // Now we ran out of space in the iovec, we must send a
      // fragment to work around that....
      this->send_fragment (addr,
                           request_id,
                           total_length,
                           fragment_size,
                           fragment_offset,
                           fragment_id,
                           fragment_count,
                           iov,
                           iovcnt);
      ++fragment_id;
      fragment_offset += fragment_size;

      // reset, not needed here...
      // iovcnt = 1;
      // fragment_size = 0;
    }
  // ACE_ASSERT (total_length == fragment_offset);
  // ACE_ASSERT (fragment_id == fragment_count);

}


void
TAO_ECG_CDR_Message_Sender::send_fragment (const ACE_INET_Addr &addr,
                                           CORBA::ULong request_id,
                                           CORBA::ULong request_size,
                                           CORBA::ULong fragment_size,
                                           CORBA::ULong fragment_offset,
                                           CORBA::ULong fragment_id,
                                           CORBA::ULong fragment_count,
                                           iovec iov[],
                                           int iovcnt)
{
  CORBA::ULong header[TAO_ECG_CDR_Message_Sender::ECG_HEADER_SIZE
                     / sizeof(CORBA::ULong)
                     + ACE_CDR::MAX_ALIGNMENT];
  char* buf = reinterpret_cast<char*> (header);
  TAO_OutputCDR cdr (buf, sizeof(header));
  cdr.write_boolean (TAO_ENCAP_BYTE_ORDER);
  // Insert some known values in the padding bytes, so we can smoke
  // test the message on the receiving end.
  cdr.write_octet ('A'); cdr.write_octet ('B'); cdr.write_octet ('C');
  cdr.write_ulong (request_id);
  cdr.write_ulong (request_size);
  cdr.write_ulong (fragment_size);
  cdr.write_ulong (fragment_offset);
  cdr.write_ulong (fragment_id);
  cdr.write_ulong (fragment_count);
  CORBA::Octet padding[4];


  // MRH
  if (checksum_)
  {
    // Compute CRC
     iov[0].iov_base = cdr.begin ()->rd_ptr ();
     iov[0].iov_len  = cdr.begin ()->length ();
     unsigned int crc = 0;
     unsigned char *crc_parts = (unsigned char *)(&crc);
     if (iovcnt > 1)
       {
         crc = ACE::crc32 (iov, iovcnt);
         crc = ACE_HTONL (crc);
       }
     for (int cnt=0; cnt<4; ++cnt)
       {
         padding[cnt] = crc_parts[cnt];
       }
   }
   else
   {
     for (int cnt=0; cnt<4; ++cnt)
     {
       padding[cnt] = 0;
     }
   }
   //End MRH
  cdr.write_octet_array (padding, 4);

  iov[0].iov_base = cdr.begin ()->rd_ptr ();
  iov[0].iov_len  = cdr.begin ()->length ();

  ssize_t n = this->dgram ().send (iov,
                                   iovcnt,
                                   addr);
  size_t expected_n = 0;
  for (int i = 0; i < iovcnt; ++i)
    expected_n += iov[i].iov_len;
  if (n > 0 && size_t(n) != expected_n)
    {
      ORBSVCS_ERROR ((LM_ERROR, ("Sent only %d out of %d bytes "
                              "for mcast fragment.\n"),
                  n,
                  expected_n));
    }

  if (n == -1)
    {
      if (errno == EWOULDBLOCK)
        {
          ORBSVCS_ERROR ((LM_ERROR, "Send of mcast fragment failed (%m).\n"));
          // @@ TODO Use a Event Channel specific exception
          throw CORBA::COMM_FAILURE ();
        }
      else
        {
          ORBSVCS_DEBUG ((LM_WARNING, "Send of mcast fragment blocked (%m).\n"));
        }
    }
  else if (n == 0)
    {
      ORBSVCS_DEBUG ((LM_WARNING, "EOF on send of mcast fragment (%m).\n"));
    }
}


CORBA::ULong
TAO_ECG_CDR_Message_Sender::compute_fragment_count (const ACE_Message_Block* begin,
                                                    const ACE_Message_Block* end,
                                                    int iov_size,
                                                    CORBA::ULong max_fragment_payload,
                                                    CORBA::ULong& total_length)
{
  CORBA::ULong fragment_count = 0;
  total_length = 0;

  CORBA::ULong fragment_size = 0;
  // Reserve the first iovec for the header...
  int iovcnt = 1;
  for (const ACE_Message_Block* b = begin;
       b != end;
       b = b->cont ())
    {
      CORBA::ULong l = b->length ();
      total_length += l;
      fragment_size += l;
      ++iovcnt;
      while (fragment_size > max_fragment_payload)
        {
          // Ran out of space, must create a fragment...
          ++fragment_count;

          // The next iovector will contain what remains of this
          // buffer, but also consider
          iovcnt = 2;
          l -= max_fragment_payload - (fragment_size - l);
          fragment_size = l;
        }
      if (fragment_size == max_fragment_payload)
        {
          ++fragment_count;
          iovcnt = 1;
          fragment_size = 0;
        }
      if (iovcnt >= iov_size)
        {
          // Ran out of space in the iovector....
          ++fragment_count;
          iovcnt = 1;
          fragment_size = 0;
        }
    }
  if (iovcnt != 1)
    {
      // Send the remaining data in another fragment
      ++fragment_count;
    }
  return fragment_count;
}

TAO_END_VERSIONED_NAMESPACE_DECL
