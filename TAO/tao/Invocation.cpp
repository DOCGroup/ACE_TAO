// $Id$

#include "tao/Invocation.h"
#include "tao/Stub.h"
#include "tao/Principal.h"

#include "tao/Timeprobe.h"
#include "tao/Object_KeyC.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Invocation.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Invocation, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Invocation_Timeprobe_Description[] =
  {
    "GIOP_Invocation::invoke - start",
    "GIOP_Invocation::invoke - end",

    "GIOP_Invocation::start - enter",
    "GIOP_Invocation::start - leave",
    "GIOP_Invocation::start - connect",
    "GIOP_Invocation::start - start_msg",
    "GIOP_Invocation::start - request_hdr"
  };

enum
  {
    TAO_GIOP_INVOCATION_INVOKE_START = 1000,
    TAO_GIOP_INVOCATION_INVOKE_END,

    TAO_GIOP_INVOCATION_START_ENTER,
    TAO_GIOP_INVOCATION_START_LEAVE,
    TAO_GIOP_INVOCATION_START_CONNECT,
    TAO_GIOP_INVOCATION_START_START_MSG,
    TAO_GIOP_INVOCATION_START_REQUEST_HDR
  };


// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Invocation_Timeprobe_Description,
                                  TAO_GIOP_INVOCATION_INVOKE_START);

#endif /* ACE_ENABLE_TIMEPROBES */

// Normal invocations don't involve any heap al; messages are
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

TAO_GIOP_Invocation::TAO_GIOP_Invocation (TAO_Stub *stub,
                                          const char *operation,
                                          TAO_ORB_Core* orb_core)
  : stub_ (stub),
    opname_ (operation),
    my_request_id_ (0),
    out_stream_ (buffer, sizeof buffer, /* ACE_CDR::DEFAULT_BUFSIZE */
                 TAO_ENCAP_BYTE_ORDER,
                 orb_core->output_cdr_buffer_allocator (),
                 orb_core->output_cdr_dblock_allocator ()),
    orb_core_ (orb_core),
    transport_ (0)
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
  if (this->transport_ != 0)
    this->transport_->idle ();
}

// The public API involves creating an invocation, starting it, filling
// in request parameters, actually performing the invocation, getting
// response parameters, and then cleaning up.  Sometimes they must be
// restarted (e.g. request forwarding).  This is the start/restart entry.

void
TAO_GIOP_Invocation::start (CORBA::Boolean is_roundtrip,
                            TAO_GIOP::Message_Type message_type,
                            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_FUNCTION_TIMEPROBE (TAO_GIOP_INVOCATION_START_ENTER);
  TAO_MINIMAL_TIMEPROBE (TAO_GIOP_INVOCATION_START_ENTER);

  // First try to bind to the appropriate address.  We do that here
  // since we may get forwarded to a different objref in the course of
  // any given call, with new start () call each time.  It's not
  // cached in the objref data since the connections change
  // asynchronously from objref invocations and this simplifies
  // connection management.
  // We also need to bind *before* marshalling, because different
  // Profiles have different ObjectKeys, thus a change of Profile can
  // result in different alignment for the buffer.
  //
  // THREADING NOTE: this connection is reserved to this call.  Also,
  // starting at this point in the call, new forwarding information
  // will not be used until/unless the call is reissued.  Correctness
  // is not affected, the call will just be forwarded later than it
  // might be in a more complex implementation.

  // @@ assert is evil, it crashes the program, changed to an
  // exception (coryan)
  // assert (this->stub_ != 0);

  if (this->stub_ == 0)
    ACE_THROW (CORBA::INV_OBJREF (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));

  // Get a pointer to the connector registry, which might be in
  // thread-specific storage, depending on the concurrency model.
  TAO_Connector_Registry *conn_reg = this->orb_core_->connector_registry ();

  if (conn_reg == 0)
    ACE_THROW (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));

  // @@ It seems like this is the right spot to re-order the profiles
  // based on the policies in the ORB.
  // The following text was here:
  //     The connection registry is also responsible for selecting the
  //     profile to use based on some policy or the current forwarding
  //     state.
  // IMHO this is not right, the connector registry only finds one
  // connector for the given policies, if the connector is not
  // available (say the user wants an ATM connection, but we don't
  // have the protocol) then we give it another profile to try.

  // Loop until a connection is established or there aren't any more
  // profiles to try.
 for (;;)
    {
      // Get the current profile...
      this->profile_ = this->stub_->profile_in_use ();

      if (this->transport_ != 0)
        this->transport_->idle ();
      int result = conn_reg->connect (this->stub_, this->transport_);
      if (result == 0)
        break;

      // Try moving to the next profile and starting over, if that
      // fails then we must raise the TRANSIENT exception.
      if (this->stub_->next_profile_retry () == 0)
        ACE_THROW (CORBA::TRANSIENT (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));
    }

  const TAO_ObjectKey& key = this->profile_->object_key();

  ACE_TIMEPROBE (TAO_GIOP_INVOCATION_START_CONNECT);

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

  if (TAO_GIOP::start_message (message_type,
                               this->out_stream_,
                               this->orb_core_) == 0)
    ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));

  ACE_TIMEPROBE (TAO_GIOP_INVOCATION_START_START_MSG);

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
  static CORBA::Principal_ptr principal = 0;

  // This static is only used to write into the CDR stream, once we
  // have real service context (needed for the messaging spec) this
  // will have to be a parameter.
  static TAO_GIOP_ServiceContextList svc_ctx;

  switch (message_type)
    {
    case TAO_GIOP::Request:

      this->write_request_header (svc_ctx,
                                  this->my_request_id_,
                                  is_roundtrip,
                                  key,
                                  this->opname_,
                                  principal);
      break;

    case TAO_GIOP::LocateRequest:
      this->out_stream_ << this->my_request_id_;
      this->out_stream_ << key;
      break;

    default:
      ACE_THROW (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));
    }

  if (!this->out_stream_.good_bit ())
    ACE_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO));

  ACE_TIMEPROBE (TAO_GIOP_INVOCATION_START_REQUEST_HDR);
}

CORBA::Boolean
TAO_GIOP_Invocation::write_request_header_std
     (const TAO_GIOP_ServiceContextList& svc_ctx,
      CORBA::ULong request_id,
      CORBA::Boolean is_roundtrip,
      const TAO_opaque& key,
      const char* opname,
      CORBA::Principal_ptr principal)
{
  this->out_stream_ << svc_ctx;
  this->out_stream_ << request_id;
  this->out_stream_ << CORBA::Any::from_boolean (is_roundtrip);
  this->out_stream_ << key;
  this->out_stream_ << opname;
  this->out_stream_ << principal;
  return 1;
}

CORBA::Boolean
TAO_GIOP_Invocation::write_request_header_lite
     (const TAO_GIOP_ServiceContextList&,
      CORBA::ULong request_id,
      CORBA::Boolean is_roundtrip,
      const TAO_opaque& key,
      const char* opname,
      CORBA::Principal_ptr)
{
  this->out_stream_ << request_id;
  this->out_stream_ << CORBA::Any::from_boolean (is_roundtrip);
  this->out_stream_ << key;
  this->out_stream_ << opname;
  return 1;
}

CORBA::Boolean
TAO_GIOP_Invocation::write_request_header
     (const TAO_GIOP_ServiceContextList& svc_ctx,
      CORBA::ULong request_id,
      CORBA::Boolean is_roundtrip,
      const TAO_opaque& key,
      const char* opname,
      CORBA::Principal_ptr principal)
{
  if (this->orb_core_->orb_params ()->use_lite_protocol ())
    return this->write_request_header_lite (svc_ctx,
                                            request_id,
                                            is_roundtrip,
                                            key,
                                            opname,
                                            principal);
  else
    return this->write_request_header_std (svc_ctx,
                                           request_id,
                                           is_roundtrip,
                                           key,
                                           opname,
                                           principal);
}


// Send request, block until any reply comes back, and unmarshal reply
// parameters as appropriate.

int
TAO_GIOP_Invocation::invoke (CORBA::Boolean is_roundtrip,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{

  if (this->transport_ == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO),
                      TAO_INVOKE_EXCEPTION);

  int result =
    this->transport_->send_request (this->orb_core_,
                                    this->out_stream_,
                                    is_roundtrip);

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

  if (result == -1)
    {
      // send_request () closed the connection, we just have to forget
      // about the hint.
      this->profile_->reset_hint ();

      return TAO_INVOKE_RESTART;
    }

  // @@ Maybe the right place to do this is once the reply is
  //    received? But what about oneways?
  this->stub_->set_valid_profile ();

  return TAO_INVOKE_OK;
}

// ****************************************************************

int
TAO_GIOP_Invocation::close_connection (void)
{
  // Special case of forwarding -- server was closing the
  // connection, which just indicates resource constraints, not an
  // error.  The client is effectively "forwarded" to the same
  // server!
  //
  // However, we must reinitialize the forwarding chain, since the
  // resource being reclaimed might also have been the process,
  // not just the connection.  Without reinitializing, we'd give
  // false error reports to applications.

  this->transport_->close_connection ();
  this->transport_->idle ();
  this->transport_ = 0;

  this->profile_->reset_hint ();
  this->profile_ = 0;

  // @@ Get rid of any forwarding profiles and reset
  //    the profile list to point to the first profile!
  // FRED For now we will not deal with recursive forwards!

  this->stub_->reset_profiles ();
  // sets the forwarding profile to 0 and deletes the old one;
  // rewinds the profiles list back to the first one.

  return TAO_INVOKE_RESTART;
}

// Handle the GIOP Reply with status = LOCATION_FORWARD
// Replace the IIOP Profile. The call is then automatically
// reinvoked by the TAO_Stub::do_static_call method.

int
TAO_GIOP_Invocation::location_forward (TAO_InputCDR &inp_stream,
                                       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // It can be assumed that the GIOP header and the reply header
  // are already handled. Further it can be assumed that the
  // reply body contains an object reference to the new object.
  // This object pointer will be now extracted.

  CORBA::Object_var object = 0;

  if ( (inp_stream >> object.inout ()) == 0)
    {
      // @@ Why whould we want to close this connection?
      //    this->transport_->close_connection ();

      // @@ If a forward exception or a LOCATION_FORWARD reply is sent
      //    then the request couldn't have completed. But we need to
      //    re-validate this to ensure "at most once" semantics.
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO),
                        TAO_INVOKE_EXCEPTION);
    }

  // The object pointer has to be changed to a TAO_Stub pointer
  // in order to extract the profile.

  TAO_Stub *stubobj = object->_stubobj ();

  if (stubobj == 0)
    {
      // @@ Why whould we want to close this connection?
      //    this->transport_->close_connection ();

      // @@ If a forward exception or a LOCATION_FORWARD reply is sent
      //    then the request couldn't have completed. But we need to
      //    re-validate this to ensure "at most once" semantics.
      ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO),
                        TAO_INVOKE_EXCEPTION);
    }

  // Make a copy of the IIOP profile in the forwarded objref,
  // reusing memory where practical.  Then delete the forwarded
  // objref, retaining only its profile list (mprofiles).
  //
  // @@ add and use a "forward count", to prevent loss of data
  // in forwarding chains during concurrent calls -- only a
  // forward that's a response to the current forward_profile should
  // be recorded here. (This is just an optimization, and is not
  // related to correctness.)

  // New for Multiple profile.  Get the MProfile list from the
  // forwarded object refererence

  this->stub_->add_forward_profiles (stubobj->get_profiles ());
  // store the new profile list and set the first forwarding profile
  // note: this has to be and is thread safe.  Also get_profiles returns
  // a pointer to a new MProfile object which we give to our
  // TAO_Stub.

  // @@ Why do we clear the environment?
  // ACE_TRY_ENV.clear ();

  // We may not need to do this since TAO_GIOP_Invocations
  // get created on a per-call basis. For now we'll play it safe.

  if (this->stub_->next_profile () == 0)
    ACE_THROW_RETURN (CORBA::TRANSIENT (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO),
                      TAO_INVOKE_EXCEPTION);

  return TAO_INVOKE_RESTART;
}

// ****************************************************************

int
TAO_GIOP_Twoway_Invocation::invoke (CORBA::ExceptionList &exceptions,
                                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,CORBA::UnknownUserException))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_INVOCATION_INVOKE_START);

  int retval = this->invoke_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  // A TAO_INVOKE_EXCEPTION status, but no exception raised means that
  // we have a user exception.
  // @@ This is a bit brittle, think about a better implementation.
  if (retval == TAO_INVOKE_EXCEPTION)
    {
      // Match the exception interface repository id with the
      // exception in the exception list.
      // This is important to decode the exception.

      CORBA::String_var buf;

      // Pull the exception ID out of the marshaling buffer.
      if (this->inp_stream_.read_string (buf.inout ()) == 0)
        {
          // @@ Why do we close the connection. Only the request
          //    failed, but the connection seems to be still
          //    valid!
          // this->transport_->close_connection ();
          ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                            TAO_INVOKE_EXCEPTION);
        }

      for (CORBA::ULong i = 0;
           i < exceptions.count ();
           i++)
        {
          CORBA::TypeCode_ptr tcp = exceptions.item (i, ACE_TRY_ENV);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          const char *xid = tcp->id (ACE_TRY_ENV);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          if (ACE_OS::strcmp (buf.in (), xid) != 0)
            continue;

          // @@ In the old days the exceptions where catched and the
          // connection was closed, that doesn't make any sense:
          // this is a client side problem, for one particular
          // request.
          // this->transport_->close_connection ();
          // ACE_RETHROW;

          const ACE_Message_Block* cdr =
            this->inp_stream_.start ();
          CORBA_Any any (tcp, cdr);
          CORBA_Exception *exception;
          ACE_NEW_THROW_EX (exception,
                            CORBA_UnknownUserException (any),
                            CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          // @@ Think about a better way to raise the exception here,
          // maybe we need some more macros?
          ACE_TRY_ENV.exception (exception);
          return TAO_INVOKE_EXCEPTION;
        }

      // If we couldn't find the right exception, report it as
      // CORBA::UNKNOWN.

      // @@ It would seem like if the remote exception is a
      //    UserException we can assume that the request was
      //    completed.
      ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                        TAO_INVOKE_EXCEPTION);
    }

  return retval;
}

// Send request, block until any reply comes back, and unmarshal reply
// parameters as appropriate.
//
// This is used by the generated stubs.

int
TAO_GIOP_Twoway_Invocation::invoke (TAO_Exception_Data *excepts,
                                    CORBA::ULong except_count,
                                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::Exception))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_INVOCATION_INVOKE_START);

  int retval = this->invoke_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  // A TAO_INVOKE_EXCEPTION status, but no exception raised means that
  // we have a user exception.
  // @@ This is a bit brittle, think about a better implementation.
  if (retval == TAO_INVOKE_EXCEPTION)
    {
      // Match the exception interface repository id with the
      // exception in the exception list.
      // This is important to decode the exception.

      CORBA::String_var buf;

      // Pull the exception ID out of the marshaling buffer.
      if (this->inp_stream_.read_string (buf.inout ()) == 0)
        {
          // @@ Why do we close the connection. Only the request
          //    failed, but the connection seems to be still
          //    valid!
          // this->transport_->close_connection ();
          ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                            TAO_INVOKE_EXCEPTION);
        }

      for (CORBA::ULong i = 0;
           i < except_count;
           i++)
        {
          CORBA::TypeCode_ptr tcp = excepts[i].tc;
          const char *xid = tcp->id (ACE_TRY_ENV);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          if (ACE_OS::strcmp (buf.in (), xid) != 0)
            continue;

          // match
          CORBA::Exception_ptr exception = excepts[i].alloc ();

          if (exception == 0)
            ACE_THROW_RETURN (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                              TAO_INVOKE_EXCEPTION);

          this->inp_stream_.decode (exception->_type (),
                                    exception, 0,
                                    ACE_TRY_ENV);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          if (TAO_debug_level > 5)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO: (%P|%t) Raising exception %s\n",
                        buf.in ()));

          // @@ Think about a better way to raise the exception here,
          // maybe we need some more macros?
          ACE_TRY_ENV.exception (exception);
          return TAO_INVOKE_EXCEPTION;
        }

      // If we couldn't find the right exception, report it as
      // CORBA::UNKNOWN.

      ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                        TAO_INVOKE_EXCEPTION);
    }

  return retval;
}

int
TAO_GIOP_Twoway_Invocation::invoke_i (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  int retval = TAO_GIOP_Invocation::invoke (1, ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);
  ACE_UNUSED_ARG (retval);

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

  // @@ Fred: if it makes sense to have a wrapper for send_request on
  //    the TAO_Transport class then it should also make sense to have
  //    one for recv_request(), right?
  TAO_GIOP::Message_Type m = TAO_GIOP::recv_request (this->transport_,
                                                     this->inp_stream_,
                                                     this->orb_core_);

  // suspend was called in TAO_Client_Connection_Handler::handle_input
  this->transport_->resume_connection (this->orb_core_->reactor ());

  switch (m)
    {
    case TAO_GIOP::Reply:
      // The reply is handled at the end of this switch() statement.
      break;

    case TAO_GIOP::CloseConnection:
      // Try the same profile again, but open a new connection.
      // If that fails then we go to the next profile.
      this->profile_->reset_hint ();
      return TAO_INVOKE_RESTART;

    case TAO_GIOP::Request:
    case TAO_GIOP::CancelRequest:
    case TAO_GIOP::LocateRequest:
    case TAO_GIOP::LocateReply:
    default:
      // These are all illegal messages to find.  If found, they could
      // be indicative of client bugs (lost track of input stream) or
      // server bugs; maybe the request was acted on, maybe not, we
      // can't tell.
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO: (%P|%t) illegal GIOP message (%s) "
                    "in response to my Request!\n",
                    TAO_GIOP::message_name (m)));
      // FALLTHROUGH

    case TAO_GIOP::MessageError:
      // @@ Maybe the transport should be closed by recv_request?
      // FALLTHROUGH

    case TAO_GIOP::CommunicationError:
      // Couldn't read it for some reason... exception's set already,
      // so just tell the other end about the trouble (closing the
      // connection) and return.
      // FALLTHROUGH

    case TAO_GIOP::EndOfFile:
      // In all those cases the message was (apparently) sent, but we
      // couldn't read the reply. To satisfy the "at most once"
      // semantics of CORBA we must raise an exception at this point
      // and *not* try to transparently restart the request.

      // We must also reset the state of this object, because the next
      // invocation may perfectly work.
      this->close_connection ();

      ACE_THROW_RETURN (CORBA::COMM_FAILURE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        TAO_INVOKE_EXCEPTION);
    }

  // Note: we only get here if the status was TAO_GIOP::Reply...

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


  // @@ Here is a big difference between GIOP 1.[01] and GIOP 1.2, the
  // order of the reply_ctx and the request_id fields!

  TAO_GIOP_ServiceContextList reply_ctx;
  CORBA::ULong request_id;
  CORBA::ULong reply_status; // TAO_GIOP_ReplyStatusType

  this->inp_stream_ >> reply_ctx;
  if (!this->inp_stream_.good_bit ())
    {
      // @@ Fred: Do we really want to close the connection here? This
      //    is a problem, but we haven't lost synchronization with the
      //    server or anything.
      this->transport_->close_connection ();
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        TAO_INVOKE_EXCEPTION);
    }

  if (!this->inp_stream_.read_ulong (request_id)
      || request_id != this->my_request_id_
      || !this->inp_stream_.read_ulong (reply_status)
      || reply_status > TAO_GIOP_LOCATION_FORWARD)
    {
      // @@ Fred: Do we really want to close the connection here? This
      //    is a problem, but we haven't lost synchronization with the
      //    server or anything.
      this->transport_->close_connection ();
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "TAO: (%P|%t) bad Response header\n"));
      ACE_THROW_RETURN (CORBA::COMM_FAILURE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        TAO_INVOKE_EXCEPTION);
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


  // @@ We must validate the reply_status variable, if it is not one
  // of the values listed in this switch statement we should:
  //  1) The and MessageError message back to the server
  //  2) Raise a COMM_FAILURE exception

  switch (reply_status)
    {
    case TAO_GIOP_NO_EXCEPTION:
      return TAO_INVOKE_OK;
      // NOTREACHED

    case TAO_GIOP_USER_EXCEPTION:
      return TAO_INVOKE_EXCEPTION;
      // NOTREACHED

    case TAO_GIOP_SYSTEM_EXCEPTION:
      {
        // @@ TODO This code is not exception safe. Notice how on
        // every exit path we have to invoke TAO_GIOP::send_error,
        // this should be handled by the destructor of some class;
        // which is disabled on the normal exit paths.
        // Plus <buf> should be stored in a CORBA::String_var

        CORBA::String_var buf;

        // Pull the exception ID out of the marshaling buffer.
        if (this->inp_stream_.read_string (buf.inout ()) == 0)
          {
            // @@ Why do we close the connection. Only the request
            //    failed, but the connection seems to be still
            //    valid!
            this->transport_->close_connection ();
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_EXCEPTION);
          }

        CORBA_Exception *exception =
          TAO_Exceptions::create_system_exception (buf.in (),
                                                   ACE_TRY_ENV);
        ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

        if (exception == 0)
          {
            if (TAO_debug_level > 0)
              ACE_ERROR ((LM_ERROR,
                          "TAO: (%P|%t) Received Reply "
                          "with SYSTEM_EXCEPTION status."
                          "But unknown or invalid exception.\n"));
            ACE_NEW_RETURN (exception,
                            CORBA::UNKNOWN,
                            TAO_INVOKE_EXCEPTION);
          }
        // @@ We can clearly use compiled marshaling in this
        //    case. All system exceptions are guaranteed to
        //    have the same fields (according to the spec).
        this->inp_stream_.decode (exception->_type (),
                                  &exception, 0,
                                  ACE_TRY_ENV);
        ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

        // @@ Think about a better way to raise the exception here,
        // maybe we need some more macros?
        ACE_TRY_ENV.exception (exception);
        return TAO_INVOKE_EXCEPTION;
      }
      // NOTREACHED

    case TAO_GIOP_LOCATION_FORWARD:
      return this->location_forward (this->inp_stream_, ACE_TRY_ENV);
    }

  return TAO_INVOKE_EXCEPTION;
}

// ****************************************************************

// Send request, block until any reply comes back

