// $Id$

#include "Direct_Object_Proxy_Impl.h"
#include "Object_Adapter.h"
#include "tao/Object.h"

ACE_RCSID(tao, TAO_Direct_Object_Proxy_Impl, "$Id$")

TAO_Direct_Object_Proxy_Impl::TAO_Direct_Object_Proxy_Impl (void)
{
  // No-op
}


TAO_Direct_Object_Proxy_Impl::~TAO_Direct_Object_Proxy_Impl (void)
{
  // No-op
}

CORBA::Boolean
TAO_Direct_Object_Proxy_Impl::_is_a (const CORBA::Object_ptr target,
                                     const CORBA::Char *logical_type_id
                                     TAO_ENV_ARG_DECL )
{

  return target->_servant ()->_is_a (logical_type_id
                                     TAO_ENV_ARG_PARAMETER);
}


#if (TAO_HAS_MINIMUM_CORBA == 0)

CORBA::Boolean
TAO_Direct_Object_Proxy_Impl::_non_existent (const CORBA::Object_ptr target
                                             TAO_ENV_ARG_DECL)
{
  CORBA::Boolean _tao_retval = 0;
  ACE_TRY
    {
      if (target->_servant () != 0)
        return target->_servant ()->_non_existent (TAO_ENV_SINGLE_ARG_PARAMETER);

      // @@ Maybe we want to change this exception...
      ACE_THROW_RETURN (CORBA::INV_OBJREF (), 0);
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

CORBA_InterfaceDef_ptr
TAO_Direct_Object_Proxy_Impl::_get_interface (const CORBA::Object_ptr target
                                              TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      if (target->_servant () != 0)
        return target->_servant ()->_get_interface (TAO_ENV_SINGLE_ARG_PARAMETER);

      // @@ Maybe we want to change this exception...
      ACE_THROW_RETURN (CORBA::INV_OBJREF (), 0);
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
    }
  ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;

  return 0;
}

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */
