//$Id$
// -- PortableServer Include --
#include "Collocated_Object_Proxy_Broker.h"
#include "Object_Adapter.h"


// -- TAO Include --
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Profile.h"
#include "tao/DomainC.h"

ACE_RCSID (tao,
           TAO_Object_Remote_Proxy_Broker,
           "$Id$")


namespace TAO
{
  CORBA::Boolean
  Collocated_Object_Proxy_Broker::_is_a (CORBA::Object_ptr target,
                                         const char *type_id
                                         ACE_ENV_ARG_DECL)
  {
    TAO_Stub *stub =
      target->_stubobj ();

    // Which collocation strategy should we use?
    if (stub != 0 &&
        stub->servant_orb_var ()->orb_core ()
        ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
      {
        TAO_Object_Adapter::Servant_Upcall servant_upcall (
          stub->servant_orb_var ()->orb_core ()
        );

      CORBA::Object_var forward_to;
      servant_upcall.prepare_for_upcall (stub->profile_in_use ()->object_key (),
                                         "_is_a",
                                         forward_to.out ()
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      servant_upcall.pre_invoke_collocated_request (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return servant_upcall.servant ()->_is_a (type_id
                                               ACE_ENV_ARG_PARAMETER);
    }

    // Direct collocation strategy is used.
    if (target->_servant () != 0)
      return target->_servant ()->_is_a (type_id
                                         ACE_ENV_ARG_PARAMETER);

    return 0;
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)

  CORBA::Boolean
  Collocated_Object_Proxy_Broker::_non_existent (CORBA::Object_ptr target
                                                 ACE_ENV_ARG_DECL)
  {
    CORBA::Boolean _tao_retval = 0;

    ACE_TRY
      {
        TAO_Stub *stub = target->_stubobj ();

        // Which collocation strategy should we use?
        if (stub != 0 &&
            stub->servant_orb_var ()->orb_core ()
            ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
          {
            TAO_Object_Adapter::Servant_Upcall servant_upcall (
              target->_stubobj ()->servant_orb_var ()->orb_core ()
              );

            CORBA::Object_var forward_to;

            servant_upcall.prepare_for_upcall (
              target->_stubobj ()->object_key (),
              "_non_existent",
              forward_to.out ()
              ACE_ENV_ARG_PARAMETER
              );
            ACE_TRY_CHECK;

            servant_upcall.pre_invoke_collocated_request (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            servant_upcall.servant ()->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        // Direct collocation strategy is used.
        else if (target->_servant () != 0)
          {
            target->_servant ()->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
      }
    ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
      {
        _tao_retval = 1;
      }
    ACE_CATCHANY
      {
        ACE_RE_THROW;
      }
    ACE_ENDTRY;

    return _tao_retval;
  }

  CORBA::Object_ptr
  Collocated_Object_Proxy_Broker::_get_component (CORBA::Object_ptr target
                                                  ACE_ENV_ARG_DECL)
  {
    CORBA::Object_var _tao_retval (CORBA::Object::_nil ());

    TAO_Stub *stub = target->_stubobj ();

    ACE_TRY
      {
        // Which collocation strategy should we use?
        if (stub != 0 &&
            stub->servant_orb_var ()->orb_core ()
              ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
          {
            TAO_Object_Adapter::Servant_Upcall servant_upcall (
                stub->servant_orb_var ()->orb_core ()
                );

            CORBA::Object_var forward_to;
            servant_upcall.prepare_for_upcall (stub->profile_in_use ()->object_key (),
                                               "_component",
                                               forward_to.out ()
                                               ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            _tao_retval =
              servant_upcall.servant ()->_get_component (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        // Direct collocation strategy is used.
        else if (target->_servant () != 0)
          {
            _tao_retval =
              target->_servant ()->_get_component (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
      }
    ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
      {
        // Ignore this exception.
      }
    ACE_CATCHANY
      {
        ACE_RE_THROW;
      }
    ACE_ENDTRY;
    ACE_CHECK_RETURN (_tao_retval.out ());

    return _tao_retval.out ();
  }

  CORBA::InterfaceDef_ptr
  Collocated_Object_Proxy_Broker::_get_interface (
      CORBA::Object_ptr target
      ACE_ENV_ARG_DECL)
  {
    CORBA::InterfaceDef_ptr _tao_retval = 0;

    TAO_Stub *stub = target->_stubobj ();

    ACE_TRY
      {
        // Which collocation strategy should we use?
        if (stub != 0 &&
            stub->servant_orb_var ()->orb_core ()
              ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
          {
            TAO_Object_Adapter::Servant_Upcall servant_upcall (
                target->_stubobj ()->servant_orb_var ()->orb_core ()
                );

            CORBA::Object_var forward_to;

            servant_upcall.prepare_for_upcall (target->_stubobj ()->object_key (),
                                               "_interface",
                                               forward_to.out ()
                                               ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            servant_upcall.pre_invoke_collocated_request (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            _tao_retval =
              servant_upcall.servant ()->_get_interface (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        // Direct collocation strategy is used.
        else if (target->_servant () != 0)
          {
            _tao_retval =
              target->_servant ()->_get_interface (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
      }
    ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
      {
        // Ignore this exception.
      }
    ACE_CATCHANY
      {
        ACE_RE_THROW;
      }
    ACE_ENDTRY;
    ACE_CHECK_RETURN (_tao_retval);

    return _tao_retval;
  }

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */

}

// -----------------------------------------------------
TAO::Collocated_Object_Proxy_Broker *
the_tao_collocated_object_proxy_broker (void)
{
  static TAO::Collocated_Object_Proxy_Broker the_broker;
  return &the_broker;
}

TAO::Object_Proxy_Broker * _TAO_collocation_Object_Proxy_Broker_Factory (
                               CORBA::Object_ptr obj
                               )
{
  ACE_UNUSED_ARG (obj);
  return the_tao_collocated_object_proxy_broker ();
}

int
_TAO_collocation_Object_Proxy_Broker_Factory_Initializer (size_t )
{
  _TAO_Object_Proxy_Broker_Factory_function_pointer =
    _TAO_collocation_Object_Proxy_Broker_Factory;

  return 0;
}

static int
_TAO_collocation_Object_Proxy_Broker_Factory_Initializer_Scarecrow =
_TAO_collocation_Object_Proxy_Broker_Factory_Initializer (ACE_reinterpret_cast (size_t,
                                                                                _TAO_collocation_Object_Proxy_Broker_Factory_Initializer));