int
TAO_GIOP_Locate_Request_Invocation::invoke (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ TODO This method is very similar to invoke_i(), we should try
  //    to refactor them.

  if (this->transport_ == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_NO),
                      TAO_INVOKE_EXCEPTION);

  int result =
    this->transport_->send_request (this->orb_core_,
                                    this->out_stream_,
                                    1);


  if (result == -1)
    {
      // send_request () closed the connection; we just set the
      // handler to 0 here.
      this->profile_->reset_hint ();

      // @@ This code abort if the connection for the currenct profile
      //    fails.  Should we transparently try new profiles until one
      //    works? Or is that something that a higher level component
      //    should decide?  Remember that LocateRequests are part of
      //    the strategy to establish a connection.
      ACE_THROW_RETURN (CORBA::TRANSIENT (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        TAO_INVOKE_EXCEPTION);
    }

  // @@ Maybe the right place to do this is once the reply is
  //    received? But what about oneways?
  this->stub_->set_valid_profile ();

  TAO_GIOP::Message_Type m = TAO_GIOP::recv_request (this->transport_,
                                                     this->inp_stream_,
                                                     this->orb_core_);

  this->transport_->resume_connection (this->orb_core_->reactor ());
  // suspend was called in TAO_Client_Connection_Handler::handle_input

  switch (m)
    {
    case TAO_GIOP::Reply:
      // Thereply is handled at the end of this switch() statement.
      break;

    case TAO_GIOP::CloseConnection:
      // Try the same profile again, but open a new connection.
      // If that fails then we go to the next profile.
      this->profile_->reset_hint ();
      return TAO_INVOKE_RESTART;

    case TAO_GIOP::Request:
    case TAO_GIOP::CancelRequest:
    case TAO_GIOP::LocateRequest:
    case TAO_GIOP::LocateReply:
    default:
      // These are all illegal messages to find.  If found, they could
      // be indicative of client bugs (lost track of input stream) or
      // server bugs; maybe the request was acted on, maybe not, we
      // can't tell.
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO: (%P|%t) illegal GIOP message (%s) "
                    "in response to my LocateRequest!\n",
                    TAO_GIOP::message_name (m)));
      // FALLTHROUGH ...

    case TAO_GIOP::MessageError:
      // @@ Maybe the transport should be closed by recv_request?
      // FALLTHROUGH

    case TAO_GIOP::CommunicationError:
      // Couldn't read it for some reason ... exception's set already,
      // so just tell the other end about the trouble (closing the
      // connection) and return.
      // FALLTHROUGH

    case TAO_GIOP::EndOfFile:
      // In all those cases the message was (apparently) sent, but we
      // couldn't read the reply. To satisfy the "at most once"
      // semantics of CORBA we must raise an exception at this point
      // and *not* try to transparently restart the request.
      // FALLTHROUGH

      this->close_connection ();

      ACE_THROW_RETURN (CORBA::COMM_FAILURE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        TAO_INVOKE_EXCEPTION);
    }

  // Note: we only get here if the status was TAO_GIOP::LocateReply

  CORBA::ULong request_id;
  CORBA::ULong locate_status; // TAO_GIOP_LocateStatusType

  if (!this->inp_stream_.read_ulong (request_id)
      || request_id != this->my_request_id_
      || !this->inp_stream_.read_ulong (locate_status))
    {
      // @@ Fred: Do we really want to close the connection here? This
      //    is a problem, but we haven't lost synchronization with the
      //    server or anything.
      this->transport_->close_connection ();
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "TAO: (%P|%t) bad Response header\n"));
      ACE_THROW_RETURN (CORBA::COMM_FAILURE (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_MAYBE),
                        TAO_INVOKE_EXCEPTION);
    }

  switch (locate_status)
    {
    case TAO_GIOP_OBJECT_HERE:
      break;

    case TAO_GIOP_UNKNOWN_OBJECT:
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                        TAO_INVOKE_EXCEPTION);
      // NOTREACHED

    case TAO_GIOP_OBJECT_FORWARD:
      return this->location_forward (this->inp_stream_, ACE_TRY_ENV);
      // NOTREACHED
    }

  return TAO_INVOKE_OK;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
