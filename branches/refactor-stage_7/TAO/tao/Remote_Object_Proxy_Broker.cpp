#include "Remote_Object_Proxy_Broker.h"
#include "Object.h"
#include "UB_String_Arguments.h"
#include "Special_Basic_Arguments.h"
#include "Invocation_Adapter.h"
#include "IFR_Client_Adapter.h"
#include "ORB_Core.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (tao,
           TAO_Object_Remote_Proxy_Broker,
           "$Id$")


namespace TAO
{
  CORBA::Boolean
  Remote_Object_Proxy_Broker::_is_a (CORBA::Object_ptr target,
                                     const char *type_id
                                     ACE_ENV_ARG_DECL)
  {
    TAO::Arg_Traits<ACE_InputCDR::to_boolean>::ret_val _tao_retval;
    TAO::Arg_Traits<CORBA::Object>::in_arg_val _tao_target (target);
    TAO::Arg_Traits<char *>::in_arg_val _tao_id (type_id);

    TAO::Argument *_tao_signature [] =
      {
        &_tao_retval,
        &_tao_target,
        &_tao_id
      };

    TAO::Invocation_Adapter tao_call (target,
                                      _tao_signature,
                                      3,
                                      "_is_a",
                                      5,
                                      0);

    tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (_tao_retval.excp ());

    return _tao_retval.retn ();
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)

  CORBA::Boolean
  Remote_Object_Proxy_Broker::_non_existent (CORBA::Object_ptr target
                                             ACE_ENV_ARG_DECL)
  {
    TAO::Arg_Traits<ACE_InputCDR::to_boolean>::ret_val _tao_retval;
    TAO::Arg_Traits<CORBA::Object>::in_arg_val _tao_target (target);

    TAO::Argument *_tao_signature [] =
      {
        &_tao_retval,
        &_tao_target
      };

    TAO::Invocation_Adapter _tao_call (target,
                                       _tao_signature,
                                       2,
                                       "_non_existent",
                                       13,
                                       0);

    _tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (_tao_retval.excp ());

    return _tao_retval.retn ();
  }

  CORBA::Object_ptr
  Remote_Object_Proxy_Broker::_get_component (CORBA::Object_ptr target
                                              ACE_ENV_ARG_DECL)
  {
    TAO::Arg_Traits<CORBA::Object>::ret_val _tao_retval;
    TAO::Arg_Traits<CORBA::Object>::in_arg_val _tao_target (target);

    TAO::Argument *_tao_signature [] =
      {
        &_tao_retval,
        &_tao_target
      };

    TAO::Invocation_Adapter _tao_call (target,
                                       _tao_signature,
                                       2,
                                       "_component",
                                       10,
                                       0);

    _tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (_tao_retval.excp ());

    return _tao_retval.retn ();
}

  CORBA::InterfaceDef_ptr
  Remote_Object_Proxy_Broker::_get_interface (CORBA::Object_ptr target
                                              ACE_ENV_ARG_DECL)
  {
    TAO_IFR_Client_Adapter *adapter =
      ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTF_REPOS (),
                        0);
    }

  return adapter->get_interface_remote (target
                                        ACE_ENV_ARG_PARAMETER);
  }

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

}

// -----------------------------------------------------

TAO::Remote_Object_Proxy_Broker *
the_tao_remote_object_proxy_broker (void)
{
  static TAO::Remote_Object_Proxy_Broker the_broker;
  return &the_broker;
}
