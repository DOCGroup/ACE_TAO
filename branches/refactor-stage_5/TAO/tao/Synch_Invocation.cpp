//$Id$
#include "Synch_Invocation.h"
#include "Profile_Transport_Resolver.h"
#include "Synch_Reply_Dispatcher.h"
#include "Transport.h"
#include "Stub.h"
#include "Bind_Dispatcher_Guard.h"
#include "operation_details.h"
#include "Pluggable_Messaging.h"
#include "Wait_Strategy.h"
#include "Messaging_SyncScopeC.h"
#include "TAOC.h"

ACE_RCSID (tao,
           Synch_Invocation,
           "$Id$")

namespace TAO
{
  Synch_Twoway_Invocation::Synch_Twoway_Invocation (Profile_Transport_Resolver &resolver,
                                                    TAO_Operation_Details &detail)
    : Remote_Invocation (resolver, detail)
  {

  }

  Invocation_Status
  Synch_Twoway_Invocation::remote_twoway (ACE_Time_Value *max_wait_time
                                          ACE_ENV_ARG_DECL)
  {
    ACE_Countdown_Time countdown (max_wait_time);

    TAO_Synch_Reply_Dispatcher rd (this->resolver_.stub ()->orb_core (),
                                   this->details_.reply_service_info ());

    // Register a reply dispatcher for this invocation. Use the
    // preallocated reply dispatcher.
    TAO_Bind_Dispatcher_Guard dispatch_guard (this->details_.request_id (),
                                              &rd,
                                              this->resolver_.transport ()->tms ());

    if (dispatch_guard.status () != 0)
      {
        // @@ What is the right way to handle this error?
        // this->close_connection ();

        ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                           CORBA::COMPLETED_NO),
                          TAO_INVOKE_FAILURE);
      }

    TAO_Target_Specification tspec;
    this->init_target_spec (tspec ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);


    TAO_OutputCDR &cdr =
      this->resolver_.transport ()->messaging_object ()->out_stream ();

    this->write_header (tspec,
                        cdr
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    this->marshal_data (cdr
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);


    countdown.update ();

    Invocation_Status s =
      this->send_message (cdr,
                          TAO_Transport::TAO_TWOWAY_REQUEST,
                          max_wait_time
                          ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (s);

    if (s != TAO_INVOKE_SUCCESS)
      return s;

    countdown.update ();

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

    s =
      this->wait_for_reply (max_wait_time,
                            rd,
                            dispatch_guard
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);


    return this->check_reply_status (rd
                                     ACE_ENV_ARG_PARAMETER);
  }


  Invocation_Status
  Synch_Twoway_Invocation::wait_for_reply (ACE_Time_Value *max_wait_time,
                                           TAO_Synch_Reply_Dispatcher &rd,
                                           TAO_Bind_Dispatcher_Guard &bd
                                           ACE_ENV_ARG_DECL)
  {
    int reply_error =
      this->resolver_.transport ()->wait_strategy ()->wait (max_wait_time,
                                                            rd);
    if (TAO_debug_level > 0 && max_wait_time != 0)
      {
        CORBA::ULong msecs = max_wait_time->msec ();

        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Synch_Twoway_Invocation::wait_for_reply, "
                    "timeout after recv is <%u> status <%d>\n",
                    msecs,
                    reply_error));
      }

    // Check the reply error.
    if (reply_error == -1)
      {
        // Unbind the dispatcher, since its of no use at this point of
        // time
        if (TAO_debug_level > 3)
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Synch_Twoway_Invocation::wait_for_reply , "
                      "recovering after an error \n"));

        if (errno == ETIME)
          {
            // If the unbind succeeds then thrown an exception to the
            // application, else just collect the reply and dispatch that to the
            // application.
            // NOTE: A fragile synchronization is provided when using the Muxed
            // Transport strategy. We could infact be a follower thread getting
            // timedout in the LF whereas the dispatching thread could be
            // on the reply_dispatcher that we created. This would lead bad
            // crashes. To get around that, the call to unbind_dispatcher ()
            // will wait on the lock on the Muxed_Transport_Strategy if
            // dispatching has started. This is fragile.
            if (bd.unbind_dispatcher () == 0)
              {
                // Just a timeout, don't close the connection or
                // anything...
                ACE_THROW_RETURN (CORBA::TIMEOUT (
                    CORBA::SystemException::_tao_minor_code (
                        TAO_TIMEOUT_SEND_MINOR_CODE,
                        errno),
                    CORBA::COMPLETED_NO),
                                  TAO_INVOKE_FAILURE);
              }
          }

        this->resolver_.transport ()->close_connection ();
        this->resolver_.stub ()->reset_profiles ();

        ACE_THROW_RETURN (CORBA::COMM_FAILURE (
            CORBA::SystemException::_tao_minor_code (
                TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
                errno),
            CORBA::COMPLETED_MAYBE),
                          TAO_INVOKE_FAILURE);
      }

    return TAO_INVOKE_SUCCESS;
  }

  Invocation_Status
  Synch_Twoway_Invocation::check_reply_status (TAO_Synch_Reply_Dispatcher &rd
                                               ACE_ENV_ARG_DECL)
  {
    // Grab the reply CDR
    TAO_InputCDR &cdr =
      rd.reply_cdr ();

    // Set the translators
    this->resolver_.transport ()->assign_translators (&cdr, 0);

    // At this point it can be assumed that the GIOP/whatever protocol
    // header and the reply header are already handled.  Further it
    // can be assumed that the reply body contains the details
    // required for further processing. All the other details should
    // have been handled in the reply dispatcher/protocol framework.
    switch (rd.reply_status ())
      {
      case TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION:
        if (this->details_.demarshal_args (rd.reply_cdr ()) == false)
          ACE_THROW_RETURN (CORBA::MARSHAL (),
                            TAO_INVOKE_FAILURE);
        break;
      case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
        // Handle the forwarding and return so the stub restarts the
        // request!
        return this->location_forward (cdr
                                       ACE_ENV_ARG_PARAMETER);
      case TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION:
        return this->handle_user_exception (cdr
                                            ACE_ENV_ARG_PARAMETER);
      case TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION:
        // return this->handle_system_exception (ACE_ENV_SINGLE_ARG_PARAMETER)
        break;
#if 0
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

        CORBA::SystemException *ex =
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

#if defined (TAO_HAS_EXCEPTIONS)
        // Without this, the call to create_system_exception() above
        // causes a memory leak. On platforms without native exceptions,
        // the CORBA::Environment class manages the memory.
        auto_ptr<CORBA::SystemException> safety (ex);
#endif

        // Raise the exception.
        ACE_ENV_RAISE (ex);

        return TAO_INVOKE_OK;
      }
      // NOTREACHED.
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
#endif

    }

    return TAO_INVOKE_SUCCESS;
  }

  Invocation_Status
  Synch_Twoway_Invocation::location_forward (TAO_InputCDR &inp_stream
                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if ((inp_stream >> this->forwarded_to_.out ()) == 0)
      {
        ACE_THROW_RETURN (CORBA::MARSHAL (),
                          TAO_INVOKE_FAILURE);
      }

    // The object pointer has to be changed to a TAO_Stub pointer
    // in order to obtain the profiles.
    TAO_Stub *stubobj =
      this->forwarded_to_->_stubobj ();

    if (stubobj == 0)
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        TAO_INVOKE_FAILURE);

    // Reset the profile in the stubs
    this->resolver_.stub ()->add_forward_profiles (stubobj->base_profiles ());

    // Is this check needed?
    /*
    if (this->resolver_.stub ()->next_profile () == 0)
      ACE_THROW (CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
          errno),
        CORBA::COMPLETED_NO));
    */
    return TAO_INVOKE_RESTART;
  }

  Invocation_Status
  Synch_Twoway_Invocation::handle_user_exception (TAO_InputCDR &cdr
                                                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Pull the exception from the stream.
    CORBA::String_var buf;

    if ((cdr >> buf.inout ()) == 0)
      {
        // Could not demarshal the exception id, raise an local
        // CORBA::MARSHAL
        ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_MAYBE),
                          TAO_INVOKE_FAILURE);
      }

    CORBA::Exception *exception =
      this->details_.corba_exception (buf.in ()
                                     ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    exception->_tao_decode (cdr
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (TAO_debug_level > 5)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO: (%P|%t) Synch_Twoway_Invocation::handle_user_exception - ")
                    ACE_TEXT ("raising exception %s\n"),
                    buf.in ()));
      }

    // @@ Think about a better way to raise the exception here,
    //    maybe we need some more macros?
