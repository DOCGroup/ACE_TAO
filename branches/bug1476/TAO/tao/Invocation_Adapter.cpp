//$Id$
#include "Invocation_Adapter.h"
#include "Profile_Transport_Resolver.h"
#include "operation_details.h"
#include "Stub.h"
#include "ORB_Core.h"
#include "Synch_Invocation.h"
#include "debug.h"
#include "Collocated_Invocation.h"
#include "Transport.h"
#include "Transport_Mux_Strategy.h"
#include "Collocation_Proxy_Broker.h"

#if !defined (__ACE_INLINE__)
# include "tao/Invocation_Adapter.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID (tao,
           Invocation_Adapter,
           "$Id$")


namespace TAO
{
  void
  Invocation_Adapter::invoke (TAO::Exception_Data *ex_data,
                              unsigned long ex_count
                              ACE_ENV_ARG_DECL)
  {
    // Should stub object be refcounted here?
    TAO_Stub *stub =
      this->get_stub (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    TAO_Operation_Details op_details (this->operation_,
                                      this->op_len_,
                                      this->number_args_ != 0,
                                      this->args_,
                                      this->number_args_,
                                      ex_data,
                                      ex_count);

    this->invoke_i (stub,
                    op_details
                    ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  Invocation_Adapter::invoke_i (TAO_Stub *stub,
                                TAO_Operation_Details &details
                                ACE_ENV_ARG_DECL)
  {
    // Cache the target to a local variable.
    CORBA::Object *effective_target = this->target_;

    CORBA::Object_var tmp_mem;

    // Initial state
    TAO::Invocation_Status status = TAO_INVOKE_START;

    ACE_Time_Value *max_wait_time = 0;

    while (status == TAO_INVOKE_START ||
           status == TAO_INVOKE_RESTART)
      {
        // Default we go to remote
        Collocation_Strategy strat = TAO_CS_REMOTE_STRATEGY;

        // If we have a collocated proxy broker we look if we maybe
        // can use a collocated invocation.
        if (cpb_ != 0)
          {
            strat =
              TAO_ORB_Core::collocation_strategy (effective_target
                                                  ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;
          }

        if (strat == TAO_CS_REMOTE_STRATEGY ||
            strat == TAO_CS_LAST)
          {
            status =
              this->invoke_remote_i (stub,
                                     details,
                                     effective_target,
                                     max_wait_time
                                     ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;
          }
        else
          {
            status =
              this->invoke_collocated_i (stub,
                                         details,
                                         effective_target,
                                         strat
                                         ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;
          }

        if (status == TAO_INVOKE_RESTART)
          {
            details.reset_request_service_info ();
            details.reset_reply_service_info ();

            tmp_mem = effective_target;

            if (TAO_debug_level > 2)
              {
                ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Invocation_Adapter::invoke_i, "
                  "handling forwarded locations \n"));
              }
          }
      }
  }

  bool
  Invocation_Adapter::get_timeout (TAO_Stub *stub,
                                   ACE_Time_Value &timeout)
  {
    bool has_timeout = false;
    this->target_->orb_core ()->call_timeout_hook (stub,
                                                   has_timeout,
                                                   timeout);

    return has_timeout;
  }

  TAO_Stub *
  Invocation_Adapter::get_stub (ACE_ENV_SINGLE_ARG_DECL) const
  {
    TAO_Stub *stub =
      this->target_->_stubobj ();

    if (stub == 0)
      ACE_THROW_RETURN (CORBA::INTERNAL (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO),
                        stub);

    return stub;
  }

  Invocation_Status
  Invocation_Adapter::invoke_collocated_i (TAO_Stub *stub,
                                           TAO_Operation_Details &details,
                                           CORBA::Object *&effective_target,
                                           Collocation_Strategy strat
                                           ACE_ENV_ARG_DECL)
  {
    // To make a collocated call we must have a collocated proxy broker, the
    // invoke_i() will make sure that we only come here when we have one
    ACE_ASSERT (cpb_ != 0);

    // Initial state
    TAO::Invocation_Status status = TAO_INVOKE_START;

    Collocated_Invocation coll_inv (this->target_,
                                    effective_target,
                                    stub,
                                    details,
                                    this->type_ == TAO_TWOWAY_INVOCATION);

    status = coll_inv.invoke (this->cpb_,
                              strat
                              ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (status == TAO_INVOKE_RESTART &&
        coll_inv.is_forwarded ())
      {
        effective_target =
            coll_inv.steal_forwarded_reference ();

        (void) this->object_forwarded (effective_target,
                                       stub
                                       ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);
      }

    return status;
  }


  Invocation_Status
  Invocation_Adapter::invoke_remote_i (TAO_Stub *stub,
                                       TAO_Operation_Details &details,
                                       CORBA::Object *&effective_target,
                                       ACE_Time_Value *&max_wait_time
                                       ACE_ENV_ARG_DECL)
  {
    ACE_Time_Value tmp_wait_time;
    bool is_timeout  =
      this->get_timeout (stub,
                         tmp_wait_time);

    if (is_timeout)
      max_wait_time = &tmp_wait_time;

// bool, need to say here it is must block

// the code here can go to a seperate method, this can then also set the details, which is now done in the invoke_oneway method
    bool block = true;
    if (this->type_ == TAO_ONEWAY_INVOCATION)
    {
       // Grab the syncscope policy from the ORB.
       bool has_synchronization = false;
       Messaging::SyncScope sync_scope;

       stub->orb_core ()->call_sync_scope_hook (stub,
                                                has_synchronization,
                                                sync_scope);
      if (has_synchronization && sync_scope == Messaging::SYNC_NONE)
      block = false;
    }

    // Create the resolver which will pick (or create) for us a
    // transport and a profile from the effective_target.
    Profile_Transport_Resolver resolver (effective_target,
                                         stub,
                                         block);

    resolver.resolve (max_wait_time
                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    // Update the request id now that we have a transport
    details.request_id (resolver.transport ()->tms ()->request_id ());

    Invocation_Status s = TAO_INVOKE_FAILURE;

    if (this->type_ == TAO_ONEWAY_INVOCATION)
      {
        return this->invoke_oneway (details,
                                    effective_target,
                                    resolver,
                                    max_wait_time
                                    ACE_ENV_ARG_PARAMETER);
      }
    else if (this->type_ == TAO_TWOWAY_INVOCATION)
      {
        // @@ NOTE:Need to change this to something better. Too many
        // hash defines meaning the same thing..
        details.response_flags (TAO_TWOWAY_RESPONSE_FLAG);
        return this->invoke_twoway (details,
                                    effective_target,
                                    resolver,
                                    max_wait_time
                                    ACE_ENV_ARG_PARAMETER);
      }

    return s;
  }

  Invocation_Status
  Invocation_Adapter::invoke_twoway (TAO_Operation_Details &op,
                                     CORBA::Object *&effective_target,
                                     Profile_Transport_Resolver &r,
                                     ACE_Time_Value *&max_wait_time
                                     ACE_ENV_ARG_DECL)
  {
    // Simple sanity check
    if (this->mode_ != TAO_SYNCHRONOUS_INVOCATION ||
        this->type_ != TAO_TWOWAY_INVOCATION)
      {
        ACE_THROW_RETURN (CORBA::INTERNAL (
            CORBA::SystemException::_tao_minor_code (
                TAO_DEFAULT_MINOR_CODE,
                EINVAL),
            CORBA::COMPLETED_NO),
                          TAO_INVOKE_FAILURE);
      }

    TAO::Synch_Twoway_Invocation synch (this->target_,
                                        r,
                                        op);

    Invocation_Status status =
      synch.remote_twoway (max_wait_time
                           ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (status == TAO_INVOKE_RESTART &&
        synch.is_forwarded ())
      {
        effective_target = synch.steal_forwarded_reference ();

        this->object_forwarded (effective_target,
                                r.stub ()
                                ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);
      }

    return status;
  }

  Invocation_Status
  Invocation_Adapter::invoke_oneway (TAO_Operation_Details &op,
                                     CORBA::Object *&effective_target,
                                     Profile_Transport_Resolver &r,
                                     ACE_Time_Value *&max_wait_time
                                     ACE_ENV_ARG_DECL)
  {
    // Grab the syncscope policy from the ORB.
    bool has_synchronization = false;
    Messaging::SyncScope sync_scope;

    r.stub ()->orb_core ()->call_sync_scope_hook (r.stub (),
                                                  has_synchronization,
                                                  sync_scope);

    if (has_synchronization)
      op.response_flags (CORBA::Octet (sync_scope));
    else
      op.response_flags (CORBA::Octet (Messaging::SYNC_WITH_TRANSPORT));

    TAO::Synch_Oneway_Invocation synch (this->target_,
                                        r,
                                        op);

    Invocation_Status s =
      synch.remote_oneway (max_wait_time
                           ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (s == TAO_INVOKE_RESTART &&
        synch.is_forwarded ())
      {
        effective_target = synch.steal_forwarded_reference ();

        this->object_forwarded (effective_target,
                                r.stub ()
                                ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);
      }

    return s;
  }

  void
  Invocation_Adapter::object_forwarded (CORBA::Object *&effective_target,
                                        TAO_Stub *stub
                                        ACE_ENV_ARG_DECL)
  {
    // The object pointer has to be changed to a TAO_Stub pointer
    // in order to obtain the profiles.
    TAO_Stub *stubobj =
      effective_target->_stubobj ();

    if (stubobj == 0)
      ACE_THROW (CORBA::INTERNAL (
        CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
          errno),
        CORBA::COMPLETED_NO));


    // Reset the profile in the stubs
    stub->add_forward_profiles (stubobj->base_profiles ());

    if (stub->next_profile () == 0)
      ACE_THROW (CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
          errno),
        CORBA::COMPLETED_NO));

    return;
  }
} // End namespace TAO
