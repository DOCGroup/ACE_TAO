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

static const char digits [] = "0123456789ABCD";
static const char *names [] =
{
  "CommunicationError",
  "EndOfFile",
  "ShortRead",
  "Request",
  "Reply",
  "CancelRequest",
  "LocateRequest",
  "LocateReply",
  "CloseConnection",
  "MessageError"
  "Fragment"
};

CORBA::ULong const TAO_GIOP::tao_specific_message_types = 3;

void
TAO_GIOP::dump_msg (const char *label,
                    const u_char *ptr,
                    size_t len)
{
  if (TAO_debug_level >= 5)
    {
      const char* message_name = "UNKNOWN MESSAGE";
      unsigned long index = ptr[7] + TAO_GIOP::tao_specific_message_types;
      if (index >= 0 && index < sizeof (names)/sizeof(names[0]))
        {
          message_name = names [index];
        }
      ACE_DEBUG ((LM_DEBUG,
                  "%s GIOP v%c.%c msg, %d data bytes, %s endian, %s",
                  label,
                  digits[ptr[4]],
                  digits[ptr[5]],
                  len - TAO_GIOP_HEADER_LEN,
                  (ptr[6] == TAO_ENCAP_BYTE_ORDER) ? "my" : "other",
                  message_name));

      if (ptr[7] == TAO_GIOP::Request)
        {
          // @@ Only works if ServiceContextList is empty....
          const CORBA::ULong *request_id =
            ACE_reinterpret_cast(const CORBA::ULong*,
                                 ptr + TAO_GIOP_HEADER_LEN + 4);
          ACE_DEBUG ((LM_DEBUG, " = %d\n", *request_id));
        }
      else if (ptr[7] == TAO_GIOP::Reply)
        {
          const CORBA::ULong *request_id =
            ACE_reinterpret_cast(const CORBA::ULong*,
                                 ptr + TAO_GIOP_HEADER_LEN + 4);
          ACE_DEBUG ((LM_DEBUG, " = %d\n", *request_id));
        }
      else
        ACE_DEBUG ((LM_DEBUG, "\n"));

      if (TAO_debug_level >= 10)
        ACE_HEX_DUMP ((LM_DEBUG,
                       (const char*)ptr,
                       len,
                       "GIOP message"));
    }
}

CORBA::Boolean
operator<<(TAO_OutputCDR &cdr,
           const TAO_GIOP_ServiceContext &x)
{
  if (cdr << x.context_id
       && cdr << x.context_data)
    return 1;
  else
    return 0;
}

CORBA::Boolean
operator>>(TAO_InputCDR &cdr,
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
operator>>(TAO_InputCDR &cdr,
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
TAO_GIOP::send_message (TAO_Transport *transport,
                        TAO_OutputCDR &stream,
                        TAO_ORB_Core *orb_core)
{

  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_SEND_REQUEST_START);

  char *buf = (char *) stream.buffer (); // ptr to first buffer
  size_t total_len = stream.total_length ();  // length of all buffers

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
  //       "messaging protocols" (maybe this is a bad name, but I want
  //       to stress the difference with the pluggable "transport
  //       protocols" that you recently completed).
  //       For example: it seems that the transport layer needs to
  //       know the size of the header and somehow needs to determine
  //       the size of the message from that header, this could be
  //       virtual methods in the MessagingProtocol class.
  //       Just a wild thought..... Carlos
  //
  size_t offset = TAO_GIOP_MESSAGE_SIZE_OFFSET;
  if (orb_core->orb_params ()->use_lite_protocol ())
    {
      header_len = TAO_GIOP_LITE_HEADER_LEN;
      offset = TAO_GIOP_LITE_MESSAGE_SIZE_OFFSET;
    }

  CORBA::ULong bodylen = total_len - header_len;

#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
  *ACE_reinterpret_cast(CORBA::ULong*,buf + offset) = bodylen;
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
                      ACE_reinterpret_cast (u_char *, buf),
                      stream.length ());

  // this guarantees to send all data (bytes) or return an error
  ssize_t n = transport->send (stream.begin ());

  if (n == -1)
    {
      if (TAO_orbdebug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO: (%P|%t) closing conn %d after fault %p\n",
                      transport->handle (),
                      "GIOP::send_request ()"));
      }
    transport->close_connection ();
    return 0;
  }

  // @@ Don't know about this one, when will we get a 0 from the write
  //    if we assume that there is data to write.  I would only expect
  //    a 0 if there was nothing to send or if nonblocking.
  if (n == 0)
    {
      if (TAO_orbdebug)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO: (%P|%t) GIOP::send_request () "
                      "EOF, closing conn %d\n",
                      transport->handle()));
        }
      transport->close_connection ();
      return 0;
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

// static CORBA::Octet
static const char close_message [TAO_GIOP_HEADER_LEN] =
{
  // 'G', 'I', 'O', 'P',
  0x47, 0x49, 0x4f, 0x50, // work on non-ASCII platforms...
  TAO_GIOP_MessageHeader::MY_MAJOR,
  TAO_GIOP_MessageHeader::MY_MINOR,
  TAO_ENCAP_BYTE_ORDER,
  TAO_GIOP::CloseConnection,
  0, 0, 0, 0
};

void
TAO_GIOP::close_connection (TAO_Transport *transport, void *)
{
  // It's important that we use a reliable shutdown after we send this
  // message, so we know it's received.
  //
  // @@ should recv and discard queued data for portability; note
  // that this won't block (long) since we never set SO_LINGER

  TAO_GIOP::dump_msg ("close_connection",
                      (const u_char *) close_message,
                      TAO_GIOP_HEADER_LEN);

  // @@ Carlos, can you please check the return value on this?
  ACE_HANDLE which = transport->handle ();
  if (transport->send ((const u_char *) close_message, TAO_GIOP_HEADER_LEN) == -1)
  {
    if (TAO_orbdebug)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) error closing connection %d\n",
                    which));
  }

  transport->close_connection ();
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) shut down transport, handle %d\n", which));
}

// Send an "I can't understand you" message -- again, the message is
// prefabricated for simplicity.  This implies abortive disconnect (at
// the application level, if not at the level of TCP).
//
// NOTE that IIOP will still benefit from TCP's orderly disconnect.

static const char
error_message [TAO_GIOP_HEADER_LEN] =
{
  // 'G', 'I', 'O', 'P',
  0x47, 0x49, 0x4f, 0x50, // work on non-ASCII platforms...
  TAO_GIOP_MessageHeader::MY_MAJOR,
  TAO_GIOP_MessageHeader::MY_MINOR,
  TAO_ENCAP_BYTE_ORDER,
  TAO_GIOP::MessageError,
  0, 0, 0, 0
};

void
TAO_GIOP::send_error (TAO_Transport *transport)
{
  // @@ How does this works with GIOP lite?

  TAO_GIOP::dump_msg ("send_error",
                      (const u_char *) error_message,
                      TAO_GIOP_HEADER_LEN);

  ACE_HANDLE which = transport->handle ();

  // @@ Carlos, can you please check to see if <send_n> should have
  // it's reply checked?
  if (transport->send ((const u_char *)error_message,
                       TAO_GIOP_HEADER_LEN) == -1)
  {
    if (TAO_orbdebug != 0)
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) error sending error to %d\n",
                  which));
  }

  if (TAO_orbdebug) {
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) aborted transport handle %d\n",
                transport->handle ()));
  }
  // @@ Why do we close the connection at this point?
  transport->close_connection ();
}

ssize_t
TAO_GIOP::read_buffer (TAO_Transport *transport,
                       char *buf,
                       size_t len)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_READ_BUFFER_START);

  ssize_t bytes_read = transport->recv (buf, len);

  if (bytes_read <= 0 && TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - %p,\n"
                  "              transport = %d, "
                  "bytes = %d, len = %d\n",
                  "TAO_GIOP::read_buffer",
                  transport->handle (),
                  bytes_read, len));
    }

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

int
TAO_GIOP::parse_header_std (ACE_Message_Block *payload,
                            TAO_GIOP_MessageHeader &header)
{
  char *buf = payload->rd_ptr ();

  // The values are hard-coded to support non-ASCII platforms
  if (!(buf [0] == 0x47    // 'G'
        && buf [1] == 0x49 // 'I'
        && buf [2] == 0x4f // 'O'
        && buf [3] == 0x50 // 'P'
        ))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) bad header, magic word [%c%c%c%c]\n",
                    buf[0], buf[1], buf[2], buf[3]));
      return -1;
    }

#if 0
  // @@ Nobody uses this magic number, no sense in wasting time here.
  header.magic[0] = 0x47;
  header.magic[1] = 0x49;
  header.magic[2] = 0x4f;
  header.magic[3] = 0x50;
#endif /* 0 */
  header.giop_version.major = buf[4];
  header.giop_version.minor = buf[5];
  header.byte_order = buf[6];
  header.message_type = buf[7];

  TAO_InputCDR cdr (payload,
                    ACE_static_cast(int,header.byte_order));

  cdr.skip_bytes (TAO_GIOP_MESSAGE_SIZE_OFFSET);
  cdr.read_ulong (header.message_size);

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) Parsed header = <%d,%d,%d,%d,%d>\n",
                  header.giop_version.major,
                  header.giop_version.minor,
                  header.byte_order,
                  header.message_type,
                  header.message_size));
    }
  return 0;
}

int
TAO_GIOP::parse_header_lite (ACE_Message_Block *payload,
                             TAO_GIOP_MessageHeader& header)
{
  char *buf = payload->rd_ptr ();

#if 0
  // @@ Nobody uses this magic number, no sense in wasting time here.
  header.magic[0] = 0x47;
  header.magic[1] = 0x49;
  header.magic[2] = 0x4f;
  header.magic[3] = 0x50;
#endif /* 0 */
  header.giop_version.major = 1;
  header.giop_version.minor = 0;
  header.byte_order = TAO_ENCAP_BYTE_ORDER;
  header.message_type = buf[4];

  TAO_InputCDR cdr (payload,
                    ACE_static_cast(int,header.byte_order));

  cdr.read_ulong (header.message_size);
  return 0;
}

int
TAO_GIOP::parse_header (TAO_ORB_Core *orb_core,
                        ACE_Message_Block *payload,
                        TAO_GIOP_MessageHeader& header)
{
  if (orb_core->orb_params ()->use_lite_protocol ())
    return TAO_GIOP::parse_header_lite (payload,
                                        header);
  else
    return TAO_GIOP::parse_header_std (payload,
                                       header);
}

CORBA::Boolean
TAO_GIOP_LocateRequestHeader::init (TAO_InputCDR &msg,
                                    CORBA::Environment &)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_LOCATE_REQUEST_HEADER_INIT_START);

  return (msg.read_ulong (this->request_id)
          && (msg >> this->object_key) != 0);
}

CORBA::Boolean
TAO_GIOP::start_message_std (TAO_GIOP::Message_Type type,
                             TAO_OutputCDR &msg)
{
  msg.reset ();

  // if (msg.size () < TAO_GIOP_HEADER_LEN)
  // return 0;

  static CORBA::Octet header[] =
  {
    // 'G', 'I', 'O', 'P',
    0x47, 0x49, 0x4f, 0x50, // work on non-ASCII platforms...
    TAO_GIOP_MessageHeader::MY_MAJOR,
    TAO_GIOP_MessageHeader::MY_MINOR,
    TAO_ENCAP_BYTE_ORDER
  };

  static int header_size =
    sizeof(header)/sizeof(header[0]);
  msg.write_octet_array (header,
                         header_size);
  msg.write_octet (type);

  // Write a dummy <size> later it is set to the right value...
  // @@ TODO Maybe we should store the OutputCDR status in
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  return 1;
}

CORBA::Boolean
TAO_GIOP::start_message_lite (TAO_GIOP::Message_Type type,
                              TAO_OutputCDR &msg)
{
  msg.reset ();

  // Write a dummy <size> later it is set to the right value...
  // @@ TODO Maybe we should store the OutputCDR status in
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  msg.write_octet (type);

  return 1;
}

CORBA::Boolean
TAO_GIOP::start_message (TAO_GIOP::Message_Type type,
                         TAO_OutputCDR &msg,
                         TAO_ORB_Core* orb_core)
{
  if (orb_core->orb_params ()->use_lite_protocol ())
    return TAO_GIOP::start_message_lite (type, msg);
  else
    return TAO_GIOP::start_message_std (type, msg);
}

const char *
TAO_GIOP::message_name (TAO_GIOP::Message_Type which)
{
  static const char *msgnames[] =
  {
    "EndOfFile (nonstd)",
    "Request (client)",
    "Reply (server)",
    "CancelRequest (client)",
    "LocateRequest (client)",
    "LocateReply (server)",
    "CloseConnection (server)",
    "MessageError (either)"
  };

  int i = (int) which + 1;
  // Add one since EndOfFile is -1.

  if (i > (int) (sizeof (msgnames) / sizeof (msgnames[0])))
    return "<Bad Value!>";
  else
    return msgnames[i];
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

// ****************************************************************

int
TAO_GIOP::read_header (TAO_Transport *transport,
                       TAO_ORB_Core *orb_core,
                       TAO_GIOP_MessageHeader &header,
                       CORBA::ULong &header_size,
                       ACE_Message_Block *payload)
{
  // Default header length.
  header_size = TAO_GIOP_HEADER_LEN;
  if (orb_core->orb_params ()->use_lite_protocol ())
    header_size = TAO_GIOP_LITE_HEADER_LEN;

  if (ACE_CDR::grow (payload, header_size) == -1)
    return -1;

  ACE_CDR::mb_align (payload);

  // Read until all the header is received.  There should be no
  // problems with locking, the header is only a few bytes so they
  // should all be available on the socket, otherwise there is a
  // problem with the underlying transport, in which case we have more 
  // problems than just this small loop.

  char* buf = payload->rd_ptr ();
  int t = header_size;
  while (t != 0)
    {
      int n = transport->recv (buf, t);
      if (n == -1)
        return -1;
      else if (n == 0 && errno != EWOULDBLOCK)
        return -1;
      buf += n;
      t -= n;
    }

  // Adjust the length of the payload
  payload->wr_ptr (header_size);

  if (TAO_GIOP::parse_header (orb_core, payload, header) == -1)
    return -1;
  return header_size;
}

int
TAO_GIOP::handle_input (TAO_Transport *transport,
                        TAO_ORB_Core *orb_core,
                        TAO_GIOP_MessageHeader &header,
                        CORBA::ULong &current_offset,
                        ACE_Message_Block *payload)
{
  if (header.message_size == 0)
    {
      current_offset = 0;
      CORBA::ULong header_size;
      if (TAO_GIOP::read_header (transport,
                                 orb_core,
                                 header,
                                 header_size,
                                 payload) == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "TAO (%P|%t) - %p\n",
                          "TAO_GIOP::handle_input, read_header"));
            }
          return -1;
        }

      if (ACE_CDR::grow (payload,
                         header_size +
                         header.message_size) == -1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - %p\n",
                      "TAO_GIOP::handle_input, ACE_CDR::grow"));
          return -1;
        }

      // Growing the buffer may have reset the rd_ptr(), but we want to
      // leave it just after the GIOP header (that was parsed already);
      ACE_CDR::mb_align (payload);
      payload->wr_ptr (header_size);
      payload->wr_ptr (header.message_size);
      payload->rd_ptr (header_size);
    }

  size_t missing_data =
    header.message_size - current_offset;
  ssize_t n =
    TAO_GIOP::read_buffer (transport,
                           payload->rd_ptr () + current_offset,
                           missing_data);
  if (n == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - %p\n",
                      "TAO_GIOP::handle_input, read_buffer[1]"));
        }
      return -1;
    }
  else if (n == 0)
    {
      if (errno == EWOULDBLOCK)
        return 0;
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - %p\n",
                      "TAO_GIOP::handle_input, read_buffer[2]"));
        }
      return -1;
    }

  current_offset += n;

  if (current_offset == header.message_size)
    {
      if (TAO_debug_level >= 5)
        {
          size_t header_len = TAO_GIOP_HEADER_LEN;
          if (orb_core->orb_params ()->use_lite_protocol ())
            header_len = TAO_GIOP_LITE_HEADER_LEN;

          char* buf = payload->rd_ptr ();
          buf -= header_len;
          size_t msg_len = payload->length () + header_len;
          TAO_GIOP::dump_msg ("recv",
                              ACE_reinterpret_cast (u_char *, buf),
                              msg_len);
        }
      return 1;
    }
  return 0;
}

int
TAO_GIOP::parse_reply (TAO_Transport *transport,
                       TAO_ORB_Core *orb_core,
                       TAO_InputCDR& input,
                       const TAO_GIOP_MessageHeader& header,
                       TAO_GIOP_ServiceContextList& reply_ctx,
                       CORBA::ULong& request_id,
                       CORBA::ULong& reply_status)
{
  switch (header.message_type)
    {
    case TAO_GIOP::Request:
      // In GIOP 1.0 and GIOP 1.1 this is an error, but it is
      // *possible* to receive requests in GIOP 1.2.  Don't handle this
      // on the firt iteration, leave it for the nearby future...
      // ERROR too.
      // @@ this->reply_handler_->error ();
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
    }

  // For GIOP 1.0 and 1.1 the reply_ctx comes first:
  // @@ Use <header.giop_version> to make this work with GIOP 1.2
  if ((input >> reply_ctx) == 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) TAO_GIOP::parse_reply, "
                    "extracting context\n"));
      return -1;
    }

  // Read the request id
  if (!input.read_ulong (request_id))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) : TAO_GIOP::parse_reply, "
                    "extracting request id"));
      return -1;
    }

  // and the reply status type.
  // status can be NO_EXCEPTION, SYSTEM_EXCEPTION, USER_EXCEPTION,
  // LOCATION_FORWARD or (on GIOP 1.2) LOCATION_FORWARD_PERM
  if (!input.read_ulong (reply_status))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) : TAO_GIOP::parse_reply, "
                    "extracting reply status\n"));
      return -1;
    }
  return 0;
}

// ****************************************************************

