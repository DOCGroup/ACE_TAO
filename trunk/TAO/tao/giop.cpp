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

#include "ace/Log_Msg.h"
#include "ace/SOCK_Stream.h"

#include "tao/giop.h"
#include "tao/orb.h"
#include "tao/orbobj.h"
#include "tao/factories.h"
#include "tao/cdr.h"
#include "tao/debug.h"


// defined by GIOP 1.0 protocol
#define	TAO_GIOP_HEADER_LEN 12		

// Apart from the length word, headers are specified to be arrays of
// bytes.  They're dealt with as such, rather than using CDR routines,
// to speed up the critical paths for message read and write.

static inline CORBA_Boolean
start_message (GIOP::MsgType type,
	       CDR &msg)
{
  msg.next = msg.buffer;		// for reused streams
  msg.remaining = msg.length;

  if (msg.bytes_remaining () < TAO_GIOP_HEADER_LEN)
    return CORBA_B_FALSE;

  msg.next [0] = 'G';
  msg.next [1] = 'I';
  msg.next [2] = 'O';
  msg.next [3] = 'P';

  msg.next [4] = GIOP::MY_MAJOR;
  msg.next [5] = GIOP::MY_MINOR;
  msg.next [6] = MY_BYTE_SEX;
  msg.next [7] = (u_char) type;

  msg.skip_bytes (TAO_GIOP_HEADER_LEN);
  return CORBA_B_TRUE;
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
  "MessageError"
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
 (ptr[7] <= GIOP::MessageError) ? names [ptr[7]] : "UNKNOWN TYPE"));

      if (TAO_debug_level >= 4)
        ACE_HEX_DUMP ((LM_DEBUG, (const char*)ptr, len, " (%P|%t) data bytes\n"));
    }
}

CORBA_Boolean
GIOP::send_message (CDR &stream,
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

  *(CORBA_Long *) (stream.buffer + 8) =
      (CORBA_Long) (buflen - TAO_GIOP_HEADER_LEN);

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
	  return CORBA_B_FALSE;
	}

      writelen = peer.send ((char _FAR *) buf, buflen);

#ifdef	DEBUG
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
	  return CORBA_B_FALSE;
	} 
      else if (writelen == 0) 
	{
	  ACE_DEBUG ((LM_DEBUG,
		      " (%P|%t) OutgoingMessage::writebuf () ... EOF, closing conn %d\n",
		      peer.get_handle ()));
          peer.close ();
	  return CORBA_B_FALSE;
	}
      if ((buflen -= writelen) != 0)
	buf += writelen;

#ifdef	DEBUG
      //
      // NOTE:  this should never be seen.  However, on Linux
      // it's been seen with UNIX domain sockets.
      //
      if (buflen)
	dmsg_filter (8, "%u more bytes to write...\n", buflen);
#endif
    }
  return CORBA_B_TRUE;
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
  GIOP::MY_MAJOR, 
  GIOP::MY_MINOR, 
  MY_BYTE_SEX, 
  GIOP::CloseConnection,
  0, 0, 0, 0
};

