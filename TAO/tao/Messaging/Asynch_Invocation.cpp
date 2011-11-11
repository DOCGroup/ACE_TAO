// -*- C++ -*-
// $Id$

#include "tao/Messaging/Asynch_Invocation.h"
#include "tao/Messaging/Asynch_Reply_Dispatcher.h"

#include "tao/Profile_Transport_Resolver.h"
#include "tao/Invocation_Utils.h"
#include "tao/operation_details.h"
#include "tao/Bind_Dispatcher_Guard.h"
#include "tao/Transport.h"
#include "tao/Muxed_TMS.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/ORB_Constants.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/PortableInterceptorC.h"
#endif /*TAO_HAS_INTERCEPTORS */

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
  Asynch_Remote_Invocation::remote_invocation (ACE_Time_Value *max_wait_time)
  {
    Invocation_Status s = TAO_INVOKE_FAILURE;

#if TAO_HAS_INTERCEPTORS == 1
    s = this->send_request_interception ();

    if (s != TAO_INVOKE_SUCCESS)
      return s;

    // We have started the interception flow. We need to call the
    // ending interception flow if things go wrong. The purpose of the
    // try block is to take care of the cases when things go wrong.
    try
      {
#endif /* TAO_HAS_INTERCEPTORS */
        TAO_Transport* const transport = this->resolver_.transport ();

        if (!transport)
          {
            // Way back, we failed to find a profile we could connect to.
            // We've come this far only so we reach the interception points
            // in case they can fix things. Time to bail....
            throw CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
          }

        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon,
                          transport->output_cdr_lock (), TAO_INVOKE_FAILURE);

        TAO_OutputCDR & cdr =
          this->resolver_.transport ()->messaging_object ()->out_stream ();

        // Oneway semantics.  See comments for below send_message()
        // call.
        cdr.message_attributes (this->details_.request_id (),
                                this->resolver_.stub (),
                                TAO_ONEWAY_REQUEST,
                                max_wait_time);

        this->write_header (cdr);

        this->marshal_data (cdr);

        // Register a reply dispatcher for this invocation. Use the
        // preallocated reply dispatcher.
        TAO_Bind_Dispatcher_Guard dispatch_guard (
          this->details_.request_id (),
          this->safe_rd_.get (),
          transport->tms ());

        // Now that we have bound the reply dispatcher to the map, just
        // loose ownership of the reply dispatcher.
        this->safe_rd_.release ();

        if (dispatch_guard.status () != 0)
          {
            // @@ What is the right way to handle this error? Do we need
            // to call the interceptors in this case?
            throw ::CORBA::INTERNAL (TAO::VMCID, CORBA::COMPLETED_NO);
          }

        // Do not unbind during destruction. We need the entry to be
        // there in the map since the reply dispatcher depends on
        // that. This is also a trigger to loose the ownership of the
        // reply dispatcher.
        dispatch_guard.status (TAO_Bind_Dispatcher_Guard::NO_UNBIND);

        // Send it as a oneway request. It will make all the required
        // paraphernalia within the ORB to fire, like buffering if
        // send blocks etc.
        s = this->send_message (cdr,
                                TAO_ONEWAY_REQUEST,
                                max_wait_time);

        ace_mon.release();

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
        Invocation_Status const tmp = this->receive_other_interception ();

        // We got an error during the interception.
        if (s == TAO_INVOKE_SUCCESS && tmp != TAO_INVOKE_SUCCESS)
          s = tmp;
#endif /* TAO_HAS_INTERCEPTORS */

        // If an error occurred just return. At this point all the
        // endpoint interception would have been invoked. The callee
        // would take care of the rest.
        if (s != TAO_INVOKE_SUCCESS)
          return s;

        // transport strategy takes care of idling transport or not
        transport->idle_after_send ();
        // release transport from resolver in any case since we don't
        // want the resolver to make the transport idle if the strategy
        // told it not to
        this->resolver_.transport_released ();

#if TAO_HAS_INTERCEPTORS == 1
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
