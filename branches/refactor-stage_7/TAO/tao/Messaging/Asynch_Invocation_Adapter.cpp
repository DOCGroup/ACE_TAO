//$Id$
#include "Asynch_Invocation_Adapter.h"
#include "Asynch_Reply_Dispatcher.h"
#include "Asynch_Invocation.h"

#include "tao/Profile_Transport_Resolver.h"
#include "tao/operation_details.h"
#include "tao/Stub.h"
#include "tao/Transport.h"
#include "tao/Muxed_TMS.h"
#include "tao/ORB_Constants.h"
#include "tao/debug.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (Messaging,
           Asynch_Invocation_Adapter,
           "$Id$")


namespace TAO
{
  Asynch_Invocation_Adapter::Asynch_Invocation_Adapter (CORBA::Object *target,
                                                        Argument **args,
                                                        int arg_number,
                                                        char *operation,
                                                        int op_len,
                                                        Collocation_Proxy_Broker *p,
                                                        Invocation_Mode m)
    : Invocation_Adapter (target,
                          args,
                          arg_number,
                          operation,
                          op_len,
                          p,
                          TAO_TWOWAY_INVOCATION,
                          m)
      , rd_ (0)
  {
  }

  void
  Asynch_Invocation_Adapter::invoke (
      Messaging::ReplyHandler_ptr reply_handler_ptr,
      const TAO_Reply_Handler_Skeleton &reply_handler_skel
      ACE_ENV_ARG_DECL)
  {
    TAO_Stub *stub =
      this->get_stub (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    if (TAO_debug_level >= 4)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "TAO_Messaging (%P|%t) - Asynch_Invocation_Adapter::"
                    "invoke\n"));
      }

    // If the reply handler is nil, we do not create a reply dispatcher.
    // The ORB will drop replies to which it cannot associate a reply
    // dispatcher.
    if (!CORBA::is_nil (reply_handler_ptr))
      {
        // New reply dispatcher on the heap, because
        // we will go out of scope and hand over the reply dispatcher
        // to the ORB.

        // @@ Need to use memory pool here..
        ACE_NEW_THROW_EX (this->rd_,
                          TAO_Asynch_Reply_Dispatcher (reply_handler_skel,
                                                       reply_handler_ptr,
                                                       stub->orb_core ()),
                          CORBA::NO_MEMORY ());
      }

    Invocation_Adapter::invoke (0, 0 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }


  Invocation_Status
  Asynch_Invocation_Adapter::invoke_twoway (
      TAO_Operation_Details &op,
      CORBA::Object *&effective_target,
      Profile_Transport_Resolver &r,
      ACE_Time_Value *&max_wait_time
      ACE_ENV_ARG_DECL)
  {
    // Simple sanity check
    if (this->mode_ != TAO_ASYNCHRONOUS_CALLBACK_INVOCATION ||
        this->type_ != TAO_TWOWAY_INVOCATION)
      {
        ACE_THROW_RETURN (CORBA::INTERNAL (
            CORBA::SystemException::_tao_minor_code (
                TAO_DEFAULT_MINOR_CODE,
                EINVAL),
            CORBA::COMPLETED_NO),
                          TAO_INVOKE_FAILURE);
      }

    auto_ptr<TAO_Asynch_Reply_Dispatcher> safe_rd (this->rd_);

    if (this->rd_)
      {
        // Cache the  transport in the reply dispatcher
        this->rd_->transport (r.transport ());

        // AMI Timeout Handling Begin
        ACE_Time_Value tmp;

        if (this->get_timeout (tmp))
          {
            this->rd_->schedule_timer (op.request_id (),
                                       *max_wait_time);
          }
      }

    TAO::Asynch_Remote_Invocation asynch (effective_target,
                                          r,
                                          op,
                                          this->rd_);
    Invocation_Status s =
      asynch.remote_invocation (max_wait_time
                                ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (s != TAO_INVOKE_FAILURE)
      (void) safe_rd.release ();

    if (s == TAO_INVOKE_RESTART &&
        asynch.is_forwarded ())
      {
        effective_target = asynch.steal_forwarded_reference ();

        this->object_forwarded (effective_target,
                                r.stub ()
                                ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);
      }

    return s;
  }

} // End namespace TAO


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr<CORBA::SystemException>;
template class auto_ptr<CORBA::Exception>;
#  if defined (ACE_LACKS_AUTO_PTR) \
      || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
           && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
template class ACE_Auto_Basic_Ptr<CORBA::SystemException>;
template class ACE_Auto_Basic_Ptr<CORBA::Exception>;
#  endif  /* ACE_LACKS_AUTO_PTR */

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr<CORBA::SystemException>
#pragma instantiate auto_ptr<CORBA::Exception>
#  if defined (ACE_LACKS_AUTO_PTR) \
      || !(defined (ACE_HAS_STANDARD_CPP_LIBRARY) \
           && (ACE_HAS_STANDARD_CPP_LIBRARY != 0))
#    pragma instantiate ACE_Auto_Basic_Ptr<CORBA::SystemException>
#    pragma instantiate ACE_Auto_Basic_Ptr<CORBA::Exception>
#  endif  /* ACE_LACKS_AUTO_PTR */

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