void
GIOP::close_connection (ACE_SOCK_Stream &peer, 
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
  GIOP::MY_MAJOR, 
  GIOP::MY_MINOR, 
  MY_BYTE_SEX, 
  GIOP::MessageError,
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
#if 1
  bytes_read = peer.recv_n (buf, len);
#else
  while (len != 0) 
    {
      int retval;

      retval = peer.recv (buf, len);

#ifdef	DEBUG
      dmsg_filter (6, "read %d bytes from connection: %d", retval, peer.get_handle ());
#endif
      if (retval <= 0)			// EOF or error
	return retval;

      len -= retval;
      buf += retval;
      bytes_read += retval;
    }
#endif
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

GIOP::MsgType
GIOP::read_message (ACE_SOCK_Stream &connection,
		    CDR &msg, 
		    CORBA_Environment &env)
{
  GIOP::MsgType	retval;
  CORBA_ULong message_size;

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
  int len = read_buffer (connection, bufptr, TAO_GIOP_HEADER_LEN);
  // Read the header into the buffer.

  if (len != TAO_GIOP_HEADER_LEN) 
    {
      if (len == 0) 
	{			// EOF
	  ACE_DEBUG ((LM_DEBUG,
		      " (%P|%t) Header EOF ... peer probably aborted connection %d\n", 
                      connection.get_handle ()));
	  return EndOfFile;
	  // XXX should probably find some way to report this without
	  // an exception, since for most servers it's not an error.
	  // Is it _never_ an error?  Not sure ...
	} 
      else if (len < 0) // error
	ACE_DEBUG ((LM_ERROR,
		    " (%P|%t) GIOP::read_message header socket error %p\n"));
      else // short read ... 
	ACE_DEBUG ((LM_ERROR,
		    " (%P|%t) GIOP::read_message header failed (short)\n"));

      env.exception (new CORBA_COMM_FAILURE (COMPLETED_MAYBE));
      return MessageError;
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
      env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));	// header
      ACE_DEBUG ((LM_DEBUG, "bad header, magic word\n"));
      return MessageError;
    }

  // Then make sure the major version is ours, and the minor version
  // is one that we understand.

  if (!(msg.buffer [4] == MY_MAJOR && msg.buffer [5] <= MY_MINOR)) 
    {
      env.exception (new CORBA_MARSHAL (COMPLETED_MAYBE));	// header
      ACE_DEBUG ((LM_DEBUG, "bad header, version\n"));
      return MessageError;
    }

  // Get the message type out and adjust the buffer's records to record
  // that we've read everything except the length.

  retval = (GIOP::MsgType) msg.buffer[7];
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

  if (len != (int) message_size) 
    {
      if (len == 0) 
	ACE_DEBUG ((LM_DEBUG,
		    " (%P|%t) GIOP::read_message body, EOF on handle %d\n", connection.get_handle ()));
      else if (len < 0) 
	ACE_DEBUG ((LM_ERROR,
		    " (%P|%t) GIOP::read_message () body %p\n"));
      else 
	ACE_DEBUG ((LM_ERROR,
		    " (%P|%t) short read, only %d of %d bytes\n", len, message_size));

      // clean up, and ...
      env.exception (new CORBA_COMM_FAILURE (COMPLETED_MAYBE));	// body
      ACE_DEBUG ((LM_DEBUG, "couldn't read rest of message\n"));
      return MessageError;
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

GIOP::Invocation::Invocation (IIOP_Object *data,
			      const char *operation,
			      CORBA_Boolean is_roundtrip) 
  : data_ (data),
    opname (operation),
    do_rsvp (is_roundtrip),
    stream (&buffer [0], sizeof buffer)
{
  // The assumption that thread ids are ints is false and horribly
  // implementation-dependent, so this code just sucks.  But, at least
  // it will compile on multiple platforms through the magic of ACE
  // :-/

  //assert (sizeof (CORBA_ULong) == sizeof (ACE_thread_t));
  ACE_thread_t me = ACE_OS::thr_self ();
  my_request_id = 0;
  
  // Copy in only as many bytes are valid, or only as many as we have
  // room for, whichever is less.  -------> What a friggin' HACK!?!?!
  memcpy (&my_request_id, &me, ACE_MIN (sizeof (me), sizeof (my_request_id)));
}

GIOP::Invocation::~Invocation (void)
{
  handler_->in_use (CORBA_B_FALSE);
}

// Octet codes for the parameters of the "Opaque" (sequence of octet)
// data type used various places internally ... a CDR encapsulation
// holding two parameters (like all sequence TypeCodes).
//
// NOTE: this **MUST** be longword aligned, which is why it's coded as
// a longword array not an octet array.  Just sticking a long in for
// padding won't work with compilers that optimize unused data out of
// existence.

static const CORBA_Long _oc_opaque [] = 
{	// CDR typecode octets
  1,				// native endian + padding; "tricky"
  10,				// ... (sequence of) octets
  0				// ... unbounded
};

CORBA_TypeCode TC_opaque (tk_sequence,
			  sizeof _oc_opaque,
 (u_char *) &_oc_opaque,
			  CORBA_B_FALSE);

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

static const CORBA_Long _oc_svc_ctx_list [] = 
{
  // START bytes of encapsulation 0
  1, // native endian + padding; "tricky"

  //
  // FIRST sequence param:  typecode for struct is complex,
  // and so uses a nested encapsulation.
  //
  tk_struct,
  72, // length of encapsulation 1 

  // START bytes of encapsulation 1 (struct params)
  1, // native endian + padding; "tricky"
  1, 0, // type ID omitted:  null string
  1, 0, // name omitted "ServiceContext"

  2, // two struct elements

  // First structure element:  name, typecode for ULong
  //
  // NOTE:  to be more strictly correct this could be a tk_alias
  // typecode ...

  1, 0, // name omitted:  "context_id"
  tk_long,

  // Second structure element: name, typecode for sequence of octet;
  // the typecode for sequence of octet is complex, there's a second
  // level of nested encapuslation here.

  1, 0, // name omitted:  "context_data"
  tk_sequence,	// sequence typecode
  16, // length of encapsulation 2

  // START bytes of encapsulation 2 (sequence params)
  1, // native endian + padding; "tricky"
  1, 0, // type ID omitted:  null string
  tk_octet, // (sequence of) octet
  0, // ... unbounded length
  // END bytes of encapsulation 2 (sequence params)

  // END bytes of encapsulation 1 (struct params)

  // SECOND sequence param:  bound of sequence (none)
  0 // unbounded seq of ServiceContext
  // END bytes of encapsulation 0 (sequence params)
};

static CORBA_TypeCode TC_ServiceContextList (tk_sequence,
					     sizeof _oc_svc_ctx_list,
 (u_char *) &_oc_svc_ctx_list,
					     CORBA_B_FALSE);

// The public API involves creating an invocation, starting it, filling
// in request parameters, actually performing the invocation, getting
// response parameters, and then cleaning up.  Sometimes they must be
// restarted (e.g. request forwarding).  This is the start/restart entry.

void
GIOP::Invocation::start (CORBA_Environment &env)
{
  const opaque	*key;

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

  ACE_MT (ACE_GUARD (ACE_Thread_Mutex, guard, lock_));

  // Get a CORBA_Object_ptr from _data using QueryInterface ()
  CORBA_Object_ptr obj = 0;

 (void) data_->QueryInterface (IID_CORBA_Object, (void **)&obj);

  // Get a pointer to the orb from the object
  CORBA_ORB_ptr orb = obj->orb ();

  // Get a reference to the client connector
  TAO_Client_Factory::CONNECTOR* con = 0;
  con = (orb->client_factory ()).connector ();

  // Determine the object key and the address to which we'll need a connection
  ACE_INET_Addr server_addr;
    
  if (data_->fwd_profile != 0)
    {
      key = &data_->fwd_profile->object_key;
      server_addr.set (data_->fwd_profile->port, data_->fwd_profile->host);
    }
  else
    {
      key = &data_->profile.object_key;
      server_addr.set (data_->profile.port, data_->profile.host);
    }
    
  // Establish the connection and get back a Client_Connection_Handler
  if (con->connect (handler_, server_addr) == -1)
    // @@ Need to figure out which exception to set...this one is
    // pretty vague.
      env.exception (new CORBA_COMM_FAILURE (COMPLETED_NO));

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

  CORBA_Boolean bt = start_message (Request, stream);

  if (bt != CORBA_B_TRUE) 
    {
      env.exception (new CORBA_MARSHAL (COMPLETED_NO));
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
  //
  static CORBA_Principal_ptr anybody = 0;
  static ServiceContextList svc_ctx;	// all zeroes

  if (stream.encode (&TC_ServiceContextList, 0, &svc_ctx, env)
      != CORBA_TypeCode::TRAVERSE_CONTINUE)
    return;

  if (!stream.put_ulong (my_request_id) 
      || !stream.put_boolean (do_rsvp)) 
    {
      env.exception (new CORBA_MARSHAL (COMPLETED_NO));
      return;
    }

  if (stream.encode (&TC_opaque,
		    key, 0, 
		    env) != CORBA_TypeCode::TRAVERSE_CONTINUE
      || stream.encode (_tc_CORBA_String, 
		       &opname, 0, 
		       env) != CORBA_TypeCode::TRAVERSE_CONTINUE
      || stream.encode (_tc_CORBA_Principal, 
		       &anybody, 0, 
		       env) != CORBA_TypeCode::TRAVERSE_CONTINUE)
    return; // right after fault
  else
    return; // no fault reported
}

extern CORBA_ExceptionList __system_exceptions;

// Send request, block until any reply comes back, and unmarshal reply
// parameters as appropriate.

GIOP::ReplyStatusType
GIOP::Invocation::invoke (CORBA_ExceptionList &exceptions,
			  CORBA_Environment &env)
{
  // Send Request, return on error or if we're done

  if (!GIOP::send_message (stream, handler_->peer ())) 
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
      env.exception (new CORBA_COMM_FAILURE (COMPLETED_MAYBE));
      return SYSTEM_EXCEPTION;
    }
  if (!do_rsvp)
    return NO_EXCEPTION;

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
  switch (GIOP::read_message (handler_->peer (), stream, env)) 
    {
    case Reply:
      // handle reply ... must be right one etc
      break;

    case CloseConnection:
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
	ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, SYSTEM_EXCEPTION));

	delete data_->fwd_profile;
	data_->fwd_profile = 0;

        handler_->peer ().close ();
        handler_->in_use (CORBA_B_FALSE);
        handler_ = 0;
	return LOCATION_FORWARD;
      }

    case Request:
    case CancelRequest:
    case LocateRequest:
    case LocateReply:
    default:
      // These are all illegal messages to find.  If found, they could
      // be indicative of client bugs (lost track of input stream) or
      // server bugs; maybe the request was acted on, maybe not, we
      // can't tell.
      ACE_DEBUG ((LM_DEBUG, " (%P|%t) illegal message in response to my Request!\n"));
      env.exception (new CORBA_COMM_FAILURE (COMPLETED_MAYBE));
      // FALLTHROUGH ...

    case MessageError:
      // Couldn't read it for some reason ... exception's set already,
      // so just tell the other end about the trouble (closing the
      // connection) and return.
      send_error (handler_->peer ());
      return SYSTEM_EXCEPTION;
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

  ServiceContextList reply_ctx;
  CORBA_ULong request_id;
  CORBA_ULong reply_status;		// GIOP::ReplyStatusType

  if (stream.decode (&TC_ServiceContextList, &reply_ctx, 0, env)
      != CORBA_TypeCode::TRAVERSE_CONTINUE) 
    {
      send_error (handler_->peer ());
      return SYSTEM_EXCEPTION;
    }

  delete [] reply_ctx.buffer;
    
  if (!stream.get_ulong (request_id)
      || request_id != my_request_id
      || !stream.get_ulong (reply_status)
      || reply_status > LOCATION_FORWARD) 
    {
      send_error (handler_->peer ());
      env.exception (new CORBA_COMM_FAILURE (COMPLETED_MAYBE));
      ACE_DEBUG ((LM_DEBUG, " (%P|%t) bad Response header\n"));
      return SYSTEM_EXCEPTION;
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
    case NO_EXCEPTION:
      break;

    case USER_EXCEPTION:
    case SYSTEM_EXCEPTION:
      {
	CORBA_String exception_id;

	// Pull the exception ID out of the marshaling buffer.
	{
	  CORBA_ULong len;

	  //
	  // Read "length" field of string, so "next" points
	  // right at the null-terminated ID.  Then get the ID.
	  //
	  if (stream.get_ulong (len) != CORBA_B_TRUE
	      || len > stream.remaining) 
	    {
	      send_error (handler_->peer ());
	      env.exception (new CORBA_MARSHAL (COMPLETED_YES));
	      return SYSTEM_EXCEPTION;
	    }
	  exception_id = (CORBA_String) stream.next;
	  stream.skip_bytes (len);
	}

	// User and system exceptions differ only in what table of
	// exception typecodes is searched.
	CORBA_ExceptionList *xlist;

	if (reply_status == USER_EXCEPTION)
	  xlist = &exceptions;
	else
	  xlist = &__system_exceptions;

	// Find it in the operation description and then use that to get
	// the typecode.  Use it to unmarshal the exception's value; if
	// that exception is not allowed by this operation, fail (next). 

	u_int i;
	CORBA_TypeCode_ptr *tcp;

	for (i = 0, tcp = xlist->buffer;
	     i < xlist->length;
	     i++, tcp++) 
	  {
	    CORBA_String xid;

	    xid = (*tcp)->id (env);
	    if (env.exception () != 0) 
	      {
		dexc (env, "invoke (), get exception ID");
		send_error (handler_->peer ());
		return SYSTEM_EXCEPTION;
	      }

	    if (ACE_OS::strcmp ((char *)exception_id, (char *)xid) == 0) 
	      {
		size_t			size;
		CORBA_Exception		*exception;

		size = (*tcp)->size (env);
		if (env.exception () != 0) 
		  {
		    dexc (env, "invoke (), get exception size");
		    send_error (handler_->peer ());
		    return SYSTEM_EXCEPTION;
		  }

		// Create the exception, fill in the generic parts
		// such as vtable, typecode ptr, refcount ... we need
		// to clean them all up together, in case of errors
		// unmarshaling.

		exception = new (new char [size]) CORBA_Exception (*tcp);

		if (stream.decode (*tcp, exception, 0, env)
		    != CORBA_TypeCode::TRAVERSE_CONTINUE) 
		  {
		    delete exception;
		    ACE_DEBUG ((LM_ERROR, " (%P|%t) invoke, unmarshal %s exception %s\n",
				(reply_status == USER_EXCEPTION) ? "user" : "system",
                               exception_id));
		    send_error (handler_->peer ());
		    return SYSTEM_EXCEPTION;
		  }
		env.exception (exception);
		return (GIOP::ReplyStatusType) reply_status;
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

	if (reply_status == USER_EXCEPTION)
	  env.exception (new CORBA_OBJ_ADAPTER (COMPLETED_YES));
	else
	  env.exception (new CORBA_INTERNAL (COMPLETED_MAYBE));
	return SYSTEM_EXCEPTION;
      }
    // NOTREACHED

    case LOCATION_FORWARD:
      {
	CORBA_Object_ptr obj;
	IIOP_Object *obj2;

	// Unmarshal the object we _should_ be calling.  We know that
	// one of the facets of this object will be an IIOP invocation
	// profile.

	if (stream.decode (_tc_CORBA_Object, 
			  &obj, 0, 
			  env) != CORBA_TypeCode::TRAVERSE_CONTINUE
	    || obj->QueryInterface (IID_IIOP_Object, 
                                    (void **)&obj2) != NOERROR)
	  {
	    dexc (env, "invoke, location forward");
	    send_error (handler_->peer ());
	    return SYSTEM_EXCEPTION;
	  }
	CORBA_release (obj);

	// Make a copy of the IIOP profile in the forwarded objref,
	// reusing memory where practical.  Then delete the forwarded
	// objref, retaining only its profile.
	//
	// XXX add and use a "forward count", to prevent loss of data
	// in forwarding chains during concurrent calls -- only a
	// forward that's a response to the current fwd_profile should
	// be recorded here. (This is just an optimization, and is not
	// related to correctness.)

	ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, SYSTEM_EXCEPTION);

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
  // system exceptions which say otherwise, and for LOCATION_FORWARD
  // responses.

  return (GIOP::ReplyStatusType) reply_status;
}

// Generic server side read + dispatch one message; returns when that
// bit of work is complete.
//
// In the typical case, the request and response buffers live on the
// stack so that the heap never gets used.  These grow if needed.

int GIOP::incoming_message (ACE_SOCK_Stream &peer,
			    ForwardFunc check_forward,
			    RequestHandler handle_request,
			    void *context,
			    CORBA_Environment &env)
  /*
int
GIOP::incoming_message (ACE_SOCK_Stream &peer,
			LocateStatusType check_forward (opaque &key,
							CORBA_Object_ptr &objref,
							void *context),
			void handle_request (RequestHeader &req,
					     CDR &req_body,
					     CDR *reply,
					     void *context,
					     CORBA_Environment &env),
			void *context,
			CORBA_Environment &env)
*/
{
  int retval = 1;		// 1==success, 0==eof, -1==error

  u_char buffer [CDR::DEFAULT_BUFSIZE];
  CDR msg (&buffer [0], sizeof buffer);

  switch (read_message (peer, msg, env))
    {
      // These messages should never be sent to the server; it's an
      // error if the peer tries.  Set the environment accordingly, as
      // it's not yet been reported as an error.
    case Reply:
    case LocateReply:
    case CloseConnection:
    default:					// Unknown message
      ACE_DEBUG ((LM_DEBUG, " (%P|%t) Illegal message received by server\n"));
      env.exception (new CORBA_COMM_FAILURE (COMPLETED_NO));
      // FALLTHROUGH

      // read_message () has already set some error in the environment
      // for all "MessageError" cases, so don't clobber it.
      //
      // General error recovery is to send MessageError to the peer
      // just in case (it'll fail on EOF) and then close the
      // connection.
    case MessageError:
      retval = -1;
      send_error (peer);
      break;

    case EndOfFile:
      retval = 0;
      break;

      // This is the common case!
    case Request:
      {
	RequestHeader req;
	CORBA_Boolean hdr_status;

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
	hdr_status = hdr_status && msg.decode (_tc_CORBA_String,
					       &req.operation,
					       0,
					       env);
	hdr_status = hdr_status && msg.decode (_tc_CORBA_Principal,
					       &req.requesting_principal,
					       0,
					       env);

	// XXX check whether hdr_status identifies a header
	// unmarshaling error, and handle appropriately

#ifdef	DEBUG
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
	    LocateStatusType status;
	    CORBA_Object_ptr fwd_ref = 0;

	    status = check_forward (req.object_key, fwd_ref, context);

	    if (status != OBJECT_HERE) 
	      {
		ServiceContextList resp_ctx;
		u_char buf2 [CDR::DEFAULT_BUFSIZE];
		CDR response (&buf2 [0], sizeof buf2);

		start_message (Reply, response);
		resp_ctx.length = 0;
		response.encode (&TC_ServiceContextList,
			      &resp_ctx,
			      0,
			      env);
		response.put_ulong (req.request_id);

		// If we're not sending a response, just clean up.

		if (!req.response_expected) 
		  {
		    if (status == OBJECT_FORWARD)
		      CORBA_release (fwd_ref);

		    //
		    // Else either forward the request ...
		    //
		  } 
		else if (status == OBJECT_FORWARD) 
		  {
		    ACE_DEBUG ((LM_DEBUG, " (%P|%t) forwarding Request message\n"));
		    response.put_ulong (LOCATION_FORWARD);
		    response.encode (_tc_CORBA_Object,
				  &fwd_ref,
				  0, 
				  env);
		    CORBA_release (fwd_ref);
 (void) send_message (response, peer);

		    // ... or report exception that the object doesn't exist.
		  } 
		else 
		  {
		    CORBA_OBJECT_NOT_EXIST exc (COMPLETED_YES);

		    response.put_ulong (SYSTEM_EXCEPTION);

 (void) response.encode (_tc_CORBA_OBJECT_NOT_EXIST,
					 &exc,
					 0,
					 env);

 (void) send_message (response, peer);
		  }

		delete [] req.object_key.buffer;
		CORBA_string_free (req.operation);
		return retval;
	      }
	  }

	// So, we read a request, now dispatch it using something more
	// primitive than a CORBA2 ServerRequest pseudo-object.

	if (req.response_expected) 
	  {
	    ServiceContextList resp_ctx;
	    u_char buf2 [CDR::DEFAULT_BUFSIZE];
	    CDR response (&buf2 [0], sizeof buf2);

	    start_message (Reply, response);
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
	CORBA_string_free (req.operation);
      }
    break;

    // Forward requests as needed; if caller hasn't provided code to
    // support forwarding, we default to doing no forwarding.
    case LocateRequest:
      {
	CORBA_ULong request_id;
	opaque key;

	msg.get_ulong (request_id);
	msg.decode (&TC_opaque, &key, 0, env);

	// we've read the request header; send a LocateReply

	u_char resp [CDR::DEFAULT_BUFSIZE];
	CDR response (resp, sizeof resp);
	CORBA_Object_ptr fwd_ref = 0;

	start_message (LocateReply, response);
	response.put_ulong (request_id);

	if (check_forward == 0) 
	  {
	    response.put_ulong (OBJECT_HERE);
	    ACE_DEBUG ((LM_DEBUG, " (%P|%t) LocateRequest response:  object is (always) here!\n"));
	  } 
	else 
	  {
	    LocateStatusType status;

	    status = check_forward (key, fwd_ref, context);
	    response.put_ulong ((CORBA_ULong) status);

	    if (status == OBJECT_FORWARD) 
	      {
		ACE_DEBUG ((LM_DEBUG, "LocateRequest response:  forward requests\n"));
		response.encode (_tc_CORBA_Object, &fwd_ref, 0, env);
	      } 
	    else if (status == OBJECT_HERE)
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

    case CancelRequest:
      {
	CORBA_ULong request_id;

	msg.get_ulong (request_id);
      }
    break;
    }

  // ... error if unconsumed data remains; is this the spot to test
  // that?
  return retval;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class CORBA_SEQUENCE<GIOP::ServiceContext>;
template class CORBA_SEQUENCE<CORBA_Octet>;
template class CORBA_SEQUENCE<CORBA_TypeCode*>;
#endif
