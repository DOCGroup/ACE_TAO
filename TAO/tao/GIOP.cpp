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

#include "tao/corba.h"
#include "tao/Timeprobe.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, GIOP, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_GIOP_Timeprobe_Description[] =
{
  "GIOP::send_request - start",
  "GIOP::send_request - end",

  "GIOP::recv_request - start",
  "GIOP::recv_request - end",

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

  TAO_GIOP_RECV_REQUEST_START,
  TAO_GIOP_RECV_REQUEST_END,

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
      ACE_DEBUG ((LM_DEBUG, "%s GIOP v%c.%c msg, %d data bytes, %s endian, %s\n",
                  label,
                  digits[ptr[4]],
                  digits[ptr[5]],
                  len - TAO_GIOP_HEADER_LEN,
                  (ptr[6] == TAO_ENCAP_BYTE_ORDER) ? "my" : "other",
                  (ptr[7] <= TAO_GIOP::MessageError) ? names [ptr[7]] : "UNKNOWN TYPE"));

      if (TAO_debug_level >= 4)
        ACE_HEX_DUMP ((LM_DEBUG, (const char*)ptr, len, "(%P|%t) data bytes\n"));
    }
}

CORBA_Boolean
operator<<(TAO_OutputCDR& cdr, const TAO_GIOP_ServiceContext& x)
{
  if ( (cdr << x.context_id)
       && (cdr << x.context_data) )
    return CORBA::B_TRUE;
  return CORBA::B_FALSE;
}

CORBA_Boolean

operator>>(TAO_InputCDR& cdr, TAO_GIOP_ServiceContext& x)
{
  if ( (cdr >> x.context_id)
       && (cdr >> x.context_data) )
    return CORBA::B_TRUE;
  return CORBA::B_FALSE;
}

CORBA_Boolean
operator<<(TAO_OutputCDR& cdr, const TAO_GIOP_ServiceContextList& x)
{
  CORBA::ULong length = x.length ();
  cdr.write_ulong (length);
  for (CORBA::ULong i = 0; i < length && cdr.good_bit (); ++i)
    cdr << x[i];
  return cdr.good_bit ();
}

CORBA_Boolean
operator>>(TAO_InputCDR& cdr, TAO_GIOP_ServiceContextList& x)
{
  CORBA::ULong length;
  cdr.read_ulong (length);
  if (cdr.good_bit ())
    {
      x.length (length);
      for (CORBA::ULong i = 0; i < length && cdr.good_bit (); ++i)
        cdr >> x[i];
    }
  return cdr.good_bit ();
}

// @@ TODO: this is a good candidate for an ACE routine, even more,
// all the code to write a Message_Block chain could be encapsulated
// in ACE.
static ssize_t
writev_n (ACE_HANDLE h, ACE_IO_Vector *iov, int iovcnt)
{
  ssize_t writelen = 0;
  int s = 0;
  while (s < iovcnt)
    {
      ssize_t n = ACE_OS::writev (h, iov + s, iovcnt - s);

      if (n == -1)
        {
          return n;
        }
      else
        {
          writelen += n;
          while (s < iovcnt && n >= iov[s].length ())
            {
              n -= iov[s].length ();
              s++;
            }
          if (n != 0)
            {
              char* base = ACE_reinterpret_cast (char*, iov[s].buffer ());

              iov[s].buffer (base + n);
              iov[s].length (iov[s].length () - n);
            }
        }
    }
  return writelen;
}

CORBA::Boolean
TAO_GIOP::send_request (TAO_SVC_HANDLER *handler,
                        TAO_OutputCDR &stream)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_SEND_REQUEST_START);

  char *buf = (char *) stream.buffer ();
  size_t buflen = stream.total_length ();

  // assert (buflen == (stream.length - stream.remaining));

  // Patch the message length in the GIOP header; it's always at the
  // same eight byte offset into the message.
  //
  // NOTE: Here would also be a fine place to calculate a digital
  // signature for the message and place it into a preallocated slot
  // in the "ServiceContext".  Similarly, this is a good spot to
  // encrypt messages (or just the message bodies) if that's needed in
  // this particular environment and that isn't handled by the
  // networking infrastructure (e.g. IPSEC).

  CORBA::ULong bodylen = buflen - TAO_GIOP_HEADER_LEN;
#if !defined (TAO_ENABLE_SWAP_ON_WRITE)
  *ACE_reinterpret_cast(CORBA::ULong*,buf + 8) = bodylen;
#else
  if (!stream->do_byte_swap ())
    {
      *ACE_reinterpret_cast(CORBA::ULong*, buf + 8) = bodylen;
    }
  else
    {
      CDR::swap_4 (ACE_reinterpret_cast(char*,&bodylen), buf + 8);
    }
#endif

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.

#if 0
  TAO_GIOP::dump_msg ("send",
                      ACE_reinterpret_cast (u_char *, buf),
                      buflen);
#endif

  TAO_SOCK_Stream &peer = handler->peer ();

  const int TAO_WRITEV_MAX = 16;
  ACE_IO_Vector iov[TAO_WRITEV_MAX];
  int iovcnt = 0;
  for (const ACE_Message_Block* i = stream.begin ();
       i != stream.end ();
       i = i->cont ())
    {
      iov[iovcnt].buffer (i->rd_ptr ());
      iov[iovcnt].length (i->length ());
      iovcnt++;

      // The buffer is full make a OS call.
      // @@ TODO this should be optimized on a per-platform basis, for
      // instance, some platforms do not implement writev() there we
      // should copy the data into a buffer and call send_n(). In
      // other cases there may be some limits on the size of the
      // iovec, there we should set TAO_WRITEV_MAX to that limit.
      if (iovcnt == TAO_WRITEV_MAX)
        {
          ssize_t n = writev_n (peer.get_handle (), iov, iovcnt);
          if (n == -1)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) closing conn %d after fault %p\n",
                          peer.get_handle (), "GIOP::send_request"));
              handler->close ();
              return CORBA::B_FALSE;
            }
          else if (n == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) GIOP::send_request (): "
                          "EOF, closing conn %d\n",
                          peer.get_handle ()));
              handler->close ();
              return CORBA::B_FALSE;
            }
          iovcnt = 0;
        }
    }

  if (iovcnt != 0)
    {
      ssize_t n = writev_n (peer.get_handle (), iov, iovcnt);
      if (n == -1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) closing conn %d after fault %p\n",
                      peer.get_handle (), "GIOP::send_request"));
          handler->close ();
          return CORBA::B_FALSE;
        }
      else if (n == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) GIOP::send_request (): "
                      "EOF, closing conn %d\n",
                      peer.get_handle ()));
          handler->close ();
          return CORBA::B_FALSE;
        }
      iovcnt = 0;
    }

  return CORBA::B_TRUE;
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

static const char
close_message [TAO_GIOP_HEADER_LEN] =
{
  'G', 'I', 'O', 'P',
  TAO_GIOP_MessageHeader::MY_MAJOR,
  TAO_GIOP_MessageHeader::MY_MINOR,
  TAO_ENCAP_BYTE_ORDER,
  TAO_GIOP::CloseConnection,
  0, 0, 0, 0
};

void
TAO_GIOP::close_connection (TAO_Client_Connection_Handler *&handler,
                            void *)
{
  // It's important that we use a reliable shutdown after we send this
  // message, so we know it's received.
  //
  // @@ should recv and discard queued data for portability; note
  // that this won't block (long) since we never set SO_LINGER

  TAO_GIOP::dump_msg ("send",
                      (const u_char *) close_message,
                      TAO_GIOP_HEADER_LEN);

  handler->peer ().send (close_message, TAO_GIOP_HEADER_LEN);
  ACE_HANDLE which = handler->peer ().get_handle ();
  handler->close ();
  handler = 0;
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) shut down socket %d\n", which));
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
TAO_GIOP::send_error (TAO_Client_Connection_Handler *&handler)
{
  TAO_GIOP::dump_msg ("send",
                      (const u_char *) error_message,
                      TAO_GIOP_HEADER_LEN);
  handler->peer ().send_n (error_message, TAO_GIOP_HEADER_LEN);
  ACE_HANDLE which = handler->peer ().get_handle ();
  handler->close ();
  handler = 0;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) aborted socket %d\n", which));
}

ssize_t
TAO_GIOP::read_buffer (TAO_SOCK_Stream &peer,
                       char *buf,
                       size_t len)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_READ_BUFFER_START);

  ssize_t bytes_read = peer.recv_n (buf, len);

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

TAO_GIOP::Message_Type
TAO_GIOP::recv_request (TAO_SVC_HANDLER *&handler,
                        TAO_InputCDR &msg)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_RECV_REQUEST_START);

  TAO_GIOP::Message_Type retval;
  CORBA::ULong message_size;
  TAO_SOCK_Stream &connection = handler->peer ();

  // Read the message header off the wire.
  //
  // THREADING NOTE: the connection manager handed us this connection
  // for exclusive use, so we need not worry about having two threads
  // interleave reads of partial messages.  This model is excellent
  // for "lightly threaded" systems (as will be the majority in the
  // near future) but makes less effective use of connection resources
  // as the "duty factor" goes down because of either long calls or
  // bursty contention during numerous short calls to the same server.

  CDR::mb_align (&msg.start_);

  if (CDR::grow (&msg.start_, TAO_GIOP_HEADER_LEN) == -1)
    return TAO_GIOP::MessageError;

  char *header = msg.start_.rd_ptr ();
  ssize_t len = TAO_GIOP::read_buffer (connection,
                                       header,
                                       TAO_GIOP_HEADER_LEN);
  // Read the header into the buffer.

  if (len != TAO_GIOP_HEADER_LEN)
    {
      switch (len)
        {
        case 0:
          if (TAO_orbdebug)
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) End of connection %d\n",
                        connection.get_handle ()));
          return TAO_GIOP::EndOfFile;
          // @@ should probably find some way to report this without
          // an exception, since for most servers it's not an error.
          // Is it _never_ an error?  Not sure ...
          /* NOTREACHED */
        case -1: // error
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) GIOP::recv_request header socket error %p\n",
                      "read_buffer"));
          break;
          /* NOTREACHED */
        default:
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) GIOP::recv_request header read failed, only %d of %d bytes\n",
                      len, TAO_GIOP_HEADER_LEN));
          break;
          /* NOTREACHED */
        }

      return TAO_GIOP::MessageError;
    }

  // NOTE: if message headers, or whome messages, get encrypted in
  // application software (rather than by the network infrastructure)
  // they should be decrypted here ...

  // First make sure it's a GIOP message of any version.

  if (!(header [0] == 'G'
        && header [1] == 'I'
        && header [2] == 'O'
        && header [3] == 'P'))
    {
      ACE_DEBUG ((LM_DEBUG, "bad header, magic word\n"));
      return TAO_GIOP::MessageError;
    }

  // Then make sure the major version is ours, and the minor version
  // is one that we understand.

  if (!(header [4] == TAO_GIOP_MessageHeader::MY_MAJOR
        && header [5] <= TAO_GIOP_MessageHeader::MY_MINOR))
    {
      ACE_DEBUG ((LM_DEBUG, "bad header, version\n"));
      return TAO_GIOP::MessageError;
    }

  // Get the message type out and adjust the buffer's records to record
  // that we've read everything except the length.

  retval = (TAO_GIOP::Message_Type) header[7];

  msg.do_byte_swap_ = (header [6] != TAO_ENCAP_BYTE_ORDER);

  // Make sure byteswapping is done if needed, and then read the
  // message size (appropriately byteswapped).

  msg.start_.rd_ptr (8);
  msg.read_ulong (message_size);

  // Make sure we have the full length in memory, growing the buffer
  // if needed.
  //
  // NOTE: We could overwrite these few bytes of header... they're
  // left around for now as a debugging aid.

  assert (message_size <= UINT_MAX);

  if (CDR::grow (&msg.start_, TAO_GIOP_HEADER_LEN + message_size) == -1)
    return TAO_GIOP::MessageError;

  // Growing the buffer may have reset the rd_ptr(), but we want to
  // leave it just after the GIOP header (that was parsed already);
  CDR::mb_align (&msg.start_);
  msg.start_.wr_ptr (TAO_GIOP_HEADER_LEN);
  msg.start_.wr_ptr (message_size);
  msg.start_.rd_ptr (TAO_GIOP_HEADER_LEN);

  char* payload = msg.start_.rd_ptr ();

  // Read the rest of this message into the buffer.

  len = TAO_GIOP::read_buffer (connection,
                               payload,
                               (size_t) message_size);

  if (len != (ssize_t) message_size)
    {
      switch (len)
        {
        case 0:
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) TAO_GIOP::recv_request body, EOF on handle %d\n",
                      connection.get_handle ()));
          break;
          /* NOTREACHED */
        case -1:
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) TAO_GIOP::recv_request () body %p\n",
                      "read_buffer"));
          break;
          /* NOTREACHED */
        default:
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) short read, only %d of %d bytes\n", len, message_size));
          break;
          /* NOTREACHED */
        }

      // clean up, and ...
      ACE_DEBUG ((LM_DEBUG, "couldn't read rest of message\n"));
      return TAO_GIOP::MessageError;
    }

  TAO_GIOP::dump_msg ("recv",
                      ACE_reinterpret_cast (u_char *, header),
                      message_size + TAO_GIOP_HEADER_LEN);
  return retval;
}

void
TAO_GIOP::make_error (TAO_OutputCDR &msg, ...)
{
  ACE_UNUSED_ARG (msg);  // just for now

  // This [static] method will be somewhat like send_error() except
  // that it won't actaully do any sending of data...it'll just stuff
  // things into the <msg> instance.
}

CORBA::Boolean
TAO_GIOP_LocateRequestHeader::init (TAO_InputCDR &msg,
                                    CORBA::Environment &env)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_LOCATE_REQUEST_HEADER_INIT_START);

  return (msg.read_ulong (this->request_id)
          && msg.decode (TC_opaque,
                         &this->object_key,
                         0,
                         env));
}

CORBA::Boolean
TAO_GIOP::start_message (TAO_GIOP::Message_Type type,
                         TAO_OutputCDR &msg)
{
  msg.reset ();

  // if (msg.size () < TAO_GIOP_HEADER_LEN)
  // return CORBA::B_FALSE;

#if 0
  msg.write_octet ('G');
  msg.write_octet ('I');
  msg.write_octet ('O');
  msg.write_octet ('P');

  msg.write_octet (TAO_GIOP_MessageHeader::MY_MAJOR);
  msg.write_octet (TAO_GIOP_MessageHeader::MY_MINOR);

  msg.write_octet (TAO_ENCAP_BYTE_ORDER);
#else
  static CORBA::Octet header[] = {
    'G', 'I', 'O', 'P',
    TAO_GIOP_MessageHeader::MY_MAJOR,
    TAO_GIOP_MessageHeader::MY_MINOR,
    TAO_ENCAP_BYTE_ORDER
  };
  static int header_size = sizeof(header)/sizeof(header[0]);
  msg.write_octet_array (header, header_size);
#endif
  msg.write_octet (type);

  // Write a dummy <size> later it is set to the right value...
  // @@ TODO Maybe we should store the OutputCDR status in
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  return CORBA::B_TRUE;
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
