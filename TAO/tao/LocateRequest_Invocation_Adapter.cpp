#include "tao/LocateRequest_Invocation_Adapter.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/operation_details.h"
#include "tao/LocateRequest_Invocation.h"
#include "tao/Transport.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/ORB_Core.h"

ACE_RCSID (tao,
           LocateRequest_Invocation_Adapter,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  LocateRequest_Invocation_Adapter::LocateRequest_Invocation_Adapter (
    CORBA::Object *target)
    : target_ (target)
    , list_ (0)
  {
  }

  void
  LocateRequest_Invocation_Adapter::invoke (ACE_ENV_SINGLE_ARG_DECL)
  {
    CORBA::Object * const effective_target = this->target_;

    TAO_Stub * const stub =
      this->target_->_stubobj ();

    if (stub == 0)
      ACE_THROW (CORBA::INTERNAL (
                     CORBA::SystemException::_tao_minor_code (
                       0,
                         EINVAL),
                        CORBA::COMPLETED_NO));

    ACE_Time_Value tmp_wait_time;
    ACE_Time_Value *max_wait_time = 0;

    bool const is_timeout  =
      this->get_timeout (tmp_wait_time);

    if (is_timeout)
      max_wait_time = &tmp_wait_time;

    Invocation_Status s = TAO_INVOKE_START;

    while (s == TAO_INVOKE_START ||
           s == TAO_INVOKE_RESTART)
      {
        Profile_Transport_Resolver resolver (effective_target,
                                             stub,
                                             true);

        ACE_TRY
          {
            resolver.init_inconsistent_policies (
                ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            resolver.resolve (max_wait_time
                              ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            // Dummy operation details that is used to instantiate the
            // LocateRequest class.
            TAO_Operation_Details op (0,
                                      0,
                                      false);

            op.request_id (resolver.transport ()->tms ()->request_id ());
            TAO::LocateRequest_Invocation synch (this->target_,
                                                 resolver,
                                                 op);

            s = synch.invoke (max_wait_time
                              ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        ACE_CATCH (CORBA::INV_POLICY, ex)
          {
            this->list_ =
              resolver.steal_inconsistent_policies ();
            ACE_RE_THROW;
          }
        ACE_CATCHANY
          {
            ACE_RE_THROW;
          }
        ACE_ENDTRY;
        ACE_CHECK;
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
} // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL
