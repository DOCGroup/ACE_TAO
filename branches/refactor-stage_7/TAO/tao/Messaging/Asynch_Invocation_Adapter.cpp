//$Id$
#include "Asynch_Invocation_Adapter.h"
#include "Asynch_Reply_Dispatcher.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/operation_details.h"
#include "tao/Stub.h"
#include "tao/corbafwd.h"
#include "tao/Transport.h"
#include "tao/Muxed_TMS.h"
#include "Asynch_Invocation.h"

ACE_RCSID (tao,
           Invocation_Adapter,
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
  Asynch_Invocation_Adapter::invoke (Messaging::ReplyHandler_ptr reply_handler_ptr,
                                     const TAO_Reply_Handler_Skeleton &reply_handler_skel
                                     ACE_ENV_ARG_DECL)
  {
    TAO_Stub *stub =
      this->get_stub (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

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


  void
  Asynch_Invocation_Adapter::invoke_remote (TAO_Stub *stub,
                                            TAO_Operation_Details &op
                                            ACE_ENV_ARG_DECL)
  {
    ACE_Time_Value tmp_wait_time;
    bool is_timeout  =
      this->get_timeout (tmp_wait_time);

    ACE_Time_Value *max_wait_time = 0;

    if (is_timeout)
      max_wait_time = &tmp_wait_time;

    // Initial state
    // TAO::Invocation_Status status = TAO_INVOKE_START;

    Profile_Transport_Resolver resolver (this->target_,
                                         stub);
    resolver.resolve (max_wait_time
                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    // Update the request id now that we have a transport
    op.request_id (resolver.transport ()->tms ()->request_id ());

    if (this->rd_)
      {
        // Cache the  transport in the reply dispatcher
        this->rd_->transport (resolver.transport ());

        // AMI Timeout Handling Begin
        if (is_timeout)
          {
            this->rd_->schedule_timer (op.request_id (),
                                       *max_wait_time);
          }
      }

    // @@ NOTE:Need to change this to something better. Too many
    // hash defines meaning the same  thing..
    op.response_flags (TAO_TWOWAY_RESPONSE_FLAG);
    TAO::Asynch_Remote_Invocation asynch (this->target_,
                                          resolver,
                                          op,
                                          this->rd_);

    (void) asynch.remote_invocation (max_wait_time
                                     ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }


} // End namespace TAO
