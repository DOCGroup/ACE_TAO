#include "GIOP_Message_NonReactive_Handler.h"
#include "GIOP_Message_NonReactive_Base.h"
#include "tao/ORB_Core.h"
#include "tao/CDR.h"
#include "tao/debug.h"
#include "tao/Transport.h"

#if !defined (__ACE_INLINE__)
# include "GIOP_Message_NonReactive_Handler.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao, GIOP_Message_NonReactive_Handler, "$Id$")


TAO_GIOP_Message_NonReactive_Handler::TAO_GIOP_Message_NonReactive_Handler (TAO_ORB_Core *orb_core,
                                                                            TAO_GIOP_Message_NonReactive_Base *mesg_base,
                                                                            size_t cdr_size)
  : TAO_GIOP_Message_Reactive_Handler (orb_core,
                                       mesg_base,
                                       cdr_size),
    mesg_base_ (mesg_base),
    input_cdr_ (orb_core->data_block_for_message_block (cdr_size),
                0,
                TAO_ENCAP_BYTE_ORDER)
{
}

int
TAO_GIOP_Message_NonReactive_Handler::read_parse_message(TAO_Transport *transport,
                                                         ACE_Time_Value *max_wait_time)
{
  // Reset the input_cdr before we do anything with it
  this->input_cdr_.reset_contents ();

  // Before we do this let us reset the
  char *buf = this->input_cdr_.rd_ptr ();

  // This is of the following pieces:
  //  - We first read the GIOP header and parse the information. We
  //    also get the size of the incoming message. We grow the size of
  //    the CDR if needed
  //  - We read the information/message in toto.
  if (this->read_message (transport,
                          buf,
                          TAO_GIOP_MESSAGE_HEADER_LEN,
                          max_wait_time) == -1)
    {
      if (TAO_debug_level > 1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) : (%N | %l) \n")
                    ACE_TEXT ("Error during header read \n")));
        return -1;
    }

  // Parse the message header
  if (this->parse_message_header_i (buf) == -1)
    {
      if (TAO_debug_level > 2)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) : (%N|%l)\n")
                      ACE_TEXT ("Error while parsing GIOP header \n")));
        }
      return -1;
    }
  // Reset the byte order now
  this->input_cdr_.reset_byte_order (this->message_state_.byte_order);

  // Calculate the message size to grow the CDR
  size_t msg_size =
    this->message_state_.message_size +
    TAO_GIOP_MESSAGE_HEADER_LEN;

  // With GIOP 1.2 we should get a fragment header
  if (this->message_state_.more_fragments &&
      this->message_state_.giop_version.minor == 2)
    {
      msg_size += TAO_GIOP_MESSAGE_FRAGMENT_HEADER;
    }

  // Grow the CDR
  if (this->input_cdr_.grow (msg_size) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Failure to grow the CDR buffers \n")
                  ACE_TEXT ("(%P|%t) to the right size")));
      return -1;
    }

  // Growing the CDR would have reset our read pointer, so set it back
  // to the right position
  if (this->input_cdr_.skip_bytes (TAO_GIOP_MESSAGE_HEADER_LEN) == 0)
    {
      if (TAO_debug_level> 3)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Failure to skip the right bytes  \n")
                    ACE_TEXT ("(%P|%t) in the CDR stream")));
      return -1;
    }

  buf = this->input_cdr_.rd_ptr ();

  // Read the rest of the message
  int retval = this->read_message (transport,
                                   buf,
                                   msg_size - TAO_GIOP_MESSAGE_HEADER_LEN,
                                   max_wait_time);
  if (retval <= 0)
    {
      if (TAO_debug_level > 1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) : (%N | %l) \n")
                    ACE_TEXT ("(%P|%t) Error during message read \n")));

      return retval;
    }

  // Parse the GIOP fragment header.
  if (this->parse_fragment_header (this->input_cdr_.rd_ptr (),
                                   this->input_cdr_.length ()))
    {
      // We had a GIOP fragment header, so skip few more bytes.
      if (this->input_cdr_.skip_bytes (TAO_GIOP_MESSAGE_FRAGMENT_HEADER) == 0)
        {
          if (TAO_debug_level> 3)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Failure to skip the right bytes  \n")
                        ACE_TEXT ("(%P|%t) in the CDR stream [2]")));
          return -1;
        }
    }

  // Now we have parsed almost every thing that is part of the GIOP
  // header. Return a succes value

  return 1;
}


int
TAO_GIOP_Message_NonReactive_Handler::read_message (TAO_Transport *transport,
                                                    char *buf,
                                                    size_t size,
                                                    ACE_Time_Value *max_wait_time)
{
  ssize_t bytes = 0;

  // As this used for transports where things are available in one
  // shot this looping should not create any problems.
  for (size_t n = size;
       n != 0;
       n -= bytes)
    {
      // We would have liked to use something like a recv_n ()
      // here. But at the time when the code was written, the MEM_Stream
      // classes had poor support  for recv_n (). Till a day when we
      // get proper recv_n (), let us stick with this. The other
      // argument that can be said against this is that, this is the
      // bad layer in which this is being done ie. recv_n is
      // simulated. But...
      bytes = transport->recv (buf,
                               n,
                               max_wait_time);

      // @@ Do we need to check for errno != EWOULDBLOCK?? and errno ==
      // @@ ECONNRESET. Does such things make sense here??
      if (bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
          bytes = 0;
          continue;
        }

      if (bytes == 0)
        return -1;


      if (bytes == 0 ||
          bytes == -1)
      {
         return -1;
      }

      buf += bytes;
    }

  return 1;
}
