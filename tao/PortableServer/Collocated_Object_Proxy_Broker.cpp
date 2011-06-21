//$Id$

#include "tao/PortableServer/Collocated_Object_Proxy_Broker.h"
#include "tao/PortableServer/Object_Adapter.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Profile.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  CORBA::Boolean
  Collocated_Object_Proxy_Broker::_is_a (CORBA::Object_ptr target,
                                         const char *type_id)
  {
    TAO_Stub *stub = target->_stubobj ();

    // Which collocation strategy should we use?
    if (stub != 0 &&
        stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ()
          == TAO_ORB_Core::THRU_POA)
      {
        TAO::Portable_Server::Servant_Upcall servant_upcall (
          stub->servant_orb_var ()->orb_core ());

      CORBA::Object_var forward_to;
      servant_upcall.prepare_for_upcall (
          stub->profile_in_use ()->object_key (),
          "_is_a",
          forward_to.out ());

      servant_upcall.pre_invoke_collocated_request ();

      return servant_upcall.servant ()->_is_a (type_id);
    }

    // Direct collocation strategy is used.
    if (target->_servant () != 0)
      {
        return target->_servant ()->_is_a (type_id);
      }

    return 0;
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)

  char *
  Collocated_Object_Proxy_Broker::_repository_id (CORBA::Object_ptr target)
  {
    TAO_Stub *stub = target->_stubobj ();
    char * _tao_retval = 0;

    try
      {
        // Which collocation strategy should we use?
        if (stub != 0 &&
            stub->servant_orb_var ()->orb_core ()
              ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
          {
            TAO::Portable_Server::Servant_Upcall servant_upcall (
                stub->servant_orb_var ()->orb_core ());

            CORBA::Object_var forward_to;
            servant_upcall.prepare_for_upcall (
                stub->profile_in_use ()->object_key (),
                "_repository_id",
                forward_to.out ());

            _tao_retval =
              servant_upcall.servant ()->_repository_id ();
          }
        // Direct collocation strategy is used.
        else if (target->_servant () != 0)
          {
            _tao_retval = target->_servant ()->_repository_id ();
          }
      }
    catch (const ::CORBA::OBJECT_NOT_EXIST&)
      {
        // Ignore this exception.
      }

    return _tao_retval;
  }

  CORBA::Boolean
  Collocated_Object_Proxy_Broker::_non_existent (CORBA::Object_ptr target)
  {
    CORBA::Boolean _tao_retval = true;

    TAO_Stub *stub = target->_stubobj ();

    // Which collocation strategy should we use?
    if (stub != 0 &&
        stub->servant_orb_var ()->orb_core ()
        ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
      {
        TAO::Portable_Server::Servant_Upcall servant_upcall (
          target->_stubobj ()->servant_orb_var ()->orb_core ());

        CORBA::Object_var forward_to;

        servant_upcall.prepare_for_upcall (
          target->_stubobj ()->object_key (),
          "_non_existent",
          forward_to.out ());

        servant_upcall.pre_invoke_collocated_request ();

        _tao_retval = servant_upcall.servant ()->_non_existent ();
      }
    // Direct collocation strategy is used.
    else if (target->_servant () != 0)
      {
        _tao_retval = target->_servant ()->_non_existent ();
      }

    return _tao_retval;
  }

#if !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  CORBA::Object_ptr
  Collocated_Object_Proxy_Broker::_get_component (CORBA::Object_ptr target)
  {
    CORBA::Object_var _tao_retval (CORBA::Object::_nil ());

    TAO_Stub *stub = target->_stubobj ();

    try
      {
        // Which collocation strategy should we use?
        if (stub != 0 &&
            stub->servant_orb_var ()->orb_core ()
              ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
          {
            TAO::Portable_Server::Servant_Upcall servant_upcall (
                stub->servant_orb_var ()->orb_core ());

            CORBA::Object_var forward_to;
            servant_upcall.prepare_for_upcall (
                stub->profile_in_use ()->object_key (),
                "_component",
                forward_to.out ());

            _tao_retval = servant_upcall.servant ()->_get_component ();
          }
        // Direct collocation strategy is used.
        else if (target->_servant () != 0)
          {
            _tao_retval = target->_servant ()->_get_component ();
          }
      }
    catch (const ::CORBA::OBJECT_NOT_EXIST&)
      {
        // Ignore this exception.
      }

    return _tao_retval._retn ();
  }

  CORBA::InterfaceDef_ptr
  Collocated_Object_Proxy_Broker::_get_interface (CORBA::Object_ptr target)
  {
    CORBA::InterfaceDef_ptr _tao_retval = 0;

    TAO_Stub *stub = target->_stubobj ();

    try
      {
        // Which collocation strategy should we use?
        if (stub != 0 &&
            stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ()
              == TAO_ORB_Core::THRU_POA)
          {
            TAO::Portable_Server::Servant_Upcall servant_upcall (
                target->_stubobj ()->servant_orb_var ()->orb_core ());

            CORBA::Object_var forward_to;

            servant_upcall.prepare_for_upcall (
                target->_stubobj ()->object_key (),
                "_interface",
                forward_to.out ());

            servant_upcall.pre_invoke_collocated_request ();

            _tao_retval =
              servant_upcall.servant ()->_get_interface ();
          }
        // Direct collocation strategy is used.
        else if (target->_servant () != 0)
          {
            _tao_retval = target->_servant ()->_get_interface ();
          }
      }
    catch (const ::CORBA::OBJECT_NOT_EXIST&)
      {
        // Ignore this exception.
      }

    return _tao_retval;
  }
#endif

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

}

// -----------------------------------------------------
TAO::Collocated_Object_Proxy_Broker *
the_tao_collocated_object_proxy_broker (void)
{
  static TAO::Collocated_Object_Proxy_Broker the_broker;
  return &the_broker;
}

TAO::Object_Proxy_Broker * _TAO_collocation_Object_Proxy_Broker_Factory (void)
{
  return the_tao_collocated_object_proxy_broker ();
}

int
_TAO_collocation_Object_Proxy_Broker_Factory_Initializer (size_t)
{
  _TAO_Object_Proxy_Broker_Factory_function_pointer =
    _TAO_collocation_Object_Proxy_Broker_Factory;

  return 0;
}

static int
_TAO_collocation_Object_Proxy_Broker_Factory_Initializer_Scarecrow =
_TAO_collocation_Object_Proxy_Broker_Factory_Initializer (
    reinterpret_cast<size_t> (_TAO_collocation_Object_Proxy_Broker_Factory_Initializer
      )
  );

TAO_END_VERSIONED_NAMESPACE_DECL
