// -*- C++ -*-
// $Id$

#include "tao/LocateRequest_Invocation_Adapter.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/operation_details.h"
#include "tao/LocateRequest_Invocation.h"
#include "tao/Transport.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/GIOP_Utils.h"
#include "tao/ORB_Core.h"
#include "tao/Stub.h"
#include "tao/SystemException.h"
#include "ace/Service_Config.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  LocateRequest_Invocation_Adapter::LocateRequest_Invocation_Adapter (
    CORBA::Object_ptr target)
    : target_ (target)
    , list_ (0)
  {
  }

  void
  LocateRequest_Invocation_Adapter::invoke (void)
  {
    CORBA::Object_var effective_target =
      CORBA::Object::_duplicate (this->target_);

    TAO_Stub * const stub =
      this->target_->_stubobj ();
    if (stub == 0)
      throw ::CORBA::INTERNAL (
                     CORBA::SystemException::_tao_minor_code (
                       0,
                         EINVAL),
                        CORBA::COMPLETED_NO);

    // The invocation has got to be within the context of the
    // corresponding ORB's configuration. Otherwise things like
    // timeout hooks, etc may not work as expected. Especially if
    // there are multiple ORB instances in the process, each with its
    // own, local configuration.
    ACE_Service_Config_Guard scg (stub->orb_core ()->configuration ());

    ACE_Time_Value tmp_wait_time;
    ACE_Time_Value *max_wait_time = 0;

    if (this->get_timeout (tmp_wait_time))
      max_wait_time = &tmp_wait_time;

    Invocation_Status s = TAO_INVOKE_START;

    while (s == TAO_INVOKE_START || s == TAO_INVOKE_RESTART)
      {
        Profile_Transport_Resolver resolver (effective_target.in (), stub, true);

        try
          {
            resolver.init_inconsistent_policies ();

            resolver.resolve (max_wait_time);

            if (!resolver.transport ())
              {
                // We failed to find a profile we could connect to.
                throw CORBA::TRANSIENT (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
              }

            // Dummy operation details that is used to instantiate the
            // LocateRequest class.
            TAO_Operation_Details op (0, 0);

            op.request_id (resolver.transport ()->tms ()->request_id ());
            TAO::LocateRequest_Invocation synch (this->target_, resolver, op);

            s = synch.invoke (max_wait_time);
            if (s == TAO_INVOKE_RESTART &&
                (synch.reply_status () == GIOP::LOCATION_FORWARD ||
                 synch.reply_status () == GIOP::LOCATION_FORWARD_PERM))
              {
                CORBA::Boolean const is_permanent_forward =
                  (synch.reply_status () == GIOP::LOCATION_FORWARD_PERM);

                effective_target = synch.steal_forwarded_reference ();

                this->object_forwarded (effective_target,
                                        resolver.stub (),
                                        is_permanent_forward);
              }
          }
        catch (const ::CORBA::INV_POLICY&)
          {
            this->list_ = resolver.steal_inconsistent_policies ();
            throw;
          }
      }

    return;
  }

  CORBA::PolicyList *
  LocateRequest_Invocation_Adapter::get_inconsistent_policies (void)
  {
    return this->list_._retn ();
  }

  bool
  LocateRequest_Invocation_Adapter::get_timeout (ACE_Time_Value &timeout)
  {
    bool has_timeout = false;
    this->target_->orb_core ()->call_timeout_hook (this->target_->_stubobj (),
                                                   has_timeout,
                                                   timeout);

    return has_timeout;
  }

  void
  LocateRequest_Invocation_Adapter::object_forwarded (
    CORBA::Object_var &effective_target,
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
          0),
        CORBA::COMPLETED_NO);

    if (stubobj == 0)
      throw ::CORBA::INTERNAL (
        CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
          EINVAL),
        CORBA::COMPLETED_NO);

    // Reset the profile in the stubs
    stub->add_forward_profiles (stubobj->base_profiles (), permanent_forward);

    if (stub->next_profile () == 0)
      throw ::CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO);

    return;
  }

} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL
