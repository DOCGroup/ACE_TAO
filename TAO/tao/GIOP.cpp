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

CORBA::Boolean
TAO_GIOP::send_request (TAO_SVC_HANDLER *handler,
                        TAO_OutputCDR &stream)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_SEND_REQUEST_START);

  char *buf = (char *) stream.buffer ();
  size_t buflen = stream.length ();

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

  *(CORBA::Long *) (buf + 8) =
    (CORBA::Long) (buflen - TAO_GIOP_HEADER_LEN);

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.

  TAO_GIOP::dump_msg ("send",
                      ACE_reinterpret_cast (u_char *, buf),
                      buflen);

  ACE_SOCK_Stream &peer = handler->peer ();

  while (buflen > 0)
    {
      if (buflen > stream.length ())
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) ?? writebuf, buflen %u > length %u\n",
                      buflen, stream.length ()));
          return CORBA::B_FALSE;
        }

      ssize_t writelen = peer.send_n (buf, buflen);

#if defined (DEBUG)
      //      dmsg_filter (6, "wrote %d bytes to connection %d",
      //           writelen, connection);
      dmsg_filter (6, "wrote %d bytes", writelen);
#endif  /* DEBUG */

      assert ((writelen >= 0
               && ((size_t)writelen) <= buflen) || writelen == -1);

      // On error or EOF, report the fault, close the connection, and
      // mark it as unusable/defunct.
      //
      // @@ on client side write errors, we may hit the case that the
      // server did a clean shutdown but we've not yet read the
      // GIOP::CloseConnection message.  If we get an error, we need
      // to see if there is such a message waiting for us, and if so
      // we should cause (full) rebinding to take place.

      if (writelen == -1)
        {
          ACE_DEBUG ((LM_ERROR,
                      "(%P|%t) %p\n", "OutgoingMessage::writebuf ()"));
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) closing conn %d after fault\n", peer.get_handle ()));
          handler->close ();
          return CORBA::B_FALSE;
        }
      else if (writelen == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) OutgoingMessage::writebuf () ... EOF, closing conn %d\n",
                      peer.get_handle ()));
          handler->close ();
          return CORBA::B_FALSE;
        }
      if ((buflen -= writelen) != 0)
        buf += writelen;

#if defined (DEBUG)
      //
      // NOTE:  this should never be seen.  However, on Linux
      // it's been seen with UNIX domain sockets.
      //
      if (buflen)
        dmsg_filter (8, "%u more bytes to write...\n", buflen);
#endif /* DEBUG */
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
TAO_GIOP::read_buffer (ACE_SOCK_Stream &peer,
                       char *buf,
                       size_t len)
{
  ssize_t bytes_read = bytes_read = peer.recv_n (buf, len);

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
                        TAO_InputCDR &msg,
                        CORBA::Environment &env)
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_RECV_REQUEST_START);
  TAO_GIOP::Message_Type      retval;
  CORBA::ULong message_size;
  ACE_SOCK_Stream &connection = handler->peer ();

  // Read the message header off the wire.
  //
  // THREADING NOTE: the connection manager handed us this connection
  // for exclusive use, so we need not worry about having two threads
  // interleave reads of partial messages.  This model is excellent
  // for "lightly threaded" systems (as will be the majority in the
  // near future) but makes less effective use of connection resources
  // as the "duty factor" goes down because of either long calls or
  // bursty contention during numerous short calls to the same server.

  CDR::mb_align (msg.start_);
  if (CDR::grow (msg.start_, TAO_GIOP_HEADER_LEN) == -1)
    {
      env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_MAYBE));
      return TAO_GIOP::MessageError;
    }

  char *header = msg.start_->rd_ptr ();
  ssize_t len = TAO_GIOP::read_buffer (connection,
                                       header,
                                       TAO_GIOP_HEADER_LEN);
  // Read the header into the buffer.

  if (len != TAO_GIOP_HEADER_LEN)
    {
      switch (len)
        {
        case 0:
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Header EOF ... peer probably aborted connection %d\n",
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

      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));
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
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));      // header
      ACE_DEBUG ((LM_DEBUG, "bad header, magic word\n"));
      return TAO_GIOP::MessageError;
    }

  // Then make sure the major version is ours, and the minor version
  // is one that we understand.

  if (!(header [4] == TAO_GIOP_MessageHeader::MY_MAJOR
        && header [5] <= TAO_GIOP_MessageHeader::MY_MINOR))
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));      // header
      ACE_DEBUG ((LM_DEBUG, "bad header, version\n"));
      return TAO_GIOP::MessageError;
    }

  // Get the message type out and adjust the buffer's records to record
  // that we've read everything except the length.

  retval = (TAO_GIOP::Message_Type) header[7];

  msg.do_byte_swap_ = (header [6] != TAO_ENCAP_BYTE_ORDER);

  // Make sure byteswapping is done if needed, and then read the
  // message size (appropriately byteswapped).

  msg.start_->rd_ptr (8);
  msg.read_ulong (message_size);

  // Make sure we have the full length in memory, growing the buffer
  // if needed.
  //
  // NOTE: We could overwrite these few bytes of header... they're
  // left around for now as a debugging aid.

  assert (message_size <= UINT_MAX);

  if (CDR::grow (msg.start_, TAO_GIOP_HEADER_LEN + message_size) == -1)
    {
      env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_MAYBE));
      return TAO_GIOP::MessageError;
    }

  // Growing the buffer may have reset the rd_ptr(), but we want to
  // leave it just after the GIOP header (that was parsed already);
  CDR::mb_align (msg.start_);
  msg.start_->wr_ptr (TAO_GIOP_HEADER_LEN);
  msg.start_->wr_ptr (message_size);
  msg.start_->rd_ptr (TAO_GIOP_HEADER_LEN);

  char* payload = msg.start_->rd_ptr ();

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
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE)); // body
      ACE_DEBUG ((LM_DEBUG, "couldn't read rest of message\n"));
      return TAO_GIOP::MessageError;
    }

  TAO_GIOP::dump_msg ("recv",
                      ACE_reinterpret_cast (u_char *, header),
                      message_size + TAO_GIOP_HEADER_LEN);
  return retval;
}

// Normal invocations don't involve any heap allocation; messages are
// constructed into stack-based buffers and are read into those
// buffers too.  Larger buffers are heap-allocated as needed.
//
// The constraint on request IDs is that no two requests from the same
// client with the same ID are outstanding at the same time.  In
// single threaded environments, this is met by any number whatever.
// When multiple threads are used, we eliminate the need for any
// locked state by using the thread ID as the request ID, since any
// given thread has at most one request outstanding at a time.
//
// NOTE: this means that if "deferred synchronous" calls get
// supported, it's done by creating a thread internally to make the
// call.  That is less disruptive (and error prone) in general than
// restructuring an ORB core in terms of asynchrony.

TAO_GIOP_Invocation::TAO_GIOP_Invocation (IIOP_Object *data,
                                          const char *operation,
                                          CORBA::Boolean is_roundtrip)
  : data_ (data),
    opname_ (operation),
    do_rsvp_ (is_roundtrip),
    my_request_id_ (0),
    out_stream_ (buffer, sizeof buffer),
    inp_stream_ (buffer, sizeof buffer),
    handler_ (0)
{
  // @@ TODO The comments here are scary, can someone please give me a
  // warm fuzzy feeling about this (coryan).

  // The assumption that thread ids are ints is false and horribly
  // implementation-dependent, so this code just sucks.  But, at least
  // it will compile on multiple platforms through the magic of ACE
  // :-/

  //assert (sizeof (CORBA::ULong) == sizeof (ACE_thread_t));
  ACE_thread_t me = ACE_OS::thr_self ();

  // Copy in only as many bytes are valid, or only as many as we have
  // room for, whichever is less.  -------> What a friggin' HACK!?!?!
  ACE_OS::memcpy (&this->my_request_id_,
                  &me,
                  ACE_MIN (sizeof (me), sizeof (this->my_request_id_)));
}

TAO_GIOP_Invocation::~TAO_GIOP_Invocation (void)
{
  if (this->handler_ != 0)
    this->handler_->idle ();
}

// Octet codes for the parameters of the "Opaque" (sequence of octet)
// data type used various places internally ... a CDR encapsulation
// holding two parameters (like all sequence TypeCodes).
//
// NOTE: this **MUST** be longword aligned, which is why it's coded as
// a longword array not an octet array.  Just sticking a long in for
// padding won't work with compilers that optimize unused data out of
// existence.

// CDR typecode octets.

static const CORBA::Long _oc_opaque [] =
{

  TAO_ENCAP_BYTE_ORDER,    // native endian + padding; "tricky"
  10,                      // ... (sequence of) octets
  0                        // ... unbounded
};

CORBA::TypeCode
TC_opaque (CORBA::tk_sequence,
           sizeof _oc_opaque,
           (char *) &_oc_opaque,
           CORBA::B_FALSE);

// Octet codes for the parameters of the ServiceContextList TypeCode
// ...  this is a CDR encapsulation holding two parameters (like all
// sequences): a TypeCode, and the bounds of the sequence (zero in
// this case).
//
// This is complicated since the Typecode for the data type for the
// sequence members is complex, a structure that nests two further
// typecodes (one is a sequence).
//
// NOTE:  this must be longword aligned!

static const CORBA::Long _oc_svc_ctx_list [] =
{
  // START bytes of encapsulation 0
  TAO_ENCAP_BYTE_ORDER, // native endian + padding; "tricky"

  //
  // FIRST sequence param:  typecode for struct is complex,
  // and so uses a nested encapsulation.
  //
  CORBA::tk_struct,
  72, // length of encapsulation 1

  // START bytes of encapsulation 1 (struct params)
  1, // native endian + padding; "tricky"
  1, 0, // type ID omitted:  null string
  1, 0, // name omitted "ServiceContext"

  2, // two struct elements

  // First structure element:  name, typecode for ULong
  //
  // NOTE:  to be more strictly correct this could be a CORBA::tk_alias
  // typecode ...

  1, 0, // name omitted:  "context_id"
  CORBA::tk_long,

  // Second structure element: name, typecode for sequence of octet;
  // the typecode for sequence of octet is complex, there's a second
  // level of nested encapuslation here.

  1, 0, // name omitted:  "context_data"
  CORBA::tk_sequence,   // sequence typecode
  16, // length of encapsulation 2

  // START bytes of encapsulation 2 (sequence params)
  1, // native endian + padding; "tricky"
  1, 0, // type ID omitted:  null string
  CORBA::tk_octet, // (sequence of) octet
  0, // ... unbounded length
  // END bytes of encapsulation 2 (sequence params)

  // END bytes of encapsulation 1 (struct params)

  // SECOND sequence param:  bound of sequence (none)
  0 // unbounded seq of ServiceContext
  // END bytes of encapsulation 0 (sequence params)
};

CORBA::TypeCode
TC_ServiceContextList (CORBA::tk_sequence,
                       sizeof _oc_svc_ctx_list,
                       (char *) &_oc_svc_ctx_list,
                       CORBA::B_FALSE);

// The public API involves creating an invocation, starting it, filling
// in request parameters, actually performing the invocation, getting
// response parameters, and then cleaning up.  Sometimes they must be
// restarted (e.g. request forwarding).  This is the start/restart entry.

void
TAO_GIOP_Invocation::start (CORBA::Environment &env)
{
  const TAO_opaque *key;

  // First try to bind to the appropriate address.  We do that here
  // since we may get forwarded to a different objref in the course of
  // any given call, with new start () call each time.  It's not
  // cached in the objref data since the connections change
  // asynchronously from objref invocations and this simplifies
  // connection management.
  //
  // THREADING NOTE: this connection is reserved to this call.  Also,
  // starting at this point in the call, new forwarding information
  // will not be used until/unless the call is reissued.  Correctness
  // is not affected, the call will just be forwarded later than it
  // might be in a more complex implementation.

  assert (data_ != 0);

  // Get a pointer to the connector, which might be in thread-specific
  // storage, depending on the concurrency model.
  TAO_CONNECTOR *con = TAO_ORB_Core_instance ()->connector ();

  // Determine the object key and the address to which we'll need a
  // connection.
  ACE_INET_Addr *server_addr_p = 0;

  {
    // Begin a new scope so we keep this lock only as long as
    // necessary
#if 0 /* Keep this around for when forwarding might be implemented (if ever) */
    ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX, guard, data_->fwd_profile_lock ()));
#endif
    if (data_->fwd_profile_i () != 0)
      {
        key = &data_->fwd_profile_i ()->object_key;
        server_addr_p = &data_->fwd_profile_i ()->object_addr ();
      }
    else
      {
        key = &data_->profile.object_key;
        server_addr_p = &data_->profile.object_addr ();
      }
  }

  if (server_addr_p == 0)
    {
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));
      return;
    }

  this->handler_ = 0;
  // Must reset handler, otherwise, <ACE_Cached_Connect_Strategy> will
  // complain.

  // Establish the connection and get back a
  // <Client_Connection_Handler>.
  if (con->connect (this->handler_,
                    *server_addr_p) == -1)
    {
      // Give users a clue to the problem.
      ACE_DEBUG ((LM_ERROR, "(%P|%t) %s:%u, connection to "
                            "%s (%s):%hu failed (%p)\n",
                  __FILE__,
                  __LINE__,
                  server_addr_p->get_host_name (),
                  server_addr_p->get_host_addr (),
                  server_addr_p->get_port_number (),
                  "errno"));

      // There might be a better exception to set, but it's unclear
      // which one should be used.  This one applies, even if it's
      // vague.
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));
      return;
    }

  // Use the ACE_SOCK_Stream from the Client_Connection_Handler for
  // communication inplace of the endpoint used below.

  // POLICY DECISION: If the client expects most agents to forward,
  // then it could try to make sure that it's been forwarded at least
  // once by eliciting it with a LocateRequest message. (Further
  // hinting in the IIOP::ProfileData could help!)
  //
  // That scenario does not match an "Inter" ORB Protocol well, since
  // bridges chain calls rather than forwarding them.  It does match
  // some kinds of "Intra" ORB scenarios well, with many agents that
  // spawn new processes talking to their clients across the net.
  //
  // At this time, the policy noted above is followed in the sense
  // that this software does NOT expect most agents to forward, so it
  // doesn't bother to probe.  Correctness is not affected; this is
  // only a quality-of-service policy.  It affects mostly performance,
  // but the "best efforts" semantics for "oneway" messages would also
  // be impacted in that some (by definition, buggy!) code which used
  // only "oneway" messages might not work at all.

  // Build the outgoing message, starting with generic GIOP header.

  CORBA::Boolean bt = TAO_GIOP::start_message (TAO_GIOP::Request,
					       this->out_stream_);

  if (bt != CORBA::B_TRUE)
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
      return;
    }

  // Then fill in the rest of the RequestHeader
  //
  // The first element of header is service context list;
  // transactional context would be acquired here using the
  // transaction service APIs.  Other kinds of context are as yet
  // undefined.
  //
  // Last element of request header is the principal; no portable way
  // to get it, we just pass empty principal (convention: indicates
  // "anybody").  Steps upward in security include passing an
  // unverified user ID, and then verifying the message (i.e. a dummy
  // service context entry is set up to hold a digital signature for
  // this message, then patched shortly before it's sent).

  static CORBA::Principal_ptr anybody = 0;
  static TAO_GIOP_ServiceContextList svc_ctx;   // all zeroes

  if (this->out_stream_.encode (&TC_ServiceContextList, 0, &svc_ctx, env)
      != CORBA::TypeCode::TRAVERSE_CONTINUE)
    return;

  if (!this->out_stream_.write_ulong (this->my_request_id_)
      || !this->out_stream_.write_boolean (this->do_rsvp_))
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
      return;
    }

  if (this->out_stream_.encode (&TC_opaque,
				key,
				0,
				env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || this->out_stream_.encode (CORBA::_tc_string,
				   &opname_,
				   0,
				   env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || this->out_stream_.encode (CORBA::_tc_Principal,
				   &anybody,
				   0,
				   env) != CORBA::TypeCode::TRAVERSE_CONTINUE)
    return; // right after fault
  else
    return; // no fault reported
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

// Send request, block until any reply comes back, and unmarshal reply
// parameters as appropriate.

TAO_GIOP_ReplyStatusType
TAO_GIOP_Invocation::invoke (CORBA::ExceptionList &exceptions,
                             CORBA::Environment &env)
{
  // Send Request, return on error or if we're done

  if (this->handler_->send_request (this->out_stream_,
                                    this->do_rsvp_) == -1)
    {
      // send_request () closed the connection; we just set the
      // handler to 0 here.
      this->handler_ = 0;

      //
      // @@ highly desirable to know whether we wrote _any_ data; if
      // we wrote none, then there's no chance the call completed and
      // applications don't have to deal with those nasty
      // indeterminate states where they can't immediatly tell if
      // what's safe to do.
      //
      // @@ also, there might have been a GIOP::CloseConnection
      // message in the input queue.  If so, this request should be
      // treated as a (full) "rebind" case.  Can't do that from this
      // point in the code however!  Some minor restructuring needs to
      // happen.
      //
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));
      return TAO_GIOP_SYSTEM_EXCEPTION;
    }
  if (!this->do_rsvp_)
    return TAO_GIOP_NO_EXCEPTION;

  // This blocks until the response is read.  In the current version,
  // there is only one client thread that ever uses this connection,
  // so most response messages are illegal.
  //
  // THREADING NOTE: to make more efficient use of connection
  // resources, we'd multiplex I/O on connections.  For example, one
  // thread would write its GIOP::Request (or GIOP::LocateRequest etc)
  // message and block for the response, then another would do the
  // same thing.  When a response came back, it would be handed to the
  // thread which requested it.
  //
  // Currently the connection manager doesn't support such fine
  // grained connection locking, and also this server implementation
  // wouldn't take advantage of that potential concurrency in requests
  // either.  There are often performance losses coming from
  // fine-grained locks being used inappropriately; there's some
  // evidence that locking at the level of requests loses on at least
  // some platforms.
  //
  // @@ In all MT environments, there's a cancellation point lurking
  // here; need to investigate.  Client threads would frequently be
  // canceled sometime during recv_request ... the correct action to
  // take on being canceled is to issue a CancelRequest message to the
  // server and then imediately let other client-side cancellation
  // handlers do their jobs.
  //
  // In C++, that basically means to unwind the stack using almost
  // normal procedures: all destructors should fire, and some "catch"
  // blocks should probably be able to handle things like releasing
  // pointers. (Without unwinding the C++ stack, resources that must
  // be freed by thread cancellation won't be freed, and the process
  // won't continue to function correctly.)  The tricky part is that
  // according to POSIX, all C stack frames must also have their
  // (explicitly coded) handlers called.  We assume a POSIX.1c/C/C++
  // environment.

  TAO_SVC_HANDLER *handler = this->handler_;
  TAO_GIOP::Message_Type m = TAO_GIOP::recv_request (handler,
						     this->inp_stream_,
						     env);
  switch (m)
    {
    case TAO_GIOP::Reply:
      // handle reply ... must be right one etc
      break;

    case TAO_GIOP::CloseConnection:
      // Special case of forwarding -- server was closing the
      // connection, which just indicates resource constraints, not an
      // error.  The client is effectively "forwarded" to the same
      // server!
      //
      // However, we must reinitialize the forwarding chain, since the
      // resource being reclaimed might also have been the process,
      // not just the connection.  Without reinitializing, we'd give
      // false error reports to applications.
      {
#if 0 /* Keep this around in case forwarding is ever implemented */
        ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, data_->fwd_profile_lock (), TAO_GIOP_SYSTEM_EXCEPTION));
#endif

        IIOP::Profile *old = data_->fwd_profile_i (0);
        delete old;

        this->handler_->close ();
        this->handler_ = 0;
        return TAO_GIOP_LOCATION_FORWARD;
      }

    case TAO_GIOP::Request:
    case TAO_GIOP::CancelRequest:
    case TAO_GIOP::LocateRequest:
    case TAO_GIOP::LocateReply:
    default:
      // These are all illegal messages to find.  If found, they could
      // be indicative of client bugs (lost track of input stream) or
      // server bugs; maybe the request was acted on, maybe not, we
      // can't tell.
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) illegal GIOP message (%s) in response to my Request!\n",
                  TAO_GIOP::message_name (m)));
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));
      // FALLTHROUGH ...

    case TAO_GIOP::MessageError:
      // Couldn't read it for some reason ... exception's set already,
      // so just tell the other end about the trouble (closing the
      // connection) and return.
      TAO_GIOP::send_error (this->handler_);
      return TAO_GIOP_SYSTEM_EXCEPTION;
    }

  // Process reply message.  Again, due to the single threading in
  // this code, only the reply to this request is allowed to be coming
  // back.
  //
  // NOTE: if the response really _isn't_ for this thread, it's now
  // treated as an error in which synchronization can't be recovered.
  // There might be cases where it _could_ be recovered ... e.g. maybe
  // for some reason the previous call couldn't pick up its response.
  // It'd be worth investigating (and handling) any such cases.
  //
  // NOTE: since this implementation supports no ORB services
  // (notably, the transaction service, which is the only one that's
  // currently defined), the reply context is discarded.  Normally
  // it'd be fed, component at a time, to the relevant services.
  //
  // NOTE: As security support kicks in, this is the right place to
  // verify a digital signature, if that is required in this
  // particular runtime security environment.  How to know if that's
  // the case?  It's likely that standard Internet IPSEC
  // infrastructure (RFC 1825 through 1827, and successors) will be
  // used to enforce many security policies; integrity and privacy
  // guarantees may be provided by the network, and need no support
  // here.

  TAO_GIOP_ServiceContextList reply_ctx;
  CORBA::ULong request_id;
  CORBA::ULong reply_status;            // TAO_GIOP_ReplyStatusType

  if (this->inp_stream_.decode (&TC_ServiceContextList, &reply_ctx, 0, env)
      != CORBA::TypeCode::TRAVERSE_CONTINUE)
    {
      TAO_GIOP::send_error (this->handler_);
      return TAO_GIOP_SYSTEM_EXCEPTION;
    }

  if (!this->inp_stream_.read_ulong (request_id)
      || request_id != this->my_request_id_
      || !this->inp_stream_.read_ulong (reply_status)
      || reply_status > TAO_GIOP_LOCATION_FORWARD)
    {
      TAO_GIOP::send_error (this->handler_);
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) bad Response header\n"));
      return TAO_GIOP_SYSTEM_EXCEPTION;
    }

  // If there was no exception, let the caller parse the normal
  // response.  Otherwise parse and handle the response; we always
  // know how to deal with the standard exceptions, and the caller
  // provides a list of allowed user-defined exceptions so that we
  // know how to unmarshal those too (without IFR consultation).
  //
  // When requests are forwarded, we just store the revised profile
  // data in this objref structure.  The expectation is that the call
  // will be reissued until someone gives up on a forwarding chain,
  // and that other calls will reap the benefit of the forwarding work
  // by this thread.
  //
  // NOTE: should ensure that from here on, all system exceptions
  // return COMPLETED_YES status ... even ones reported by code which
  // we call.

  switch (reply_status)
    {
    case TAO_GIOP_NO_EXCEPTION:
      break;

    case TAO_GIOP_USER_EXCEPTION:
    case TAO_GIOP_SYSTEM_EXCEPTION:
      {
        char* buf;

        // Pull the exception ID out of the marshaling buffer.
        {
	  if (this->inp_stream_.read_string (buf) == CORBA::B_FALSE)
            {
              TAO_GIOP::send_error (this->handler_);
              env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_YES));
              return TAO_GIOP_SYSTEM_EXCEPTION;
            }
        }
        // User and system exceptions differ only in what table of
        // exception typecodes is searched.
        CORBA::ExceptionList *xlist;

        if (reply_status == TAO_GIOP_USER_EXCEPTION)
          xlist = &exceptions;
        else
          xlist = &TAO_Exceptions::system_exceptions;

        // Find it in the operation description and then use that to
        // get the typecode.  Use it to unmarshal the exception's
        // value; if that exception is not allowed by this operation,
        // fail (next).

        for (CORBA::ULong i = 0;
             i < xlist->count ();
             i++)
          {
	    CORBA::TypeCode_ptr tcp = xlist->item (i, env);

            const char *xid = tcp->id (env);

            if (env.exception () != 0)
              {
                dexc (env, "invoke (), get exception ID");
                TAO_GIOP::send_error (this->handler_);
                return TAO_GIOP_SYSTEM_EXCEPTION;
              }

            if (ACE_OS::strcmp (buf, (char *)xid) == 0)
              {
                size_t size;
                CORBA::Exception *exception;

                size = tcp->size (env);
                if (env.exception () != 0)
                  {
                    dexc (env, "invoke (), get exception size");
                    TAO_GIOP::send_error (this->handler_);
                    return TAO_GIOP_SYSTEM_EXCEPTION;
                  }

                // Create the exception, fill in the generic parts
                // such as vtable, typecode ptr, refcount ... we need
                // to clean them all up together, in case of errors
                // unmarshaling.

                exception = new (new char [size]) CORBA::Exception (tcp);

                if (this->inp_stream_.decode (tcp, exception, 0, env)
                    != CORBA::TypeCode::TRAVERSE_CONTINUE)
                  {
                    delete exception;
                    ACE_DEBUG ((LM_ERROR, "(%P|%t) invoke, unmarshal %s exception %s\n",
                                (reply_status == TAO_GIOP_USER_EXCEPTION) ? "user" : "system",
                                buf));
                    TAO_GIOP::send_error (this->handler_);
                    return TAO_GIOP_SYSTEM_EXCEPTION;
                  }
                env.exception (exception);
                return (TAO_GIOP_ReplyStatusType) reply_status;
              }
          }

        // If we couldn't find this exception's typecode, report it as
        // an OA error since the skeleton passed an exception that was
        // not allowed by the operation's IDL definition.  In the case
        // of a dynamic skeleton it's actually an implementation bug.
        //
        // It's known to be _very_ misleading to try reporting this as
        // any kind of marshaling error (unless minor codes are made
        // to be _very_ useful) ... folk try to find/fix ORB bugs that
        // don't exist, not bugs in/near the implementation code.

        if (reply_status == TAO_GIOP_USER_EXCEPTION)
          env.exception (new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_YES));
        else
          env.exception (new CORBA::INTERNAL (CORBA::COMPLETED_MAYBE));
        return TAO_GIOP_SYSTEM_EXCEPTION;
      }
    // NOTREACHED

    case TAO_GIOP_LOCATION_FORWARD:
      {
        CORBA::Object_ptr obj;
        IIOP_Object *obj2;

        // Unmarshal the object we _should_ be calling.  We know that
        // one of the facets of this object will be an IIOP invocation
        // profile.

        if (this->inp_stream_.decode (CORBA::_tc_Object,
				      &obj, 0,
				      env) != CORBA::TypeCode::TRAVERSE_CONTINUE
            || obj->QueryInterface (IID_IIOP_Object,
                                    (void **) &obj2) != TAO_NOERROR)
          {
            dexc (env, "invoke, location forward");
            TAO_GIOP::send_error (this->handler_);
            return TAO_GIOP_SYSTEM_EXCEPTION;
          }
        CORBA::release (obj);

        // Make a copy of the IIOP profile in the forwarded objref,
        // reusing memory where practical.  Then delete the forwarded
        // objref, retaining only its profile.
        //
        // @@ add and use a "forward count", to prevent loss of data
        // in forwarding chains during concurrent calls -- only a
        // forward that's a response to the current fwd_profile should
        // be recorded here. (This is just an optimization, and is not
        // related to correctness.)

#if 0 /* Keep this around in case forwarding is ever implemented. */
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, data_->fwd_profile_lock (), TAO_GIOP_SYSTEM_EXCEPTION);
#endif

        IIOP::Profile *old = data_->fwd_profile_i (new IIOP::Profile (obj2->profile));
        delete old;

        obj2->Release ();

        env.clear ();

        // Make sure a new connection is used next time.
        this->handler_->close ();
        this->handler_ = 0;
        // We may not need to do this since TAO_GIOP_Invocations
        // get created on a per-call basis. For now we'll play it safe.
      }
    break;
    }

  // All standard exceptions from here on in the call path know for
  // certain that the call "completed" ... except in the case of
  // system exceptions which say otherwise, and for
  // TAO_GIOP_LOCATION_FORWARD responses.

  return (TAO_GIOP_ReplyStatusType) reply_status;
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
  return (msg.read_ulong (this->request_id)
          && msg.decode (&TC_opaque,
                         &this->object_key,
                         0,
                         env));
}

// Initialize the request header from <msg>, setting <env> for errors.

CORBA::Boolean
TAO_GIOP_RequestHeader::init (TAO_InputCDR &msg,
                              CORBA::Environment &env)
{
  CORBA::Boolean hdr_status;

  // Tear out the service context ... we currently ignore it, but it
  // should probably be passed to each ORB service as appropriate
  // (e.g. transactions, security).
  //
  // NOTE: As security support kicks in, this is a good place to
  // verify a digital signature, if that is required in this security
  // environment.  It may be required even when using IPSEC security
  // infrastructure.

  hdr_status = msg.decode (&TC_ServiceContextList,
                           &this->service_info,
                           0,
                           env);

  // Get the rest of the request header ...

  hdr_status = hdr_status && msg.read_ulong (this->request_id);
  hdr_status = hdr_status && msg.read_boolean (this->response_expected);
  hdr_status = hdr_status && msg.decode (&TC_opaque,
                                         &this->object_key,
                                         0,
                                         env);
  hdr_status = hdr_status && msg.decode (CORBA::_tc_string,
                                         &this->operation,
                                         0,
                                         env);
  hdr_status = hdr_status && msg.decode (CORBA::_tc_Principal,
                                         &this->requesting_principal,
                                         0,
                                         env);
  return hdr_status;
}

CORBA::Boolean
TAO_GIOP::start_message (TAO_GIOP::Message_Type type,
			 TAO_OutputCDR &msg)
{
  msg.reset ();

  // if (msg.size () < TAO_GIOP_HEADER_LEN)
  // return CORBA::B_FALSE;

  msg.write_octet ('G');
  msg.write_octet ('I');
  msg.write_octet ('O');
  msg.write_octet ('P');

  msg.write_octet (TAO_GIOP_MessageHeader::MY_MAJOR);
  msg.write_octet (TAO_GIOP_MessageHeader::MY_MINOR);

  msg.write_octet (TAO_ENCAP_BYTE_ORDER);
  msg.write_octet (type);

  // Write a dummy <size> later it is set to the right value...
  // @@ TODO Maybe we should store the OutputCDR status in
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  return CORBA::B_TRUE;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Unbounded_Sequence<TAO_GIOP_ServiceContext>;
template class TAO_Unbounded_Sequence<TAO_IOP_TaggedComponent>;
template class TAO_Unbounded_Object_Sequence<CORBA::TypeCode>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Unbounded_Sequence<TAO_GIOP_ServiceContext>
#pragma instantiate TAO_Unbounded_Sequence<TAO_IOP_TaggedComponent>
#pragma instantiate TAO_Unbounded_Object_Sequence<CORBA::TypeCode>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
