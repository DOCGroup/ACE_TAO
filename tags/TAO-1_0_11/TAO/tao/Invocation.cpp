// $Id$

#include "tao/Invocation.h"
#include "tao/Stub.h"
#include "tao/Principal.h"

#include "tao/Timeprobe.h"
#include "tao/Object_KeyC.h"
#include "tao/debug.h"
#include "tao/Pluggable.h"
#include "tao/Connector_Registry.h"
#include "tao/Wait_Strategy.h"
#include "tao/Transport_Mux_Strategy.h"

#include "tao/Messaging_Policy_i.h"
#include "tao/Client_Priority_Policy.h"

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
    "GIOP_Invocation::start - start_msg"
  };

enum
  {
    TAO_GIOP_INVOCATION_INVOKE_START = 1000,
    TAO_GIOP_INVOCATION_INVOKE_END,

    TAO_GIOP_INVOCATION_START_ENTER,
    TAO_GIOP_INVOCATION_START_LEAVE,
    TAO_GIOP_INVOCATION_START_CONNECT,
    TAO_GIOP_INVOCATION_START_START_MSG
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
    request_id_ (0),
    out_stream_ (buffer, sizeof buffer, /* ACE_CDR::DEFAULT_BUFSIZE */
                 TAO_ENCAP_BYTE_ORDER,
                 orb_core->output_cdr_buffer_allocator (),
                 orb_core->output_cdr_dblock_allocator (),
                 orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                 orb_core->to_iso8859 (),
                 orb_core->to_unicode ()),
    orb_core_ (orb_core),
    transport_ (0),
    max_wait_time_ (0)
{
}

TAO_GIOP_Invocation::~TAO_GIOP_Invocation (void)
{
}

TAO_Profile *
TAO_GIOP_Invocation::select_profile_based_on_policy
(CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
#if (TAO_HAS_CORBA_MESSAGING == 0)

  ACE_UNUSED_ARG (ACE_TRY_ENV);
  this->profile_ = this->stub_->profile_in_use ();
  return this->profile_;

#else

  TAO_Client_Priority_Policy *policy =
    this->stub_->client_priority ();

  if (policy == 0)
    // Policy is not set.
    {
      this->profile_ = this->stub_->profile_in_use ();
      return this->profile_;
    }
  else
    // Policy is set.
    {
      TAO::PrioritySpecification priority_spec =
        policy->priority_specification (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      TAO::PrioritySelectionMode mode = priority_spec.mode;

      // Don't care about priority.
      if (mode == TAO::USE_NO_PRIORITY)
        {
          this->profile_ = this->stub_->profile_in_use ();
          return this->profile_;
        }

      // Care about priority.

      // Determine  priority range used to select the profile.
      CORBA::Short min_priority;
      CORBA::Short max_priority;

      if (mode == TAO::USE_PRIORITY_RANGE)
        {
          min_priority = priority_spec.min_priority;
          max_priority = priority_spec.max_priority;
        }
      else
        // mode == TAO::USE_THREAD_PRIORITY
        {
          if (this->orb_core_->get_thread_priority (min_priority) == -1)
            ACE_THROW_RETURN (CORBA::DATA_CONVERSION (1,
                                                      CORBA::COMPLETED_NO),
                              0);
          max_priority = min_priority;
        }
      if (TAO_debug_level > 3)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) - matching priority range %d %d\n"),
                    min_priority,
                    max_priority));

      // Select a base profile whose priority is in the range.
      this->profile_ = 0;
      const TAO_MProfile& mprofile = this->stub_->base_profiles ();

      for (TAO_PHandle i = 0; i != mprofile.profile_count (); ++i)
        {
          const TAO_Profile *profile = mprofile.get_profile (i);
          const TAO_Tagged_Components &tc = profile->tagged_components ();
          CORBA::Short profile_priority;
          if (tc.get_tao_priority (profile_priority) == 0)
            continue;

          if (profile_priority >= min_priority
              && profile_priority <= max_priority)
            {
              this->profile_ =
                ACE_const_cast(TAO_Profile*,profile);
              break;
            }
        }

      if (this->profile_ == 0)
        ACE_THROW_RETURN (CORBA::INV_POLICY (),
                          0);
      else
        return this->profile_;
    }

#endif /* TAO_HAS_CORBA_MESSAGING == 0 */

}

// The public API involves creating an invocation, starting it, filling
// in request parameters, actually performing the invocation, getting
// response parameters, and then cleaning up.  Sometimes they must be
// restarted (e.g. request forwarding).  This is the start/restart entry.

void
TAO_GIOP_Invocation::start (CORBA::Environment &ACE_TRY_ENV)
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
    ACE_THROW (CORBA::INTERNAL ());

  // Get a pointer to the connector registry, which might be in
  // thread-specific storage, depending on the concurrency model.
  TAO_Connector_Registry *conn_reg =
    this->orb_core_->connector_registry ();

  if (conn_reg == 0)
    ACE_THROW (CORBA::INTERNAL ());

  // @@ It seems like this is the right spot to re-order the profiles
  // based on the policies in the ORB.
  // IMHO the connector registry only finds one
  // connector for the given policies, if the connector is not
  // available (say the user wants an ATM connection, but we don't
  // have the protocol) then we give it another profile to try.
  // So the invocation Object should handle policy decisions.

#if (TAO_HAS_CORBA_MESSAGING == 1)
  TAO_RelativeRoundtripTimeoutPolicy_i *timeout =
    this->stub_->relative_roundtrip_timeout ();

  // If max_wait_time is not zero then this is not the first attempt
  // to send the request, the timeout value includes *all* those
  // attempts.
  if (this->max_wait_time_ == 0
      && timeout != 0)
    {
      TimeBase::TimeT t =
        timeout->relative_expiry (ACE_TRY_ENV);
      ACE_CHECK;
      TimeBase::TimeT seconds = t / 10000000u;
      TimeBase::TimeT microseconds = (t % 10000000u) / 10;
      this->max_wait_time_value_.set (ACE_U64_TO_U32(seconds),
                                      ACE_U64_TO_U32(microseconds));
      this->max_wait_time_ = &this->max_wait_time_value_;

      if (TAO_debug_level > 0)
        {
          CORBA::ULong msecs =
            ACE_static_cast(CORBA::ULong, microseconds / 1000);
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("TAO (%P|%t) Timeout is <%u>\n"),
                      msecs));
        }
    }
#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  ACE_Countdown_Time countdown (this->max_wait_time_);
  // Loop until a connection is established or there aren't any more
  // profiles to try.
  for (;;)
    {
      // Select the profile for this invocation.
      this->select_profile_based_on_policy (ACE_TRY_ENV);
      ACE_CHECK;

      // Get the transport object.
      if (this->transport_ != 0)
        this->transport_->idle ();

      countdown.update ();
      int result = conn_reg->connect (this->profile_,
                                      this->transport_,
                                      this->max_wait_time_);
      countdown.update ();
      if (result == 0)
        break;

      if (errno == ETIME)
        {
          ACE_THROW (CORBA::TIMEOUT (
              CORBA_SystemException::_tao_minor_code (
                  TAO_TIMEOUT_CONNECT_MINOR_CODE,
                  errno),
              CORBA::COMPLETED_NO));
        }

      // Try moving to the next profile and starting over, if that
      // fails then we must raise the TRANSIENT exception.
      if (this->stub_->next_profile_retry () == 0)
        ACE_THROW (CORBA::TRANSIENT (
          CORBA_SystemException::_tao_minor_code (
            TAO_INVOCATION_CONNECT_MINOR_CODE,
            errno),
          CORBA::COMPLETED_NO));
    }

  // Obtain unique request id from the RMS.
  this->request_id_ = this->transport_->tms()->request_id ();

  countdown.update ();
}

void
TAO_GIOP_Invocation::prepare_header (CORBA::Octet response_flags,
                                     CORBA_Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
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

  if (TAO_GIOP::write_request_header (this->service_info_,
                                      this->request_id_,
                                      response_flags,
                                      this->profile_->object_key (),
                                      this->opname_,
                                      principal,
                                      this->out_stream_,
                                      this->orb_core_) == 0)
    ACE_THROW (CORBA::MARSHAL ());
}

// Send request.
int
TAO_GIOP_Invocation::invoke (CORBA::Boolean is_roundtrip,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Countdown_Time countdown (this->max_wait_time_);

  if (this->transport_ == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      TAO_INVOKE_EXCEPTION);

  // @@ Alex: the <is_roundtrip> flag will be tricky when we move to
  //    AMI: now it is used both to indicate the the CORBA request in
  //    a twoway and that the send_request() operation should block.
  //    Even for oneways: with AMI it is possible to wait for a
  //    response (empty) for oneways, just to make sure that they
  //    arrive, there are policies to control that.
  countdown.update ();

  int result =
    this->transport_->send_request (this->stub_,
                                    this->orb_core_,
                                    this->out_stream_,
                                    is_roundtrip,
                                    this->max_wait_time_);
  countdown.update ();

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
      if (errno == ETIME)
        {
          ACE_THROW_RETURN (
              CORBA::TIMEOUT (
                  CORBA_SystemException::_tao_minor_code (
                      TAO_TIMEOUT_SEND_MINOR_CODE,
                      errno
                    ),
                  CORBA::COMPLETED_NO
                ),
              TAO_INVOKE_EXCEPTION
            );
        }
      this->transport_->close_connection ();
      this->transport_ = 0;

      this->profile_->reset_hint ();

      return TAO_INVOKE_RESTART;
    }

  // @@ Maybe the right place to do this is once the reply is
  //    received? But what about oneways?
  // @@ Preserve semantics for the case when no ClientPriorityPolicy
  // is set/used.  For the case when it is, we don't want to set
  // anything because usage cases of various profiles based on
  // priorities aren't fully supported by MProfiles & friends (yet).
  if (this->stub_->profile_in_use () == this->profile_)
    this->stub_->set_valid_profile ();

  return TAO_INVOKE_OK;
}

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
      ACE_THROW_RETURN (CORBA::MARSHAL (),
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
      ACE_THROW_RETURN (CORBA::INTERNAL (),
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

  this->stub_->add_forward_profiles (*stubobj->make_profiles ());
  // store the new profile list and set the first forwarding profile
  // note: this has to be and is thread safe.  Also make_profiles() returns
  // a pointer to a new MProfile object which we give to our
  // TAO_Stub.

  // @@ Why do we clear the environment?
  // ACE_TRY_ENV.clear ();

  // We may not need to do this since TAO_GIOP_Invocations
  // get created on a per-call basis. For now we'll play it safe.

  if (this->stub_->next_profile () == 0)
    ACE_THROW_RETURN (
        CORBA::TRANSIENT (
            CORBA_SystemException::_tao_minor_code (
                TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
                errno
              ),
            CORBA::COMPLETED_NO
          ),
        TAO_INVOKE_EXCEPTION
      );

  return TAO_INVOKE_RESTART;
}

// ****************************************************************

TAO_GIOP_Twoway_Invocation::~TAO_GIOP_Twoway_Invocation (void)
{
  if (this->transport_ != 0)
    this->transport_->idle ();
}

void
TAO_GIOP_Twoway_Invocation::start (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_GIOP_Invocation::start (ACE_TRY_ENV);
  ACE_CHECK;

  this->transport_->start_request (this->orb_core_,
                                   this->profile_,
                                   this->out_stream_,
                                   ACE_TRY_ENV);
}

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
      if (this->inp_stream ().read_string (buf.inout ()) == 0)
        {
          // @@ Why do we close the connection. Only the request
          //    failed, but the connection seems to be still
          //    valid!
          // this->transport_->close_connection ();
          ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_YES),
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
            this->inp_stream ().start ();
          CORBA_Any any (tcp, 0, cdr);
          CORBA_Exception *exception;

          ACE_NEW_THROW_EX (exception,
                            CORBA_UnknownUserException (any),
                            CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_YES));
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          // @@ Think about a better way to raise the exception here,
          //    maybe we need some more macros?
          ACE_TRY_ENV.exception (exception);  // We can not use ACE_THROW here.
          return TAO_INVOKE_EXCEPTION;
        }

      // If we couldn't find the right exception, report it as
      // CORBA::UNKNOWN.

      // @@ It would seem like if the remote exception is a
      //    UserException we can assume that the request was
      //    completed.
      ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                        CORBA::COMPLETED_YES),
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
      if (this->inp_stream ().read_string (buf.inout ()) == 0)
        {
          // @@ Why do we close the connection. Only the request
          //    failed, but the connection seems to be still
          //    valid!
          // this->transport_->close_connection ();
          ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_YES),
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
            ACE_THROW_RETURN (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                                CORBA::COMPLETED_YES),
                              TAO_INVOKE_EXCEPTION);

          this->inp_stream ().decode (exception->_type (),
                                      exception, 0,
                                      ACE_TRY_ENV);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          if (TAO_debug_level > 5)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("TAO: (%P|%t) Raising exception %s\n"),
                        buf.in ()));

          // @@ Think about a better way to raise the exception here,
          //    maybe we need some more macros?
          ACE_TRY_ENV.exception (exception); // We can not use ACE_THROW here.
          return TAO_INVOKE_EXCEPTION;
        }

      // If we couldn't find the right exception, report it as
      // CORBA::UNKNOWN.

      ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                        CORBA::COMPLETED_YES),
                        TAO_INVOKE_EXCEPTION);
    }

  return retval;
}

int
TAO_GIOP_Twoway_Invocation::invoke_i (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Register a reply dispatcher for this invocation. Use the
  // preallocated reply dispatcher.

  // Bind.
  int retval =
    this->transport_->tms ()->bind_dispatcher (this->request_id_,
                                               &this->rd_);
  if (retval == -1)
    {
      // @@ What is the right way to handle this error?
      this->close_connection ();

      ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                        TAO_INVOKE_EXCEPTION);
    }

  // Just send the request, without trying to wait for the reply.
  retval = TAO_GIOP_Invocation::invoke (1, ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (retval != TAO_INVOKE_OK)
    return retval;

  // This blocks until the response is read.  In the current version,
  // there is only one client thread that ever uses this connection,
  // so most response messages are illegal.
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

  // Get the reply status.

  // Wait for the reply.

  if (TAO_debug_level > 0 && this->max_wait_time_ != 0)
    {
      CORBA::ULong msecs =
        this->max_wait_time_->msec ();

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("TAO (%P|%t) Timeout on recv is <%u>\n"),
                  msecs));
    }

  int reply_error =
    this->transport_->wait_strategy ()->wait (this->max_wait_time_,
                                              this->rd_.reply_received ());


  if (TAO_debug_level > 0 && this->max_wait_time_ != 0)
    {
      CORBA::ULong msecs =
        this->max_wait_time_->msec ();

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("TAO (%P|%t) Timeout after recv is <%u> status <%d>\n"),
                  msecs, reply_error));
    }

  // Check the reply error.

  if (reply_error == -1)
    {
      if (errno == ETIME)
        {
          // Just a timeout, don't close the connection or
          // anything...
          ACE_THROW_RETURN (CORBA::TIMEOUT (
              CORBA_SystemException::_tao_minor_code (
                  TAO_TIMEOUT_SEND_MINOR_CODE,
                  errno),
              CORBA::COMPLETED_NO),
            TAO_INVOKE_EXCEPTION);
        }

      this->close_connection ();

      ACE_THROW_RETURN (CORBA::COMM_FAILURE (
          CORBA_SystemException::_tao_minor_code (
            TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
            errno),
          CORBA::COMPLETED_MAYBE),
        TAO_INVOKE_EXCEPTION);
    }

  // @@ Alex: the old version of this had some error handling code,
  //    like:  this->profile_->reset_hint ()
  //    Can you make sure we don't forget to do that on exceptions
  //    and/or errors.
  //    BTW, think about native exceptions where if the exception is
  //    raised in the wait() method you won't get a chance
  //    to do that kind of error handling.  Do you really need
  //    exceptions in the transport objects?

  CORBA::ULong reply_status = this->rd_.reply_status ();

  switch (reply_status)
    {
    case TAO_GIOP_NO_EXCEPTION:
      // Return so that the STUB can demarshal the reply.
      return TAO_INVOKE_OK;

    case TAO_GIOP_USER_EXCEPTION:
      // Return so the exception can be handled.
      return TAO_INVOKE_EXCEPTION;

    case TAO_GIOP_SYSTEM_EXCEPTION:
      {
        // @@ Add the location macros for this exceptions...

        CORBA::String_var type_id;

        if ((this->inp_stream () >> type_id.inout ()) == 0)
          {
            // Could not demarshal the exception id, raise an local
            // CORBA::MARSHAL
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_OK);
          }

        CORBA::ULong minor = 0;
        CORBA::ULong completion = 0;

        if ((this->inp_stream () >> minor) == 0
            || (this->inp_stream () >> completion) == 0)
          ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_MAYBE),
                            TAO_INVOKE_OK);

        CORBA::SystemException* ex =
          TAO_Exceptions::create_system_exception (type_id.in (),
                                                   ACE_TRY_ENV);
        ACE_CHECK_RETURN (TAO_INVOKE_OK);

        if (ex == 0)
          {
            // @@ We should raise a CORBA::NO_MEMORY, but we ran out
            //    of memory already. We need a pre-allocated, TSS,
            //    CORBA::NO_MEMORY instance
            ACE_NEW_RETURN (ex,
                            CORBA::UNKNOWN,
                            TAO_INVOKE_EXCEPTION);
          }

        ex->minor (minor);
        ex->completed (CORBA::CompletionStatus (completion));


        // @@ There should be a better way to raise this exception!
        //    This code works for both native and emulated exceptions,
        //    but it is ugly.
        ACE_TRY_ENV.exception (ex); // We can not use ACE_THROW here.
        return TAO_INVOKE_OK;
      }
      // NOTREACHED.

    case TAO_GIOP_LOCATION_FORWARD:
      // Handle the forwarding and return so the stub restarts the
      // request!
      return this->location_forward (this->inp_stream (), ACE_TRY_ENV);
      // NOT REACHED.
    }

  return TAO_INVOKE_OK;
}

// ****************************************************************

TAO_GIOP_Oneway_Invocation::
TAO_GIOP_Oneway_Invocation (TAO_Stub *stub,
                            const char *operation,
                            TAO_ORB_Core *orb_core)
  : TAO_GIOP_Invocation (stub, operation, orb_core),
    sync_scope_ (TAO::SYNC_WITH_TRANSPORT)
{
#if (TAO_HAS_CORBA_MESSAGING == 1)
  TAO_Sync_Scope_Policy *ssp = stub->sync_scope ();

  if (ssp)
    {
      this->sync_scope_ = ssp->synchronization ();
    }
#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
}

TAO_GIOP_Oneway_Invocation::~TAO_GIOP_Oneway_Invocation (void)
{
  if (this->transport_ != 0)
    this->transport_->idle ();
}

void
TAO_GIOP_Oneway_Invocation::start (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_GIOP_Invocation::start (ACE_TRY_ENV);
  ACE_CHECK;

  this->transport_->start_request (this->orb_core_,
                                   this->profile_,
                                   this->out_stream_,
                                   ACE_TRY_ENV);
}

int
TAO_GIOP_Oneway_Invocation::invoke (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->sync_scope_ == TAO::SYNC_WITH_TRANSPORT
      || this->sync_scope_ == TAO::SYNC_NONE
      || this->sync_scope_ == TAO::SYNC_FLUSH)
    {
      return TAO_GIOP_Invocation::invoke (0,
                                          ACE_TRY_ENV);
    }

  // Create this only if a reply is required.
  TAO_Synch_Reply_Dispatcher rd (this->orb_core_,
                                 this->service_info_);

  // The rest of this function is very similar to
  // TWO_GIOP_Twoway_Invocation::invoke_i, because we must
  // wait for a reply. See comments in that code.
  int retval =
    this->transport_->tms ()->bind_dispatcher (this->request_id_,
                                               &rd);
  if (retval == -1)
    {
      // @@ What is the right way to handle this error?
      this->close_connection ();

      ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                        TAO_INVOKE_EXCEPTION);
    }

  // Send the request.
  retval = TAO_GIOP_Invocation::invoke (1,
                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (retval != TAO_INVOKE_OK)
    return retval;

  // Wait for the reply.
  if (TAO_debug_level > 0 && this->max_wait_time_ != 0)
    {
      CORBA::ULong msecs =
        this->max_wait_time_->msec ();

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("TAO (%P|%t) Timeout on recv is <%u>\n"),
                  msecs));
    }

  int reply_error =
    this->transport_->wait_strategy ()->wait (this->max_wait_time_,
                                              rd.reply_received ());


  if (TAO_debug_level > 0 && this->max_wait_time_ != 0)
    {
      CORBA::ULong msecs =
        this->max_wait_time_->msec ();

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("TAO (%P|%t) Timeout after recv is <%u> status <%d>\n"),
                  msecs, reply_error));
    }

  // Check the reply error.
  if (reply_error == -1)
    {
      if (errno == ETIME)
        {
          // Just a timeout, don't close the connection or
          // anything...
          ACE_THROW_RETURN (CORBA::TIMEOUT (
              CORBA_SystemException::_tao_minor_code (
                  TAO_TIMEOUT_SEND_MINOR_CODE,
                  errno),
              CORBA::COMPLETED_NO),
            TAO_INVOKE_EXCEPTION);
        }

      this->close_connection ();

      ACE_THROW_RETURN (CORBA::COMM_FAILURE (
          CORBA_SystemException::_tao_minor_code (
            TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
            errno),
          CORBA::COMPLETED_MAYBE),
        TAO_INVOKE_EXCEPTION);
    }

  CORBA::ULong reply_status = rd.reply_status ();

  switch (reply_status)
    {
    case TAO_GIOP_NO_EXCEPTION:
      // Return so that the STUB can demarshal the reply.
      return TAO_INVOKE_OK;

    case TAO_GIOP_USER_EXCEPTION:
      {
        // Pull the exception from the stream.
        CORBA::String_var buf;

        if ((rd.reply_cdr () >> buf.inout ()) == 0)
          {
            // Could not demarshal the exception id, raise an local
            // CORBA::MARSHAL
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_EXCEPTION);
          }

        // This kind of exception shouldn't happen with oneways,
        // but if it does, we turn it into a CORBA::UNKNOWN exception.
        ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_YES),
                          TAO_INVOKE_EXCEPTION);
      }

    case TAO_GIOP_SYSTEM_EXCEPTION:
      {
        // @@ Add the location macros for these exceptions...

        CORBA::String_var type_id;

        if ((rd.reply_cdr () >> type_id.inout ()) == 0)
          {
            // Could not demarshal the exception id, raise a local
            // CORBA::MARSHAL
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_OK);
          }

        CORBA::ULong minor = 0;
        CORBA::ULong completion = 0;

        if ((rd.reply_cdr () >> minor) == 0
            || (rd.reply_cdr () >> completion) == 0)
          ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_MAYBE),
                            TAO_INVOKE_OK);

        CORBA::SystemException* ex =
          TAO_Exceptions::create_system_exception (type_id.in (),
                                                   ACE_TRY_ENV);
        ACE_CHECK_RETURN (TAO_INVOKE_OK);

        if (ex == 0)
          {
            // @@ We should raise a CORBA::NO_MEMORY, but we ran out
            //    of memory already. We need a pre-allocated, TSS,
            //    CORBA::NO_MEMORY instance
            ACE_NEW_RETURN (ex,
                            CORBA::UNKNOWN,
                            TAO_INVOKE_EXCEPTION);
          }

        ex->minor (minor);
        ex->completed (CORBA::CompletionStatus (completion));

        // @@ There should be a better way to raise this exception!
        //    This code works for both native and emulated exceptions,
        //    but it is ugly.
        ACE_TRY_ENV.exception (ex); // We can not use ACE_THROW here.
        return TAO_INVOKE_OK;
      }

    case TAO_GIOP_LOCATION_FORWARD:
      // Handle the forwarding and return so the stub restarts the
      // request!
      return this->location_forward (rd.reply_cdr (),
                                     ACE_TRY_ENV);
    }

  return TAO_INVOKE_OK;
}

