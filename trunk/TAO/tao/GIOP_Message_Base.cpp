//$Id$

#include "tao/GIOP_Message_Base.h"
#include "tao/operation_details.h"
#include "tao/GIOP_Utils.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Base.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, GIOP_Message_Base, "$Id$")

TAO_GIOP_Message_Base::TAO_GIOP_Message_Base (void)
{

}

TAO_GIOP_Message_Base::~TAO_GIOP_Message_Base (void)
{

}

CORBA::Boolean
TAO_GIOP_Message_Base::
  write_protocol_header (TAO_Pluggable_Message_Type t,
                         TAO_OutputCDR &msg)
{
  // Reset the message type
  msg.reset ();

  TAO_GIOP_Message_Type type = TAO_GIOP_MESSAGERROR;

  // @@ Bala: notice that you have to do this translation between the
  // 'pluggable' types and the GIOP types (there are more efficient
  // ways to do this, btw).
  // But only GIOP implements exactly these messages, does that sound
  // really like a good idea?
  // @@ Carlos: I would be interested to now the efficient method. To
  // be frank, I don't know of any. The reason I have put this is to
  // mask the GIOP specificness of the incoming parameter type. Let me
  // know what you feel.

  // First convert the Pluggable type to the GIOP specific type.
  switch (t)
    {
    case (TAO_PLUGGABLE_MESSAGE_REQUEST):
      type = TAO_GIOP_REQUEST;
      break;
    case (TAO_PLUGGABLE_MESSAGE_REPLY):
      type = TAO_GIOP_REPLY;
      break;
    case (TAO_PLUGGABLE_MESSAGE_CANCELREQUEST):
      type = TAO_GIOP_CANCELREQUEST;
      break;
    case (TAO_PLUGGABLE_MESSAGE_LOCATEREQUEST):
      type = TAO_GIOP_LOCATEREQUEST;
      break;
    case (TAO_PLUGGABLE_MESSAGE_LOCATEREPLY):
      type = TAO_GIOP_LOCATEREPLY;
      break;
    case (TAO_PLUGGABLE_MESSAGE_CLOSECONNECTION):
      type = TAO_GIOP_CLOSECONNECTION;
      break;
    case (TAO_PLUGGABLE_MESSAGE_MESSAGERROR):
      type = TAO_GIOP_MESSAGERROR;
      break;
    case (TAO_PLUGGABLE_MESSAGE_FRAGMENT):
      type = TAO_GIOP_FRAGMENT;
      break;
    }

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
  msg.write_octet (this->major_version ());
  msg.write_octet (this->minor_version ());

  // We are putting the byte order. But at a later date if we support
  // fragmentation and when we want to use the other 6 bits in this
  // octet we can have a virtual function do this for us as the
  // version info , Bala
  msg.write_octet (TAO_ENCAP_BYTE_ORDER);

  msg.write_octet ((CORBA::Octet) type);

  // Write a dummy <size> later it is set to the right value...
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  return 1;
}


CORBA::Boolean
TAO_GIOP_Message_Base::
  write_message_header (const TAO_Operation_Details &opdetails,
                        TAO_Pluggable_Header_Type header_type,
                        TAO_Target_Specification &spec,
                        TAO_OutputCDR &cdr)
{
  // @@ Bala: Why not do this right, the application should call the
  // methods below directly!
  // @@Carlos: The idea was that the application (or the user of this
  // method)  shouldn't know what is being done.
  // An afterthought, please see my comments in the file
  // Pluggable_Messaging.h
  // @@ Bala: I think we agree by know that the application knows very
  // well if it is creating a request or a locate request, and that
  // using an enum for that, instead of calling an operation is just
  // more obscure.... Of course, the final implementation can share
  // code in some private method that takes the enum as an argument.
  switch (header_type)
    {
    case TAO_PLUGGABLE_MESSAGE_REQUEST_HEADER:
      return this->write_request_header (opdetails,
                                         spec,
                                         cdr);
    case TAO_PLUGGABLE_MESSAGE_LOCATE_REQUEST_HEADER:
      return this->write_locate_request_header (opdetails.request_id (),
                                                spec,
                                                cdr);
    default:
      if (TAO_debug_level > 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t|%N|%l) Wrong header type \n")),
                          0);

    }

  return 1;
}


int
TAO_GIOP_Message_Base::send_message (TAO_Transport *transport,
                                     TAO_OutputCDR &stream,
                                     ACE_Time_Value *max_wait_time,
                                     TAO_Stub *stub,
                                     int two_way)
{
  // Get the header length
  const size_t header_len = TAO_GIOP_MESSAGE_HEADER_LEN ;

  // Get the message size offset
  const size_t offset = TAO_GIOP_MESSAGE_SIZE_OFFSET;

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
  if (TAO_debug_level > 2)
    {
      this->dump_msg ("send",
                      ACE_reinterpret_cast (u_char *,
                                            buf),
                      stream.length ());
    }
  return this->transport_message (transport,
                                  stream,
                                  two_way,
                                  stub,
                                  max_wait_time);
}

int
TAO_GIOP_Message_Base::
  handle_input (TAO_Transport *transport,
                TAO_ORB_Core * /*orb_core*/,
                TAO_Message_State_Factory &mesg_state,
                ACE_Time_Value *max_wait_time)
{
  TAO_GIOP_Message_State *state =
    ACE_dynamic_cast (TAO_GIOP_Message_State *,
                      &mesg_state);

  if (state->header_received () == 0)
    {
      int retval =
        TAO_GIOP_Utils::read_bytes_input (transport,
                                          state->cdr,
                                          TAO_GIOP_MESSAGE_HEADER_LEN ,
                                          max_wait_time);
      if (retval == -1 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - \n")
                      ACE_TEXT ("TAO_GIOP_Message_Base::handle_input \n")));
          return -1;
        }


      if (this->parse_magic_bytes (state) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) -"),
                        ACE_TEXT ("TAO_GIOP_Message_Base::handle_input, parse_magic_bytes \n")));
          return -1;
        }

      // Read the rest of the stuff. That should be read by the
      // corresponding states
      if (this->parse_header (state) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t|%N%l) -\n"),
                        ACE_TEXT ("TAO_GIOP_Message_Base::handle_input \n")));
          return -1;
        }

      if (state->cdr.grow (TAO_GIOP_MESSAGE_HEADER_LEN  +
                           state->message_size) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t|%N|%l) - %p\n"),
                        ACE_TEXT ("ACE_CDR::grow")));
          return -1;
        }

      // Growing the buffer may have reset the rd_ptr(), but we want
      // to leave it just after the GIOP header (that was parsed
      // already);
      state->cdr.skip_bytes (TAO_GIOP_MESSAGE_HEADER_LEN );
    }

  size_t missing_data =
    state->message_size - state->current_offset;

  ssize_t n =
    TAO_GIOP_Utils::read_buffer (transport,
                                 state->cdr.rd_ptr () + state->current_offset,
                                 missing_data,
                                 max_wait_time);

   if (n == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - %p\n"),
                    ACE_TEXT ("TAO_GIOP_Message_Base::handle_input, read_buffer[1] \n")));
      return -1;
    }
  else if (n == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - %p\n"),
                    ACE_TEXT ("TAO_GIOP_Message_Base::handle_input, read_buffer[2]\n")));
      return -1;
    }

   state->current_offset += n;

  if (state->current_offset == state->message_size)
    {
      if (TAO_debug_level >= 4)
        {
          size_t header_len = TAO_GIOP_MESSAGE_HEADER_LEN ;

          // Need to include GIOPlite too.
          char *buf = state->cdr.rd_ptr ();
          buf -= header_len;
          size_t msg_len = state->cdr.length () + header_len;
          this->dump_msg ("recv",
                          ACE_reinterpret_cast (u_char *,
                                                buf),
                          msg_len);
        }
    }

  return state->is_complete ();
}

void
TAO_GIOP_Message_Base::dump_msg (const char *label,
                                 const u_char *ptr,
                                 size_t len)
{
  static const char digits [] = "0123456789ABCD";
  static const char *names [] =
  {
    "Request",
    "Reply",
    "CancelRequest",
    "LocateRequest",
    "LocateReply",
    "CloseConnection",
    "MessageError"
    "Fragment"
  };

  if (TAO_debug_level >= 5)
    {
      // Message name.
      const char *message_name = "UNKNOWN MESSAGE";
      u_long slot = ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET];
      if (slot < sizeof (names)/sizeof(names[0]))
        message_name = names [slot];

      // Byte order.
      int byte_order = ptr[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & 0x01;

      // request/reply id.
      CORBA::ULong tmp = 0;
      CORBA::ULong *id = &tmp;

      if (ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REQUEST ||
          ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == TAO_GIOP_REPLY)
        {
          // @@ Only works if ServiceContextList is empty....
          if (this->minor_version () < 2)
            {
              id = ACE_reinterpret_cast (CORBA::ULong *,
                                         (char * ) (ptr + TAO_GIOP_MESSAGE_HEADER_LEN  + 4));

            }
          else
            {
              id = ACE_reinterpret_cast (CORBA::ULong *,
                                         (char * ) (ptr + TAO_GIOP_MESSAGE_HEADER_LEN));
            }
        }

      // Print.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P | %t):%s GIOP v%c.%c msg, ")
                  ACE_TEXT ("%d data bytes, %s endian, %s = %d\n"),
                  label,
                  digits[ptr[TAO_GIOP_VERSION_MAJOR_OFFSET]],
                  digits[ptr[TAO_GIOP_VERSION_MINOR_OFFSET]],
                  len - TAO_GIOP_MESSAGE_HEADER_LEN ,
                  (byte_order == TAO_ENCAP_BYTE_ORDER) ? "my" : "other",
                  message_name,
                  *id));

      if (TAO_debug_level >= 10)
        ACE_HEX_DUMP ((LM_DEBUG,
                       (const char *) ptr,
                       len,
                       ACE_TEXT ("GIOP message")));


    }
}

// Send an "I can't understand you" message -- again, the message is
// prefabricated for simplicity.  This implies abortive disconnect (at
// the application level, if not at the level of TCP).
//
// NOTE that IIOP will still benefit from TCP's orderly disconnect.
int
TAO_GIOP_Message_Base::send_error (TAO_Transport *transport)
{
  const char
    error_message [TAO_GIOP_MESSAGE_HEADER_LEN] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50, // 'P'
    this->major_version (),
    this->minor_version (),
    TAO_ENCAP_BYTE_ORDER,
    TAO_GIOP_MESSAGERROR,
    0, 0, 0, 0
  };

  // @@ Q: How does this works with GIOP lite?
  //    A: It doesn't

  this->dump_msg ("send_error",
                  (const u_char *) error_message,
                  TAO_GIOP_MESSAGE_HEADER_LEN );

  ACE_HANDLE which = transport->handle ();

  int result = transport->send ((const u_char *)error_message,
                                TAO_GIOP_MESSAGE_HEADER_LEN );
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%N|%l|%P|%t) error sending error to %d\n"),
                    which));
    }

  return result;
}


int
TAO_GIOP_Message_Base::parse_magic_bytes (TAO_GIOP_Message_State  *state)
{
  // Grab the read pointer
  char *buf = state->cdr.rd_ptr ();

  // The values are hard-coded to support non-ASCII platforms.
  if (!(buf [0] == 0x47      // 'G'
        && buf [1] == 0x49   // 'I'
        && buf [2] == 0x4f   // 'O'
        && buf [3] == 0x50)) // 'P'
    {
      // For the present...
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) bad header, magic word [%c%c%c%c]\n"),
                    buf[0],
                    buf[1],
                    buf[2],
                    buf[3]));
      return -1;
    }

  if (this->validate_version (state) == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%N|%l|%P|%t) Error in validating")
                      ACE_TEXT ("revision \n")));
          return -1;
        }
    }

  return 0;
}

int
TAO_GIOP_Message_Base::parse_header (TAO_GIOP_Message_State *state)
{
  char *buf = state->cdr.rd_ptr ();

  // Let us be specific that it is for 1.0
  if (this->minor_version () == 0 &&
      this->major_version () == 1)
    {
      state->byte_order = buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET];
      if (TAO_debug_level > 2
          && state->byte_order != 0 && state->byte_order != 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) invalid byte order <%d>")
                      ACE_TEXT (" for version <1.0>\n"),
                      state->byte_order));
          return -1;
        }
    }
  else
    {
      state->byte_order     =
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x01);
      state->more_fragments =
        (CORBA::Octet) (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]& 0x02);

      if (TAO_debug_level > 2
          && (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & ~0x3) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) invalid flags for <%d>")
                      ACE_TEXT (" for version <%d %d> \n"),
                      buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET],
                      this->major_version (),
                      this->minor_version ()));
          return -1;
        }
    }

  // Get the message type
  state->message_type = buf[TAO_GIOP_MESSAGE_TYPE_OFFSET];

  // Reset our input CDR stream
  state->cdr.reset_byte_order (state->byte_order);


  state->cdr.skip_bytes (TAO_GIOP_MESSAGE_SIZE_OFFSET);
  state->cdr.read_ulong (state->message_size);

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Parsed header = <%d,%d,%d,%d,%d>\n"),
                  this->major_version (),
                  this->minor_version (),
                  state->byte_order,
                  state->message_type,
                  state->message_size));
    }

  return 1;
}


// Server sends an "I'm shutting down now, any requests you've sent me
// can be retried" message to the server.  The message is prefab, for
// simplicity.
//
// NOTE: this is IIOP-specific though it doesn't look like it is.  It
// relies on a TCP-ism: orderly disconnect, which doesn't exist in all
// transport protocols.  Versions of GIOP atop some transport that's
// lacking orderly disconnect must define some transport-specific
// handshaking (e.g. the XNS/SPP handshake convention) in order to
// know that the same transport semantics are provided when shutdown
// is begun with messages "in flight". (IIOP doesn't report false
// errors in the case of "clean shutdown", because it relies on
// orderly disconnect as provided by TCP.  This quality of service is
// required to write robust distributed systems.)

void
TAO_GIOP_Message_Base::
  send_close_connection (const TAO_GIOP_Version &version,
                         TAO_Transport *transport,
                         void *)
{

  // static CORBA::Octet
  // I hate  this in every method. Till the time I figure out a way
  // around  I will have them here hanging around.
  const char close_message [TAO_GIOP_MESSAGE_HEADER_LEN] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50, // 'P'
    version.major,
    version.minor,
    TAO_ENCAP_BYTE_ORDER,
    TAO_GIOP_CLOSECONNECTION,
    0, 0, 0, 0
  };

  // It's important that we use a reliable shutdown after we send this
  // message, so we know it's received.
  //
  // @@ should recv and discard queued data for portability; note
  // that this won't block (long) since we never set SO_LINGER

  this->dump_msg ("send_close_connection",
                  (const u_char *) close_message,
                  TAO_GIOP_MESSAGE_HEADER_LEN);

  ACE_HANDLE which = transport->handle ();
  if (which == ACE_INVALID_HANDLE)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::send_close_connection -"
                    " connection already closed\n"));
      return;
    }

  if (transport->send ((const u_char *) close_message,
                       TAO_GIOP_MESSAGE_HEADER_LEN) == -1)
    {
      if (TAO_orbdebug)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) error closing connection %d\n",
                    which));
    }

  transport->close_connection ();
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) shut down transport, handle %d\n",
              which));

}
