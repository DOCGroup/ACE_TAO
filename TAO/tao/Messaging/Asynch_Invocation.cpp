//$Id$
#include "Asynch_Invocation.h"
#include "Asynch_Reply_Dispatcher.h"

#include "tao/Profile_Transport_Resolver.h"
#include "tao/Invocation_Utils.h"
#include "tao/operation_details.h"
#include "tao/Bind_Dispatcher_Guard.h"
#include "tao/Transport.h"
#include "tao/Muxed_TMS.h"
#include "tao/Pluggable_Messaging.h"
#include "tao/ORB_Constants.h"

ACE_RCSID (tao,
           Synch_Invocation,
           "$Id$")

namespace TAO
{
  Asynch_Remote_Invocation::Asynch_Remote_Invocation (
      CORBA::Object_ptr otarget,
      Profile_Transport_Resolver &resolver,
      TAO_Operation_Details &detail,
      TAO_Asynch_Reply_Dispatcher_Base *rd,
      bool response_expected)
    : Synch_Twoway_Invocation (otarget,
                               resolver,
                               detail,
                               response_expected)
      , rd_ (rd)
  {
  }

  Invocation_Status
  Asynch_Remote_Invocation::remote_invocation (ACE_Time_Value *max_wait_time
                                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::Exception))
  {
    // Register a reply dispatcher for this invocation. Use the
    // preallocated reply dispatcher.
    TAO_Bind_Dispatcher_Guard dispatch_guard (this->details_.request_id (),
                                              this->rd_,
                                              this->resolver_.transport ()->tms ());


    if (dispatch_guard.status () != 0)
      {
        // @@ What is the right way to handle this error? Do we need
        // to call the interceptors in this case?
        ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                           CORBA::COMPLETED_NO),
                          TAO_INVOKE_FAILURE);
      }

    TAO_Target_Specification tspec;
    this->init_target_spec (tspec ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    Invocation_Status s = TAO_INVOKE_FAILURE;

#if TAO_HAS_INTERCEPTORS == 1
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
    // try block is to take care of the cases when things go wrong.
    ACE_TRY
      {
        this->write_header (tspec,
                            cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        this->marshal_data (cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        s =
          this->send_message (cdr,
                              TAO_Transport::TAO_TWOWAY_REQUEST,
                              max_wait_time
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

#if TAO_HAS_INTERCEPTORS == 1
        // Nothing great on here. If we get a restart during send or a
        // proper send, we are supposed to call receiver_other ()
        // interception point. So we do that here
        Invocation_Status tmp =
          this->receive_other_interception (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        // We got an error during the interception.
        if (s == TAO_INVOKE_SUCCESS && tmp != TAO_INVOKE_SUCCESS)
          s = tmp;
#endif /*TAO_HAS_INTERCEPTORS */

        // If an error occurred just return. At this point all the
        // endpoint interception would have been invoked. The callee
        // wold take care of the rest.
        if (s != TAO_INVOKE_SUCCESS)
          return s;

        // Do not unbind during destruction. We need the entry to be
        // there in the map since the reply dispctaher depends on
        // that.
        dispatch_guard.status (TAO_Bind_Dispatcher_Guard::NO_UNBIND);

        // NOTE: Not sure how things are handles with exclusive muxed
        // strategy.
        if (this->resolver_.transport ()->idle_after_send ())
          (void) this->resolver_.transport_released ();

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
}
