// -*- C++ -*-
// $Id$

#include "tao/Invocation_Adapter.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/operation_details.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Synch_Invocation.h"
#include "tao/debug.h"
#include "tao/Collocated_Invocation.h"
#include "tao/Transport.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Collocation_Proxy_Broker.h"
#include "tao/GIOP_Utils.h"
#include "tao/TAOC.h"
#include "tao/SystemException.h"
#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
# include "tao/Invocation_Adapter.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Invocation_Adapter::~Invocation_Adapter (void)
  {
  }

  void
  Invocation_Adapter::invoke (TAO::Exception_Data *ex_data,
                              unsigned long ex_count)
  {
    // Should stub object be refcounted here?
    TAO_Stub *stub = this->get_stub ();

    TAO_Operation_Details op_details (this->operation_,
                                      this->op_len_,
                                      this->args_,
                                      this->number_args_,
                                      ex_data,
                                      ex_count);

    this->invoke_i (stub, op_details);
  }

  void
  Invocation_Adapter::invoke_i (TAO_Stub *stub, TAO_Operation_Details &details)
  {
    // The invocation has got to be within the context of the
    // corresponding ORB's configuration. Otherwise things like
    // timeout hooks, etc may not work as expected. Especially if
    // there are multiple ORB instances in the process, each with its
    // own, local configuration.
    ACE_Service_Config_Guard scg (stub->orb_core ()->configuration ());

    // Cache the target to a local variable.
    CORBA::Object_var effective_target =
      CORBA::Object::_duplicate (this->target_);

    // Initial state
    TAO::Invocation_Status status = TAO_INVOKE_START;
    ACE_Time_Value *max_wait_time = 0;
    ACE_Time_Value tmp_wait_time = ACE_Time_Value::zero;
    if (this->get_timeout (stub, tmp_wait_time))
      {
        max_wait_time= &tmp_wait_time;
      }

    while (status == TAO_INVOKE_START || status == TAO_INVOKE_RESTART)
      {
        // Default we go to remote
        Collocation_Strategy strat = TAO_CS_REMOTE_STRATEGY;

        // If we have a collocated proxy broker we look if we maybe
        // can use a collocated invocation.  Similarly, if the
        // target object reference contains a pointer to a servant,
        // the object reference also refers to a collocated object.
        if (cpb_ != 0 || effective_target->_servant () != 0)
          {
            strat = TAO_ORB_Core::collocation_strategy (effective_target.in ());
          }

        if (strat == TAO_CS_REMOTE_STRATEGY || strat == TAO_CS_LAST)
          {
            status =
              this->invoke_remote_i (stub,
                                     details,
                                     effective_target,
                                     max_wait_time);
          }
        else
          {
            if (strat == TAO_CS_THRU_POA_STRATEGY)
              {
                (void) this->set_response_flags (stub, details);
              }

            status =
              this->invoke_collocated_i (stub,
                                         details,
                                         effective_target,
                                         strat);
          }

        if (status == TAO_INVOKE_RESTART)
          {
            details.reset_request_service_info ();
            details.reset_reply_service_info ();

            if (TAO_debug_level > 2)
              {
                ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Invocation_Adapter::invoke_i, ")
                  ACE_TEXT("handling forwarded locations\n")));
              }
          }
      }
  }

  bool
  Invocation_Adapter::get_timeout (TAO_Stub *stub, ACE_Time_Value &timeout)
  {
    bool has_timeout = false;
    this->target_->orb_core ()->call_timeout_hook (stub, has_timeout, timeout);

    return has_timeout;
  }

  TAO_Stub *
  Invocation_Adapter::get_stub (void) const
  {
    TAO_Stub * const stub = this->target_->_stubobj ();

    if (stub == 0)
      throw ::CORBA::INTERNAL (
        CORBA::SystemException::_tao_minor_code (
          TAO::VMCID,
          EINVAL),
        CORBA::COMPLETED_NO);

    return stub;
  }

  Invocation_Status
  Invocation_Adapter::invoke_collocated_i (TAO_Stub *stub,
                                           TAO_Operation_Details &details,
                                           CORBA::Object_var &effective_target,
                                           Collocation_Strategy strat)
  {
    // To make a collocated call we must have a collocated proxy broker, the
    // invoke_i() will make sure that we only come here when we have one
    ACE_ASSERT (cpb_ != 0
                || (strat == TAO_CS_THRU_POA_STRATEGY
                    && effective_target->_servant () != 0));

    // Initial state
    TAO::Invocation_Status status = TAO_INVOKE_START;

    Collocated_Invocation coll_inv (this->target_,
                                    effective_target.in (),
                                    stub,
                                    details,
                                    this->type_ == TAO_TWOWAY_INVOCATION);

    status = coll_inv.invoke (this->cpb_, strat);

    if (status == TAO_INVOKE_RESTART &&
        (coll_inv.reply_status () == GIOP::LOCATION_FORWARD ||
         coll_inv.reply_status () == GIOP::LOCATION_FORWARD_PERM))
      {
        CORBA::Boolean const is_permanent_forward =
          (coll_inv.reply_status () == GIOP::LOCATION_FORWARD_PERM);

        effective_target = coll_inv.steal_forwarded_reference ();

        this->object_forwarded (effective_target, stub, is_permanent_forward);
      }

    return status;
  }

  void
  Invocation_Adapter::set_response_flags (
    TAO_Stub *stub,
    TAO_Operation_Details &details)
  {
    switch (this->type_)
      {
      case TAO_ONEWAY_INVOCATION:
        {
          // Grab the syncscope policy from the ORB.
          Messaging::SyncScope sync_scope;

          bool has_synchronization = false;

          stub->orb_core ()->call_sync_scope_hook (stub,
                                                   has_synchronization,
                                                   sync_scope);
          if (has_synchronization)
            details.response_flags (CORBA::Octet (sync_scope));
          else
            details.response_flags (
              CORBA::Octet (Messaging::SYNC_WITH_TRANSPORT));
          break;
        }
      case TAO_TWOWAY_INVOCATION:
        {
          // @@note: Need to change this to something better. Too many
          // hash defines meaning the same things.
          details.response_flags (TAO_TWOWAY_RESPONSE_FLAG);
          break;
        }
      }
  }

  Invocation_Status
  Invocation_Adapter::invoke_remote_i (TAO_Stub *stub,
                                       TAO_Operation_Details &details,
                                       CORBA::Object_var &effective_target,
                                       ACE_Time_Value *&max_wait_time)
  {
    (void) this->set_response_flags (stub, details);

    CORBA::Octet const rflags = details.response_flags ();
    bool const block_connect =
      rflags != static_cast<CORBA::Octet> (Messaging::SYNC_NONE)
      && rflags != static_cast<CORBA::Octet> (TAO::SYNC_DELAYED_BUFFERING);

    // Create the resolver which will pick (or create) for us a
    // transport and a profile from the effective_target.
    Profile_Transport_Resolver resolver (
      effective_target.in (),
      stub,
      block_connect);

    resolver.resolve (max_wait_time);

    if (TAO_debug_level)
      {
        if (max_wait_time && *max_wait_time == ACE_Time_Value::zero)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Invocation_Adapter::invoke_remote_i, ")
                      ACE_TEXT ("max wait time consumed during transport resolution\n")));
      }

    // Update the request id now that we have a transport
    if (resolver.transport ())
      {
        details.request_id (resolver.transport ()->tms ()->request_id ());
      }

    switch (this->type_)
      {
        case TAO_ONEWAY_INVOCATION:
          {
            return this->invoke_oneway (details,
                                        effective_target,
                                        resolver,
                                        max_wait_time);
          }
        case TAO_TWOWAY_INVOCATION:
          {
            return this->invoke_twoway (details,
                                        effective_target,
                                        resolver,
                                        max_wait_time);

          }
      }
    return TAO_INVOKE_FAILURE;
  }

  Invocation_Status
  Invocation_Adapter::invoke_twoway (TAO_Operation_Details &details,
                                     CORBA::Object_var &effective_target,
                                     Profile_Transport_Resolver &r,
                                     ACE_Time_Value *&max_wait_time)
  {
    // Simple sanity check
    if (this->mode_ != TAO_SYNCHRONOUS_INVOCATION ||
        this->type_ != TAO_TWOWAY_INVOCATION)
      {
        throw ::CORBA::INTERNAL (
          CORBA::SystemException::_tao_minor_code (
            TAO::VMCID,
            EINVAL),
          CORBA::COMPLETED_NO);
      }

    TAO::Synch_Twoway_Invocation synch (this->target_,  r, details);

    Invocation_Status const status = synch.remote_twoway (max_wait_time);

    if (status == TAO_INVOKE_RESTART &&
        (synch.reply_status () == GIOP::LOCATION_FORWARD ||
         synch.reply_status () == GIOP::LOCATION_FORWARD_PERM))
      {
        CORBA::Boolean const is_permanent_forward =
          (synch.reply_status () == GIOP::LOCATION_FORWARD_PERM);

        effective_target = synch.steal_forwarded_reference ();

        this->object_forwarded (effective_target,
                                r.stub (),
                                is_permanent_forward);
      }

    return status;
  }

  Invocation_Status
  Invocation_Adapter::invoke_oneway (TAO_Operation_Details &details,
                                     CORBA::Object_var &effective_target,
                                     Profile_Transport_Resolver &r,
                                     ACE_Time_Value *&max_wait_time)
  {
    TAO::Synch_Oneway_Invocation synch (this->target_, r, details);

    Invocation_Status const s = synch.remote_oneway (max_wait_time);

    if (s == TAO_INVOKE_RESTART &&
        (synch.reply_status () == GIOP::LOCATION_FORWARD ||
         synch.reply_status () == GIOP::LOCATION_FORWARD_PERM))
      {
        CORBA::Boolean const is_permanent_forward =
          (synch.reply_status () == GIOP::LOCATION_FORWARD_PERM);

        effective_target = synch.steal_forwarded_reference ();

        this->object_forwarded (effective_target,
                                r.stub (),
                                is_permanent_forward);
      }

    return s;
  }

  void
  Invocation_Adapter::object_forwarded (CORBA::Object_var &effective_target,
                                        TAO_Stub *stub,
                                        CORBA::Boolean permanent_forward)
  {
    // The object pointer has to be changed to a TAO_Stub pointer
    // in order to obtain the profiles.
    TAO_Stub *stubobj = 0;

    bool nil_forward_ref = false;
    if (CORBA::is_nil (effective_target.in ()))
      nil_forward_ref = true;
    else
      {
        stubobj =
          effective_target->_stubobj ();

        if (stubobj && stubobj->base_profiles ().size () == 0)
          nil_forward_ref = true;
      }

    if (nil_forward_ref)
      throw ::CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
          errno),
        CORBA::COMPLETED_NO);

    if (stubobj == 0)
      throw ::CORBA::INTERNAL (
        CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
          errno),
        CORBA::COMPLETED_NO);

    // Reset the profile in the stubs
    stub->add_forward_profiles (stubobj->base_profiles (), permanent_forward);

    if (stub->next_profile () == 0)
      throw ::CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
          errno),
        CORBA::COMPLETED_NO);
  }
} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL
