// @(#)giop.cpp	1.10 95/09/21
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// GIOP:	Utility routines for sending, receiving GIOP messages
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
// XXX there is lots of unverified I/O here.  In all cases, if an
// error is detected when marshaling or unmarshaling, it should be
// reported.

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
#include "tao/giop.i"
#endif /* __ACE_INLINE__ */

#if 0
#include "ace/Log_Msg.h"
#include "ace/SOCK_Stream.h"

#include "tao/corba.h"

#include "tao/giop.h"
#include "tao/orb.h"
#include "tao/orbobj.h"
#include "tao/factories.h"
#include "tao/cdr.h"
#include "tao/debug.h"
#include "tao/sequence.h"
#endif

// Apart from the length word, headers are specified to be arrays of
// bytes.  They're dealt with as such, rather than using CDR routines,
// to speed up the critical paths for message read and write.

static inline CORBA::Boolean
start_message (TAO_GIOP_MsgType type,
	       CDR &msg)
{
  msg.next = msg.buffer;		// for reused streams
  msg.remaining = msg.length;

  if (msg.bytes_remaining () < TAO_GIOP_HEADER_LEN)
    return CORBA::B_FALSE;

  msg.next [0] = 'G';
  msg.next [1] = 'I';
  msg.next [2] = 'O';
  msg.next [3] = 'P';

  msg.next [4] = MY_MAJOR;
  msg.next [5] = MY_MINOR;
  msg.next [6] = MY_BYTE_SEX;
  msg.next [7] = (u_char) type;

  msg.skip_bytes (TAO_GIOP_HEADER_LEN);
  return CORBA::B_TRUE;
}

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

static void
dump_msg (const char *label, 
	  const u_char *ptr,
	  size_t len)
{
  if (TAO_debug_level >= 2) 
    {
      ACE_DEBUG ((LM_DEBUG, "%s GIOP v%c.%c msg, %d data bytes, %s endian, %s\n",
                  label, digits[ptr[4]], digits[ptr[5]],
                  len - TAO_GIOP_HEADER_LEN,
                  (ptr[6] == MY_BYTE_SEX) ? "my" : "other",
                  (ptr[7] <= TAO_GIOP_MessageError) ? names [ptr[7]] : "UNKNOWN TYPE"));

      if (TAO_debug_level >= 4)
        ACE_HEX_DUMP ((LM_DEBUG, (const char*)ptr, len, " (%P|%t) data bytes\n"));
    }
}

CORBA::Boolean
TAO_GIOP::send_message (CDR &stream,
                        ACE_SOCK_Stream &peer)
{
  char *buf = (char *) stream.buffer;
  size_t buflen = stream.next - stream.buffer;
  int writelen;

  assert (buflen == (stream.length - stream.remaining));

  // Patch the message length in the GIOP header; it's always at the
  // same eight byte offset into the message.
  //
  // NOTE: Here would also be a fine place to calculate a digital
  // signature for the message and place it into a preallocated slot
  // in the "ServiceContext".  Similarly, this is a good spot to
  // encrypt messages (or just the message bodies) if that's needed in
  // this particular environment and that isn't handled by the
  // networking infrastructure (e.g. IPSEC).

  *(CORBA::Long *) (stream.buffer + 8) =
      (CORBA::Long) (buflen - TAO_GIOP_HEADER_LEN);

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.

  dump_msg ("send", stream.buffer, buflen);
  while (buflen > 0) 
    {
      if (buflen > stream.length) 
	{
	  ACE_DEBUG ((LM_DEBUG, " (%P|%t) ?? writebuf, buflen %u > length %u\n",
                      buflen, stream.length));
	  return CORBA::B_FALSE;
	}

      writelen = peer.send ((char _FAR *) buf, buflen);

#if defined (DEBUG)
      //      dmsg_filter (6, "wrote %d bytes to connection %d",
      //	   writelen, connection);
      dmsg_filter (6, "wrote %d bytes", writelen);
#endif	/* DEBUG */

      assert ((writelen >= 0 
	       && ((size_t)writelen) <= buflen) || writelen == -1);

      // On error or EOF, report the fault, close the connection, and
      // mark it as unusable/defunct.
      //
      // XXX on client side write errors, we may hit the case that the
      // server did a clean shutdown but we've not yet read the
      // GIOP::CloseConnection message.  If we get an error, we need
      // to see if there is such a message waiting for us, and if so
      // we should cause (full) rebinding to take place.

      if (writelen == -1) 
	{
	  ACE_DEBUG ((LM_ERROR,
		      " (%P|%t) %p\n", "OutgoingMessage::writebuf ()"));
	  ACE_DEBUG ((LM_DEBUG,
		      " (%P|%t) closing conn %d after fault\n", peer.get_handle ()));
          peer.close ();
	  return CORBA::B_FALSE;
	} 
      else if (writelen == 0) 
	{
	  ACE_DEBUG ((LM_DEBUG,
		      " (%P|%t) OutgoingMessage::writebuf () ... EOF, closing conn %d\n",
		      peer.get_handle ()));
          peer.close ();
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
  MY_MAJOR, 
  MY_MINOR, 
  MY_BYTE_SEX, 
  TAO_GIOP_CloseConnection,
  0, 0, 0, 0
};

void
TAO_GIOP::close_connection (ACE_SOCK_Stream &peer, 
                            void *)
{
  // It's important that we use a reliable shutdown after we send this
  // message, so we know it's received.
  //
  // XXX should recv and discard queued data for portability; note
  // that this won't block (long) since we never set SO_LINGER

  dump_msg ("send", (const u_char *) close_message, TAO_GIOP_HEADER_LEN);

  peer.send (close_message, TAO_GIOP_HEADER_LEN);
  peer.close ();
  ACE_DEBUG ((LM_DEBUG,
	      " (%P|%t) shut down socket %d\n", peer.get_handle ()));
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
  MY_MAJOR, 
  MY_MINOR, 
  MY_BYTE_SEX, 
  TAO_GIOP_MessageError,
  0, 0, 0, 0
};

static inline void
send_error (ACE_SOCK_Stream &peer)
{
  dump_msg ("send", (const u_char *) error_message, TAO_GIOP_HEADER_LEN);
  peer.send (error_message, TAO_GIOP_HEADER_LEN);
  peer.close ();
  ACE_DEBUG ((LM_DEBUG, " (%P|%t) aborted socket %d\n", peer.get_handle ()));
}

// @@ Can't we remove this stuff and replace it with recv_n () on the
// <peer>?

// Loop on data read ... this is required with some implementations of
// sockets (e.g. winsock, HP/UX) since even when async mode is not
// set, recv () won't block until the requested amount of data is
// available.

static int
read_buffer (ACE_SOCK_Stream &peer,
	     char *buf,
	     size_t len)
{
  int bytes_read = 0;
  bytes_read = peer.recv_n (buf, len);
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

TAO_GIOP_MsgType
TAO_GIOP::read_message (ACE_SOCK_Stream &connection,
                        CDR &msg, 
                        CORBA::Environment &env)
{
  TAO_GIOP_MsgType	retval;
  CORBA::ULong message_size;

  // Read the message header off the wire.
  //
  // THREADING NOTE: the connection manager handed us this connection
  // for exclusive use, so we need not worry about having two threads
  // interleave reads of partial messages.  This model is excellent
  // for "lightly threaded" systems (as will be the majority in the
  // near future) but makes less effective use of connection resources
  // as the "duty factor" goes down because of either long calls or
  // bursty contention during numerous short calls to the same server.

  assert (msg.length > TAO_GIOP_HEADER_LEN);

  msg.next = msg.buffer;
  msg.remaining = TAO_GIOP_HEADER_LEN;

  char *bufptr = (char _FAR *) msg.buffer;
  ssize_t len = read_buffer (connection, bufptr, TAO_GIOP_HEADER_LEN);
  // Read the header into the buffer.

  if (len != TAO_GIOP_HEADER_LEN)
    {
      switch (len)
        {
        case 0:
          ACE_DEBUG ((LM_DEBUG,
                      " (%P|%t) Header EOF ... peer probably aborted connection %d\n", 
                      connection.get_handle ()));
          return TAO_GIOP_EndOfFile;
          // XXX should probably find some way to report this without
          // an exception, since for most servers it's not an error.
          // Is it _never_ an error?  Not sure ...
          /* NOTREACHED */
        case -1: // error
          ACE_DEBUG ((LM_ERROR,
                      " (%P|%t) GIOP::read_message header socket error %p\n",
                      "read_buffer"));
          break;
          /* NOTREACHED */
        default:
          ACE_DEBUG ((LM_ERROR,
                      " (%P|%t) GIOP::read_message header read failed, only %d of %d bytes\n",
                      len, TAO_GIOP_HEADER_LEN));
          break;
          /* NOTREACHED */
        }

      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));
      return TAO_GIOP_MessageError;
    }

  // NOTE: if message headers, or whome messages, get encrypted in
  // application software (rather than by the network infrastructure)
  // they should be decrypted here ...

  // First make sure it's a GIOP message of any version.

  if (!(msg.buffer [0] == 'G' 
	&& msg.buffer [1] == 'I'
	&& msg.buffer [2] == 'O' 
	&& msg.buffer [3] == 'P')) 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));	// header
      ACE_DEBUG ((LM_DEBUG, "bad header, magic word\n"));
      return TAO_GIOP_MessageError;
    }

  // Then make sure the major version is ours, and the minor version
  // is one that we understand.

  if (!(msg.buffer [4] == MY_MAJOR && msg.buffer [5] <= MY_MINOR)) 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_MAYBE));	// header
      ACE_DEBUG ((LM_DEBUG, "bad header, version\n"));
      return TAO_GIOP_MessageError;
    }

  // Get the message type out and adjust the buffer's records to record
  // that we've read everything except the length.

  retval = (TAO_GIOP_MsgType) msg.buffer[7];
  msg.skip_bytes (8);

  // Make sure byteswapping is done if needed, and then read the
  // message size (appropriately byteswapped).

  msg.do_byteswap = (msg.buffer [6] != MY_BYTE_SEX);
  msg.get_ulong (message_size);

  // Make sure we have the full length in memory, growing the buffer
  // if needed.
  //
  // NOTE: We could overwrite these few bytes of header... they're
  // left around for now as a debugging aid.

  assert (message_size <= UINT_MAX);

  if ((TAO_GIOP_HEADER_LEN + message_size) > msg.length)
    msg.grow ((size_t) (TAO_GIOP_HEADER_LEN + message_size));

  msg.remaining = (size_t) message_size;
  bufptr = (char *) & msg.buffer [TAO_GIOP_HEADER_LEN];

  // Read the rest of this message into the buffer.

  len = read_buffer (connection, bufptr, (size_t) message_size);

  if (len != (ssize_t) message_size) 
    {
      switch (len)
        {
        case 0:
          ACE_DEBUG ((LM_DEBUG,
                      " (%P|%t) TAO_GIOP::read_message body, EOF on handle %d\n",
                      connection.get_handle ()));
          break;
          /* NOTREACHED */
        case -1:
          ACE_DEBUG ((LM_ERROR,
                      " (%P|%t) TAO_GIOP::read_message () body %p\n",
                      "read_buffer"));
          break;
          /* NOTREACHED */
        default:
          ACE_DEBUG ((LM_ERROR,
                      " (%P|%t) short read, only %d of %d bytes\n", len, message_size));
          break;
          /* NOTREACHED */
        }

      // clean up, and ...
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));	// body
      ACE_DEBUG ((LM_DEBUG, "couldn't read rest of message\n"));
      return TAO_GIOP_MessageError;
    }

  dump_msg ("recv", msg.buffer, (size_t) (message_size + TAO_GIOP_HEADER_LEN));
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
    opname (operation),
    do_rsvp (is_roundtrip),
    stream (&buffer [0], sizeof buffer)
{
  // The assumption that thread ids are ints is false and horribly
  // implementation-dependent, so this code just sucks.  But, at least
  // it will compile on multiple platforms through the magic of ACE
  // :-/

  //assert (sizeof (CORBA::ULong) == sizeof (ACE_thread_t));
  ACE_thread_t me = ACE_OS::thr_self ();
  my_request_id = 0;
  
  // Copy in only as many bytes are valid, or only as many as we have
  // room for, whichever is less.  -------> What a friggin' HACK!?!?!
  memcpy (&my_request_id, &me, ACE_MIN (sizeof (me), sizeof (my_request_id)));
}

TAO_GIOP_Invocation::~TAO_GIOP_Invocation (void)
{
  handler_->in_use (CORBA::B_FALSE);
}

// Octet codes for the parameters of the "Opaque" (sequence of octet)
// data type used various places internally ... a CDR encapsulation
// holding two parameters (like all sequence TypeCodes).
//
// NOTE: this **MUST** be longword aligned, which is why it's coded as
// a longword array not an octet array.  Just sticking a long in for
// padding won't work with compilers that optimize unused data out of
// existence.

static const CORBA::Long _oc_opaque [] = 
{	// CDR typecode octets
  1,				// native endian + padding; "tricky"
  10,				// ... (sequence of) octets
  0				// ... unbounded
};

CORBA::TypeCode
TC_opaque (CORBA::tk_sequence,
           sizeof _oc_opaque,
           (u_char *) &_oc_opaque,
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
  1, // native endian + padding; "tricky"

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
  CORBA::tk_sequence,	// sequence typecode
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
                       (u_char *) &_oc_svc_ctx_list,
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

  // @@ Why is this lock here, i.e., what is it protecting?  Can
  // we remove it?
  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, guard, lock_));

  // Get a CORBA::Object_ptr from _data using QueryInterface ()
  CORBA::Object_ptr obj = 0;

 (void) data_->QueryInterface (IID_CORBA_Object, (void **)&obj);

  // Get a pointer to the orb from the object
  CORBA::ORB_ptr orb = obj->orb ();

  // Get a reference to the client connector
  //  TAO_Client_Factory::CONNECTOR* con = 0;
  TAO_Client_Strategy_Factory::CONNECTOR* con = 0;
  con = orb->client_factory ()->connector ();

  // Determine the object key and the address to which we'll need a
  // connection.
  ACE_INET_Addr server_addr;
    
  if (data_->fwd_profile != 0)
    {
      key = &data_->fwd_profile->object_key;
      server_addr.set (data_->fwd_profile->port,
		       data_->fwd_profile->host);
    }
  else
    {
      key = &data_->profile.object_key;
      server_addr.set (data_->profile.port,
		       data_->profile.host);
    }
    
  // Establish the connection and get back a Client_Connection_Handler
  if (con->connect (handler_, server_addr) == -1)
    // @@ Need to figure out which exception to set...this one is
    // pretty vague.
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));

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

  CORBA::Boolean bt = start_message (TAO_GIOP_Request, stream);

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
  static TAO_GIOP_ServiceContextList svc_ctx;	// all zeroes

  if (stream.encode (&TC_ServiceContextList, 0, &svc_ctx, env)
      != CORBA::TypeCode::TRAVERSE_CONTINUE)
    return;

  if (!stream.put_ulong (my_request_id) 
      || !stream.put_boolean (do_rsvp)) 
    {
      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_NO));
      return;
    }

  if (stream.encode (&TC_opaque,
		    key, 0, 
		    env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.encode (CORBA::_tc_string, 
		       &opname, 0, 
		       env) != CORBA::TypeCode::TRAVERSE_CONTINUE
      || stream.encode (CORBA::_tc_Principal, 
		       &anybody, 0, 
		       env) != CORBA::TypeCode::TRAVERSE_CONTINUE)
    return; // right after fault
  else
    return; // no fault reported
}

extern CORBA::ExceptionList __system_exceptions;

// Send request, block until any reply comes back, and unmarshal reply
// parameters as appropriate.

TAO_GIOP_ReplyStatusType
TAO_GIOP_Invocation::invoke (CORBA::ExceptionList &exceptions,
			  CORBA::Environment &env)
{
  // Send Request, return on error or if we're done

  if (!TAO_GIOP::send_message (stream, handler_->peer ())) 
    {
      // send_message () closed the connection; we just release it here.
      //
      // XXX highly desirable to know whether we wrote _any_ data; if
      // we wrote none, then there's no chance the call completed and
      // applications don't have to deal with those nasty
      // indeterminate states where they can't immediatly tell if
      // what's safe to do.
      //
      // XXX also, there might have been a GIOP::CloseConnection
      // message in the input queue.  If so, this request should be
      // treated as a (full) "rebind" case.  Can't do that from this
      // point in the code however!  Some minor restructuring needs to
      // happen.
      //
      handler_ = 0;
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));
      return TAO_GIOP_SYSTEM_EXCEPTION;
    }
  if (!do_rsvp)
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
  // XXX In all MT environments, there's a cancellation point lurking
  // here; need to investigate.  Client threads would frequently be
  // canceled sometime during read_message ... the correct action to
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
  //
  switch (TAO_GIOP::read_message (handler_->peer (), stream, env)) 
    {
    case TAO_GIOP_Reply:
      // handle reply ... must be right one etc
      break;

    case TAO_GIOP_CloseConnection:
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
	ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, TAO_GIOP_SYSTEM_EXCEPTION));

	delete data_->fwd_profile;
	data_->fwd_profile = 0;

        handler_->peer ().close ();
        handler_->in_use (CORBA::B_FALSE);
        handler_ = 0;
	return TAO_GIOP_LOCATION_FORWARD;
      }

    case TAO_GIOP_Request:
    case TAO_GIOP_CancelRequest:
    case TAO_GIOP_LocateRequest:
    case TAO_GIOP_LocateReply:
    default:
      // These are all illegal messages to find.  If found, they could
      // be indicative of client bugs (lost track of input stream) or
      // server bugs; maybe the request was acted on, maybe not, we
      // can't tell.
      ACE_DEBUG ((LM_DEBUG, " (%P|%t) illegal message in response to my Request!\n"));
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));
      // FALLTHROUGH ...

    case TAO_GIOP_MessageError:
      // Couldn't read it for some reason ... exception's set already,
      // so just tell the other end about the trouble (closing the
      // connection) and return.
      send_error (handler_->peer ());
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
  CORBA::ULong reply_status;		// TAO_GIOP_ReplyStatusType

  if (stream.decode (&TC_ServiceContextList, &reply_ctx, 0, env)
      != CORBA::TypeCode::TRAVERSE_CONTINUE) 
    {
      send_error (handler_->peer ());
      return TAO_GIOP_SYSTEM_EXCEPTION;
    }

  delete [] reply_ctx.buffer;
    
  if (!stream.get_ulong (request_id)
      || request_id != my_request_id
      || !stream.get_ulong (reply_status)
      || reply_status > TAO_GIOP_LOCATION_FORWARD) 
    {
      send_error (handler_->peer ());
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_MAYBE));
      ACE_DEBUG ((LM_DEBUG, " (%P|%t) bad Response header\n"));
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
	CORBA::String exception_id;

	// Pull the exception ID out of the marshaling buffer.
	{
	  CORBA::ULong len;

	  //
	  // Read "length" field of string, so "next" points
	  // right at the null-terminated ID.  Then get the ID.
	  //
	  if (stream.get_ulong (len) != CORBA::B_TRUE
	      || len > stream.remaining) 
	    {
	      send_error (handler_->peer ());
	      env.exception (new CORBA::MARSHAL (CORBA::COMPLETED_YES));
	      return TAO_GIOP_SYSTEM_EXCEPTION;
	    }
	  exception_id = (CORBA::String) stream.next;
	  stream.skip_bytes (len);
	}

	// User and system exceptions differ only in what table of
	// exception typecodes is searched.
	CORBA::ExceptionList *xlist;

	if (reply_status == TAO_GIOP_USER_EXCEPTION)
	  xlist = &exceptions;
	else
	  xlist = &__system_exceptions;

	// Find it in the operation description and then use that to get
	// the typecode.  Use it to unmarshal the exception's value; if
	// that exception is not allowed by this operation, fail (next). 

	u_int i;
	CORBA::TypeCode_ptr *tcp;

	for (i = 0, tcp = xlist->buffer;
	     i < xlist->length;
	     i++, tcp++) 
	  {
	    CORBA::String xid;

	    xid = (*tcp)->id (env);
	    if (env.exception () != 0) 
	      {
		dexc (env, "invoke (), get exception ID");
		send_error (handler_->peer ());
		return TAO_GIOP_SYSTEM_EXCEPTION;
	      }

	    if (ACE_OS::strcmp ((char *)exception_id, (char *)xid) == 0) 
	      {
		size_t			size;
		CORBA::Exception		*exception;

		size = (*tcp)->size (env);
		if (env.exception () != 0) 
		  {
		    dexc (env, "invoke (), get exception size");
		    send_error (handler_->peer ());
		    return TAO_GIOP_SYSTEM_EXCEPTION;
		  }

		// Create the exception, fill in the generic parts
		// such as vtable, typecode ptr, refcount ... we need
		// to clean them all up together, in case of errors
		// unmarshaling.

		exception = new (new char [size]) CORBA::Exception (*tcp);

		if (stream.decode (*tcp, exception, 0, env)
		    != CORBA::TypeCode::TRAVERSE_CONTINUE) 
		  {
		    delete exception;
		    ACE_DEBUG ((LM_ERROR, " (%P|%t) invoke, unmarshal %s exception %s\n",
				(reply_status == TAO_GIOP_USER_EXCEPTION) ? "user" : "system",
                               exception_id));
		    send_error (handler_->peer ());
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

	if (stream.decode (CORBA::_tc_Object, 
			  &obj, 0, 
			  env) != CORBA::TypeCode::TRAVERSE_CONTINUE
	    || obj->QueryInterface (IID_IIOP_Object, 
                                    (void **)&obj2) != NOERROR)
	  {
	    dexc (env, "invoke, location forward");
	    send_error (handler_->peer ());
	    return TAO_GIOP_SYSTEM_EXCEPTION;
	  }
	CORBA::release (obj);

	// Make a copy of the IIOP profile in the forwarded objref,
	// reusing memory where practical.  Then delete the forwarded
	// objref, retaining only its profile.
	//
	// XXX add and use a "forward count", to prevent loss of data
	// in forwarding chains during concurrent calls -- only a
	// forward that's a response to the current fwd_profile should
	// be recorded here. (This is just an optimization, and is not
	// related to correctness.)

	ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, TAO_GIOP_SYSTEM_EXCEPTION);

	delete data_->fwd_profile;
	data_->fwd_profile = new IIOP::ProfileBody (obj2->profile);

	obj2->Release ();

	env.clear ();

	//
	// Make sure a new connection is used next time.
	//
	handler_ = 0;           // not sure this is correct!
      }
    break;
    }

  // All standard exceptions from here on in the call path know for
  // certain that the call "completed" ... except in the case of
  // system exceptions which say otherwise, and for TAO_GIOP_LOCATION_FORWARD
  // responses.

  return (TAO_GIOP_ReplyStatusType) reply_status;
}

