// $Id$

#include "Direct_Object_Proxy_Impl.h"
#include "Object_Adapter.h"
#include "tao/Object.h"

ACE_RCSID (PortableServer,
           TAO_Direct_Object_Proxy_Impl,
           "$Id$")


TAO_Direct_Object_Proxy_Impl::~TAO_Direct_Object_Proxy_Impl (void)
{
}

CORBA::Boolean
TAO_Direct_Object_Proxy_Impl::_is_a (const CORBA::Object_ptr target,
                                     const CORBA::Char *logical_type_id
                                     ACE_ENV_ARG_DECL )
{

  return target->_servant ()->_is_a (logical_type_id
                                     ACE_ENV_ARG_PARAMETER);
}


#if (TAO_HAS_MINIMUM_CORBA == 0)

CORBA::Boolean
TAO_Direct_Object_Proxy_Impl::_non_existent (const CORBA::Object_ptr target
                                             ACE_ENV_ARG_DECL)
{
  CORBA::Boolean _tao_retval = 0;
  ACE_TRY
    {
      if (target->_servant () != 0)
        return target->_servant ()->_non_existent (
          ACE_ENV_SINGLE_ARG_PARAMETER);

      // @@ Maybe we want to change this exception...
      ACE_TRY_THROW (CORBA::INV_OBJREF ());
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      _tao_retval = 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  return _tao_retval;
}

CORBA::InterfaceDef_ptr
TAO_Direct_Object_Proxy_Impl::_get_interface (const CORBA::Object_ptr target
                                              ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      if (target->_servant () != 0)
        {
          // CORBA::InterfaceDef_var not available.
          CORBA::InterfaceDef_ptr r =
            target->_servant ()->_get_interface (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          return r;
        }
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      // Ignore this exception.
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);  // CORBA::InterfaceDef::_nil() not
                         // available.

  // @@ Maybe we want to change this exception...
  ACE_THROW_RETURN (CORBA::INV_OBJREF (),
                    0);  // CORBA::InterfaceDef::_nil() not
                         // available.
}

CORBA::Object_ptr
TAO_Direct_Object_Proxy_Impl::_get_component (const CORBA::Object_ptr target
                                              ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      if (target->_servant () != 0)
        {
          CORBA::Object_var r =
            target->_servant ()->_get_component (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          return r._retn ();
        }
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      // Ignore this exception.
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // @@ Maybe we want to change this exception...
  ACE_THROW_RETURN (CORBA::INV_OBJREF (), CORBA::Object::_nil ());
}

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */
