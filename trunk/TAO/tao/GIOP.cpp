// $Id$

// @(#)giop.cpp 1.10 95/09/21
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// GIOP:        Utility routines for sending, receiving GIOP messages
//
// Note that the Internet IOP is just the TCP-specific mapping of the
// General IOP.  Areas where other protocols may map differently
// include use of record streams (TCP has none), orderly disconnect
// (TCP has it), endpoint addressing (TCP uses host + port), security
// (Internet security should be leveraged by IIOP) and more.
//
// NOTE: There are a few places where this code knows that it's really
// talking IIOP instead of GIOP.  No rush to fix this so long as we
// are really not running atop multiple connection protocols.
//
// THREADING NOTE: currently, the connection manager eliminates tricky
// threading issues by providing this code with the same programming
// model both in threaded and unthreaded environments.  Since the GIOP
// APIs were all designed to be reentrant, this makes threading rather
// simple!
//
// That threading model is that the thread making (or handling) a call
// is given exclusive access to a connection for the duration of a
// call, so that no multiplexing or demultiplexing is needed.  That
// is, locking is at the "connection level" rather than "message
// level".
//
// The down side of this simple threading model is that utilization of
// system resources (mostly connections, but to some extent network
// I/O) in some kinds of environments can be inefficient.  However,
// simpler threading models are much easier to get properly debugged,
// and often perform better.  Also, such environments haven't been
// seen to be any kind of problem; the model can be changed later if
// needed, it's just an internal implementation detail.  Any portable
// ORB client is not allowed to rely on semantic implications of such
// a model.
//
// @@ there is lots of unverified I/O here.  In all cases, if an
// error is detected when marshaling or unmarshaling, it should be
// reported.
// @@ Some dependance on the specific underlying transport protocol used.
//    This must be removed in order to support pluggable protocols.
//    TAO_Connector and TAO_Transport objects will be introduced
//    to abstract away the specific transport protocol used.  We will
//    just expose behavior (methods) to all transport protocols that
//    can be used with GIOP!  fredk

#include "tao/GIOP.h"
#include "tao/Timeprobe.h"
#include "tao/GIOP_Server_Request.h"
#include "tao/CDR.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/POA.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, GIOP, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_GIOP_Timeprobe_Description[] =
{
  "GIOP::send_request - start",
  "GIOP::send_request - end",

  "GIOP::read_buffer - start",
  "GIOP::read_buffer - end",

  "GIOP::LocateRequestHeader_init - start",
  "GIOP::LocateRequestHeader_init - end"
};

enum
{
  // Timeprobe description table start key
  TAO_GIOP_SEND_REQUEST_START = 100,
  TAO_GIOP_SEND_REQUEST_END,

  TAO_GIOP_RECV_MESSAGE_START,
  TAO_GIOP_RECV_MESSAGE_END,

  TAO_GIOP_READ_BUFFER_START,
  TAO_GIOP_READ_BUFFER_END,

  TAO_GIOP_LOCATE_REQUEST_HEADER_INIT_START,
  TAO_GIOP_LOCATE_REQUEST_HEADER_INIT_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_GIOP_Timeprobe_Description,
                                  TAO_GIOP_SEND_REQUEST_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_GIOP_Message_State::TAO_GIOP_Message_State (TAO_ORB_Core* orb_core)
  : byte_order (TAO_ENCAP_BYTE_ORDER),
    more_fragments (0),
    message_type (TAO_GIOP::MessageError),
    message_size (0),
    current_offset (0),
    cdr (orb_core->create_input_cdr_data_block (ACE_CDR::DEFAULT_BUFSIZE),
         TAO_ENCAP_BYTE_ORDER,
         orb_core),
    fragments_begin (0),
    fragments_end (0)
{
  // Note that we need to use the ASCII values so the code will be
  // portable to platforms that use different character sets, such as
  // MVS (which uses EBCDIC).
  this->magic[0] = 0x47;   // 'G'
  this->magic[1] = 0x49;   // 'I'
  this->magic[2] = 0x4f;   // 'O'
  this->magic[3] = 0x50;   // 'P'

  giop_version.major = TAO_DEF_GIOP_MAJOR;
  giop_version.minor = TAO_DEF_GIOP_MINOR;
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


// ****************************************************************

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

void
TAO_GIOP::dump_msg (const char *label,
                    const u_char *ptr,
                    size_t len)
{
  if (TAO_debug_level >= 5)
    {
      const char *message_name = "UNKNOWN MESSAGE";
      u_long slot = ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET];
      if (slot < sizeof (names)/sizeof(names[0]))
        message_name = names [slot];
      int byte_order = ptr[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & 0x01;
      ACE_DEBUG ((LM_DEBUG,
                  "%s GIOP v%c.%c msg, %d data bytes, %s endian, %s",
                  label,
                  digits[ptr[TAO_GIOP_VERSION_MAJOR_OFFSET]],
                  digits[ptr[TAO_GIOP_VERSION_MINOR_OFFSET]],
                  len - TAO_GIOP_HEADER_LEN,
                  (byte_order == TAO_ENCAP_BYTE_ORDER) ? "my" : "other",
                  message_name));

      if (ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == TAO_GIOP::Request)
        {
          // @@ Only works if ServiceContextList is empty....
          const CORBA::ULong *request_id =
            ACE_reinterpret_cast (const CORBA::ULong *,
                                  ptr + TAO_GIOP_HEADER_LEN + 4);
          ACE_DEBUG ((LM_DEBUG,
                      " = %d\n",
                      *request_id));
        }
      else if (ptr[TAO_GIOP_MESSAGE_TYPE_OFFSET] == TAO_GIOP::Reply)
        {
          const CORBA::ULong *request_id =
            ACE_reinterpret_cast (const CORBA::ULong *,
                                  ptr + TAO_GIOP_HEADER_LEN + 4);
          ACE_DEBUG ((LM_DEBUG,
                      " = %d\n",
                      *request_id));
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    "\n"));

      if (TAO_debug_level >= 10)
        ACE_HEX_DUMP ((LM_DEBUG,
                       (const char *) ptr,
                       len,
                       "GIOP message"));
    }
}

CORBA::Boolean
operator<< (TAO_OutputCDR &cdr,
            const TAO_GIOP_ServiceContext &x)
{
  if (cdr << x.context_id
      && cdr << x.context_data)
    return 1;
  else
    return 0;
}

CORBA::Boolean
operator>> (TAO_InputCDR &cdr,
            TAO_GIOP_ServiceContext &x)
{
  if (cdr >> x.context_id
      && cdr >> x.context_data)
    return 1;
  else
    return 0;
}

CORBA::Boolean
operator<< (TAO_OutputCDR &cdr,
            const TAO_GIOP_ServiceContextList &x)
{
  CORBA::ULong length = x.length ();

  cdr.write_ulong (length);

  for (CORBA::ULong i = 0;
       i < length && cdr.good_bit ();
       ++i)
    cdr << x[i];

  return cdr.good_bit ();
}

CORBA::Boolean
operator>> (TAO_InputCDR &cdr,
            TAO_GIOP_ServiceContextList &x)
{
  CORBA::ULong length;

  cdr.read_ulong (length);

  if (cdr.good_bit ())
    {
      x.length (length);

      for (CORBA::ULong i = 0;
           i < length && cdr.good_bit ();
           ++i)
        cdr >> x[i];
    }
  return cdr.good_bit ();
}

CORBA::Boolean
TAO_GIOP::start_message (const TAO_GIOP_Version &version,
                         TAO_GIOP::Message_Type type,
                         TAO_OutputCDR &msg,
                         TAO_ORB_Core* orb_core)
{
  if (orb_core->orb_params ()->use_lite_protocol ())
    return TAO_GIOP::start_message_lite (version, type, msg);
  else
    return TAO_GIOP::start_message_std (version, type, msg);
}

CORBA::Boolean
TAO_GIOP::write_request_header (CORBA::ULong request_id,
                                CORBA::Boolean is_roundtrip,
                                const TAO_opaque& key,
                                const char *opname,
                                CORBA::Principal_ptr principal,
                                TAO_OutputCDR &msg,
                                TAO_ORB_Core *orb_core)
{
  if (orb_core->orb_params ()->use_lite_protocol ())
    return TAO_GIOP::write_request_header_lite (orb_core->service_context (),
                                                request_id,
                                                is_roundtrip,
                                                key,
                                                opname,
                                                principal,
                                                msg);
  else
    return TAO_GIOP::write_request_header_std (orb_core->service_context (),
                                               request_id,
                                               is_roundtrip,
                                               key,
                                               opname,
                                               principal,
                                               msg);
}

// Write the GIOP locate request header.

CORBA::Boolean
TAO_GIOP::write_locate_request_header (CORBA::ULong request_id,
                                       const TAO_opaque &key,
                                       TAO_OutputCDR &msg)
{
  msg << request_id;
  msg << key;

  return 1;
}

int
TAO_GIOP::send_message (TAO_Transport *transport,
                        TAO_OutputCDR &stream,
                        TAO_ORB_Core *orb_core)
{

  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_SEND_REQUEST_START);

  // Ptr to first buffer.
  char *buf = (char *) stream.buffer ();

  // Length of all buffers.
  size_t total_len =
    stream.total_length ();

  // assert (buflen == (stream.length - stream.remaining));

  // Patch the message length in the GIOP header; it's always at the
  // same eight byte offset into the message.
  //
  // NOTE: Here would also be a fine place to calculate a digital
  // signature for the message and place it into a preallocated slot
  // in the "ServiceContext".  Similarly, this is a good spot to
  // encrypt messages (or just the message bodies) if that's needed in
  // this particular environment and that isn't handled by the
  // networking infrastructure (e.g., IPSEC).

  size_t header_len = TAO_GIOP_HEADER_LEN;
  // Fred: this is actually a good start to think about pluggable
  // "messaging protocols" (maybe this is a bad name, but I want to
  // stress the difference with the pluggable "transport protocols"
  // that you recently completed).  For example: it seems that the
  // transport layer needs to know the size of the header and somehow
  // needs to determine the size of the message from that header, this
  // could be virtual methods in the MessagingProtocol class.  Just a
  // wild thought..... Carlos

  size_t offset = TAO_GIOP_MESSAGE_SIZE_OFFSET;
  if (orb_core->orb_params ()->use_lite_protocol ())
    {
      header_len = TAO_GIOP_LITE_HEADER_LEN;
      offset = TAO_GIOP_LITE_MESSAGE_SIZE_OFFSET;
    }

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

  TAO_GIOP::dump_msg ("send",
                      ACE_reinterpret_cast (u_char *,
                                            buf),
                      stream.length ());

  // This guarantees to send all data (bytes) or return an error.
  ssize_t n = transport->send (stream.begin ());

  if (n == -1)
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO: (%P|%t) closing conn %d after fault %p\n",
                    transport->handle (),
                    "GIOP::send_request ()"));

    transport->close_connection ();
    return -1;
  }

  // EOF.
  if (n == 0)
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO: (%P|%t) GIOP::send_request () "
                    "EOF, closing conn %d\n",
                    transport->handle()));
      transport->close_connection ();
      return -1;
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
TAO_GIOP::close_connection (const TAO_GIOP_Version& version,
                            TAO_Transport *transport,
                            void *)
{
  // static CORBA::Octet
  const char close_message [TAO_GIOP_HEADER_LEN] =
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
    TAO_GIOP::CloseConnection,
    0, 0, 0, 0
  };

  // It's important that we use a reliable shutdown after we send this
  // message, so we know it's received.
  //
  // @@ should recv and discard queued data for portability; note
  // that this won't block (long) since we never set SO_LINGER

  TAO_GIOP::dump_msg ("close_connection",
                      (const u_char *) close_message,
                      TAO_GIOP_HEADER_LEN);

  ACE_HANDLE which = transport->handle ();
  if (which == ACE_INVALID_HANDLE)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::close_connection -"
                    " connection already closed\n"));
      return;
    }

  if (transport->send ((const u_char *) close_message,
                       TAO_GIOP_HEADER_LEN) == -1)
    if (TAO_orbdebug)
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) error closing connection %d\n",
                  which));

  transport->close_connection ();
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) shut down transport, handle %d\n",
              which));
}

// Send an "I can't understand you" message -- again, the message is
// prefabricated for simplicity.  This implies abortive disconnect (at
// the application level, if not at the level of TCP).
//
// NOTE that IIOP will still benefit from TCP's orderly disconnect.

void
TAO_GIOP::send_error (const TAO_GIOP_Version &version,
                      TAO_Transport *transport)
{
  const char
    error_message [TAO_GIOP_HEADER_LEN] =
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
    TAO_GIOP::MessageError,
    0, 0, 0, 0
  };

  // @@ Q: How does this works with GIOP lite?
  //    A: It doesn't

  TAO_GIOP::dump_msg ("send_error",
                      (const u_char *) error_message,
                      TAO_GIOP_HEADER_LEN);

  ACE_HANDLE which = transport->handle ();

  if (transport->send ((const u_char *)error_message,
                       TAO_GIOP_HEADER_LEN) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) error sending error to %d\n",
                    which));
    }
}

ssize_t
TAO_GIOP::read_buffer (TAO_Transport *transport,
                       char *buf,
                       size_t len)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_READ_BUFFER_START);

  ssize_t bytes_read = transport->recv (buf, len);

  if (bytes_read <= 0 && TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - %p,\n"
                "              transport = %d, "
                "bytes = %d, len = %d\n",
                "TAO_GIOP::read_buffer",
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

void
TAO_GIOP_LocateRequestHeader::init (TAO_InputCDR &msg,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_LOCATE_REQUEST_HEADER_INIT_START);

  if (msg.read_ulong (this->request_id) == 0
      || (msg >> this->object_key) == 0)
    ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                               CORBA::COMPLETED_NO));
}

TAO_GIOP_ReplyStatusType
TAO_GIOP::convert_CORBA_to_GIOP_exception (CORBA::exception_type corba_type)
{
  switch (corba_type)
    {
    case CORBA::NO_EXCEPTION:
      return TAO_GIOP_NO_EXCEPTION;

    case CORBA::SYSTEM_EXCEPTION:
      return TAO_GIOP_SYSTEM_EXCEPTION;

    case CORBA::USER_EXCEPTION:
      return TAO_GIOP_USER_EXCEPTION;

    default:
      // Don't know what to do here??
      return TAO_GIOP_SYSTEM_EXCEPTION;
    }
}

int
TAO_GIOP::read_header (TAO_Transport *transport,
                       TAO_ORB_Core *orb_core,
                       TAO_GIOP_Message_State &state,
                       CORBA::ULong &header_size,
                       TAO_InputCDR &input)
{
  // Default header length.
  header_size = TAO_GIOP_HEADER_LEN;
  if (orb_core->orb_params ()->use_lite_protocol ())
    header_size = TAO_GIOP_LITE_HEADER_LEN;

  if (input.grow (header_size) == -1)
    return -1;

  // Read until all the header is received.  There should be no
  // problems with locking, the header is only a few bytes so they
  // should all be available on the socket, otherwise there is a
  // problem with the underlying transport, in which case we have more
  // problems than just this small loop.

  char *buf = input.rd_ptr ();
  ssize_t n;

  for (int t = header_size;
       t != 0;
       t -= n)
    {
      n = transport->recv (buf, t);
      if (n == -1)
        return -1;
      else if (n == 0 && errno != EWOULDBLOCK)
        return -1;
      buf += n;
    }

  if (TAO_GIOP::parse_header (orb_core,
                              input,
                              state) == -1)
    {
      TAO_GIOP::send_error (state.giop_version, transport);
      return -1;
    }
  return header_size;
}

int
TAO_GIOP::handle_input (TAO_Transport *transport,
                        TAO_ORB_Core *orb_core,
                        TAO_GIOP_Message_State &state)
{
  if (state.header_received () == 0)
    {
      CORBA::ULong header_size;
      if (TAO_GIOP::read_header (transport,
                                 orb_core,
                                 state,
                                 header_size,
                                 state.cdr) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - %p\n",
                        "TAO_GIOP::handle_input, read_header"));
          return -1;
        }

      if (state.cdr.grow (header_size +
                          state.message_size) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - %p\n",
                        "TAO_GIOP::handle_input, ACE_CDR::grow"));
          return -1;
        }

      // Growing the buffer may have reset the rd_ptr(), but we want
      // to leave it just after the GIOP header (that was parsed
      // already);
      state.cdr.skip_bytes (header_size);
    }

  size_t missing_data =
    state.message_size - state.current_offset;
  ssize_t n =
    TAO_GIOP::read_buffer (transport,
                           state.cdr.rd_ptr () + state.current_offset,
                           missing_data);
  if (n == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "TAO_GIOP::handle_input, read_buffer[1]"));
      return -1;
    }
  else if (n == 0)
    {
      if (errno == EWOULDBLOCK)
        return 0;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "TAO_GIOP::handle_input, read_buffer[2]"));
      return -1;
    }

  state.current_offset += n;

  if (state.current_offset == state.message_size)
    {
      if (TAO_debug_level >= 5)
        {
          size_t header_len = TAO_GIOP_HEADER_LEN;
          if (orb_core->orb_params ()->use_lite_protocol ())
            header_len = TAO_GIOP_LITE_HEADER_LEN;

          char *buf = state.cdr.rd_ptr ();
          buf -= header_len;
          size_t msg_len = state.cdr.length () + header_len;
          TAO_GIOP::dump_msg ("recv",
                              ACE_reinterpret_cast (u_char *,
                                                    buf),
                              msg_len);
        }
    }

  return state.is_complete ();
}

int
TAO_GIOP::parse_reply (TAO_Transport *transport,
                       TAO_ORB_Core *orb_core,
                       TAO_GIOP_Message_State &state,
                       TAO_GIOP_ServiceContextList &reply_ctx,
                       CORBA::ULong &request_id,
                       CORBA::ULong &reply_status)
{
  switch (state.message_type)
    {
    case TAO_GIOP::Request:
      // In GIOP 1.0 and GIOP 1.1 this is an error, but it is
      // *possible* to receive requests in GIOP 1.2.  Don't handle
      // this on the firt iteration, leave it for the nearby future...
      // ERROR too.  @@ this->reply_handler_->error ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) %N:%l handle_client_input: "
                         "request.\n"),
                        -1);

    case TAO_GIOP::CancelRequest:
    case TAO_GIOP::LocateRequest:
    case TAO_GIOP::CloseConnection:
    default:
      // @@ Errors for the time being.
      // @@ this->reply_handler_->error ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) %N:%l handle_client_input: "
                         "wrong message.\n"),
                        -1);

    case TAO_GIOP::LocateReply:
    case TAO_GIOP::Reply:
      // Handle after the switch.
      break;

    case TAO_GIOP::Fragment:
      // Never happens:
      break;
    }

  // For GIOP 1.0 and 1.1 the reply_ctx comes first:
  // @@ Use <header.giop_version> to make this work with GIOP 1.2
  if ((state.cdr >> reply_ctx) == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::parse_reply, "
                    "extracting context\n"));
      return -1;
    }

  // Read the request id
  if (!state.cdr.read_ulong (request_id))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) : TAO_GIOP::parse_reply, "
                    "extracting request id"));
      return -1;
    }

  // and the reply status type.  status can be NO_EXCEPTION,
  // SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD or (on GIOP
  // 1.2) LOCATION_FORWARD_PERM
  if (!state.cdr.read_ulong (reply_status))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) : TAO_GIOP::parse_reply, "
                    "extracting reply status\n"));
      return -1;
    }
  return 0;
}

