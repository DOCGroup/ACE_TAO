//$Id$
#include "Invocation_Base.h"

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
                                    Invocation_Type mode)
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
  Invocation_Base::invoke (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    TAO_Stub *stub =
      this->target_->stub ();

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

    Connection_Resolver resolver (stub);

    TAO_Transport *t =
      resolver.resolve (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;


    if (this->type_ == TAO_ONEWAY_INVOCATION)
      {
        // @@ Do Nothing at this point
      }
    else if (this->type_ == TAO_TWOWAY_INVOCATION
             && this->mode_ == TAO_SYNCHRONOUS_INVOCATION)
      {
        TAO::Synch_Twoway_Invocation synch;
        synch.invoke ();
        synch_invocation.wait ();
      }
    else if (this->type_ == TAO_TWOWAY_INVOCATION
             && this->mode_ == TAO_ASYNCHRONOUS_INVOCATION)
      {
        // @@ Do Nothing at this point
      }
  }


} // End namespace TAO
