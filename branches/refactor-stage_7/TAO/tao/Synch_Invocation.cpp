//$Id$

#include "Synch_Invocation.h"
#include "Profile_Transport_Resolver.h"
#include "Profile.h"
#include "Synch_Reply_Dispatcher.h"
#include "Transport.h"
#include "Stub.h"
#include "Bind_Dispatcher_Guard.h"
#include "operation_details.h"
#include "Pluggable_Messaging.h"
#include "Wait_Strategy.h"
#include "debug.h"
#include "ORB_Constants.h"
#include "Messaging_SyncScopeC.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (tao,
           Synch_Invocation,
           "$Id$")

namespace TAO
{
  Synch_Twoway_Invocation::Synch_Twoway_Invocation (CORBA::Object_ptr otarget,
                                                    Profile_Transport_Resolver &resolver,
                                                    TAO_Operation_Details &detail,
                                                    bool response_expected)
    : Remote_Invocation (otarget,
                         resolver,
                         detail,
                         response_expected)
  {
  }

  Invocation_Status
  Synch_Twoway_Invocation::remote_twoway (ACE_Time_Value *max_wait_time
                                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::Exception))
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
        // @@ What is the right way to handle this error? Do we need
        // to call the interceptors in this case?
        this->resolver_.transport ()->close_connection ();

        ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                           CORBA::COMPLETED_NO),
                          TAO_INVOKE_FAILURE);
      }




    TAO_Target_Specification tspec;
    this->init_target_spec (tspec ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    Invocation_Status s = TAO_INVOKE_FAILURE;


#if TAO_HAS_INTERCEPTORS == 1
    // Start the interception point here..
    s =
      this->send_request_interception (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (s != TAO_INVOKE_SUCCESS)
      return s;
#endif /*TAO_HAS_INTERCEPTORS */

    TAO_OutputCDR &cdr =
      this->resolver_.transport ()->messaging_object ()->out_stream ();

    // We have started the interception flow. We need to call the
    // ending interception flow if things go wrong. The purpose of the
    // try block is to do just this.
    ACE_TRY
      {
        this->write_header (tspec,
                            cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        this->marshal_data (cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;


        countdown.update ();

        s =
          this->send_message (cdr,
                              TAO_Transport::TAO_TWOWAY_REQUEST,
                              max_wait_time
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

#if TAO_HAS_INTERCEPTORS == 1
        // If the above call returns a restart due to connection
        // failure then call the receive_other interception point
        // before we leave.
        if (s == TAO_INVOKE_RESTART)
          {
            s =
              this->receive_other_interception (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
#endif /*TAO_HAS_INTERCEPTORS */

        if (s != TAO_INVOKE_SUCCESS)
          return s;

        countdown.update ();

        // For some strategies one may want to release the transport
        // back to  cache. If the idling is successfull let the
        // resolver about that.
        if (this->resolver_.transport ()->idle_after_send ())
          this->resolver_.transport_released ();

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
        ACE_TRY_CHECK;


        s = this->check_reply_status (rd
                                      ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        // For some strategies one may want to release the transport
        // back to  cache after receiving the reply. If the idling is
        // successfull let the resolver about that.
        if (this->resolver_.transport ()->idle_after_reply ())
          this->resolver_.transport_released ();

#if TAO_HAS_INTERCEPTORS == 1
        if (s == TAO_INVOKE_SUCCESS)
          {
            s =
              this->receive_reply_interception (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        else if (s == TAO_INVOKE_RESTART)
          {
            s =
              this->receive_other_interception (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
#endif /*TAO_HAS_INTERCEPTORS */
      }
    ACE_CATCHANY
      {
#if TAO_HAS_INTERCEPTORS == 1
        PortableInterceptor::ReplyStatus status =
          this->handle_any_exception (&ACE_ANY_EXCEPTION
                                      ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (status == PortableInterceptor::LOCATION_FORWARD ||
            status == PortableInterceptor::TRANSPORT_RETRY)
          s = TAO_INVOKE_RESTART;
        else if (status == PortableInterceptor::SYSTEM_EXCEPTION
            || status == PortableInterceptor::USER_EXCEPTION)
#endif /*TAO_HAS_INTERCEPTORS*/
          ACE_RE_THROW;
      }
# if defined (ACE_HAS_EXCEPTIONS) \
     && defined (ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS)
    ACE_CATCHALL
      {
#if TAO_HAS_INTERCEPTORS == 1
        PortableInterceptor::ReplyStatus st =
          this->handle_all_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (st == PortableInterceptor::LOCATION_FORWARD ||
            st == PortableInterceptor::TRANSPORT_RETRY)
          s = TAO_INVOKE_RESTART;
        else
#endif /*TAO_HAS_INTERCEPTORS == 1*/
          ACE_RE_THROW;
      }
# endif  /* ACE_HAS_EXCEPTIONS &&
            ACE_HAS_BROKEN_UNEXPECTED_EXCEPTION*/
    ACE_ENDTRY;
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    return s;
  }

  Invocation_Status
  Synch_Twoway_Invocation::wait_for_reply (ACE_Time_Value *max_wait_time,
                                           TAO_Synch_Reply_Dispatcher &rd,
                                           TAO_Bind_Dispatcher_Guard &bd
                                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
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
        {
          Reply_Guard mon (this,
                           TAO_INVOKE_FAILURE);
          if (this->details_.demarshal_args (cdr) == false)
            {
              ACE_THROW_RETURN (CORBA::MARSHAL (),
                                TAO_INVOKE_FAILURE);
            }

          mon.set_status (TAO_INVOKE_SUCCESS);
        }
        break;
      case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
        return this->location_forward (cdr
                                       ACE_ENV_ARG_PARAMETER);
      case TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION:
        return this->handle_user_exception (cdr
                                            ACE_ENV_ARG_PARAMETER);
      case TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION:
        return this->handle_system_exception (cdr
                                              ACE_ENV_ARG_PARAMETER);

      case TAO_PLUGGABLE_MESSAGE_NEEDS_ADDRESSING_MODE:
        {
          Reply_Guard mon (this,
                           TAO_INVOKE_FAILURE);
          // We have received a message with a request to change the
          // addressing mode. First let us read the mode that the
          // server/agent asks for.
          CORBA::Short addr_mode = 0;

          if (cdr.read_short (addr_mode) == 0)
            {
              // Could not demarshal the addressing disposition, raise an local
              // CORBA::MARSHAL
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_FAILURE);
            }

          // Now set this addressing mode in the profile, so that
          // the next invocation need not go through this.
          this->resolver_.profile ()->addressing_mode (addr_mode
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

          mon.set_status (TAO_INVOKE_RESTART);

          // Now restart the invocation
          return TAO_INVOKE_RESTART;
        }
    }
    return TAO_INVOKE_SUCCESS;
  }

  Invocation_Status
  Synch_Twoway_Invocation::location_forward (TAO_InputCDR &inp_stream
                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    Reply_Guard mon (this,
                     TAO_INVOKE_FAILURE);

    if (TAO_debug_level > 3)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::location_forward ")
                    ACE_TEXT ("being handled \n")));
      }

    if ((inp_stream >> this->forwarded_to_.out ()) == 0)
      {
        ACE_THROW_RETURN (CORBA::MARSHAL (
            CORBA::SystemException::_tao_minor_code (
              TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
              errno),
            CORBA::COMPLETED_NO),
                          TAO_INVOKE_FAILURE);
      }

    // The object pointer has to be changed to a TAO_Stub pointer
    // in order to obtain the profiles.
    TAO_Stub *stubobj =
      this->forwarded_to_->_stubobj ();

    if (stubobj == 0)
      ACE_THROW_RETURN (CORBA::INTERNAL (
          CORBA::SystemException::_tao_minor_code (
            TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
            errno),
          CORBA::COMPLETED_NO),
                        TAO_INVOKE_FAILURE);

    // Reset the profile in the stubs
    this->resolver_.stub ()->add_forward_profiles (stubobj->base_profiles ());

    if (this->resolver_.stub ()->next_profile () == 0)
      ACE_THROW_RETURN (CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
          errno),
        CORBA::COMPLETED_NO),
                        TAO_INVOKE_FAILURE);

    mon.set_status (TAO_INVOKE_RESTART);

    return TAO_INVOKE_RESTART;
  }

  Invocation_Status
  Synch_Twoway_Invocation::handle_user_exception (TAO_InputCDR &cdr
                                                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::Exception))
  {
    Reply_Guard mon (this,
                     TAO_INVOKE_FAILURE);

    if (TAO_debug_level > 3)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Synch_Twoway_Invocation::"
                  "handle_user_exception \n"));

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
                    "TAO (%P|%t) - Synch_Twoway_Invocation::"
                    "handle_user_exception - "
                    "raising exception %s\n",
                    buf.in ()));
      }

    mon.set_status (TAO_INVOKE_USER_EXCEPTION);

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

    return TAO_INVOKE_USER_EXCEPTION;
  }

  Invocation_Status
  Synch_Twoway_Invocation::handle_system_exception (TAO_InputCDR &cdr
                                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    Reply_Guard mon (this,
                     TAO_INVOKE_FAILURE);

    if (TAO_debug_level > 3)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Synch_Twoway_Invocation::"
                  "handle_system_exception \n"));

    CORBA::String_var type_id;

    if ((cdr >> type_id.inout ()) == 0)
      {
        // Could not demarshal the exception id, raise an local
        // CORBA::MARSHAL
        ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_MAYBE),
                          TAO_INVOKE_FAILURE);
      }

    CORBA::ULong minor = 0;
    CORBA::ULong completion = 0;

    if ((cdr >> minor) == 0
        || (cdr >> completion) == 0)
      {
        ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_MAYBE),
                          TAO_INVOKE_FAILURE);
      }

    CORBA::SystemException *ex =
      TAO_Exceptions::create_system_exception (type_id.in ()
                                               ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (ex == 0)
      {
        // @@ We should raise a CORBA::NO_MEMORY, but we ran out
        //    of memory already. We need a pre-allocated, TSS,
        //    CORBA::NO_MEMORY instance
        ACE_NEW_RETURN (ex,
                        CORBA::UNKNOWN,
                        TAO_INVOKE_FAILURE);
      }

    ex->minor (minor);
    ex->completed (CORBA::CompletionStatus (completion));

#if defined (TAO_HAS_EXCEPTIONS)
    // Without this, the call to create_system_exception() above
    // causes a memory leak. On platforms without native exceptions,
    // the CORBA::Environment class manages the memory.
    auto_ptr<CORBA::SystemException> safety (ex);
#endif
    if (TAO_debug_level > 4)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Synch_Twoway_Invocation::"
                  "handle_system_exception  about to raise\n"));

    mon.set_status (TAO_INVOKE_SYSTEM_EXCEPTION);

    // Raise the exception.
    ACE_ENV_RAISE (ex);

    return TAO_INVOKE_SYSTEM_EXCEPTION;
  }
/*================================================================================*/

  Synch_Oneway_Invocation::Synch_Oneway_Invocation (CORBA::Object_ptr otarget,
                                                    Profile_Transport_Resolver &r,
                                                    TAO_Operation_Details &d)
    : Synch_Twoway_Invocation (otarget,
                               r,
                               d,
                               false)
  {
  }



  Invocation_Status
  Synch_Oneway_Invocation::remote_oneway (ACE_Time_Value *max_wait_time
                                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::Exception))
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

    Invocation_Status s = TAO_INVOKE_FAILURE;

#if TAO_HAS_INTERCEPTORS == 1
    s = this->send_request_interception (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (s != TAO_INVOKE_SUCCESS)
      return s;
#endif /*TAO_HAS_INTERCEPTORS */

    TAO_OutputCDR &cdr =
      this->resolver_.transport ()->messaging_object ()->out_stream ();

    ACE_TRY
      {
        this->write_header (tspec,
                            cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        this->marshal_data (cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        countdown.update ();

        if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_TRANSPORT))
          {
            s = this->send_message (cdr,
                                    TAO_Transport::TAO_TWOWAY_REQUEST,
                                    max_wait_time
                                    ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        else
          {
            s = this->send_message (cdr,
                                    TAO_Transport::TAO_ONEWAY_REQUEST,
                                    max_wait_time
                                    ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }

#if TAO_HAS_INTERCEPTORS == 1
        s =
          this->receive_other_interception (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
#endif /*TAO_HAS_INTERCEPTORS */
      }
    ACE_CATCHANY
      {
#if TAO_HAS_INTERCEPTORS == 1
        PortableInterceptor::ReplyStatus status =
          this->handle_any_exception (&ACE_ANY_EXCEPTION
                                      ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (status == PortableInterceptor::LOCATION_FORWARD ||
            status == PortableInterceptor::TRANSPORT_RETRY)
          s = TAO_INVOKE_RESTART;
        else if (status == PortableInterceptor::SYSTEM_EXCEPTION
            || status == PortableInterceptor::USER_EXCEPTION)
#endif /*TAO_HAS_INTERCEPTORS*/
          ACE_RE_THROW;
      }
# if defined (ACE_HAS_EXCEPTIONS) \
     && defined (ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS)
    ACE_CATCHALL
      {
#if TAO_HAS_INTERCEPTORS == 1
        PortableInterceptor::ReplyStatus st =
          this->handle_all_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (st == PortableInterceptor::LOCATION_FORWARD ||
            st == PortableInterceptor::TRANSPORT_RETRY)
          s = TAO_INVOKE_RESTART;
        else
#endif /*TAO_HAS_INTERCEPTORS == 1*/
          ACE_RE_THROW;
      }
# endif  /* ACE_HAS_EXCEPTIONS &&
            ACE_HAS_BROKEN_UNEXPECTED_EXCEPTION*/
    ACE_ENDTRY;
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    return s;
  }

  /*================================================================================*/

  Reply_Guard::Reply_Guard (Invocation_Base *b,
                            Invocation_Status s)
    : invocation_ (b)
    , status_ (s)
  {
  }

  Reply_Guard::~Reply_Guard (void)
  {
    this->invocation_->reply_received (this->status_);
  }

  void
  Reply_Guard::set_status (Invocation_Status s)
  {
    this->status_ = s;
  }
}
