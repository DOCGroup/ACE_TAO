//$Id$
#include "tao/debug.h"
#include "tao/GIOP_Utils.h"
#include "tao/ORB_Core.h"


#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Utils.i"
#endif /* __ACE_INLINE__ */



TAO_GIOP_Message_State::TAO_GIOP_Message_State (TAO_ORB_Core* orb_core)
  : byte_order (TAO_ENCAP_BYTE_ORDER),
    more_fragments (0),
    message_type (TAO_GIOP_MESSAGERROR),
    message_size (0),
    current_offset (0),
    cdr (orb_core->create_input_cdr_data_block (ACE_CDR::DEFAULT_BUFSIZE),
         TAO_ENCAP_BYTE_ORDER,
         orb_core),
    fragments_begin (0),
    fragments_end (0)
{
  //giop_version.major = TAO_DEF_GIOP_MAJOR;
  //giop_version.minor = TAO_DEF_GIOP_MINOR;
}

TAO_GIOP_Message_State::~TAO_GIOP_Message_State (void)
{
  //no-op
}

int
TAO_GIOP_Message_State::is_complete ()
{
  if (this->message_size != this->current_offset)
    return 0;

  if (this->more_fragments)
    {
      // This is only one fragment of the complete Request....
      ACE_Message_Block* current =
        this->cdr.steal_contents ();
      if (this->fragments_begin == 0)
        {
          this->first_fragment_byte_order = this->byte_order;
          this->first_fragment_giop_version = this->giop_version;
          this->first_fragment_message_type = this->message_type;
          this->fragments_end = this->fragments_begin = current;
          this->reset ();
          return 0;
        }

      return this->append_fragment (current);
    }

  if (this->fragments_begin != 0)
    {
      // This is the last message, but we must defragment before
      // sending

      ACE_Message_Block* current =
        this->cdr.steal_contents ();
      if (this->append_fragment (current) == -1)
        return -1;

      // Copy the entire chain into the input CDR.....
      this->cdr.reset (this->fragments_begin,
                       this->first_fragment_byte_order);
      ACE_Message_Block::release (this->fragments_begin);
      this->fragments_begin = 0;
      this->fragments_end = 0;
      
      this->byte_order = this->first_fragment_byte_order;
      this->giop_version = this->first_fragment_giop_version;
      this->message_type = this->first_fragment_message_type;

      /*FALLTHROUGH*/
    }
  // else
  // {
  // This message has no more fragments, and there where no fragments
  // before it, just return... notice that this->cdr has the right
  // contents.
  // }

  return 1;
}

int
TAO_GIOP_Message_State::append_fragment (ACE_Message_Block* current)
{
  this->fragments_end->cont (current);
  this->fragments_end = this->fragments_end->cont ();

  if (this->first_fragment_byte_order != this->byte_order
      || this->first_fragment_giop_version.major != this->giop_version.major
      || this->first_fragment_giop_version.minor != this->giop_version.minor)
    {
      // Yes, print it out in all debug levels!
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) incompatible fragments:\n"
                  "   Different GIOP versions or byte order\n"));
      this->reset ();
      return -1;
    }
  this->reset ();
  return 0;
}


int
TAO_GIOP_Utils::
  read_bytes_input (TAO_Transport *transport,
                    TAO_InputCDR &input,
                    CORBA::ULong read_size,
                    ACE_Time_Value *value)
{
  // Grow the size of CDR stream
  if (input.grow (read_size) == -1)
    return -1;
  
  // Read until all the header is received.  There should be no
  // problems with locking, the header is only a few bytes so they
  // should all be available on the socket, otherwise there is a
  // problem with the underlying transport, in which case we have more
  // problems than just this small loop.
  char *buf = input.rd_ptr ();
  ssize_t n = 0;
  
  for (int t = read_size;
       t != 0;
       t -= n)
    {
      n = transport->recv (buf, t, value);
      if (n == -1)
        return -1;
      else if (n == 0) // @@ TODO && errno != EWOULDBLOCK)
        return -1;
      buf += n;
    }

  return 1;
}


ssize_t
TAO_GIOP_Utils::read_buffer (TAO_Transport *transport,
                             char *buf,
                             size_t len,
                             ACE_Time_Value *max_wait_time)
{
  ssize_t bytes_read = transport->recv (buf, len, max_wait_time);

  if (bytes_read <= 0 && TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("TAO (%P|%t|%N|%l) - %p,\n")
                ASYS_TEXT ("              transport = %d, ")
                ASYS_TEXT ("bytes = %d, len = %d\n"),
                ASYS_TEXT ("read_buffer"),
                transport->handle (),
                bytes_read,
                len));

  if (bytes_read == -1 && errno == ECONNRESET)
    {
      // @@ Is this OK??

      // We got a connection reset (TCP RSET) from the other side,
      // i.e., they didn't initiate a proper shutdown.
      //
      // Make it look like things are OK to the upper layer.
      bytes_read = 0;
      errno = 0;
    }

  return bytes_read;
}


TAO_GIOP_Reply_Status_Type
TAO_GIOP_Utils::
  convert_CORBA_to_GIOP_exception (CORBA::exception_type corba_type)  
{

  switch (corba_type)
    {
    case CORBA::NO_EXCEPTION:
      ACE_DEBUG ((LM_DEBUG,
                  "(%N|%l) \n"));
      return TAO_GIOP_NO_EXCEPTION;

    case CORBA::SYSTEM_EXCEPTION:
      ACE_DEBUG ((LM_DEBUG,
                  "(%N|%l) \n"));
      return TAO_GIOP_SYSTEM_EXCEPTION;

    case CORBA::USER_EXCEPTION:
      ACE_DEBUG ((LM_DEBUG,
                  "(%N|%l) \n"));
      return TAO_GIOP_USER_EXCEPTION;

    default:
      ACE_DEBUG ((LM_DEBUG,
                  "(%N|%l) \n"));
      // Don't know what to do here??
      return TAO_GIOP_SYSTEM_EXCEPTION;
    }
}
