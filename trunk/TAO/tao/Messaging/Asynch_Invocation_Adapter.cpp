//$Id$
#include "tao/Messaging/Asynch_Invocation_Adapter.h"
#include "tao/Messaging/Asynch_Reply_Dispatcher.h"
#include "tao/Messaging/Asynch_Invocation.h"

#include "tao/Profile_Transport_Resolver.h"
#include "tao/operation_details.h"
#include "tao/Stub.h"
#include "tao/Transport.h"
#include "tao/Muxed_TMS.h"
#include "tao/ORB_Constants.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/GIOP_Utils.h"


ACE_RCSID (Messaging,
           Asynch_Invocation_Adapter,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Asynch_Invocation_Adapter::Asynch_Invocation_Adapter (
    CORBA::Object *target,
    Argument **args,
    int arg_number,
    const char *operation,
    size_t op_len,
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
    , safe_rd_ ()
  {
  }

  void
  Asynch_Invocation_Adapter::invoke (
    Messaging::ReplyHandler_ptr reply_handler_ptr,
    const TAO_Reply_Handler_Skeleton &reply_handler_skel
    ACE_ENV_ARG_DECL)
  {
    TAO_Stub * stub =
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
        // New reply dispatcher on the heap or allocator, because
        // we will go out of scope and hand over the reply dispatcher
        // to the ORB.

        TAO_Asynch_Reply_Dispatcher *rd = 0;

        // Get the allocator we could use.
        ACE_Allocator* ami_allocator =
          stub->orb_core ()->lane_resources().ami_response_handler_allocator();

        // If we have an allocator, use it, else use the heap.
        if (ami_allocator)
          {
            ACE_NEW_MALLOC (
              rd,
              static_cast<TAO_Asynch_Reply_Dispatcher *> (
                ami_allocator->malloc (sizeof (TAO_Asynch_Reply_Dispatcher))),
              TAO_Asynch_Reply_Dispatcher (reply_handler_skel,
                                           reply_handler_ptr,
                                           stub->orb_core (),
                                           ami_allocator));
          }
        else
          {
            ACE_NEW (rd,
                     TAO_Asynch_Reply_Dispatcher (reply_handler_skel,
                                                  reply_handler_ptr,
                                                  stub->orb_core (),
                                                  0));
          }

        if (rd == 0)
          {
            ACE_THROW (CORBA::NO_MEMORY ());
          }

        this->safe_rd_.reset (rd);
      }

    Invocation_Adapter::invoke (0, 0 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  Asynch_Invocation_Adapter::invoke (
    TAO::Exception_Data *ex,
    unsigned long ex_count
    ACE_ENV_ARG_DECL)
  {
    Invocation_Adapter::invoke (ex, ex_count  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  Invocation_Status
  Asynch_Invocation_Adapter::invoke_twoway (
    TAO_Operation_Details &op,
    CORBA::Object_var &effective_target,
    Profile_Transport_Resolver &r,
    ACE_Time_Value *&max_wait_time
    ACE_ENV_ARG_DECL)
  {
    // Simple sanity check
    if (this->mode_ != TAO_ASYNCHRONOUS_CALLBACK_INVOCATION
        || this->type_ != TAO_TWOWAY_INVOCATION)
      {
        ACE_THROW_RETURN (CORBA::INTERNAL (
            CORBA::SystemException::_tao_minor_code (
                TAO::VMCID,
                EINVAL),
            CORBA::COMPLETED_NO),
                          TAO_INVOKE_FAILURE);
      }

    if (this->safe_rd_.get ())
      {
        // Cache the  transport in the reply dispatcher
        this->safe_rd_->transport (r.transport ());

        // AMI Timeout Handling Begin
        ACE_Time_Value tmp;

        if (this->get_timeout (r.stub (),
                               tmp))
          {
            this->safe_rd_->schedule_timer (
                op.request_id (),
                *max_wait_time
                ACE_ENV_ARG_PARAMETER);
            ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);
          }
      }

    // Loose ownership of the reply dispatcher
    TAO::Asynch_Remote_Invocation asynch (
       effective_target.in (),
       r,
       op,
       this->safe_rd_.release ());

    Invocation_Status s =
      asynch.remote_invocation (max_wait_time
                                ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (s == TAO_INVOKE_RESTART &&
        asynch.is_forwarded ())
      {
        effective_target = asynch.steal_forwarded_reference ();

#if TAO_HAS_INTERCEPTORS == 1
        const CORBA::Boolean permanent_forward =
            (asynch.reply_status() == TAO_GIOP_LOCATION_FORWARD_PERM);
#else
        const CORBA::Boolean permanent_forward = false;
#endif

        this->object_forwarded (effective_target,
                                r.stub (),
                                permanent_forward
                                ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);
      }

    return s;
  }

} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL
