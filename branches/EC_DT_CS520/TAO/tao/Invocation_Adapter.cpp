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

    // If we have a collocation proxy broker in our process space for
    // <this->target_>, just take a guess and invoke the collocated
    // path.
    if (this->cpb_)
      {
        this->invoke_collocated (stub,
                                 op_details
                                 ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
    else
      {
        this->invoke_remote (stub,
                             op_details
                             ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }

    return;
  }

  void
  Invocation_Adapter::invoke_collocated (TAO_Stub *stub,
                                         TAO_Operation_Details &details
                                         ACE_ENV_ARG_DECL)
  {
    // Cache the target to a local variable.
    // @@ NOTE: Leak if forwarded
    CORBA::Object *effective_target = this->target_;

    // Initial state
    TAO::Invocation_Status status = TAO_INVOKE_START;

    ACE_Time_Value *max_wait_time = 0;

    while (status == TAO_INVOKE_START ||
           status == TAO_INVOKE_RESTART)
      {
        // This is a second test of the target to determine whether it
        // is really collocated or not.
        Collocation_Strategy strat =
          this->cpb_->get_strategy (effective_target
                                    ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

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
            Collocated_Invocation coll_inv (effective_target,
                                            this->target_,
                                            stub,
                                            details,
                                            this->type_ == TAO_TWOWAY_INVOCATION);

            status = coll_inv.invoke (this->cpb_,
                                      strat
                                      ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;

            if (status == TAO_INVOKE_RESTART &&
                coll_inv.is_forwarded ())
              {
                effective_target =
                  coll_inv.steal_forwarded_reference ();

                (void) this->object_forwarded (effective_target,
                                               stub
                                               ACE_ENV_ARG_PARAMETER);
                ACE_CHECK;
              }
          }

        if (TAO_debug_level > 2 &&
            status == TAO_INVOKE_RESTART)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - Invocation_Adapter::invoke_collocated, ",
                        "handling forwarded locations \n"));
          }
      }
  }

  void
  Invocation_Adapter::invoke_remote (TAO_Stub *stub,
                                     TAO_Operation_Details &op
                                     ACE_ENV_ARG_DECL)
  {
    // @@NOTE: Leak if forwarding does happen
    CORBA::Object *effective_target = this->target_;

    ACE_Time_Value *max_wait_time = 0;

    // Initial state
    TAO::Invocation_Status status = TAO_INVOKE_START;

    while (status == TAO_INVOKE_START ||
           status == TAO_INVOKE_RESTART)
      {
        status = this->invoke_remote_i (stub,
                                        op,
                                        effective_target,
                                        max_wait_time
                                        ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (TAO_debug_level > 2 &&
            status == TAO_INVOKE_RESTART)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Invocation_Adapter::invoke_collocated, ")
                        ACE_TEXT ("handling forwarded locations \n")));
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
                        0);

    return stub;
  }


  Invocation_Status
  Invocation_Adapter::invoke_remote_i (TAO_Stub *stub,
                                       TAO_Operation_Details &op,
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

    // Create the resolver which will pick (or create) for us a
    // transport and a profile from the effective_target.
    Profile_Transport_Resolver resolver (effective_target,
                                         stub);

    resolver.resolve (max_wait_time
                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    // Update the request id now that we have a transport
    op.request_id (resolver.transport ()->tms ()->request_id ());

    Invocation_Status s = TAO_INVOKE_FAILURE;

    if (this->type_ == TAO_ONEWAY_INVOCATION)
      {
        return this->invoke_oneway (op,
                                    effective_target,
                                    resolver,
                                    max_wait_time
                                    ACE_ENV_ARG_PARAMETER);
      }
    else if (this->type_ == TAO_TWOWAY_INVOCATION)
      {
        // @@ NOTE:Need to change this to something better. Too many
        // hash defines meaning the same  thing..
        op.response_flags (TAO_TWOWAY_RESPONSE_FLAG);
        return this->invoke_twoway (op,
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
