//$Id$
#include "Invocation_Adapter.h"
#include "Object.h"
#include "Profile_Transport_Resolver.h"
#include "operation_details.h"
#include "Stub.h"
#include "ORB_Core.h"
//@@ Need to remove when we take care of the note below.
#include "corbafwd.h"
#include "Synch_Invocation.h"
#include "Collocated_Invocation.h"
#include "Transport.h"
#include "Transport_Mux_Strategy.h"
#include "Messaging_SyncScopeC.h"
#include "Collocation_Proxy_Broker.h"

ACE_RCSID (tao,
           Invocation_Adapter,
           "$Id$")


namespace TAO
{
  Invocation_Adapter::Invocation_Adapter (CORBA::Object *target,
                                    Argument **args,
                                    int arg_number,
                                    char *operation,
                                    int op_len,
                                    Collocation_Proxy_Broker *p,
                                    Invocation_Type type,
                                    Invocation_Mode mode)
    : target_ (target)
    , args_ (args)
    , number_args_  (arg_number)
    , operation_ (operation)
    , op_len_ (op_len)
    , cpb_ (p)
    , type_ (type)
    , mode_ (mode)
  {
  }


  void
  Invocation_Adapter::invoke (TAO_Exception_Data *ex_data,
                              unsigned long ex_count
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Should stub object be refcounted here?
    TAO_Stub *stub =
      this->target_->_stubobj ();

    if (stub == 0)
      ACE_THROW (CORBA::INTERNAL (
                   CORBA::SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                   CORBA::COMPLETED_NO));

    TAO_Operation_Details op_details (this->operation_,
                                      this->op_len_,
                                      this->number_args_ != 0,
                                      this->args_,
                                      this->number_args_,
                                      ex_data,
                                      ex_count);
    if (this->cpb_)
      {
        this->invoke_collocated (stub,
                                 op_details
                                 ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        return;
      }

    this->invoke_remote (stub,
                         op_details
                         ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  Invocation_Adapter::invoke_collocated (TAO_Stub *stub,
                                         TAO_Operation_Details &details
                                         ACE_ENV_ARG_DECL)
  {
    CORBA::Object *local_target = this->target_;

    // Initial state
    TAO::Invocation_Status status = TAO_INVOKE_START;

    while (status == TAO_INVOKE_START ||
           status == TAO_INVOKE_RESTART)
      {
        Collocation_Strategy strat =
          this->cpb_->get_strategy (local_target
                                    ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (strat == TAO_CS_REMOTE_STRATEGY ||
            strat == TAO_CS_LAST)
          {
            this->invoke_remote (stub,
                                 details
                                 ACE_ENV_ARG_PARAMETER);

            ACE_CHECK;

            return;
          }

        Collocated_Invocation coll_inv (stub,
                                        details);

        status = coll_inv.invoke (this->cpb_,
                                  local_target,
                                  strat
                                  ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        if (status == TAO_INVOKE_RESTART)
          local_target = coll_inv.steal_forwarded_reference ();
      }
  }

  void
  Invocation_Adapter::invoke_remote (TAO_Stub *stub,
                                     TAO_Operation_Details &op
                                     ACE_ENV_ARG_DECL)
  {
    CORBA::Object *local_target = this->target_;

    ACE_Time_Value tmp_wait_time;
    bool is_timeout  =
      this->get_timeout (tmp_wait_time);

    ACE_Time_Value *max_wait_time = 0;

    if (is_timeout)
      max_wait_time = &tmp_wait_time;

    // Initial state
    TAO::Invocation_Status status = TAO_INVOKE_START;

    while (status == TAO_INVOKE_START ||
           status == TAO_INVOKE_RESTART)
      {
        Profile_Transport_Resolver resolver (local_target,
                                             stub);

        resolver.resolve (max_wait_time
                          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        // Update the request id now that we have a transport
        op.request_id (resolver.transport ()->tms ()->request_id ());

        if (this->type_ == TAO_ONEWAY_INVOCATION)
          {
            int has_synchronization = 0;
            Messaging::SyncScope sync_scope;
            stub->orb_core ()->call_sync_scope_hook (stub,
                                                     has_synchronization,
                                                     sync_scope);

            op.response_flags (sync_scope);

            TAO::Synch_Oneway_Invocation synch (resolver,
                                                op);

            status =
              synch.remote_oneway (max_wait_time
                                   ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;
          }
        else if (this->type_ == TAO_TWOWAY_INVOCATION
                 && this->mode_ == TAO_SYNCHRONOUS_INVOCATION)
          {
            // @@ NOTE:Need to change this to something better. Too many
            // hash defines meaning the same  thing..
            op.response_flags (TAO_TWOWAY_RESPONSE_FLAG);
            TAO::Synch_Twoway_Invocation synch (resolver,
                                                op);

            status =
              synch.remote_twoway (max_wait_time
                                   ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;

            if (status == TAO_INVOKE_RESTART)
              local_target = synch.steal_forwarded_reference ();
          }
        else if (this->type_ == TAO_TWOWAY_INVOCATION
                 && this->mode_ == TAO_ASYNCHRONOUS_CALLBACK_INVOCATION)
          {
            // Should never get here..
            ACE_THROW (CORBA::INTERNAL (
               CORBA::SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 EINVAL),
               CORBA::COMPLETED_NO));
          }

      }
  }

  bool
  Invocation_Adapter::get_timeout (ACE_Time_Value &timeout)
  {
    bool has_timeout;
    this->target_->orb_core ()->call_timeout_hook (this->target_->_stubobj (),
                                                   has_timeout,
                                                   timeout);

    return has_timeout;
  }

} // End namespace TAO
