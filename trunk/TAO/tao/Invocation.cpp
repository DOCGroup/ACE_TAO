// $Id$


#include "Invocation.h"
#include "Principal.h"
#include "Stub.h"

#include "Timeprobe.h"
#include "Dynamic_Adapter.h"
#include "Object_KeyC.h"
#include "debug.h"
#include "Transport.h"
#include "Pluggable.h"
#include "Connector_Registry.h"
#include "Wait_Strategy.h"
#include "Transport_Mux_Strategy.h"
#include "Bind_Dispatcher_Guard.h"
#include "Endpoint.h"
#include "Base_Transport_Property.h"

#include "Messaging_Policy_i.h"
#include "GIOP_Utils.h"
#include "ORB_Core.h"
#include "Pluggable_Messaging_Utils.h"
#include "Endpoint_Selector_Factory.h"

#include "ace/Dynamic_Service.h"

#if !defined (__ACE_INLINE__)
# include "Invocation.i"
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
// ****************************************************************

TAO_GIOP_Invocation::TAO_GIOP_Invocation (void)
  : stub_ (0),
    op_details_ (0,
                 0,
                 0),
    out_stream_ (),
    orb_core_ (0),
    transport_ (0),
    endpoint_selector_ (0),
    inconsistent_policies_ (0),
    profile_ (0),
    endpoint_ (0),
    max_wait_time_ (0),
    restart_flag_ (0),
    forward_reference_ (),
    received_location_forward_ (0),
    profile_index_ (0)
{
}

TAO_GIOP_Invocation::TAO_GIOP_Invocation (TAO_Stub *stub,
                                          const char *operation,
                                          CORBA::ULong opname_len,
                                          CORBA::Boolean argument_flag,
                                          TAO_ORB_Core *orb_core,
                                          int byte_order)
  : stub_ (stub),
    op_details_ (operation,
                 opname_len,
                 argument_flag),
    out_stream_ (this->buffer_,
                 sizeof this->buffer_, /* ACE_CDR::DEFAULT_BUFSIZE */
                 byte_order,
                 orb_core->output_cdr_buffer_allocator (),
                 orb_core->output_cdr_dblock_allocator (),
                 orb_core->output_cdr_msgblock_allocator (),
                 orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                 TAO_DEF_GIOP_MAJOR,
                 TAO_DEF_GIOP_MINOR,
                 orb_core->to_iso8859 (),
                 orb_core->to_unicode ()),
    orb_core_ (orb_core),
    transport_ (0),
    endpoint_selector_ (0),
    inconsistent_policies_ (0),
    profile_ (0),
    endpoint_ (0),
    max_wait_time_ (0),
    restart_flag_ (0),
    forward_reference_ (),
    received_location_forward_ (0),
    profile_index_ (0)
{
}

TAO_GIOP_Invocation::~TAO_GIOP_Invocation (void)
{
  TAO_Transport::release (this->transport_);
}

// The public API involves creating an invocation, starting it, filling
// in request parameters, actually performing the invocation, getting
// response parameters, and then cleaning up.  Sometimes they must be
// restarted (e.g. request forwarding).  This is the start/restart entry.

void
TAO_GIOP_Invocation::start (ACE_ENV_SINGLE_ARG_DECL)
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
    {
      ACE_THROW (CORBA::INTERNAL (
                   CORBA_SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                CORBA::COMPLETED_NO));
    }

  // Initialize endpoint selection strategy.
  if (this->endpoint_selector_ == 0)
    {
      this->endpoint_selector_ =
        this->orb_core_->endpoint_selector_factory ()->get_selector (
          this
           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (this->max_wait_time_ == 0)
    {
      int has_timeout;
      this->orb_core_->call_timeout_hook (this->stub_,
                                          has_timeout,
                                          this->max_wait_time_value_);
      if (has_timeout)
        {
          this->max_wait_time_ = &this->max_wait_time_value_;
        }
    }

  ACE_Countdown_Time countdown (this->max_wait_time_);
  this->countdown_ = &countdown;

  this->endpoint_selector_->select_endpoint (this
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Set the unique request ID associated with this request.
  this->op_details_.request_id (this->transport_->tms ()->request_id ());

  // Make sure that you have the right object key
  this->target_spec_.target_specifier (this->profile_->object_key ());
}

int
TAO_GIOP_Invocation::perform_call (TAO_Transport_Descriptor_Interface &desc
                                   ACE_ENV_ARG_DECL)
{
  // Get the transport object.
  if (this->transport_ != 0)
    {
      this->transport_->make_idle ();
    }

  // Release the transport prior to connecting.
  // In most cases the transport_ will already be zero.
  TAO_Transport::release (this->transport_);
  this->transport_ = 0;

  // Get a pointer to the connector registry, which might be in
  // thread-specific storage, depending on the concurrency model.
  TAO_Connector_Registry *conn_reg =
    this->orb_core_->connector_registry ();

  if (conn_reg == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (
                        CORBA_SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          EINVAL),
                        CORBA::COMPLETED_NO),
                        1);
    }

  // Obtain a connection.
  int result = conn_reg->connect (this,
                                  &desc
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  if (result == 0)
    {
      // Now that we have the client connection handler object we need to
      // set the right messaging protocol for in the client side transport.
      const TAO_GIOP_Message_Version& version = this->profile_->version ();
      result = this->transport_->messaging_init (version.major,
                                                     version.minor);

      // Set the giop version of the out stream
      this->out_stream_.set_version (version.major, version.minor);

      if (result == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%N|%l|%p|%t) ")
                          ACE_TEXT ("messaging_init() failed\n")));
            }
        }
      else
        return 1;
    }

  if (errno == ETIME)
    {
      ACE_THROW_RETURN (CORBA::TIMEOUT (
              CORBA_SystemException::_tao_minor_code (
                  TAO_TIMEOUT_CONNECT_MINOR_CODE,
                  errno),
              CORBA::COMPLETED_NO),
              1);
    }

  // Update the remaining time for this call.
  this->countdown_->update ();

  // Return that we'd like to try another endpoint.
  return 0;
}

void
TAO_GIOP_Invocation::prepare_header (CORBA::Octet response_flags
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Then fill in the rest of the RequestHeader

  // Fill up the service context lists
  // First lookup at the services to see whether they have anything to
  // add to the service context lists
  this->orb_core_->service_context_list (this->stub_,
                                         this->request_service_context (),
                                         this->restart_flag_
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Set the target specification mode
  switch (this->profile_->addressing_mode ())
    {
    case TAO_Target_Specification::Key_Addr:
      this->target_spec_.target_specifier (
            this->profile_->object_key ());
      break;

    case TAO_Target_Specification::Profile_Addr:
      this->target_spec_.target_specifier (
                this->profile_->create_tagged_profile ()
              );
      break;

    case TAO_Target_Specification::Reference_Addr:
      // We need to call the method seperately. If there is no
      // IOP::IOR info, the call would create the info and return the
      // index that we need.
      CORBA::ULong index = 0;

      IOP::IOR *ior_info = 0;
      int retval = this->stub_->create_ior_info (ior_info,
                                                 index
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (retval == -1)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) Error in finding index for \n")
                          ACE_TEXT ("IOP::IOR \n")));
            }
          return;
        }

      this->target_spec_.target_specifier (*ior_info,
                                           index);
      break;
    }

  // Update the response flags
  this->op_details_.response_flags (response_flags);

  // Send the request for the header
  if (this->transport_->generate_request_header (this->op_details_,
                                                 this->target_spec_,
                                                 this->out_stream_)
        == -1)
    {
      ACE_THROW (CORBA::MARSHAL ());
    }
}

// Send request.
int
TAO_GIOP_Invocation::invoke (CORBA::Boolean is_synchronous
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_Countdown_Time countdown (this->max_wait_time_);

  if (this->transport_ == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        TAO_INVOKE_EXCEPTION);
    }

  int result =
    this->transport_->send_request (this->stub_,
                                    this->orb_core_,
                                    this->out_stream_,
                                    is_synchronous,
                                    this->max_wait_time_);

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

      this->endpoint_->reset_hint ();

      this->restart_flag_ = 1;

      return TAO_INVOKE_RESTART;
    }

  // Indicate that the endpoint/profile was used successfully.
  // @@ Maybe the right place to do this is once the reply is
  //    received? But what about oneways?
  this->endpoint_selector_->success (this);

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
  // this->transport_->idle ();
  TAO_Transport::release (this->transport_);
  this->transport_ = 0;

  this->endpoint_->reset_hint ();
  this->endpoint_ = 0;
  this->profile_ = 0;

  this->endpoint_selector_->close_connection (this);

  return TAO_INVOKE_RESTART;
}

// Handle the GIOP Reply with status = LOCATION_FORWARD
// Replace the IIOP Profile.
int
TAO_GIOP_Invocation::location_forward (TAO_InputCDR &inp_stream
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // It can be assumed that the GIOP header and the reply header
  // are already handled.  Further it can be assumed that the
  // reply body contains an object reference to the new object.
  // This object pointer will be now extracted.

  if ((inp_stream >> this->forward_reference_.out ()) == 0)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (),
                        TAO_INVOKE_EXCEPTION);
    }

  // The object pointer has to be changed to a TAO_Stub pointer
  // in order to obtain the profiles.
  TAO_Stub *stubobj = this->forward_reference_->_stubobj ();

  this->location_forward_i (stubobj ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

  return TAO_INVOKE_RESTART;
}

int
TAO_GIOP_Invocation::location_forward (CORBA::Object_ptr forward
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (forward))
    ACE_THROW_RETURN (CORBA::INV_OBJREF (), TAO_INVOKE_EXCEPTION);

  this->forward_reference_ = CORBA::Object::_duplicate (forward);

  // The object pointer has to be changed to a TAO_Stub pointer
  // in order to obtain the profiles.
  TAO_Stub *stubobj = forward->_stubobj ();

  this->location_forward_i (stubobj ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

  return TAO_INVOKE_RESTART;
}

void
TAO_GIOP_Invocation::location_forward_i (TAO_Stub *stubobj
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Add the profiles in the given stub object to the list of forward
  // profiles.
  if (stubobj == 0)
    ACE_THROW (CORBA::INTERNAL ());

  // Initialize endpoint selection strategy.
  if (this->endpoint_selector_ == 0)
    {
      this->endpoint_selector_ =
        this->orb_core_->endpoint_selector_factory ()->get_selector (
          this
           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  // Modify the state as appropriate to include new forwarding
  // profiles.
  this->endpoint_selector_->forward (this,
                                     stubobj->base_profiles ()
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->received_location_forward_ = 1;

  this->restart_flag_ = 1;
}

// ****************************************************************

TAO_GIOP_Synch_Invocation::TAO_GIOP_Synch_Invocation (void)
  : rd_ (0,
         this->op_details_.reply_service_info ())
{
}

TAO_GIOP_Synch_Invocation::~TAO_GIOP_Synch_Invocation (void)
{
  if (this->transport_ != 0)
    {
      this->transport_->idle_after_reply ();
    }
}

int
TAO_GIOP_Synch_Invocation::invoke_i (CORBA::Boolean is_locate_request
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Register a reply dispatcher for this invocation. Use the
  // preallocated reply dispatcher.

  // Bind.

  TAO_Transport_Mux_Strategy *tms = this->transport_->tms ();

  TAO_Bind_Dispatcher_Guard dispatch_guard (
    this->op_details_.request_id (),
    &this->rd_,
    tms);
  int &status = dispatch_guard.status ();

  if (status == -1)
    {
      // @@ What is the right way to handle this error?
      this->close_connection ();

      ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                        TAO_INVOKE_EXCEPTION);
    }

  // Just send the request, without trying to wait for the reply.
  int retval = TAO_GIOP_Invocation::invoke (1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (retval);

  if (retval != TAO_INVOKE_OK)
    {
      return retval;
    }

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
      CORBA::ULong msecs = this->max_wait_time_->msec ();

      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Synch_Invocation::invoke_i, "
                  "timeout on recv is <%u>\n",
                  msecs));
    }

  int reply_error =
    this->transport_->wait_strategy ()->wait (this->max_wait_time_,
                                              this->rd_);


  if (TAO_debug_level > 0 && this->max_wait_time_ != 0)
    {
      CORBA::ULong msecs = this->max_wait_time_->msec ();

      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) Synch_Invocation::invoke_i, "
                  "timeout after recv is <%u> status <%d>\n",
                  msecs,
                  reply_error));
    }

  // Check the reply error.

  if (reply_error == -1)
    {
      // The guard automatically unbinds the dispatcher.
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

      // As there is an error set the status flag to -1
      // @@ This is a hack. The problem is -- when an error occurs we
      // @@ try to close the connection. This just goes and destroys
      // @@ the transport object and so the TMS. When the Bind
      // @@ Dispatcher Guard goes out of scope the destructor is
      // @@ called and it uses the TMS, which is "bad". Let us have
      // @@ this for the time being -- Bala
      status = -1;
      // Call the ORB Core which would check whether we need to really
      // raise an exception or are we going to base our decision on the
      // loaded services.
      return this->orb_core_->service_raise_comm_failure (this,
                                                          this->profile_
                                                          ACE_ENV_ARG_PARAMETER);
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

  if (is_locate_request)
    {
      // A locate request checks a different set of enum return values,
      // so we return to the TAO_GIOP_Locate_Request_Invocation caller
      // to do that.
      return reply_status;
    }

  // Set the reply status to zero before we get out. This is no longer required
  switch (reply_status)
    {
    case TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION:
      // Return so that the STUB can demarshal the reply.
      return TAO_INVOKE_OK;

    case TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION:
        // Return so the exception can be handled.
        return TAO_INVOKE_EXCEPTION;

    case TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION:
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
          {
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_OK);
          }

        CORBA::SystemException* ex =
          TAO_Exceptions::create_system_exception (type_id.in ()
                                                   ACE_ENV_ARG_PARAMETER);
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

        // Raise the exception.
        ACE_ENV_RAISE (ex);

        return TAO_INVOKE_OK;
      }
      // NOTREACHED.

    case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
      // Handle the forwarding and return so the stub restarts the
      // request!
      return this->location_forward (this->inp_stream ()
                                     ACE_ENV_ARG_PARAMETER);
    case TAO_PLUGGABLE_MESSAGE_NEEDS_ADDRESSING_MODE:
      {
        // We have received an exception with a request to change the
        // addressing mode. First let us read the mode that the
        // server/agent asks for.
        CORBA::Short addr_mode = 0;
        if (this->inp_stream ().read_short (addr_mode) == 0)
          {
            // Could not demarshal the addressing disposition, raise an local
            // CORBA::MARSHAL
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_OK);
          }

        // Now set this addressing mode in the profile, so that
        // the next invocation need not go through this.
        this->profile_->addressing_mode (addr_mode
                                         ACE_ENV_ARG_PARAMETER);

        // Now restart the invocation
        return TAO_INVOKE_RESTART;
      }

    }

  return TAO_INVOKE_OK;
}

// ****************************************************************

void
TAO_GIOP_Twoway_Invocation::start (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_GIOP_Invocation::start (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->rd_.state_changed (TAO_LF_Event::LFS_ACTIVE);
}


void
TAO_GIOP_Twoway_Invocation::reset_states (void)
{
  // Reset all the states. This is called by the FT service.
  this->rd_.reset_state (TAO_LF_Event::LFS_IDLE);

  this->profile (0);
  this->endpoint (0);
  this->restart_flag_ = 0;
  this->received_location_forward_ = 0;

  // Anything else that needs resetting??
}

// Send request, block until any reply comes back, and unmarshal reply
// parameters as appropriate.

int
TAO_GIOP_Twoway_Invocation::invoke (TAO_Exception_Data *excepts,
                                    CORBA::ULong except_count
                                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::Exception))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_INVOCATION_INVOKE_START);

  int retval = this->invoke_i (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (retval);

  // A TAO_INVOKE_EXCEPTION status, but no exception raised means that
  // we have a user exception.
  // @@ This is a bit brittle, think about a better implementation.
  if (retval == TAO_INVOKE_EXCEPTION)
    {
      // Pull the exception from the stream.
      CORBA::String_var buf;

      if ((this->inp_stream () >> buf.inout ()) == 0)
        {
          // Could not demarshal the exception id, raise an local
          // CORBA::MARSHAL
          ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_MAYBE),
                            TAO_INVOKE_EXCEPTION);
        }

      // Match the exception interface repository id with the
      // exception in the exception list.
      // This is important to decode the exception.

      for (CORBA::ULong i = 0; i < except_count; ++i)
        {
          if (ACE_OS::strcmp (buf.in (), excepts[i].id) != 0)
            {
              continue;
            }

          // match
          CORBA::Exception *exception = excepts[i].alloc ();

          if (exception == 0)
            {
              ACE_THROW_RETURN (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                                  CORBA::COMPLETED_YES),
                                TAO_INVOKE_EXCEPTION);
            }

          exception->_tao_decode (this->inp_stream ()
                                   ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          if (TAO_debug_level > 5)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO: (%P|%t) Raising exception %s\n"),
                          buf.in ()));
            }

          // @@ Think about a better way to raise the exception here,
          //    maybe we need some more macros?
#if defined (TAO_HAS_EXCEPTIONS)
          // Direct throw because we don't have the ACE_TRY_ENV.
          exception->_raise ();
#else
          ACE_TRY_ENV.exception (exception); // We can not use ACE_THROW here.
#endif
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

// ****************************************************************

TAO_GIOP_Oneway_Invocation::TAO_GIOP_Oneway_Invocation (
    TAO_Stub *stub,
    const char *operation,
    CORBA::ULong opname_len,
    CORBA::Boolean argument_flag,
    TAO_ORB_Core *orb_core,
    int byte_order
  )
  : TAO_GIOP_Synch_Invocation (stub,
                               operation,
                               opname_len,
                               argument_flag,
                               orb_core,
                               byte_order),
    sync_scope_ (TAO::SYNC_WITH_TRANSPORT)
{
  int has_synchronization = 0;
  int scope = this->sync_scope_;
  this->orb_core_->call_sync_scope_hook (this->stub_,
                                         has_synchronization,
                                         scope);

  this->sync_scope_ = scope;
}

void
TAO_GIOP_Oneway_Invocation::start (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_GIOP_Invocation::start (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

}

int
TAO_GIOP_Oneway_Invocation::invoke (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->sync_scope_ == TAO::SYNC_NONE
      || this->sync_scope_ == TAO::SYNC_EAGER_BUFFERING
      || this->sync_scope_ == TAO::SYNC_DELAYED_BUFFERING)
    {
      return TAO_GIOP_Invocation::invoke (0 ACE_ENV_ARG_PARAMETER);
    }
  if (this->sync_scope_ == TAO::SYNC_WITH_TRANSPORT)
    {
      return TAO_GIOP_Invocation::invoke (1 ACE_ENV_ARG_PARAMETER);
    }

  int retval = this->invoke_i (0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

  // A TAO_INVOKE_EXCEPTION status, but no exception raised means that
  // we have a user exception.
  // @@ This is a bit brittle, think about a better implementation.
  if (retval == TAO_INVOKE_EXCEPTION)
    {
      // Pull the exception from the stream.
      CORBA::String_var buf;

      if ((this->inp_stream () >> buf.inout ()) == 0)
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

  return retval;
}

// ****************************************************************

void
TAO_GIOP_Locate_Request_Invocation::start (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_GIOP_Invocation::start (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->transport_->generate_locate_request (this->target_spec_,
                                             this->op_details_,
                                             this->out_stream_);
  this->rd_.state_changed (TAO_LF_Event::LFS_ACTIVE);
}

// Send request, block until any reply comes back.
int
TAO_GIOP_Locate_Request_Invocation::invoke (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->transport_ == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        TAO_INVOKE_EXCEPTION);
    }

  CORBA::ULong locate_status = this->invoke_i (1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

  switch (locate_status)
    {
    case TAO_GIOP_OBJECT_HERE:
      break;
    case TAO_GIOP_UNKNOWN_OBJECT:
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (TAO_DEFAULT_MINOR_CODE,
                                                 CORBA::COMPLETED_YES),
                        TAO_INVOKE_EXCEPTION);
      // NOTREACHED.
    case TAO_GIOP_OBJECT_FORWARD:
      return this->location_forward (this->inp_stream ()
                                      ACE_ENV_ARG_PARAMETER);
    case TAO_GIOP_LOC_SYSTEM_EXCEPTION:
      {
        // Pull the exception from the stream.
        CORBA::String_var buf;

        if ((this->inp_stream () >> buf.inout ()) == 0)
          {
            // Could not demarshal the exception id, raise a local
            // CORBA::MARSHAL exception.
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
    case TAO_GIOP_LOC_NEEDS_ADDRESSING_MODE:
      {
        // We have received an exception with a request to change the
        // addressing mode. First let us read the mode that the
        // server/agent asks for.
        CORBA::Short addr_mode = 0;

        if (this->inp_stream ().read_short (addr_mode) == 0)
          {
            // Could not demarshal the addressing disposition, raise a local
            // CORBA::MARSHAL exception.
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_OK);
          }

        // Now set this addressing mode in the profile, so that
        // the next invocation need not go through this.
        this->profile_->addressing_mode (addr_mode
                                         ACE_ENV_ARG_PARAMETER);

        // Restart the invocation.
        return TAO_INVOKE_RESTART;
      }
    }

  return TAO_INVOKE_OK;
}