#if defined (TAO_HAS_EXCEPTIONS)
    // If we have native exceptions, we must manage the memory allocated
    // by the call above to alloc(). Otherwise the Environment class
    // manages the memory.
    auto_ptr<CORBA::Exception> safety (exception);

    // Direct throw because we don't have the ACE_TRY_ENV.
    exception->_raise ();
#else
    // We can not use ACE_THROW here.
    ACE_TRY_ENV.exception (exception);
#endif

    return TAO_INVOKE_SUCCESS;
  }

/*================================================================================*/

  Synch_Oneway_Invocation::Synch_Oneway_Invocation (Profile_Transport_Resolver &r,
                                                    TAO_Operation_Details &d)
    : Synch_Twoway_Invocation (r, d)
  {
  }



  Invocation_Status
  Synch_Oneway_Invocation::remote_oneway (ACE_Time_Value *max_wait_time
                                          ACE_ENV_ARG_DECL)
  {
    ACE_Countdown_Time countdown (max_wait_time);

    const CORBA::Octet response_flags =
       this->details_.response_flags ();

    if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_SERVER) ||
        response_flags == CORBA::Octet (Messaging::SYNC_WITH_TARGET))

      return Synch_Twoway_Invocation::remote_twoway (max_wait_time
                                                     ACE_ENV_ARG_PARAMETER);

    TAO_Target_Specification tspec;
    this->init_target_spec (tspec ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    TAO_OutputCDR &cdr =
      this->resolver_.transport ()->messaging_object ()->out_stream ();

    this->write_header (tspec,
                        cdr
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    this->marshal_data (cdr
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    countdown.update ();

    if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_TRANSPORT))
      {
        return this->send_message (cdr,
                                   TAO_Transport::TAO_TWOWAY_REQUEST,
                                   max_wait_time
                                   ACE_ENV_ARG_PARAMETER);
      }

    return this->send_message (cdr,
                               TAO_Transport::TAO_ONEWAY_REQUEST,
                               max_wait_time
                               ACE_ENV_ARG_PARAMETER);
  }

}
