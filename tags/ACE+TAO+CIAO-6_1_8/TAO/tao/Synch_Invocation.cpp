// $Id$

#include "tao/Synch_Invocation.h"
#include "tao/Invocation_Retry_State.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/Profile.h"
#include "tao/Synch_Reply_Dispatcher.h"
#include "tao/Transport.h"
#include "tao/Stub.h"
#include "tao/Bind_Dispatcher_Guard.h"
#include "tao/operation_details.h"
#include "tao/Wait_Strategy.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/Messaging_SyncScopeC.h"
#include "tao/ORB_Core.h"
#include "tao/Service_Context.h"
#include "tao/SystemException.h"
#include "ace/Intrusive_Auto_Ptr.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/PortableInterceptorC.h"
#endif /*TAO_HAS_INTERCEPTORS */

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_string.h"
#include "tao/ORB_Time_Policy.h"

#if !defined (__ACE_INLINE__)
# include "tao/Synch_Invocation.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Synch_Twoway_Invocation::Synch_Twoway_Invocation (
    CORBA::Object_ptr otarget,
    Profile_Transport_Resolver &resolver,
    TAO_Operation_Details &detail,
    bool response_expected)
    : Remote_Invocation (otarget,
                         resolver,
                         detail,
                         response_expected)
    , retry_state_ (0)
  {
  }

  void
  Synch_Twoway_Invocation::set_retry_state (Invocation_Retry_State *retry_state)
  {
    this->retry_state_ = retry_state;
  }

  Invocation_Status
  Synch_Twoway_Invocation::remote_twoway (ACE_Time_Value *max_wait_time)
  {
    TAO::ORB_Countdown_Time countdown (max_wait_time);

    TAO_Synch_Reply_Dispatcher *rd_p = 0;
    ACE_NEW_NORETURN (rd_p, TAO_Synch_Reply_Dispatcher (this->resolver_.stub ()->orb_core (),
                                          this->details_.reply_service_info ()));
    if (!rd_p)
      {
        throw ::CORBA::NO_MEMORY ();
      }

    ACE_Intrusive_Auto_Ptr<TAO_Synch_Reply_Dispatcher> rd(rd_p, false);

    Invocation_Status s = TAO_INVOKE_FAILURE;

#if TAO_HAS_INTERCEPTORS == 1
    // Start the interception point here..
    s = this->send_request_interception ();

    if (s != TAO_INVOKE_SUCCESS)
      return s;

    // We have started the interception flow. We need to call the
    // ending interception flow if things go wrong. The purpose of the
    // try block is to do just this.
    try
      {
#endif /*TAO_HAS_INTERCEPTORS */
        TAO_Transport* const transport = this->resolver_.transport ();

        if (!transport)
          {
            if (this->retry_state_ &&
                this->retry_state_->forward_on_exception_increment(FOE_TRANSIENT))
                  {
                    if (TAO_debug_level > 0)
                      ACE_DEBUG ((LM_INFO,
                                  ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::")
                                  ACE_TEXT ("remote_twoway retrying on TRANSIENT ")
                                  ACE_TEXT ("exception\n")));
                    this->retry_state_->next_profile_retry (*this->stub ());
                    return TAO_INVOKE_RESTART;
                  }
            else
              {
                // Way back, we failed to find a profile we could connect to.
                // We've come this far only so we reach the interception points
                // in case they can fix things. Time to bail....
                throw CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
              }

          }

        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon,
                          transport->output_cdr_lock (), TAO_INVOKE_FAILURE);

        TAO_OutputCDR &cdr = transport->out_stream ();

        cdr.message_attributes (this->details_.request_id (),
                                this->resolver_.stub (),
                                TAO_Message_Semantics (),
                                max_wait_time);

        this->write_header (cdr);

        this->marshal_data (cdr);

        // Register a reply dispatcher for this invocation. Use the
        // preallocated reply dispatcher.
        TAO_Bind_Dispatcher_Guard dispatch_guard (
          this->details_.request_id (),
          rd.get (),
          transport->tms ());

        if (dispatch_guard.status () != 0)
          {
            // @@ What is the right way to handle this error? Why should
            // we close the connection?
            transport->close_connection ();

            throw ::CORBA::INTERNAL (0, CORBA::COMPLETED_NO);
          }

        countdown.update ();

        s = this->send_message (cdr,
                                TAO_Message_Semantics (),
                                max_wait_time);

        ace_mon.release();

#if TAO_HAS_INTERCEPTORS == 1
        // @@NOTE: Too much code repetition.
        // If the above call returns a restart due to connection
        // failure then call the receive_other interception point
        // before we leave.
        if (s == TAO_INVOKE_RESTART)
          {
            Invocation_Status const tmp = this->receive_other_interception ();

            if (tmp != TAO_INVOKE_SUCCESS)
              s = tmp;
          }
#endif /* TAO_HAS_INTERCEPTORS */

        if (s != TAO_INVOKE_SUCCESS)
          return s;

        countdown.update ();

        // For some strategies one may want to release the transport
        // back to cache. If the idling is successful let the
        // resolver about that.
        if (transport->idle_after_send ())
          this->resolver_.transport_released ();

        // @@ In all MT environments, there's a cancellation point lurking
        // here; need to investigate.  Client threads would frequently be
        // canceled sometime during recv_request ... the correct action to
        // take on being canceled is to issue a CancelRequest message to the
        // server and then immediately let other client-side cancellation
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

        s = this->wait_for_reply (max_wait_time, *rd.get (), dispatch_guard);

#if TAO_HAS_INTERCEPTORS == 1
        if (s == TAO_INVOKE_RESTART)
          {
            Invocation_Status const tmp = this->receive_other_interception ();

            // Push the latest values for the return..
            if (tmp != TAO_INVOKE_SUCCESS)
              s = tmp;
          }
#endif /* TAO_HAS_INTERCEPTORS */

        if (s != TAO_INVOKE_SUCCESS)
          return s;

        // What happens when the above call returns an error through
        // the return value? That would be bogus as per the contract
        // in the interface. The call violated the contract
        s = this->check_reply_status (*rd.get ());

        // For some strategies one may want to release the transport
        // back to  cache after receiving the reply.
        if (transport->idle_after_reply ())
          this->resolver_.transport_released ();

#if TAO_HAS_INTERCEPTORS == 1
        Invocation_Status tmp = TAO_INVOKE_FAILURE;
        if (s == TAO_INVOKE_RESTART)
          {
            tmp = this->receive_other_interception ();
          }
        else if (s == TAO_INVOKE_SUCCESS)
          {
            tmp = this->receive_reply_interception ();
          }
        if (tmp != TAO_INVOKE_SUCCESS)
          s = tmp;
      }
    catch ( ::CORBA::Exception& ex)
      {
        PortableInterceptor::ReplyStatus const status =
          this->handle_any_exception (&ex);

        if (status == PortableInterceptor::LOCATION_FORWARD ||
            status == PortableInterceptor::TRANSPORT_RETRY)
          s = TAO_INVOKE_RESTART;
        else if (status == PortableInterceptor::SYSTEM_EXCEPTION
                 || status == PortableInterceptor::USER_EXCEPTION)
          throw;
      }
    catch (...)
      {
        // Notify interceptors of non-CORBA exception, and propagate
        // that exception to the caller.

         PortableInterceptor::ReplyStatus const st =
           this->handle_all_exception ();

         if (st == PortableInterceptor::LOCATION_FORWARD ||
             st == PortableInterceptor::TRANSPORT_RETRY)
           s = TAO_INVOKE_RESTART;
         else
           throw;
      }
#endif /* TAO_HAS_INTERCEPTORS */

    return s;
  }

  Invocation_Status
  Synch_Twoway_Invocation::wait_for_reply (ACE_Time_Value *max_wait_time,
                                           TAO_Synch_Reply_Dispatcher &rd,
                                           TAO_Bind_Dispatcher_Guard &bd)
  {
    /*
     * Precondition: The call went to the remote
     * peer. <ACE_Thread::self> is waiting for the reply.
     *
     * Postcondition: Any error during a wait is marked by raising an
     * exception. Success alone is returned through the return value.
     */

    bool const
      expired = (max_wait_time && ACE_Time_Value::zero == *max_wait_time);
    if (expired)
      errno = ETIME;
    int const
      reply_error = expired ? -1 :
        this->resolver_.transport ()->wait_strategy ()->wait (max_wait_time, rd);

    if (TAO_debug_level > 0 && max_wait_time)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::wait_for_reply, ")
                    ACE_TEXT ("timeout after recv is <%u> status <%d>\n"),
                    max_wait_time->msec (),
                    reply_error));
      }

    // Check the reply error.
    if (reply_error == -1)
      {
        // Unbind the dispatcher, since its of no use at this point of
        // time
        if (TAO_debug_level > 3)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::")
                        ACE_TEXT ("wait_for_reply, ")
                        ACE_TEXT ("recovering after an error\n")));
          }

        // You the smarty, don't try to moving the unbind_dispatcher
        // () call since it looks like it is repeated twice. That
        // could land you in trouble. If you don't believe this
        // warning go ahead and try. Try running tests to see what is
        // going on ;)
        if (errno == ETIME)
          {
            // If the unbind succeeds then thrown an exception to the
            // application, else just collect the reply and dispatch
            // that to the application.
            //
            // NOTE: A fragile synchronization is provided when using
            // the Muxed Transport strategy. We could infact be a
            // follower thread getting timedout in the LF whereas the
            // dispatching thread could be on the reply_dispatcher
            // that we created. This would lead bad crashes. To get
            // around that, the call to unbind_dispatcher () will wait
            // on the lock on the Muxed_Transport_Strategy if
            // dispatching has started. This is fragile.
            if (bd.unbind_dispatcher () == 0)
              {
                // Just a timeout with completed_maybe, don't close
                // the connection or  anything
                throw ::CORBA::TIMEOUT (
                  CORBA::SystemException::_tao_minor_code (
                    TAO_TIMEOUT_RECV_MINOR_CODE,
                    errno),
                  CORBA::COMPLETED_MAYBE);
              }
          }
        else
          {
            (void) bd.unbind_dispatcher ();
            this->resolver_.transport ()->close_connection ();

            if (this->retry_state_ &&
                this->resolver_.transport ()->connection_closed_on_read() &&
                this->retry_state_->forward_on_reply_closed_increment ())
              {
                if (TAO_debug_level > 4)
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::")
                              ACE_TEXT ("wait_for_reply, forward profile on ")
                              ACE_TEXT ("connection closed\n")));
                this->retry_state_->next_profile_retry (*this->stub ());
                return TAO_INVOKE_RESTART;
              }

            try
              {
                return
                  this->stub()->orb_core ()->service_raise_comm_failure (
                    this->details_.request_service_context ().service_info (),
                    this->resolver_.profile ());

              }
            catch (const ::CORBA::Exception&)
              {
                if (this->retry_state_ == 0 ||
                    !this->retry_state_->forward_on_exception_limit_used ())
                  {
                    this->resolver_.stub ()->reset_profiles ();
                  }
                throw;
              }
          }
      }

    return TAO_INVOKE_SUCCESS;
  }

  Invocation_Status
  Synch_Twoway_Invocation::check_reply_status (TAO_Synch_Reply_Dispatcher &rd)
  {
    /*
     * Precondition: We probably got a reply. <ACE_Thread::self> is
     * checking the status of the reply
     *
     * Postcondition: Any error while reading the reply is marked by
     * raising an exception. LOCATION_FORWARDED replies are marked by
     * returning a restart since that is what needed to be done by the
     * callee.
     */
    TAO_InputCDR &cdr = rd.reply_cdr ();

    // Set the translators
    this->resolver_.transport ()->assign_translators (&cdr, 0);

    // At this point it can be assumed that the GIOP/whatever protocol
    // header and the reply header are already handled.  Further it
    // can be assumed that the reply body contains the details
    // required for further processing. All the other details should
    // have been handled in the reply dispatcher/protocol framework.
    switch (rd.reply_status ())
      {
      case GIOP::NO_EXCEPTION:
        {
          Reply_Guard mon (this, TAO_INVOKE_FAILURE);
          if (this->details_.demarshal_args (cdr) == false)
            {
              throw ::CORBA::MARSHAL ();
            }

          mon.set_status (TAO_INVOKE_SUCCESS);
        }
        break;
      case GIOP::LOCATION_FORWARD:
        return this->location_forward (cdr);
      case GIOP::LOCATION_FORWARD_PERM:
        {
          // Unmarshal the location forward object and set the
          // variable this->forward_to_.
          Invocation_Status const s = this->location_forward (cdr);
          if (s != TAO_INVOKE_FAILURE)
            {
              // de-marshalling of permanent object reference was successfull
              CORBA::Boolean const permanent_forward_condition =
                this->stub ()->orb_core ()->is_permanent_forward_condition
                  (this->forwarded_to_.in (),
                   this->request_service_context ());

              if (!permanent_forward_condition)
                {
                   // permanent condition not given
                    if (TAO_debug_level > 3)
                        ACE_DEBUG ((LM_DEBUG,
                                    ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::")
                                    ACE_TEXT ("check_reply_status: unexpected ")
                                    ACE_TEXT ("LOCATION_FORWARD_PERM reply\n")));

                   throw ::CORBA::INTERNAL (0, CORBA::COMPLETED_NO);
                }

              // This is the only place where we set in permanent forward state.
              this->reply_status (GIOP::LOCATION_FORWARD_PERM);
            }

          return s;
        }
      case GIOP::USER_EXCEPTION:
        return this->handle_user_exception (cdr);
      case GIOP::SYSTEM_EXCEPTION:
        return this->handle_system_exception (cdr);

      case GIOP::NEEDS_ADDRESSING_MODE:
        {
          Reply_Guard mon (this, TAO_INVOKE_FAILURE);
          // We have received a message with a request to change the
          // addressing mode. First let us read the mode that the
          // server/agent asks for.
          CORBA::Short addr_mode = 0;

          if (cdr.read_short (addr_mode) == 0)
            {
              // Could not demarshal the addressing disposition, raise an local
              // CORBA::MARSHAL
              throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
            }

          // Now set this addressing mode in the profile, so that
          // the next invocation need not go through this.
          this->resolver_.profile ()->addressing_mode (addr_mode);

          mon.set_status (TAO_INVOKE_RESTART);

          // Now restart the invocation
          return TAO_INVOKE_RESTART;
        }
    }
    return TAO_INVOKE_SUCCESS;
  }

  Invocation_Status
  Synch_Twoway_Invocation::location_forward (TAO_InputCDR &inp_stream)
  {
    Reply_Guard mon (this, TAO_INVOKE_FAILURE);

    if (TAO_debug_level > 3)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::")
                    ACE_TEXT ("location_forward being handled\n")));
      }

    CORBA::Object_var fwd;

    if (!(inp_stream >> fwd))
      {
        throw ::CORBA::MARSHAL (
          CORBA::SystemException::_tao_minor_code (
            TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
            EINVAL),
          CORBA::COMPLETED_NO);
      }

    this->forwarded_reference (fwd.in ());

    mon.set_status (TAO_INVOKE_RESTART);

    return TAO_INVOKE_RESTART;
  }

  Invocation_Status
  Synch_Twoway_Invocation::handle_user_exception (TAO_InputCDR &cdr)
  {
    Reply_Guard mon (this,
                     TAO_INVOKE_FAILURE);

    if (TAO_debug_level > 3)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::")
                  ACE_TEXT ("handle_user_exception\n")));

    // Pull the exception from the stream.
    CORBA::String_var buf;

    if (!(cdr >> buf.inout ()))
      {
        // Could not demarshal the exception id, raise an local
        // CORBA::MARSHAL
        throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
      }

    CORBA::Exception *exception = this->details_.corba_exception (buf.in ());

    exception->_tao_decode (cdr);

    if (TAO_debug_level > 5)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::")
                    ACE_TEXT ("handle_user_exception - ")
                    ACE_TEXT ("raising exception %C\n"),
                    buf.in ()));
      }

    mon.set_status (TAO_INVOKE_USER_EXCEPTION);

    // We must manage the memory allocated
    // by the call above to alloc().
    auto_ptr<CORBA::Exception> safety (exception);

    exception->_raise ();

    return TAO_INVOKE_USER_EXCEPTION;
  }

  Invocation_Status
  Synch_Twoway_Invocation::handle_system_exception (TAO_InputCDR &cdr)
  {
    Reply_Guard mon (this, TAO_INVOKE_FAILURE);

    if (TAO_debug_level > 3)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::")
                  ACE_TEXT ("handle_system_exception\n")));

    CORBA::String_var type_id;

    if (!(cdr >> type_id.inout ()))
      {
        // Could not demarshal the exception id, raise an local
        // CORBA::MARSHAL
        throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
      }

    CORBA::ULong minor = 0;
    CORBA::ULong completion = 0;

    if (!(cdr >> minor) || !(cdr >> completion))
      {
        throw ::CORBA::MARSHAL (0, CORBA::COMPLETED_MAYBE);
      }

    bool retry_on_exception = false;
    bool do_forward = false;

    const TAO_ORB_Parameters *orb_params = this->stub ()->orb_core ()->orb_params ();

    if (this->retry_state_ &&
        this->retry_state_->forward_on_exception_limit_used () &&
        (CORBA::CompletionStatus) completion == CORBA::COMPLETED_NO)
      {
        if ((ACE_OS_String::strcmp (type_id.in (),
                                   "IDL:omg.org/CORBA/TRANSIENT:1.0") == 0 &&
             this->retry_state_->forward_on_exception_increment (TAO::FOE_TRANSIENT)) ||
            (ACE_OS_String::strcmp (type_id.in (),
                                   "IDL:omg.org/CORBA/COMM_FAILURE:1.0") == 0 &&
             this->retry_state_->forward_on_exception_increment (TAO::FOE_COMM_FAILURE)) ||
            (ACE_OS_String::strcmp (type_id.in (),
                                   "IDL:omg.org/CORBA/OBJECT_NOT_EXIST:1.0") == 0 &&
             this->retry_state_->forward_on_exception_increment (TAO::FOE_OBJECT_NOT_EXIST)) ||
            (ACE_OS_String::strcmp (type_id.in (),
                                   "IDL:omg.org/CORBA/INV_OBJREF:1.0") == 0 &&
             this->retry_state_->forward_on_exception_increment (TAO::FOE_INV_OBJREF))
            )
          {
            retry_on_exception = true;
            this->retry_state_->sleep_at_starting_profile (*this->stub ());
          }
      }
    else
      {
        int foe_kind = orb_params->forward_once_exception();

        retry_on_exception =
          (CORBA::CompletionStatus) completion != CORBA::COMPLETED_YES
          && (((foe_kind & TAO::FOE_TRANSIENT) == 0
               && ACE_OS_String::strcmp (type_id.in (),
                                         "IDL:omg.org/CORBA/TRANSIENT:1.0") == 0) ||
              ACE_OS_String::strcmp (type_id.in (),
                                     "IDL:omg.org/CORBA/OBJ_ADAPTER:1.0") == 0 ||
              ACE_OS_String::strcmp (type_id.in (),
                                     "IDL:omg.org/CORBA/NO_RESPONSE:1.0") == 0 ||
              ((foe_kind & TAO::FOE_COMM_FAILURE) == 0
               && ACE_OS_String::strcmp (type_id.in (),
                                         "IDL:omg.org/CORBA/COMM_FAILURE:1.0") == 0) ||
              (orb_params->forward_invocation_on_object_not_exist ()
               && ACE_OS_String::strcmp (type_id.in (),
                                         "IDL:omg.org/CORBA/OBJECT_NOT_EXIST:1.0") == 0) ||
              (do_forward = ! this->stub ()->forwarded_on_exception ()
               && ((((foe_kind & TAO::FOE_OBJECT_NOT_EXIST) == TAO::FOE_OBJECT_NOT_EXIST)
                    && (ACE_OS_String::strcmp (type_id.in (),
                                               "IDL:omg.org/CORBA/OBJECT_NOT_EXIST:1.0") == 0)) ||
                   (((foe_kind & TAO::FOE_COMM_FAILURE) == TAO::FOE_COMM_FAILURE)
                    && (ACE_OS_String::strcmp (type_id.in (),
                                               "IDL:omg.org/CORBA/COMM_FAILURE:1.0") == 0)) ||
                   (((foe_kind & TAO::FOE_TRANSIENT) == TAO::FOE_TRANSIENT)
                    && (ACE_OS_String::strcmp (type_id.in (),
                                               "IDL:omg.org/CORBA/TRANSIENT:1.0") == 0)) ||
                   (((foe_kind & TAO::FOE_INV_OBJREF) == TAO::FOE_INV_OBJREF)
                    && (ACE_OS_String::strcmp (type_id.in (),
                                               "IDL:omg.org/CORBA/INV_OBJREF:1.0") == 0)))));
      }

    if (retry_on_exception)
      {
        // If we are here then possibly we'll need a restart.
        mon.set_status (TAO_INVOKE_RESTART);

        if (TAO_debug_level > 4)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::")
                      ACE_TEXT ("handle_system_exception, profile forwarding ")
                      ACE_TEXT ("on exception "),
                      type_id.in (),
                      ACE_TEXT ("\n")));

        if (do_forward)
          this->stub ()->forwarded_on_exception (true);

        // Start the special case for FTCORBA.
        /**
          * There has been a unanimous view that this is not the
          * right way to do things. But a need to be compliant is
          * forcing us into this.
          */
        Invocation_Status const s =
          this->stub ()->orb_core ()->service_raise_transient_failure (
            this->details_.request_service_context ().service_info (),
            this->resolver_.profile ());

        if (s == TAO_INVOKE_RESTART)
          return s;

        // Attempt profile retry.
        /**
         * @note A location forwarding loop may occur where a client
         *       is bounced from the original target to the forwarded
         *       target and back if the application is not equipped to
         *       handle retries of previously called targets.  TAO may
         *       be able to help in this case but it ultimately ends
         *       up being an application issue.
         */
        if (completion != CORBA::COMPLETED_MAYBE &&
            this->resolver_.stub ()->next_profile_retry ())
          {
            return TAO_INVOKE_RESTART;
          }

        // Fall through and raise an exception.
        mon.set_status (TAO_INVOKE_FAILURE);
      }

    CORBA::SystemException *ex = TAO::create_system_exception (type_id.in ());

    if (ex == 0)
      {
        // @@ We should raise a CORBA::NO_MEMORY, but we ran out
        //    of memory already. We need a pre-allocated, TSS,
        //    CORBA::NO_MEMORY instance
        ACE_NEW_RETURN (ex,
                        CORBA::UNKNOWN,
                        TAO_INVOKE_FAILURE);
      }

    // Without this, the call to create_system_exception() above
    // causes a memory leak. On platforms without native exceptions,
    // the CORBA::Environment class manages the memory.
    auto_ptr<CORBA::SystemException> safety (ex);

    ex->minor (minor);
    ex->completed (CORBA::CompletionStatus (completion));

    if (TAO_debug_level > 4)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Synch_Twoway_Invocation::")
                  ACE_TEXT ("handle_system_exception, about to raise\n")));

    mon.set_status (TAO_INVOKE_SYSTEM_EXCEPTION);

    // Raise the exception.
    ex->_raise ();

    return TAO_INVOKE_SYSTEM_EXCEPTION;
  }

  // =========================================================================

  Synch_Oneway_Invocation::Synch_Oneway_Invocation (
    CORBA::Object_ptr otarget,
    Profile_Transport_Resolver &r,
    TAO_Operation_Details &d)
    : Synch_Twoway_Invocation (otarget, r, d, false)
  {
  }

  Invocation_Status
  Synch_Oneway_Invocation::remote_oneway (ACE_Time_Value *max_wait_time)
  {
    TAO::ORB_Countdown_Time countdown (max_wait_time);

    CORBA::Octet const response_flags = this->details_.response_flags ();

    Invocation_Status s = TAO_INVOKE_FAILURE;

    if (response_flags == CORBA::Octet (Messaging::SYNC_WITH_SERVER) ||
        response_flags == CORBA::Octet (Messaging::SYNC_WITH_TARGET))
      {
        s = Synch_Twoway_Invocation::remote_twoway (max_wait_time);

        return s;
      }

#if TAO_HAS_INTERCEPTORS == 1
    s = this->send_request_interception ();

    if (s != TAO_INVOKE_SUCCESS)
      return s;

    try
      {
#endif /*TAO_HAS_INTERCEPTORS */
        TAO_Transport* const transport = this->resolver_.transport ();

        if (!transport)
          {
            if (this->retry_state_ &&
                this->retry_state_->forward_on_exception_increment(FOE_TRANSIENT))
              {
                if (TAO_debug_level > 0)
                  ACE_DEBUG ((LM_INFO,
                              ACE_TEXT ("TAO (%P|%t) - Synch_Oneway_Invocation::")
                              ACE_TEXT ("remote_oneway retrying on TRANSIENT ")
                              ACE_TEXT ("exception\n")));
                this->retry_state_->next_profile_retry (*this->stub ());
                return TAO_INVOKE_RESTART;
              }
            else
              {
                // Way back, we failed to find a profile we could connect to.
                // We've come this far only so we reach the interception points
                // in case they can fix things. Time to bail....
                throw CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
              }

          }
        else
          {
            ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, transport->output_cdr_lock (),
                              TAO_INVOKE_FAILURE);

            TAO_OutputCDR &cdr = transport->out_stream ();

            cdr.message_attributes (this->details_.request_id (),
                                    this->resolver_.stub (),
                                    TAO_Message_Semantics (TAO_Message_Semantics::
                                                           TAO_ONEWAY_REQUEST),
                                    max_wait_time);

            this->write_header (cdr);

            this->marshal_data (cdr);

            countdown.update ();

            if (transport->is_connected ())
              {
                // We have a connected transport so we can send the message
                s = this->send_message (cdr,
                                        TAO_Message_Semantics (TAO_Message_Semantics::
                                                               TAO_ONEWAY_REQUEST),
                                        max_wait_time);

                if (transport->wait_strategy ()->non_blocking () == 0 &&
                    transport->orb_core ()->client_factory ()->use_cleanup_options ())
                  {
                    if (!transport->wait_strategy ()->is_registered())
                      {
                        ACE_Event_Handler * const eh =
                          transport->event_handler_i ();

                        ACE_Reactor * const r =
                          transport->orb_core ()->reactor ();

                        if (r->register_handler (eh, ACE_Event_Handler::READ_MASK) == -1)
                          {
                            if (TAO_debug_level > 0)
                              ACE_ERROR ((LM_ERROR,
                                          ACE_TEXT ("TAO (%P|%t) - Synch_Oneway_Invocation::")
                                          ACE_TEXT ("remote_oneway transport[%d] ")
                                          ACE_TEXT ("registration withreactor ")
                                          ACE_TEXT ("returned an error\n"),
                                          transport->id ()));
                          }
                        else
                          {
                            // Only set this flag when registration succeeds
                            transport->wait_strategy ()->is_registered (true);
                          }
                      }
                  }

              }
            else
              {
                if (TAO_debug_level > 4)
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) - Synch_Oneway_Invocation::")
                              ACE_TEXT ("remote_oneway, queueing message\n")));

                if (transport->format_queue_message (cdr,
                                                     max_wait_time,
                                                     this->resolver_.stub()) != 0)
                  {
                    s = TAO_INVOKE_FAILURE;
                  }
              }
          }

#if TAO_HAS_INTERCEPTORS == 1
        s = this->receive_other_interception ();
      }
    catch ( ::CORBA::Exception& ex)
      {
        PortableInterceptor::ReplyStatus const status =
          this->handle_any_exception (&ex);

        if (status == PortableInterceptor::LOCATION_FORWARD ||
            status == PortableInterceptor::TRANSPORT_RETRY)
          s = TAO_INVOKE_RESTART;
        else if (status == PortableInterceptor::SYSTEM_EXCEPTION
                 || status == PortableInterceptor::USER_EXCEPTION)
          throw;
      }
    catch (...)
      {
        // Notify interceptors of non-CORBA exception, and propagate
        // that exception to the caller.

        PortableInterceptor::ReplyStatus const st =
          this->handle_all_exception ();

        if (st == PortableInterceptor::LOCATION_FORWARD ||
            st == PortableInterceptor::TRANSPORT_RETRY)
          s = TAO_INVOKE_RESTART;
        else
          throw;
      }
#endif /* TAO_HAS_INTERCEPTORS */

    return s;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
