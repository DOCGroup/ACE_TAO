// -*- C++ -*-
// $Id$

#include "tao/Remote_Object_Proxy_Broker.h"
#include "tao/CORBA_String.h"
#include "tao/UB_String_Arguments.h"
#include "tao/Special_Basic_Arguments.h"
#include "tao/Invocation_Adapter.h"
#include "tao/IFR_Client_Adapter.h"
#include "tao/ORB_Core.h"
#include "tao/SystemException.h"

#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  CORBA::Boolean
  Remote_Object_Proxy_Broker::_is_a (CORBA::Object_ptr target,
                                     const char *type_id)
  {
    TAO::Arg_Traits<ACE_InputCDR::to_boolean>::ret_val _tao_retval;
    TAO::Arg_Traits<char *>::in_arg_val _tao_id (type_id);

    TAO::Argument *_tao_signature [] =
      {
        &_tao_retval,
        &_tao_id
      };

    TAO::Invocation_Adapter tao_call (target,
                                      _tao_signature,
                                      2,
                                      "_is_a",
                                      5,
                                      0);

    tao_call.invoke (0, 0);

    return _tao_retval.retn ();
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)

  CORBA::Boolean
  Remote_Object_Proxy_Broker::_non_existent (CORBA::Object_ptr target)
  {
    TAO::Arg_Traits<ACE_InputCDR::to_boolean>::ret_val _tao_retval;

    TAO::Argument *_tao_signature [] =
      {
        &_tao_retval
      };

    TAO::Invocation_Adapter _tao_call (target,
                                       _tao_signature,
                                       1,
                                       "_non_existent",
                                       13,
                                       0);

    _tao_call.invoke (0, 0);

    return _tao_retval.retn ();
  }

  CORBA::Object_ptr
  Remote_Object_Proxy_Broker::_get_component (CORBA::Object_ptr target)
  {
    TAO::Arg_Traits<CORBA::Object>::ret_val _tao_retval;

    TAO::Argument *_tao_signature [] =
      {
        &_tao_retval
      };

    TAO::Invocation_Adapter _tao_call (target,
                                       _tao_signature,
                                       1,
                                       "_component",
                                       10,
                                       0);

    _tao_call.invoke (0, 0);

    return _tao_retval.retn ();
  }

  CORBA::InterfaceDef_ptr
  Remote_Object_Proxy_Broker::_get_interface (CORBA::Object_ptr target)
  {
    TAO_IFR_Client_Adapter *adapter =
      ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance (
        TAO_ORB_Core::ifr_client_adapter_name ()
      );

  if (adapter == 0)
    {
      throw ::CORBA::INTF_REPOS ();
    }

  return adapter->get_interface_remote (target);
  }

  char *
  Remote_Object_Proxy_Broker::_repository_id (CORBA::Object_ptr target)
  {
    TAO::Arg_Traits< ::CORBA::Char *>::ret_val _tao_retval;

    TAO::Argument *_tao_signature [] =
      {
        &_tao_retval
      };

    TAO::Invocation_Adapter _tao_call (target,
                                       _tao_signature,
                                       1,
                                       "_repository_id",
                                       14,
                                       0);

    _tao_call.invoke (0, 0);

    return _tao_retval.retn ();
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

TAO_END_VERSIONED_NAMESPACE_DECL