int
TAO_GIOP::process_server_message (TAO_Transport *transport,
                                  TAO_ORB_Core *orb_core,
                                  TAO_InputCDR &input,
                                  const TAO_GIOP_MessageHeader& header)
{
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
#if defined(ACE_HAS_PURIFY)
  (void) ACE_OS::memset (repbuf, '\0', sizeof (repbuf));
#endif /* ACE_HAS_PURIFY */
  TAO_OutputCDR output (repbuf, sizeof(repbuf),
                        TAO_ENCAP_BYTE_ORDER,
                        orb_core->output_cdr_buffer_allocator (),
                        orb_core->output_cdr_dblock_allocator (),
                        orb_core->orb_params ()->cdr_memcpy_tradeoff ());

  int result = 0;
  int error_encountered = 0;
  CORBA::Boolean response_required = 0;
  CORBA::ULong request_id = 0;

  CORBA::Environment &ACE_TRY_ENV = CORBA::default_environment ();
  ACE_TRY
    {
      TAO_MINIMAL_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_RECEIVE_REQUEST_END);

      // Check to see if we've been cancelled cooperatively.
      if (orb_core->orb ()->should_shutdown () != 0)
        error_encountered = 1;
      else
        {
          switch (header.message_type)
            {
            case TAO_GIOP::Request:
              // Message was successfully read, so handle it.  If we
              // encounter any errors, <output> will be set
              // appropriately by the called code, and -1 will be
              // returned.
              if (TAO_GIOP::process_server_request (transport,
                                                    orb_core,
                                                    input,
                                                    output,
                                                    response_required,
                                                    request_id,
                                                    ACE_TRY_ENV) == -1)
                error_encountered = 1;
              ACE_TRY_CHECK;
              break;

            case TAO_GIOP::LocateRequest:
              if (TAO_GIOP::process_server_locate (transport,
                                                   orb_core,
                                                   input,
                                                   output,
                                                   response_required,
                                                   request_id,
                                                   ACE_TRY_ENV) == -1)
                error_encountered = 1;
              ACE_TRY_CHECK;
              break;

              // These messages should never be sent to the server;
              // it's an error if the peer tries.  Set the environment
              // accordingly, as it's not yet been reported as an
              // error.
            case TAO_GIOP::Reply:
            case TAO_GIOP::LocateReply:
            case TAO_GIOP::CloseConnection:
            default:   // Unknown message
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) Illegal message received by server\n"));
              ACE_TRY_THROW (CORBA::COMM_FAILURE ());
              // NOTREACHED

            case TAO_GIOP::CommunicationError:
            case TAO_GIOP::MessageError:
              // Here, MessageError can either mean condition for
              // GIOP::MessageError happened or a GIOP message was
              // not successfully received.  Sending back of
              // GIOP::MessageError is handled in TAO_GIOP::parse_header.
              error_encountered = 1;
              break;
            }
        }
    }
  ACE_CATCHANY                  // Only CORBA exceptions are caught here.
    {
      if (response_required)
        return TAO_GIOP::send_reply_exception (transport,
                                               orb_core,
                                               request_id,
                                               &ACE_ANY_EXCEPTION);
      else
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) exception thrown "
                          "but client is not waiting a response\n"));
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO: ");
            }

          // It is unfotunate that an exception (probably a system
          // exception) was thrown by the upcall code (even by the
          // user) when the client was not expecting a response.
          // However, in this case, we cannot close the connection
          // down, since it really isn't the client's fault.
          result = 0;
        }
      return result;
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      // @@ TODO some c++ exception or another, but what do we do with
      //    it?
      // We are supposed to map it into a CORBA::UNKNOWN exception.
      // BTW, this cannot be detected if using the <env> mapping.
      //   If we have native exceptions but no support for them
      //   in the ORB we should still be able to catch it.
      //   If we don't have native exceptions it couldn't have been
      //   raised in the first place!

      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) closing conn %d after C++ exception %p\n",
                  transport.get_handle (),
                  "TAO_GIOP::process_server_message"));
      return -1;
    }
#endif /* TAO_HAS_EXCEPTIONS */
  ACE_ENDTRY;

  if (response_required)
    {
      if (!error_encountered)
        TAO_GIOP::send_message (transport,
                                output,
                                orb_core);
      else
        {
          // No exception but some kind of error, yet a response is
          // required.
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "TAO: (%P|%t) %s: closing conn, no exception, "
                        "but expecting response\n",
                        "TAO_GIOP::process_server_message"));
          return -1;
        }
    }
  else if (error_encountered)
    {
      // No exception, no response expected, but an error ocurred,
      // close the socket.
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO: (%P|%t) %s: closing conn, no exception, "
                    "but expecting response\n",
                    "TAO_GIOP::process_server_message"));
      return -1;
    }

  TAO_MINIMAL_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_HANDLE_INPUT_END);

  return result;
}


int
TAO_GIOP::process_server_request (TAO_Transport *transport,
                                  TAO_ORB_Core* orb_core,
                                  TAO_InputCDR &input,
                                  TAO_OutputCDR &output,
                                  CORBA::Boolean &response_required,
                                  CORBA::ULong &request_id,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // This will extract the request header, set <response_required> as
  // appropriate.
  TAO_GIOP_ServerRequest request (input,
                                  output,
                                  orb_core,
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // The request_id_ field in request will be 0 if something went
  // wrong before it got a chance to read it out.
  request_id = request.request_id ();

  response_required = request.response_expected ();

#if !defined (TAO_NO_IOR_TABLE)

  const CORBA::Octet *object_key = request.object_key ().get_buffer ();

  if (ACE_OS::memcmp (object_key,
                      &TAO_POA::objectkey_prefix[0],
                      TAO_POA::TAO_OBJECTKEY_PREFIX_SIZE) != 0)
    {
      ACE_CString object_id (ACE_reinterpret_cast (const char *, object_key),
                             request.object_key ().length (),
                             0,
                             0);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Simple Object key %s. Doing the Table Lookup ...\n",
                    object_id.c_str ()));

      CORBA::Object_ptr object_reference;

      // Do the Table Lookup.
      int status =
        orb_core->orb ()->_tao_find_in_IOR_table (object_id,
                                                  object_reference);

      // If ObjectID not in table or reference is nil raise OBJECT_NOT_EXIST.

      if (CORBA::is_nil (object_reference) || status == -1)
        ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), -1);

      // ObjectID present in the table with an associated NON-NULL reference.
      // Throw a forward request exception.

      CORBA::Object_ptr dup = CORBA::Object::_duplicate (object_reference);

      // @@ We could simply write the response at this point...
      ACE_THROW_RETURN (PortableServer::ForwardRequest (dup), -1);
    }

#endif /* TAO_NO_IOR_TABLE */

  orb_core->object_adapter ()->dispatch_servant (request.object_key (),
                                                 request,
                                                 0,
                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_GIOP::process_server_locate (TAO_Transport *transport,
                                 TAO_ORB_Core* orb_core,
                                 TAO_InputCDR &input,
                                 TAO_OutputCDR &output,
                                 CORBA::Boolean &response_required,
                                  CORBA::ULong &request_id,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  //  TAO_FUNCTION_PP_TIMEPROBE (TAO_SERVER_CONNECTION_HANDLER_HANDLE_LOCATE_START);

  // This will extract the request header, set <response_required> as
  // appropriate.
  TAO_GIOP_LocateRequestHeader locateRequestHeader;

  request_id = locateRequestHeader.request_id;
  response_required = 0;
  if (locateRequestHeader.init (input, ACE_TRY_ENV) == 0)
    return -1;
  ACE_CHECK_RETURN (-1);

  // Copy the request ID to be able to respond in case of an
  // exception.
  request_id = locateRequestHeader.request_id;
  response_required = 1;

  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
  TAO_OutputCDR dummy_output (repbuf, sizeof(repbuf));
  // This output CDR is not used!

  TAO_ObjectKey tmp_key (locateRequestHeader.object_key.length (),
                         locateRequestHeader.object_key.length (),
                         locateRequestHeader.object_key.get_buffer (),
                         0);

  CORBA::Object_var forward_location_var;
  TAO_GIOP_LocateStatusType status;

  TAO_GIOP_ServerRequest serverRequest (locateRequestHeader.request_id,
                                        response_required,
                                        tmp_key,
                                        "_non_existent",
                                        dummy_output,
                                        orb_core,
                                        ACE_TRY_ENV);

  ACE_TRY
    {
      orb_core->object_adapter ()->dispatch_servant (serverRequest.object_key (),
                                                     serverRequest,
                                                     0,
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (serverRequest.exception_type () == TAO_GIOP_NO_EXCEPTION)
        {
          // we got no exception, so the object is here
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
      forward_location_var = forward_request.forward_reference;
      ACE_DEBUG ((LM_DEBUG,
                  "handle_locate has been called: forwarding\n"));
    }
#endif /* TAO_HAS_MINIMUM_CORBA */
  ACE_CATCHANY
    {
      // Normal exception, so the object is not here
      status = TAO_GIOP_UNKNOWN_OBJECT;
      ACE_DEBUG ((LM_DEBUG,
                  "handle_locate has been called: not here\n"));
    }
  ACE_ENDTRY;

  // Create the response.
  TAO_GIOP::start_message (TAO_GIOP::LocateReply,
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
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Server_Connection_Handler::handle_locate - "
                          "error marshaling forwarded reference\n"));
            }
          response_required = 0;
          return -1;
        }
    }

  return 0;
}

int
TAO_GIOP::send_reply_exception (TAO_Transport *transport,
                                TAO_ORB_Core* orb_core,
                                CORBA::ULong request_id,
                                CORBA::Exception *x)
{
  // Create a new output CDR stream
  TAO_OutputCDR output;

  // Construct a REPLY header.
  TAO_GIOP::start_message (TAO_GIOP::Reply, output, orb_core);

  // A new try/catch block, but if something goes wrong now we
  // have no hope, just abort.
  ACE_TRY_NEW_ENV
    {
      // create and write a dummy context
      TAO_GIOP_ServiceContextList resp_ctx;
      resp_ctx.length (0);
      output << resp_ctx;

      // Write the request ID
      output.write_ulong (request_id);

#if !defined (TAO_HAS_MINIMUM_CORBA)

      // @@ TODO This is the place to conditionally compile
      // forwarding. It certainly seems easy to strategize too,
      // just invoke an strategy to finish marshalling the
      // response.

      // Now we check for Forwarding ***************************

      // Try to narrow to ForwardRequest
      PortableServer::ForwardRequest_ptr forward_request_ptr =
        PortableServer::ForwardRequest::_narrow (x);

      // If narrowing of exception succeeded
      if (forward_request_ptr != 0
          && !CORBA::is_nil (forward_request_ptr->forward_reference.in ()))
        {
          // write the reply_status
          output.write_ulong (TAO_GIOP_LOCATION_FORWARD);

          // write the object reference into the stream
          CORBA::Object_ptr object_ptr =
            forward_request_ptr->forward_reference.in();

          output << object_ptr;
        }
      // end of the forwarding code ****************************
      else

#endif /* TAO_HAS_MINIMUM_CORBA */

        {
          // Write the exception
          CORBA::TypeCode_ptr except_tc = x->_type ();

          CORBA::exception_type extype = CORBA::USER_EXCEPTION;
          if (CORBA::SystemException::_narrow (x) != 0)
            extype = CORBA::SYSTEM_EXCEPTION;

          // write the reply_status
          output.write_ulong (TAO_GIOP::convert_CORBA_to_GIOP_exception (extype));

          // @@ Any way to implement this without interpretive
          //    marshaling???
          output.encode (except_tc, x, 0, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA_Exception, ex)
    {
      // now we know, that while handling the error an other error
      // happened -> no hope, close connection.

      // close the handle
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) cannot marshal exception %p\n",
                  transport->handle (),
                  "TAO_GIOP::send_reply_exception"));
      return -1;
    }
  ACE_ENDTRY;

  return TAO_GIOP::send_message (transport, output, orb_core);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Unbounded_Sequence<TAO_GIOP_ServiceContext>;
template class TAO_Unbounded_Sequence<TAO_IOP_TaggedComponent>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Unbounded_Sequence<TAO_GIOP_ServiceContext>
#pragma instantiate TAO_Unbounded_Sequence<TAO_IOP_TaggedComponent>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
