// $Id$

#include "POA.h"
#include "ThruPOA_Object_Proxy_Impl.h"

#include "tao/ORB_Core.h"
#include "tao/Stub.h"


ACE_RCSID(tao, TAO_ThruPOA_Object_Proxy_Impl, "$Id$")

TAO_ThruPOA_Object_Proxy_Impl::TAO_ThruPOA_Object_Proxy_Impl (void)
{
  // No-op
}


TAO_ThruPOA_Object_Proxy_Impl::~TAO_ThruPOA_Object_Proxy_Impl (void)
{
  // No-op
}

CORBA::Boolean
TAO_ThruPOA_Object_Proxy_Impl::_is_a (const CORBA::Object_ptr target,
                                      const CORBA::Char *logical_type_id,
                                      CORBA_Environment &ACE_TRY_ENV)
{
  TAO_Object_Adapter::Servant_Upcall
    servant_upcall (target->_stubobj ()->servant_orb_var ()->orb_core ());

  CORBA::Object_var forward_to;
  servant_upcall.prepare_for_upcall (
    target->_object_key (),
    "_is_a",
    forward_to.out (),
    ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  return servant_upcall.servant ()->_is_a (logical_type_id, ACE_TRY_ENV);
}


#if (TAO_HAS_MINIMUM_CORBA == 0)

CORBA::Boolean
TAO_ThruPOA_Object_Proxy_Impl::_non_existent (const CORBA::Object_ptr target,
                                              CORBA_Environment &ACE_TRY_ENV)
{
  CORBA::Boolean _tao_retval = 0;
  ACE_TRY
    {
      TAO_Object_Adapter::Servant_Upcall
        servant_upcall (target->_stubobj ()->servant_orb_var ()->orb_core ());

      CORBA::Object_var forward_to;
      servant_upcall.prepare_for_upcall (
         target->_object_key (),
         "_non_existent",
         forward_to.out (),
         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      return servant_upcall.servant ()->_non_existent (ACE_TRY_ENV);
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

#endif // TAO_HAS_MINIMUM_CORBA == 0 //