void
TAO_GIOP::process_server_message (TAO_Transport *transport,
                                  TAO_ORB_Core *orb_core,
                                  TAO_InputCDR &input,
                                  const TAO_GIOP_Message_State &state)
{
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
#if defined(ACE_HAS_PURIFY)
  (void) ACE_OS::memset (repbuf,
                         '\0',
                         sizeof repbuf);
#endif /* ACE_HAS_PURIFY */
  TAO_OutputCDR output (repbuf,
                        sizeof repbuf,
                        TAO_ENCAP_BYTE_ORDER,
                        orb_core->output_cdr_buffer_allocator (),
                        orb_core->output_cdr_dblock_allocator (),
                        orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                        orb_core->to_iso8859 (),
                        orb_core->to_unicode ());

  TAO_MINIMAL_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_RECEIVE_REQUEST_END);

  switch (state.message_type)
    {
    case TAO_GIOP::Request:
      // The following two routines will either raise an exception
      // or successfully write the response into <output>
      TAO_GIOP::process_server_request (transport,
                                        orb_core,
                                        input,
                                        output,
                                        state.giop_version);
      break;

    case TAO_GIOP::LocateRequest:
      TAO_GIOP::process_server_locate (transport,
                                       orb_core,
                                       input,
                                       output,
                                       state.giop_version);
      break;

    case TAO_GIOP::MessageError:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) MessageError received by server\n"));
      break;

      // These messages should never be sent to the server; it's an
      // error if the peer tries.  Set the environment accordingly,
      // as it's not yet been reported as an error.
    case TAO_GIOP::Reply:
    case TAO_GIOP::LocateReply:
    case TAO_GIOP::CloseConnection:
    default:   // Unknown message
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) Illegal message received by server\n"));
      TAO_GIOP::send_error (state.giop_version, transport);
      break;
    }
  TAO_MINIMAL_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_END);
}

void
TAO_GIOP::process_server_request (TAO_Transport *transport,
                                  TAO_ORB_Core* orb_core,
                                  TAO_InputCDR &input,
                                  TAO_OutputCDR &output,
                                  const TAO_GIOP_Version& version)
{
  CORBA::ULong request_id;
  CORBA::ULong response_required;
  CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ();
  ACE_TRY
    {
      // This will extract the request header, set <response_required>
      // as appropriate.

      int parse_error;
      TAO_GIOP_ServerRequest request (input,
                                      output,
                                      orb_core,
                                      version,
                                      parse_error);

      request_id = request.request_id ();
      response_required = request.response_expected ();

      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));


#if !defined (TAO_NO_IOR_TABLE)
      const CORBA::Octet *object_key =
        request.object_key ().get_buffer ();

      if (ACE_OS::memcmp (object_key,
                          &TAO_POA::objectkey_prefix[0],
                          TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
        {
          ACE_CString object_id (ACE_reinterpret_cast (const char *,
                                                       object_key),
                                 request.object_key ().length (),
                                 0,
                                 0);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Simple Object key %s. Doing the Table Lookup ...\n",
                        object_id.c_str ()));

          CORBA::Object_ptr object_reference =
            CORBA::Object::_nil ();

          // Do the Table Lookup.
          int status =
            orb_core->orb ()->_tao_find_in_IOR_table (object_id,
                                                      object_reference);

          // If ObjectID not in table or reference is nil raise
          // OBJECT_NOT_EXIST.

          if (status == -1 || CORBA::is_nil (object_reference))
            ACE_TRY_THROW (CORBA::OBJECT_NOT_EXIST ());

          // ObjectID present in the table with an associated NON-NULL
          // reference.  Throw a forward request exception.

          CORBA::Object_ptr dup =
            CORBA::Object::_duplicate (object_reference);

          // @@ We could simply write the response at this point...
          ACE_TRY_THROW (PortableServer::ForwardRequest (dup));
        }

#endif /* TAO_NO_IOR_TABLE */

      orb_core->object_adapter ()->dispatch_servant (request.object_key (),
                                                     request,
                                                     0,
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
#if !defined (TAO_HAS_MINIMUM_CORBA)
  ACE_CATCH (PortableServer::ForwardRequest, forward_request)
    {
      TAO_GIOP::start_message (version,
                               TAO_GIOP::Reply,
                               output,
                               orb_core);
      TAO_GIOP_ServiceContextList resp_ctx;
      resp_ctx.length (0);
      output << resp_ctx;

      output.write_ulong (request_id);

      output.write_ulong (TAO_GIOP_LOCATION_FORWARD);

      CORBA::Object_ptr object_ptr =
        forward_request.forward_reference.in();

      output << object_ptr;
    }
#else
  ACE_UNUSED_ARG (request_id);
#endif /* TAO_HAS_MINIMUM_CORBA */
  // Only CORBA exceptions are caught here.
  ACE_CATCHANY
    {
      if (response_required)
        {
          if (TAO_GIOP::send_reply_exception (version,
                                              transport,
                                              orb_core,
                                              request_id,
                                              &ACE_ANY_EXCEPTION) == -1)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            "TAO: (%P|%t) %p: cannot send exception\n",
                            "TAO_GIOP::process_server_message"));
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                   "TAO: ");
            }

          return;
        }
      else if (TAO_debug_level > 0)
        {
          // It is unfotunate that an exception (probably a system
          // exception) was thrown by the upcall code (even by the
          // user) when the client was not expecting a response.
          // However, in this case, we cannot close the connection
          // down, since it really isn't the client's fault.

          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) exception thrown "
                      "but client is not waiting a response\n"));
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "TAO: ");
        }
      return;
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // @@ TODO some c++ exception or another, but what do we do with
      //    it?
      // We are supposed to map it into a CORBA::UNKNOWN exception.
      // BTW, this cannot be detected if using the <env> mapping.  If
      // we have native exceptions but no support for them in the ORB
      // we should still be able to catch it.  If we don't have native
      // exceptions it couldn't have been raised in the first place!
      if (response_required)
        {
          CORBA::UNKNOWN exception
            (CORBA::SystemException::_tao_minor_code
             (TAO_UNHANDLED_SERVER_CXX_EXCEPTION, 0),
             CORBA::COMPLETED_MAYBE);

          if (TAO_GIOP::send_reply_exception (version,
                                              transport,
                                              orb_core,
                                              request_id,
                                              &exception) == -1)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            "TAO: (%P|%t) %p: cannot send exception\n",
                            "TAO_GIOP::process_server_message"));
              ACE_PRINT_EXCEPTION (exception, "TAO: ");
            }
        }
      else if (TAO_debug_level > 0)
        // It is unfotunate that an exception (probably a system
        // exception) was thrown by the upcall code (even by the user)
        // when the client was not expecting a response.  However, in
        // this case, we cannot close the connection down, since it
        // really isn't the client's fault.
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) exception thrown "
                    "but client is not waiting a response\n"));
      return;
    }
#else
  ACE_UNUSED_ARG (response_required);
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;

  if (response_required
      && TAO_GIOP::send_message (transport,
                                 output,
                                 orb_core) == -1
      && TAO_debug_level > 0)
    // No exception but some kind of error, yet a response is
    // required.
    ACE_ERROR ((LM_ERROR,
                "TAO: (%P|%t) %p: cannot send reply\n",
                "TAO_GIOP::process_server_message"));

}

void
TAO_GIOP::process_server_locate (TAO_Transport *transport,
                                 TAO_ORB_Core* orb_core,
                                 TAO_InputCDR &input,
                                 TAO_OutputCDR &output,
                                 const TAO_GIOP_Version& version)
{
  //  TAO_FUNCTION_PP_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_HANDLE_LOCATE_START);

  // This will extract the request header, set <response_required> as
  // appropriate.
  TAO_GIOP_LocateRequestHeader locateRequestHeader;

  TAO_GIOP_LocateStatusType status = TAO_GIOP_UNKNOWN_OBJECT;
  CORBA::Object_var forward_location_var;

  CORBA::Boolean response_required = 1;

  CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ();
  ACE_TRY
    {
      locateRequestHeader.init (input, ACE_TRY_ENV);
      ACE_TRY_CHECK;

#if !defined (TAO_NO_IOR_TABLE)

      const CORBA::Octet *object_key =
        locateRequestHeader.object_key.get_buffer ();

      if (ACE_OS::memcmp (object_key,
                          &TAO_POA::objectkey_prefix[0],
                          TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
        {
          ACE_CString object_id (ACE_reinterpret_cast (const char *,
                                                       object_key),
                                 locateRequestHeader.object_key.length (),
                                 0,
                                 0);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Simple Object key %s. Doing the Table Lookup ...\n",
                        object_id.c_str ()));

          CORBA::Object_ptr object_reference;

          // Do the Table Lookup.
          int find_status =
            orb_core->orb ()->_tao_find_in_IOR_table (object_id,
                                                      object_reference);

          // If ObjectID not in table or reference is nil raise
          // OBJECT_NOT_EXIST.

          if (CORBA::is_nil (object_reference)
              || find_status == -1)
            ACE_TRY_THROW (CORBA::OBJECT_NOT_EXIST ());

          // ObjectID present in the table with an associated NON-NULL
          // reference.  Throw a forward request exception.

          CORBA::Object_ptr dup =
            CORBA::Object::_duplicate (object_reference);

          // @@ We could simply write the response at this point...
          ACE_TRY_THROW (PortableServer::ForwardRequest (dup));
        }
#endif /* TAO_NO_IOR_TABLE */

      // Execute a fake request to find out if the object is there or
      // if the POA can activate it on demand...
      char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
      TAO_OutputCDR dummy_output (repbuf,
                                  sizeof repbuf);
      // This output CDR is not used!

      TAO_ObjectKey tmp_key (locateRequestHeader.object_key.length (),
                             locateRequestHeader.object_key.length (),
                             locateRequestHeader.object_key.get_buffer (),
                             0);

      int parse_error;
      TAO_GIOP_ServerRequest serverRequest (locateRequestHeader.request_id,
                                            response_required,
                                            tmp_key,
                                            "_non_existent",
                                            dummy_output,
                                            orb_core,
                                            version,
                                            parse_error);
      if (parse_error != 0)
        ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                       CORBA::COMPLETED_NO));

      orb_core->object_adapter ()->dispatch_servant (serverRequest.object_key (),
                                                     serverRequest,
                                                     0,
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (serverRequest.exception_type () == TAO_GIOP_NO_EXCEPTION)
        {
          // We got no exception, so the object is here.
          status = TAO_GIOP_OBJECT_HERE;
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO: (%P|%t) handle_locate() : found\n"));
        }
      else if (serverRequest.exception_type () != TAO_GIOP_NO_EXCEPTION)
        {
          forward_location_var = serverRequest.forward_location ();
          if (!CORBA::is_nil (forward_location_var.in ()))
            {
              status = TAO_GIOP_OBJECT_FORWARD;
              ACE_DEBUG ((LM_DEBUG,
                          "handle_locate has been called: forwarding\n"));
            }
          else
            {
              // Normal exception, so the object is not here
              status = TAO_GIOP_UNKNOWN_OBJECT;
              ACE_DEBUG ((LM_DEBUG,
                          "handle_locate has been called: not here\n"));
            }
        }

    }
#if !defined (TAO_HAS_MINIMUM_CORBA)
  ACE_CATCH (PortableServer::ForwardRequest, forward_request)
    {
      status = TAO_GIOP_OBJECT_FORWARD;
      forward_location_var =
        forward_request.forward_reference;
      ACE_DEBUG ((LM_DEBUG,
                  "handle_locate has been called: forwarding\n"));
    }
#endif /* TAO_HAS_MINIMUM_CORBA */
  ACE_CATCHANY
    {
      // Normal exception, so the object is not here
      status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::process_server_locate - "
                    "CORBA exception raised\n"));
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // Normal exception, so the object is not here
      status = TAO_GIOP_UNKNOWN_OBJECT;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::process_server_locate - "
                    "C++ exception raised\n"));
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;

  // Create the response.
  TAO_GIOP::start_message (version,
                           TAO_GIOP::LocateReply,
                           output,
                           orb_core);
  output.write_ulong (locateRequestHeader.request_id);
  output.write_ulong (status);

  if (status == TAO_GIOP_OBJECT_FORWARD)
    {
      CORBA::Object_ptr object_ptr = forward_location_var.in ();
      if ((output << object_ptr) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) TAO_GIOP::process_server_request -"
                        " cannot marshal object reference\n"));
        }
    }

  if (TAO_GIOP::send_message (transport,
                              output,
                              orb_core) == -1
      && TAO_debug_level > 0)
    ACE_ERROR ((LM_ERROR,
                "TAO: (%P|%t) %p: cannot send reply\n",
                "TAO_GIOP::process_server_message"));
}

int
TAO_GIOP::send_reply_exception (const TAO_GIOP_Version &version,
                                TAO_Transport *transport,
                                TAO_ORB_Core* orb_core,
                                CORBA::ULong request_id,
                                CORBA::Exception *x)
{
  // Create a new output CDR stream

  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
#if defined(ACE_HAS_PURIFY)
  (void) ACE_OS::memset (repbuf,
                         '\0',
                         sizeof repbuf);
#endif /* ACE_HAS_PURIFY */
  TAO_OutputCDR output (repbuf,
                        sizeof repbuf,
                        TAO_ENCAP_BYTE_ORDER,
                        orb_core->output_cdr_buffer_allocator (),
                        orb_core->output_cdr_dblock_allocator (),
                        orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                        orb_core->to_iso8859 (),
                        orb_core->to_unicode ());

  // Construct a REPLY header.
  TAO_GIOP::start_message (version,
                           TAO_GIOP::Reply,
                           output,
                           orb_core);

  // A new try/catch block, but if something goes wrong now we have no
  // hope, just abort.
  ACE_TRY_NEW_ENV
    {
      // create and write a dummy context
      TAO_GIOP_ServiceContextList resp_ctx;
      resp_ctx.length (0);
      output << resp_ctx;

      // Write the request ID
      output.write_ulong (request_id);

      // Write the exception
      CORBA::TypeCode_ptr except_tc = x->_type ();

      CORBA::exception_type extype =
        CORBA::USER_EXCEPTION;

      if (CORBA::SystemException::_narrow (x) != 0)
        extype = CORBA::SYSTEM_EXCEPTION;

      // write the reply_status
      output.write_ulong (TAO_GIOP::convert_CORBA_to_GIOP_exception (extype));

      // @@ Any way to implement this without interpretive
      //    marshaling???
      output.encode (except_tc,
                     x,
                     0,
                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA_Exception, ex)
    {
      // Now we know that while handling the error an other error
      // happened -> no hope, close connection.

      // Close the handle.
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) cannot marshal exception %p\n",
                  transport->handle (),
                  "TAO_GIOP::send_reply_exception"));
      return -1;
    }
  ACE_ENDTRY;

  return TAO_GIOP::send_message (transport,
                                 output,
                                 orb_core);
}

CORBA::Boolean
TAO_GIOP::start_message_std (const TAO_GIOP_Version &version,
                             TAO_GIOP::Message_Type type,
                             TAO_OutputCDR &msg)
{
  msg.reset ();

  // if (msg.size () < TAO_GIOP_HEADER_LEN)
  // return 0;

  static CORBA::Octet magic[] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50, // 'P'
  };

  static int magic_size = sizeof(magic)/sizeof(magic[0]);
  msg.write_octet_array (magic, magic_size);
  msg.write_octet (version.major);
  msg.write_octet (version.minor);
  msg.write_octet (TAO_ENCAP_BYTE_ORDER);
  msg.write_octet (type);

  // Write a dummy <size> later it is set to the right value...
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  return 1;
}

CORBA::Boolean
TAO_GIOP::start_message_lite (const TAO_GIOP_Version &version,
                              TAO_GIOP::Message_Type type,
                              TAO_OutputCDR &msg)
{
  msg.reset ();

  // Write a dummy <size> later it is set to the right value...  @@
  // TODO Maybe we should store the OutputCDR status in
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  msg.write_octet (type);

  return 1;
}

CORBA::Boolean
TAO_GIOP::write_request_header_std (const TAO_GIOP_ServiceContextList& svc_ctx,
                                    CORBA::ULong request_id,
                                    CORBA::Boolean is_roundtrip,
                                    const TAO_opaque& key,
                                    const char* opname,
                                    CORBA::Principal_ptr principal,
                                    TAO_OutputCDR &out_stream)
{
  out_stream << svc_ctx;
  out_stream << request_id;
  // @@ Messaging: this is where the extra synchronization information
  //    should be stored.
  out_stream << CORBA::Any::from_boolean (is_roundtrip);
  out_stream << key;
  out_stream << opname;
  out_stream << principal;
  return 1;
}

CORBA::Boolean
TAO_GIOP::write_request_header_lite (const TAO_GIOP_ServiceContextList&,
                                     CORBA::ULong request_id,
                                     CORBA::Boolean is_roundtrip,
                                     const TAO_opaque &key,
                                     const char* opname,
                                     CORBA::Principal_ptr,
                                     TAO_OutputCDR &out_stream)
{
  out_stream << request_id;
  out_stream << CORBA::Any::from_boolean (is_roundtrip);
  out_stream << key;
  out_stream << opname;
  return 1;
}

int
TAO_GIOP::parse_header (TAO_ORB_Core *orb_core,
                        TAO_InputCDR &input,
                        TAO_GIOP_Message_State& state)
{
  if (orb_core->orb_params ()->use_lite_protocol ())
    return TAO_GIOP::parse_header_lite (input, state);
  else
    return TAO_GIOP::parse_header_std (input, state);
}

int
TAO_GIOP::parse_header_std (TAO_InputCDR &input,
                            TAO_GIOP_Message_State &state)
{
  char *buf = input.rd_ptr ();

  // The values are hard-coded to support non-ASCII platforms.
  if (!(buf [0] == 0x47      // 'G'
        && buf [1] == 0x49   // 'I'
        && buf [2] == 0x4f   // 'O'
        && buf [3] == 0x50)) // 'P'
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) bad header, magic word [%c%c%c%c]\n",
                    buf[0],
                    buf[1],
                    buf[2],
                    buf[3]));
      return -1;
    }

  state.giop_version.major = buf[TAO_GIOP_VERSION_MAJOR_OFFSET];
  state.giop_version.minor = buf[TAO_GIOP_VERSION_MINOR_OFFSET];

  if (state.giop_version.major != TAO_DEF_GIOP_MAJOR
      || state.giop_version.minor > TAO_DEF_GIOP_MINOR)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) bad version <%d.%d>\n",
                    state.giop_version.major,
                    state.giop_version.minor));
      return -1;
    }

  if (state.giop_version.minor == 0)
    {
      state.byte_order = buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET];
      if (TAO_debug_level > 2
          && state.byte_order != 0 && state.byte_order != 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) invalid byte order <%d>"
                      " for version <1.0>\n",
                      state.byte_order));
          return -1;
        }
    }
  else
    {
      state.byte_order     = buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & 0x01;
      state.more_fragments = buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & 0x02;
      if (TAO_debug_level > 2
          && state.giop_version.minor == 1
          && (buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET] & ~0x3) != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) invalid flags for <%d>"
                      " for version <1.1>\n",
                      buf[TAO_GIOP_MESSAGE_FLAGS_OFFSET]));
          return -1;
        }

    }

  state.message_type = buf[TAO_GIOP_MESSAGE_TYPE_OFFSET];

  input.reset_byte_order (state.byte_order);
  input.skip_bytes (TAO_GIOP_MESSAGE_SIZE_OFFSET);
  input.read_ulong (state.message_size);

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) Parsed header = <%d,%d,%d,%d,%d>\n",
                  state.giop_version.major,
                  state.giop_version.minor,
                  state.byte_order,
                  state.message_type,
                  state.message_size));
    }
  return 0;
}

int
TAO_GIOP::parse_header_lite (TAO_InputCDR &input,
                             TAO_GIOP_Message_State &state)
{
  char *buf = input.rd_ptr ();

  state.giop_version.major = TAO_DEF_GIOP_MAJOR;
  state.giop_version.minor = TAO_DEF_GIOP_MINOR;
  state.byte_order = TAO_ENCAP_BYTE_ORDER;
  state.message_type = buf[TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET];

  input.reset_byte_order (state.byte_order);
  input.read_ulong (state.message_size);
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Unbounded_Sequence<TAO_GIOP_ServiceContext>;
template class TAO_Unbounded_Sequence<TAO_IOP_TaggedComponent>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Unbounded_Sequence<TAO_GIOP_ServiceContext>
#pragma instantiate TAO_Unbounded_Sequence<TAO_IOP_TaggedComponent>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
