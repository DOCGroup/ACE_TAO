//$Id$
#include "Invocation_Base.h"
#include "Object.h"
#include "Profile_Transport_Resolver.h"
#include "operation_details.h"
#include "Stub.h"
#include "ORB_Core.h"
//@@ Need to remove when we take care o fthe note below.
#include "corbafwd.h"
#include "Synch_Invocation.h"
#include "Transport.h"
#include "Transport_Mux_Strategy.h"
#include "Messaging_SyncScopeC.h"

ACE_RCSID (tao,
           Invocation_Base,
           "$Id$")


namespace TAO
{
Invocation_Base::Invocation_Base (CORBA::Object *target,
                                  Argument **args,
                                  int arg_number,
                                  char *operation,
                                  int op_len,
                                  Invocation_Type type,
                                  Invocation_Mode mode)
    : target_ (target)
    , args_ (args)
    , number_args_  (arg_number)
    , operation_ (operation)
    , op_len_ (op_len)
    , type_ (type)
    , mode_ (mode)
  {
  }


  void
  Invocation_Base::invoke (TAO_Exception_Data *ex_data,
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


    // @@ Note: We may want somthing like this for FT
    /*if (stub->orb_core ()->is_service_invocation ())
      return stub->orb_core ()->invoke_services ();
    */

    // Initial state
    TAO::Invocation_Status status = TAO_INVOKE_START;

    TAO_Operation_Details op_details (this->operation_,
                                      this->op_len_,
                                      this->number_args_ != 0,
                                      ex_data,
                                      ex_count);

    /*while (status != TAO_INVOKE_SUCCESS ||
           status != TAO_INVOKE_FAILURE)
           {*/
        Profile_Transport_Resolver resolver (stub);

        resolver.resolve (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;

        // Update the request id now that we have a transport
        op_details.request_id (resolver.transport ()->tms ()->request_id ());

        if (this->type_ == TAO_ONEWAY_INVOCATION)
          {
            int has_synchronization = 0;
            Messaging::SyncScope sync_scope;
            stub->orb_core ()->call_sync_scope_hook (stub,
                                                     has_synchronization,
                                                     sync_scope);

            op_details.response_flags (sync_scope);

            TAO::Synch_Oneway_Invocation synch (resolver,
                                                op_details);

            status =
              synch.communicate (this->args_,
                                 this->number_args_
                                 ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;
          }
        else if (this->type_ == TAO_TWOWAY_INVOCATION
                 && this->mode_ == TAO_SYNCHRONOUS_INVOCATION)
          {
            // @@ NOTE:Need to change this to something better. Too many
            // hash defines meaning the same  thing..
            op_details.response_flags (TAO_TWOWAY_RESPONSE_FLAG);
            TAO::Synch_Twoway_Invocation synch (resolver,
                                                op_details);

            status =
              synch.communicate (this->args_,
                                 this->number_args_
                                 ACE_ENV_ARG_PARAMETER);
            ACE_CHECK;
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
        /*}*/
  }


} // End namespace TAO
