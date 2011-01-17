// -*- C++ -*-
// $Id$

#include "tao/Messaging/Asynch_Invocation_Adapter.h"
#include "tao/Messaging/Asynch_Reply_Dispatcher.h"
#include "tao/Messaging/Asynch_Invocation.h"
#include "tao/Messaging/AMI_Arguments_Converter_Impl.h"

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
    const TAO_Reply_Handler_Stub &reply_handler_stub)
  {
    TAO_Stub * stub =
      this->get_stub ();

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
              TAO_Asynch_Reply_Dispatcher (reply_handler_stub,
                                           reply_handler_ptr,
                                           stub->orb_core (),
                                           ami_allocator));
          }
        else
          {
            ACE_NEW (rd,
                     TAO_Asynch_Reply_Dispatcher (reply_handler_stub,
                                                  reply_handler_ptr,
                                                  stub->orb_core (),
                                                  0));
          }

        if (rd == 0)
          {
            throw ::CORBA::NO_MEMORY ();
          }

        this->safe_rd_.reset (rd);
      }

    Invocation_Adapter::invoke (0, 0);
  }

  void
  Asynch_Invocation_Adapter::invoke (
    TAO::Exception_Data *ex,
    unsigned long ex_count
    )
  {
    Invocation_Adapter::invoke (ex, ex_count );
  }

  Invocation_Status
  Asynch_Invocation_Adapter::invoke_collocated_i (
    TAO_Stub *stub,
    TAO_Operation_Details &details,
    CORBA::Object_var &effective_target,
    Collocation_Strategy strat)
  {
    if (stub->orb_core ()->orb_params ()->ami_collication ())
      {
        // When doing a collocation asynch invocation we shouldn't use the
        // stub args but use the skel args
        details.use_stub_args (false);

        TAO_AMI_Arguments_Converter_Impl* ami_arguments_converter
          = ACE_Dynamic_Service<TAO_AMI_Arguments_Converter_Impl>::instance (
            "AMI_Arguments_Converter");
        details.cac (ami_arguments_converter);

        // Release the owner ship of the reply dispatcher
        details.reply_dispatcher (this->safe_rd_.release ());

        return Invocation_Adapter::invoke_collocated_i (stub,
                                                        details,
                                                        effective_target,
                                                        strat);
      }
    else
      {
        ACE_Time_Value *max_wait_time = 0;
        return Invocation_Adapter::invoke_remote_i (stub,
                                                    details,
                                                    effective_target,
                                                    max_wait_time);
      }
  }

  Invocation_Status
  Asynch_Invocation_Adapter::invoke_twoway (
    TAO_Operation_Details &op,
    CORBA::Object_var &effective_target,
    Profile_Transport_Resolver &r,
    ACE_Time_Value *&max_wait_time)
  {
    // Simple sanity check
    if (this->mode_ != TAO_ASYNCHRONOUS_CALLBACK_INVOCATION
        || this->type_ != TAO_TWOWAY_INVOCATION)
      {
        throw ::CORBA::INTERNAL (
          CORBA::SystemException::_tao_minor_code (
            TAO::VMCID,
            EINVAL),
          CORBA::COMPLETED_NO);
      }

    if (this->safe_rd_.get () && r.transport ())
      {
        this->safe_rd_->transport (r.transport ());
        // AMI Timeout Handling Begin
        ACE_Time_Value tmp;

        if (this->get_timeout (r.stub (), tmp))
          {
            this->safe_rd_->schedule_timer (op.request_id (), *max_wait_time);
          }
      }

    // Loose ownership of the reply dispatcher
    TAO::Asynch_Remote_Invocation asynch (
       effective_target.in (),
       r,
       op,
       this->safe_rd_.release ());

    Invocation_Status const s = asynch.remote_invocation (max_wait_time);

    if (s == TAO_INVOKE_RESTART &&
        (asynch.reply_status () == GIOP::LOCATION_FORWARD ||
         asynch.reply_status () == GIOP::LOCATION_FORWARD_PERM))
      {
        CORBA::Boolean const permanent_forward =
          (asynch.reply_status () == GIOP::LOCATION_FORWARD_PERM);

        effective_target = asynch.steal_forwarded_reference ();

        this->object_forwarded (effective_target, r.stub (), permanent_forward);
      }

    return s;
  }

} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL
