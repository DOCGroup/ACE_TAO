#include "LocateRequest_Invocation_Adapter.h"
#include "Profile_Transport_Resolver.h"
#include "operation_details.h"
#include "LocateRequest_Invocation.h"
#include "Transport.h"
#include "Transport_Mux_Strategy.h"
#include "ORB_Core.h"

ACE_RCSID (tao,
           LocateRequest_Invocation_Adapter,
           "$Id$")

namespace TAO
{
  LocateRequest_Invocation_Adapter::LocateRequest_Invocation_Adapter (
      CORBA::Object *target)
    : target_ (target)
      , list_ (0)
  {
  }

  bool
  LocateRequest_Invocation_Adapter::invoke (ACE_ENV_SINGLE_ARG_DECL)
  {
    CORBA::Object *effective_target = this->target_;

    TAO_Stub *stub =
      this->target_->_stubobj ();

    if (stub == 0)
      ACE_THROW_RETURN (CORBA::INTERNAL (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO),
                        0);

    ACE_Time_Value tmp_wait_time;
    ACE_Time_Value *max_wait_time = 0;

    bool is_timeout  =
      this->get_timeout (tmp_wait_time);

    if (is_timeout)
      max_wait_time = &tmp_wait_time;


    Invocation_Status s = TAO_INVOKE_START;

    while (s == TAO_INVOKE_START ||
           s == TAO_INVOKE_RESTART)
      {
        Profile_Transport_Resolver resolver (effective_target,
                                             stub);

        ACE_TRY
          {
            resolver.resolve (max_wait_time
                              ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            // Dummy operation details that is used to instantiate the
            // LocateRequest class.
            TAO_Operation_Details op (0,
                                      0,
                                      0);

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
              resolver.inconsistent_policies ();
            ACE_RE_THROW;
          }
        ACE_CATCHANY
          {
            ACE_RE_THROW;
          }
        ACE_ENDTRY;
        ACE_CHECK_RETURN (false);
      }

    return true;
  }

  CORBA::PolicyList *
  LocateRequest_Invocation_Adapter::get_inconsistent_policies (void)
  {
    return this->list_.out ();
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
