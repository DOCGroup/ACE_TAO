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
#include "tao/CDR.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, GIOP, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_GIOP_Timeprobe_Description[] =
  {
    "GIOP::send_request - start",
    "GIOP::send_request - end",

    "GIOP::recv_message - start",
    "GIOP::recv_message - end",

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
  "Request",
  "Reply",
  "CancelRequest",
  "LocateRequest",
  "LocateReply",
  "CloseConnection",
  "MessageError",
  "EndOfFile"
};

void
TAO_GIOP::dump_msg (const char *label,
                    const u_char *ptr,
                    size_t len)
{
  if (TAO_debug_level >= 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s GIOP v%c.%c msg, %d data bytes, %s endian, %s\n",
                  label,
                  digits[ptr[4]],
                  digits[ptr[5]],
                  len - TAO_GIOP_HEADER_LEN,
                  (ptr[6] == TAO_ENCAP_BYTE_ORDER) ? "my" : "other",
                  (ptr[7] <= TAO_GIOP::MessageError) ? names [ptr[7]] : "UNKNOWN TYPE"));

      if (TAO_debug_level >= 4)
        ACE_HEX_DUMP ((LM_DEBUG,
                       (const char*)ptr,
                       len,
                       "\n"));
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
TAO_GIOP::send_request (TAO_Transport  *transport,
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
  'G', 'I', 'O', 'P',
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
  'G', 'I', 'O', 'P',
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

  if (bytes_read == -1 && errno == ECONNRESET)
    {
      // We got a connection reset (TCP RSET) from the other side,
      // i.e., they didn't initiate a proper shutdown.
      //
      // Make it look like things are OK to the upper layer.
      bytes_read = 0;
      errno = 0;
    }

  return bytes_read;
}

// Read the message header, plus any data part of the message, setting
// stuff up so that CDR byteswaps data as appropriate.  Errors are
// reported to be MessageError messages.
//
// NOTE: this code is structured to issue two read () calls for each
// incoming message.  Alternative structures (e.g. with a user-space
// buffer per connection, or networking code handing off entire GIOP
// messages) can reduce the overhead of these calls to the networking
// code; correctness and simplicity drove this implementation more
// than efficiency.
//
// NOTE: as always, counting system calls associated with I/O gives
// you a good basic understanding of the tuning issues.  On the server
// side, there is normally select/read/read/write per invocation.  The
// call to select () can be omitted by allocating a thread to each
// connection; in some cases, that alone has almost doubled
// performance.  The two read () calls can be made into one by fancy
// buffering.  How fast could it be with both optimizations applied?

// I am now making this call non-blocking. For reading the header it
// is not non-blocking. But for reading the rest of the message, it is
// non-blocking. Total size and the current offset of the incoming
// message is kept at the Transport class. Flag <block> is to force
// blocking for the full reply.  Useful <wait_on_Read> strategies.  
TAO_GIOP::Message_Type
TAO_GIOP::recv_message (TAO_Transport *transport,
                        TAO_InputCDR &msg,
                        TAO_ORB_Core* orb_core,
                        TAO_GIOP_Version & /* version */,
                        int block)
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_RECV_MESSAGE_START);
  
  // Read the message header off the wire.
  //
  // THREADING NOTE: the connection manager handed us this connection
  // for exclusive use, so we need not worry about having two threads
  // interleave reads of partial messages.  This model is excellent
  // for "lightly threaded" systems (as will be the majority in the
  // near future) but makes less effective use of connection resources
  // as the "duty factor" goes down because of either long calls or
  // bursty contention during numerous short calls to the same server.
  
  // Default header length.
  ssize_t header_len = TAO_GIOP_HEADER_LEN;

  TAO_GIOP::Message_Type retval;
  CORBA::ULong message_size;
  ssize_t len;
  char *header = 0;

  if (!transport->message_size ())
    {
      // This is the first read for this message.
      ACE_CDR::mb_align (&msg.start_);
      
      if (orb_core->orb_params ()->use_lite_protocol ())
        header_len = TAO_GIOP_LITE_HEADER_LEN;
      
      if (ACE_CDR::grow (&msg.start_,
                         header_len) == -1)
        // This should probably be an exception.
        return TAO_GIOP::CommunicationError;
      
      header = msg.start_.rd_ptr ();
      len = TAO_GIOP::read_buffer (transport,
                                   header,
                                   header_len);
      // Read the header into the buffer.

      if (len != header_len)
        {
          switch (len)
            {
            case 0:
              if (TAO_orbdebug)
                ACE_DEBUG ((LM_DEBUG,
                            "(%t) End of connection, transport handle %d\n",
                            transport->handle ()));
              return TAO_GIOP::EndOfFile;
              // @@ should probably find some way to report this without
              // an exception, since for most servers it's not an error.
              // Is it _never_ an error?  Not sure ...
              /* NOTREACHED */
              
            case -1: // error
              if (TAO_orbdebug)
                ACE_DEBUG ((LM_ERROR,
                            "(%P|%t) GIOP::recv_message header %p\n",
                            "read_buffer"));
              break;
              /* NOTREACHED */
              
            default:
              if (TAO_orbdebug)
                ACE_DEBUG ((LM_ERROR,
                            "(%P|%t) GIOP::recv_message header read failed, "
                            "only %d of %d bytes\n",
                            len,
                            header_len));
              break;
              /* NOTREACHED */
            }

          return TAO_GIOP::CommunicationError;
        }
      
      // NOTE: if message headers, or whole messages, get encrypted in
      // application software (rather than by the network infrastructure)
      // they should be decrypted here ...
      
      // First make sure it's a GIOP message of any version.
      
      if (TAO_GIOP::parse_header (msg,
                                  msg.do_byte_swap_,
                                  retval,
                                  message_size,
                                  orb_core) == -1)
        {
          TAO_GIOP::send_error (transport);
          // We didn't really receive anything useful here.
          return TAO_GIOP::CommunicationError;
        }
      
      // Make sure we have the full length in memory, growing the
      // buffer if needed.
      //
      // NOTE: We could overwrite these few bytes of header... they're  
      // left around for now as a debugging aid.
          
      assert (message_size <= UINT_MAX);
      
      if (ACE_CDR::grow (&msg.start_,
                         header_len + message_size) == -1)
        return TAO_GIOP::CommunicationError;
          
      // Growing the buffer may have reset the rd_ptr(), but we want to
      // leave it just after the GIOP header (that was parsed already);
      ACE_CDR::mb_align (&msg.start_);
      msg.start_.wr_ptr (header_len);
      msg.start_.wr_ptr (message_size);
      msg.start_.rd_ptr (header_len);
          
      // Keep the message_size with the Transport object. 
      transport->message_size (message_size);
    }
  
  // Header is read already. Read the rest of this message into the
  // buffer. 

  char* payload = msg.start_.rd_ptr ();

  if (block)
    {
      len = TAO_GIOP::read_buffer (transport,
                                   payload,
                                   (size_t) transport->message_size () -
                                   transport->message_offset ());
      
      // @@ Take care of this when you change the above to non-blocking. 
      if (len != (ssize_t) message_size)
        {
          switch (len)
            {
            case 0:
              // Update the offset in the Transport object.
              if (transport->incr_message_offset (len) == -1)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "%N:%l:TAO_GIOP::recv_message: "
                                   "Failed to update the offset\n"),
                                  TAO_GIOP::CommunicationError);
              
              if (TAO_orbdebug)
                ACE_DEBUG ((LM_DEBUG,
                            "(%t) End of connection, transport handle %d\n",
                            transport->handle ()));
              
              return TAO_GIOP::EndOfFile;
              /* NOTREACHED */
              
            case -1:
              if (TAO_orbdebug)
                ACE_DEBUG ((LM_ERROR,
                            "(%P|%t) TAO_GIOP::recv_message - body %p\n",
                            "read_buffer"));
              break;
              /* NOTREACHED */
              
            default:
              if (TAO_orbdebug)
                ACE_DEBUG ((LM_ERROR,
                            "TAO: (%P|%t) GIOP::recv_message body read failed, "
                            "only %d of %d bytes\n",
                            len,
                            message_size));
              break;
              /* NOTREACHED */
            }
          return TAO_GIOP::CommunicationError;
        }
    }
  else
    {
      // @@ Make this non-blocking !!! (alex).
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO:%N:%l:(%P | %t):"
                         "Not implemented\n"),
                        retval);
    }

  TAO_GIOP::dump_msg ("recv",
                      ACE_reinterpret_cast (u_char *, header),
                      message_size + header_len);

  return retval;
}

int
TAO_GIOP::parse_header_std (TAO_InputCDR &cdr,
                            int &do_byte_swap,
                            TAO_GIOP::Message_Type &message_type,
                            CORBA::ULong &message_size)
{
  char *header = cdr.start_.rd_ptr ();

  if (!(header [0] == 'G'
        && header [1] == 'I'
        && header [2] == 'O'
        && header [3] == 'P'))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO: (%P|%t) bad header, magic word [%c%c%c%c]\n",
                  header[0], header[1], header[2], header[3]));
      return -1;
    }

  // Then make sure the major version is ours, and the minor version
  // is one that we understand.

  if (!(header [4] == TAO_GIOP_MessageHeader::MY_MAJOR
        && header [5] <= TAO_GIOP_MessageHeader::MY_MINOR))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO: (%P|%t) bad header, version [%c%c]\n",
                  header[4], header[5]));
      return -1;
    }

  // Get the message type out and adjust the buffer's records to
  // record that we've read everything except the length.

  message_type = (TAO_GIOP::Message_Type) header[7];

  do_byte_swap = (header [6] != TAO_ENCAP_BYTE_ORDER);

  // Make sure byteswapping is done if needed, and then read the
  // message size (appropriately byteswapped).

  cdr.start_.rd_ptr (8);
  cdr.read_ulong (message_size);

  return 0;
}

int
TAO_GIOP::parse_header_lite (TAO_InputCDR &cdr,
                             int &do_byte_swap,
                             TAO_GIOP::Message_Type &message_type,
                             CORBA::ULong &message_size)
{
  do_byte_swap = 0;

  char *header = cdr.start_.rd_ptr ();

  // Get the message type out and adjust the buffer's records to
  // record that we've read everything except the length.
  message_type = (TAO_GIOP::Message_Type) header[4];

  cdr.read_ulong (message_size);

  cdr.start_.rd_ptr (1);

  return 0;
}

int
TAO_GIOP::parse_header (TAO_InputCDR &cdr,
                        int &do_byte_swap,
                        TAO_GIOP::Message_Type &message_type,
                        CORBA::ULong &message_size,
                        TAO_ORB_Core *orb_core)
{
  if (orb_core->orb_params ()->use_lite_protocol ())
    return TAO_GIOP::parse_header_lite (cdr,
                                        do_byte_swap,
                                        message_type,
                                        message_size);
  else
    return TAO_GIOP::parse_header_std (cdr,
                                       do_byte_swap,
                                       message_type,
                                       message_size);
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
    'G', 'I', 'O', 'P',
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
TAO_GIOP::convert_CORBA_to_GIOP_exception (CORBA::ExceptionType corba_type)
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Unbounded_Sequence<TAO_GIOP_ServiceContext>;
template class TAO_Unbounded_Sequence<TAO_IOP_TaggedComponent>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Unbounded_Sequence<TAO_GIOP_ServiceContext>
#pragma instantiate TAO_Unbounded_Sequence<TAO_IOP_TaggedComponent>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