void
TAO_GIOP::make_error (CDR &msg, ...)
{
  ACE_UNUSED_ARG (msg);  // just for now
  
  // This [static] method will be somewhat like send_error() except
  // that it won't actaully do any sending of data...it'll just stuff
  // things into the <msg> instance.
}


// Generic server side read + dispatch one message; returns when that
// bit of work is complete.
//
// In the typical case, the request and response buffers live on the
// stack so that the heap never gets used.  These grow if needed.

int
TAO_GIOP::incoming_message (ACE_SOCK_Stream &peer,
                        TAO_GIOP_ForwardFunc check_forward,
                        TAO_GIOP_RequestHandler handle_request,
                        void *context,
                        CORBA::Environment &env)
{
  int retval = 1;		// 1==success, 0==eof, -1==error

  u_char buffer [CDR::DEFAULT_BUFSIZE];
  CDR msg (&buffer [0], sizeof buffer);

  switch (read_message (peer, msg, env))
    {
      // These messages should never be sent to the server; it's an
      // error if the peer tries.  Set the environment accordingly, as
      // it's not yet been reported as an error.
    case TAO_GIOP_Reply:
    case TAO_GIOP_LocateReply:
    case TAO_GIOP_CloseConnection:
    default:					// Unknown message
      ACE_DEBUG ((LM_DEBUG, " (%P|%t) Illegal message received by server\n"));
      env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));
      // FALLTHROUGH

      // read_message () has already set some error in the environment
      // for all "MessageError" cases, so don't clobber it.
      //
      // General error recovery is to send MessageError to the peer
      // just in case (it'll fail on EOF) and then close the
      // connection.
    case TAO_GIOP_MessageError:
      retval = -1;
      send_error (peer);
      break;

    case TAO_GIOP_EndOfFile:
      retval = 0;
      break;

      // This is the common case!
    case TAO_GIOP_Request:
      {
	TAO_GIOP_RequestHeader req;
	CORBA::Boolean hdr_status;

	// Tear out the service context ... we currently ignore it,
	// but it should probably be passed to each ORB service as
	// appropriate (e.g. transactions, security).
	//
	// NOTE: As security support kicks in, this is a good place to
	// verify a digital signature, if that is required in this
	// security environment.  It may be required even when using
	// IPSEC security infrastructure.

	hdr_status = msg.decode (&TC_ServiceContextList,
				 &req.service_info,
				 0, 
				 env);

	// Get the rest of the request header ...

	hdr_status = hdr_status && msg.get_ulong (req.request_id);
	hdr_status = hdr_status && msg.get_boolean (req.response_expected);
	hdr_status = hdr_status && msg.decode (&TC_opaque, 
					       &req.object_key,
					       0, 
					       env);
	hdr_status = hdr_status && msg.decode (CORBA::_tc_string,
					       &req.operation,
					       0,
					       env);
	hdr_status = hdr_status && msg.decode (CORBA::_tc_Principal,
					       &req.requesting_principal,
					       0,
					       env);

	// XXX check whether hdr_status identifies a header
	// unmarshaling error, and handle appropriately

#if defined (DEBUG)
	if (TAO_debug_level >= 3) 
	  {
            ACE_DEBUG ((LM_DEBUG, " (%P|%t) %sRequest ID %#lx from FD %d\n",
                       req.response_expected ? "" : "Oneway ",
                       req.request_id, peer.get_handle ()));
	    if (TAO_debug_level >= 4) 
	      {
		ACE_HEX_DUMP ((LM_DEBUG, (char*)req.object_key.buffer,
			     req.object_key.length, " (%P|%t) object key\n"));
		ACE_DEBUG ((LM_DEBUG,
			    " (%P|%t) opname '%s'\n", req.operation));
		if (req.requesting_principal) 
		  ACE_HEX_DUMP ((LM_DEBUG,
				 (char*)req.requesting_principal->id.buffer,
				 req.requesting_principal->id.length,
				 " (%P|%t) client principal\n"));
		else
		  ACE_DEBUG ((LM_DEBUG, " (%P|%t) client principal (EMPTY)\n"));
	      }

	    // NOTE: describe any service context, and how many bytes
	    // of non-header data were sent.
	  }
#endif /* DEBUG */

	// Verify that we're to dispatch the request within this
	// particular process.

	if (check_forward != 0) 
	  {
	    TAO_GIOP_LocateStatusType status;
	    CORBA::Object_ptr fwd_ref = 0;

	    status = check_forward (req.object_key, fwd_ref, context);

	    if (status != TAO_GIOP_OBJECT_HERE) 
	      {
		TAO_GIOP_ServiceContextList resp_ctx;
		u_char buf2 [CDR::DEFAULT_BUFSIZE];
		CDR response (&buf2 [0], sizeof buf2);

		start_message (TAO_GIOP_Reply, response);
		resp_ctx.length = 0;
		response.encode (&TC_ServiceContextList,
			      &resp_ctx,
			      0,
			      env);
		response.put_ulong (req.request_id);

		// If we're not sending a response, just clean up.

		if (!req.response_expected) 
		  {
		    if (status == TAO_GIOP_OBJECT_FORWARD)
		      CORBA::release (fwd_ref);

		    //
		    // Else either forward the request ...
		    //
		  } 
		else if (status == TAO_GIOP_OBJECT_FORWARD) 
		  {
		    ACE_DEBUG ((LM_DEBUG, " (%P|%t) forwarding Request message\n"));
		    response.put_ulong (TAO_GIOP_LOCATION_FORWARD);
		    response.encode (CORBA::_tc_Object,
				  &fwd_ref,
				  0, 
				  env);
		    CORBA::release (fwd_ref);
		    (void) send_message (response, peer);

		    // ... or report exception that the object doesn't exist.
		  } 
		else 
		  {
		    CORBA::OBJECT_NOT_EXIST exc (CORBA::COMPLETED_YES);

		    response.put_ulong (TAO_GIOP_SYSTEM_EXCEPTION);

		    (void) response.encode (CORBA::_tc_OBJECT_NOT_EXIST,
					 &exc,
					 0,
					 env);

		    (void) send_message (response, peer);
		  }

		delete [] req.object_key.buffer;
		CORBA::string_free (req.operation);
		return retval;
	      }
	  }

	// So, we read a request, now dispatch it using something more
	// primitive than a CORBA2 ServerRequest pseudo-object.

	if (req.response_expected) 
	  {
	    TAO_GIOP_ServiceContextList resp_ctx;
	    u_char buf2 [CDR::DEFAULT_BUFSIZE];
	    CDR response (&buf2 [0], sizeof buf2);

	    start_message (TAO_GIOP_Reply, response);
	    resp_ctx.length = 0;
	    response.encode (&TC_ServiceContextList,
			  &resp_ctx,
			  0,
			  env);
	    response.put_ulong (req.request_id);

	    handle_request (req, msg, &response, context, env);

	    // "handle_request" routine puts ReplyStatusType then
	    // parameters.

	    (void) send_message (response, peer);
	  } 
	else
	  handle_request (req, msg, 0, context, env);

	delete [] req.object_key.buffer;
	CORBA::string_free (req.operation);
      }
    break;

    // Forward requests as needed; if caller hasn't provided code to
    // support forwarding, we default to doing no forwarding.
    case TAO_GIOP_LocateRequest:
      {
	CORBA::ULong request_id;
	TAO_opaque key;

	msg.get_ulong (request_id);
	msg.decode (&TC_opaque, &key, 0, env);

	// we've read the request header; send a LocateReply

	u_char resp [CDR::DEFAULT_BUFSIZE];
	CDR response (resp, sizeof resp);
	CORBA::Object_ptr fwd_ref = 0;

	start_message (TAO_GIOP_LocateReply, response);
	response.put_ulong (request_id);

	if (check_forward == 0) 
	  {
	    response.put_ulong (TAO_GIOP_OBJECT_HERE);
	    ACE_DEBUG ((LM_DEBUG, " (%P|%t) LocateRequest response:  object is (always) here!\n"));
	  } 
	else 
	  {
	    TAO_GIOP_LocateStatusType status;

	    status = check_forward (key, fwd_ref, context);
	    response.put_ulong ((CORBA::ULong) status);

	    if (status == TAO_GIOP_OBJECT_FORWARD) 
	      {
		ACE_DEBUG ((LM_DEBUG, "LocateRequest response:  forward requests\n"));
		response.encode (CORBA::_tc_Object, &fwd_ref, 0, env);
	      } 
	    else if (status == TAO_GIOP_OBJECT_HERE)
	      ACE_DEBUG ((LM_DEBUG, "LocateRequest response:  object is here!\n"));
	    else 
	      ACE_DEBUG ((LM_DEBUG, "LocateRequest response:  no such object\n"));
	  }
	(void) send_message (response, peer);
      }
    break;

    // Cancel request -- ignored this implementation.
    //
    // THREADING NOTE: Handling it would require (a) some thread to
    // read the CancelRequest while one was working on handling the
    // Request, (b) a way to find the thread working on that request,
    // (c) using thread cancellation to alert that thread that the
    // work it's been doing can safely be discarded.  Also of course
    // (d) making the various worker threads cleanly handle
    // cancellation, and (e) modifying client code to send a
    // CancelRequest when it's been canceled.

    case TAO_GIOP_CancelRequest:
      {
	CORBA::ULong request_id;

	msg.get_ulong (request_id);
      }
    break;
    }

  // ... error if unconsumed data remains; is this the spot to test
  // that?
  return retval;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class CORBA_SEQUENCE<TAO_GIOP_ServiceContext>;
template class CORBA_SEQUENCE<CORBA::Octet>;
template class CORBA_SEQUENCE<CORBA::TypeCode*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate CORBA_SEQUENCE<TAO_GIOP_ServiceContext>
#pragma instantiate CORBA_SEQUENCE<CORBA::Octet>
#pragma instantiate CORBA_SEQUENCE<CORBA::TypeCode*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
