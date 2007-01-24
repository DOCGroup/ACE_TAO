//$Id$

#include "tao/Messaging/Asynch_Invocation.h"
#include "tao/Messaging/Asynch_Reply_Dispatcher.h"

#include "tao/Profile_Transport_Resolver.h"
#include "tao/Invocation_Utils.h"
#include "tao/operation_details.h"
#include "tao/Bind_Dispatcher_Guard.h"
#include "tao/Transport.h"
#include "tao/Muxed_TMS.h"
#include "tao/Pluggable_Messaging.h"
#include "tao/ORB_Constants.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/PortableInterceptorC.h"
#endif /*TAO_HAS_INTERCEPTORS */

ACE_RCSID (Messaging,
           Asynch_Invocation,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
      , safe_rd_ (rd)
  {
  }

  Invocation_Status
  Asynch_Remote_Invocation::remote_invocation (ACE_Time_Value *max_wait_time
                                               )
    ACE_THROW_SPEC ((CORBA::Exception))
  {
    TAO_Target_Specification tspec;
    this->init_target_spec (tspec);

    TAO_OutputCDR &cdr =
      this->resolver_.transport ()->messaging_object ()->out_stream ();

    Invocation_Status s = TAO_INVOKE_FAILURE;

#if TAO_HAS_INTERCEPTORS == 1
    s =
      this->send_request_interception ();

    if (s != TAO_INVOKE_SUCCESS)
      return s;
#endif /*TAO_HAS_INTERCEPTORS */

    // We have started the interception flow. We need to call the
    // ending interception flow if things go wrong. The purpose of the
    // try block is to take care of the cases when things go wrong.
    try
      {
        // Oneway semantics.  See comments for below send_message()
        // call.
        cdr.message_attributes (this->details_.request_id (),
                                this->resolver_.stub (),
                                TAO_Transport::TAO_ONEWAY_REQUEST,
                                max_wait_time);

        this->write_header (tspec,
                            cdr
                           );

        this->marshal_data (cdr
                           );

        // Register a reply dispatcher for this invocation. Use the
        // preallocated reply dispatcher.
        TAO_Bind_Dispatcher_Guard dispatch_guard (
          this->details_.request_id (),
          this->safe_rd_.get (),
          this->resolver_.transport ()->tms ());

        // Now that we have bound the reply dispatcher to the map, just
        // loose ownership of the reply dispatcher.
        this->safe_rd_.release ();

        if (dispatch_guard.status () != 0)
          {
            // @@ What is the right way to handle this error? Do we need
            // to call the interceptors in this case?
            ACE_THROW_RETURN (CORBA::INTERNAL (TAO::VMCID,
                                               CORBA::COMPLETED_NO),
                              TAO_INVOKE_FAILURE);
          }

        // Do not unbind during destruction. We need the entry to be
        // there in the map since the reply dispatcher depends on
        // that. This is also a trigger to loose the ownership of the
        // reply dispatcher.
        dispatch_guard.status (TAO_Bind_Dispatcher_Guard::NO_UNBIND);

        // Send it as a oneway request. It will make all the required
        // paraphernalia within the ORB to fire, like buffering if
        // send blocks etc.
        s =
          this->send_message (cdr,
                              TAO_Transport::TAO_ONEWAY_REQUEST,
                              max_wait_time
                             );

#if TAO_HAS_INTERCEPTORS == 1
        // NOTE: We don't need to do the auto_ptr <> trick. We got here
        // in the first place since the message was sent properly,
        // which implies a reply would be available. Therefore the
        // reply dispatcher should be available for another thread to
        // collect and dispatch the reply. In MT cases, things are
        // more hairy. Just imagine what happens when another thread
        // is collecting the reply when we are happily invoking
        // interceptors?

        // Nothing great on here. If we get a restart during send or a
        // proper send, we are supposed to call receiver_other ()
        // interception point. So we do that here
        Invocation_Status tmp =
          this->receive_other_interception ();

        // We got an error during the interception.
        if (s == TAO_INVOKE_SUCCESS && tmp != TAO_INVOKE_SUCCESS)
          s = tmp;
#endif /*TAO_HAS_INTERCEPTORS */

        // If an error occurred just return. At this point all the
        // endpoint interception would have been invoked. The callee
        // would take care of the rest.
        if (s != TAO_INVOKE_SUCCESS)
          return s;

        // NOTE: Not sure how things are handles with exclusive muxed
        // strategy.
        if (this->resolver_.transport ()->idle_after_send ())
          (void) this->resolver_.transport_released ();

      }
    catch ( ::CORBA::Exception& ex)
      {
#if TAO_HAS_INTERCEPTORS == 1
        PortableInterceptor::ReplyStatus const status =
          this->handle_any_exception (&ACE_ANY_EXCEPTION);

        if (status == PortableInterceptor::LOCATION_FORWARD ||
            status == PortableInterceptor::TRANSPORT_RETRY)
          s = TAO_INVOKE_RESTART;
        else if (status == PortableInterceptor::SYSTEM_EXCEPTION
            || status == PortableInterceptor::USER_EXCEPTION)
#endif /*TAO_HAS_INTERCEPTORS*/
          throw;
      }

    return s;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
