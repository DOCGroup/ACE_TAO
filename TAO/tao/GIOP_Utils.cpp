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


void
TAO_GIOP_Utils::dump_msg (const char * /*label*/,
                         const u_char * /*ptr*/,
                         size_t /*len*/)
{
  if (TAO_debug_level >= 5)
    {
      // I will have to print out all the relevant debug messages!!
      // Let me not wory about that now. I will get back to that at a
      // later date!!
    }
}

CORBA::Boolean
TAO_GIOP_Utils::start_message (const TAO_GIOP_Version &version,
                               TAO_GIOP_Message_Type t,
                               TAO_OutputCDR &msg)
{
  static CORBA::Octet magic[] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50, // 'P'
  };

  static int magic_size = sizeof (magic)/sizeof (magic[0]);
  msg.write_octet_array (magic, magic_size);

  msg.write_octet (version.major);
  msg.write_octet (version.minor);
  msg.write_octet (TAO_ENCAP_BYTE_ORDER);
  
  msg.write_octet ((CORBA::Octet) t);

  // Write a dummy <size> later it is set to the right value...
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  return 1;
}


int
TAO_GIOP_Utils::send_message (TAO_Transport *transport,
                              TAO_OutputCDR &stream,
                              size_t header_len,
                              size_t offset,
                              ACE_Time_Value *max_wait_time,
                              TAO_Stub *stub)
{
   // Ptr to first buffer.
  char *buf = (char *) stream.buffer ();

  // Length of all buffers.
  size_t total_len =
    stream.total_length ();

  // NOTE: Here would also be a fine place to calculate a digital
  // signature for the message and place it into a preallocated slot
  // in the "ServiceContext".  Similarly, this is a good spot to
  // encrypt messages (or just the message bodies) if that's needed in
  // this particular environment and that isn't handled by the
  // networking infrastructure (e.g., IPSEC).
  
  CORBA::ULong bodylen = total_len - header_len;

#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
  *ACE_reinterpret_cast (CORBA::ULong *, buf + offset) = bodylen;
#else
  if (!stream->do_byte_swap ())
    *ACE_reinterpret_cast (CORBA::ULong *,
                           buf + offset) = bodylen;
  else
    ACE_CDR::swap_4 (ACE_reinterpret_cast (char *,
                                           &bodylen),
                     buf + offset);
#endif /* ACE_ENABLE_SWAP_ON_WRITE */

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.
  TAO_GIOP_Utils::dump_msg ("send",
                            ACE_reinterpret_cast (u_char *,
                                                  buf),
                            stream.length ());
 
  // This guarantees to send all data (bytes) or return an error.
  ssize_t n = transport->send (stub,
                               stream.begin (),
                               max_wait_time);
  
  if (n == -1)
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO: (%P|%t) closing conn %d after fault %p\n",
                    transport->handle (),
                    "GIOP_Message_Factory::send_message ()"));

      return -1;
    }

  // EOF.
  if (n == 0)
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO: (%P|%t) GIOP::send_message () "
                    "EOF, closing conn %d\n",
                    transport->handle()));
      return -1;
    }
 
  return 1;
}



int
TAO_GIOP_Utils::read_bytes_input (TAO_Transport *transport,
                                  TAO_InputCDR &input,
                                  CORBA::ULong read_size,
                                  ACE_Time_Value *max_wait_time)
{
  //@@ Need to do something here for GIOPlite.

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
      n = transport->recv (buf, t, max_wait_time);
      if (n == -1)
        return -1;
      else if (n == 0) // @@ TODO && errno != EWOULDBLOCK)
        return -1;
      buf += n;
    }

  return 1;
}


int
TAO_GIOP_Utils::parse_giop_header (TAO_GIOP_Message_State *state,
                                   TAO_InputCDR &input)
{
  char *buf = input.rd_ptr ();

  // Let us be specific that it is for 1.0
  if (state->giop_version.minor == 0 &&
      state->giop_version.major == 1)
    {
      state->byte_order = buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET];
      if (TAO_debug_level > 2
          && state->byte_order != 0 && state->byte_order != 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) invalid byte order <%d>"
                      " for version <1.0>\n",
                      state->byte_order));
          return -1;
        }
    }
  // Other revisions
  else
    {
      state->byte_order     = 
        (CORBA::Octet)(buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x01); 
      state->more_fragments = 
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x02); 
      
      if (TAO_debug_level > 2
          && (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & ~0x3) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) invalid flags for <%d>"
                      " for version <%d %d> \n",
                      buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET],
                      state->giop_version.major,
                      state->giop_version.minor));
          return -1;
        }

    }

  // Get the message type
  state->message_type = buf[TAO_GIOP_MESSAGE_TYPE_OFFSET];

  // Reset our input CDR stream
  input.reset_byte_order (state->byte_order);
  
  
  input.skip_bytes (TAO_GIOP_MESSAGE_SIZE_OFFSET);
  input.read_ulong (state->message_size);

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) Parsed header = <%d,%d,%d,%d,%d>\n",
                  state->giop_version.major,
                  state->giop_version.minor,
                  state->byte_order,
                  state->message_type,
                  state->message_size));
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
                ASYS_TEXT ("TAO (%P|%t) - %p,\n")
                ASYS_TEXT ("              transport = %d, ")
                ASYS_TEXT ("bytes = %d, len = %d\n"),
                ASYS_TEXT ("TAO_GIOP::read_buffer"),
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