// ****************************************************************

TAO_GIOP_Locate_Request_Invocation::~TAO_GIOP_Locate_Request_Invocation (void)
{
  if (this->transport_ != 0)
    this->transport_->idle ();
}

// Send request, block until any reply comes back.
void
TAO_GIOP_Locate_Request_Invocation::start (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_GIOP_Invocation::start (ACE_TRY_ENV);
  ACE_CHECK;

  this->transport_->start_locate (this->orb_core_,
                                  this->profile_,
                                  this->request_id_,
                                  this->out_stream_,
                                  ACE_TRY_ENV);
}

int
TAO_GIOP_Locate_Request_Invocation::invoke (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ TODO This method is very similar to invoke_i(), we should try
  //    to refactor them.

  if (this->transport_ == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      TAO_INVOKE_EXCEPTION);

  // Register a reply dispatcher for this invocation. Use the
  // preallocated reply dispatcher.

  // Bind.
  int retval =
    this->transport_->tms ()->bind_dispatcher (this->request_id_,
                                               &this->rd_);
  if (retval == -1)
    {
      // @@ What is the right way to handle this error?
      this->close_connection ();

      ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                        TAO_INVOKE_EXCEPTION);
    }

  int result =
    this->transport_->send_request (this->stub_,
                                    this->orb_core_,
                                    this->out_stream_,
                                    1,
                                    this->max_wait_time_);


  if (result == -1)
    {
      this->transport_->close_connection ();
      this->transport_ = 0;

      this->profile_->reset_hint ();

      // @@ This code abort if the connection for the currenct profile
      //    fails.  Should we transparently try new profiles until one
      //    works? Or is that something that a higher level component
      //    should decide?  Remember that LocateRequests are part of
      //    the strategy to establish a connection.
      ACE_THROW_RETURN (CORBA::TRANSIENT (
        CORBA_SystemException::_tao_minor_code (
          TAO_INVOCATION_SEND_REQUEST_MINOR_CODE,
          errno),
        CORBA::COMPLETED_MAYBE),
        TAO_INVOKE_EXCEPTION);
    }

  // @@ Maybe the right place to do this is once the reply is
  //    received? But what about oneways?
  // @@ Preserve semantics for the case when no ClientPriorityPolicy
  // is set/used.  For the case when it is, we don't want to set
  // anything because usage cases of various profiles based on
  // priorities aren't fully supported by MProfiles & friends (yet).
  if (this->stub_->profile_in_use () == this->profile_)
    this->stub_->set_valid_profile ();

  // Wait for the reply.

  int reply_error =
    this->transport_->wait_strategy ()->wait (this->max_wait_time_,
                                              this->rd_.reply_received ());

  // Check the reply error.
  if (reply_error == -1)
    {
      if (errno == ETIME)
        {
          // Just a timeout, don't close the connection or
          // anything...
          ACE_THROW_RETURN (CORBA::TIMEOUT (
              CORBA_SystemException::_tao_minor_code (
                  TAO_TIMEOUT_SEND_MINOR_CODE,
                  errno),
              CORBA::COMPLETED_NO),
            TAO_INVOKE_EXCEPTION);
        }

      this->close_connection ();

      ACE_THROW_RETURN (CORBA::COMM_FAILURE (TAO_DEFAULT_MINOR_CODE,
                                             CORBA::COMPLETED_MAYBE),
                        TAO_INVOKE_EXCEPTION);
    }

  CORBA::ULong locate_status = this->rd_.reply_status ();

  switch (locate_status)
    {
    case TAO_GIOP_OBJECT_HERE:
      break;

    case TAO_GIOP_UNKNOWN_OBJECT:
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (TAO_DEFAULT_MINOR_CODE,
                                                 CORBA::COMPLETED_YES),
                        TAO_INVOKE_EXCEPTION);
      // NOTREACHED

    case TAO_GIOP_OBJECT_FORWARD:
      // When ClientPriorityPolicy is not set/used, behave normally.
      if (this->stub_->profile_in_use () == this->profile_)
        return this->location_forward (this->inp_stream (),
                                       ACE_TRY_ENV);
      else
        // else, don't change profile structures - we are not ready
        // to handle location forwards with priority profiles yet (not enough
        // support from MProfiles ...)
        return TAO_INVOKE_RESTART;
      // NOTREACHED
    }

  return TAO_INVOKE_OK;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
