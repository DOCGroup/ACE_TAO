// $Id$

#include "RTScheduler_Manager.h"

#if ! defined (__ACE_INLINE__)
#include "RTScheduler_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO, RT_ORB, "$Id$")

TAO_RTScheduler_Manager::TAO_RTScheduler_Manager (void)
{
 
}

TAO_RTScheduler_Manager::~TAO_RTScheduler_Manager (void)
{
}

TAO_RTScheduler_Manager_ptr
TAO_RTScheduler_Manager::_narrow (CORBA::Object_ptr obj
                                       ACE_ENV_ARG_DECL)
{
  return TAO_RTScheduler_Manager::_unchecked_narrow (obj ACE_ENV_ARG_PARAMETER);
}

TAO_RTScheduler_Manager_ptr
TAO_RTScheduler_Manager::_unchecked_narrow (CORBA::Object_ptr obj
                                                 ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (obj))
    return TAO_RTScheduler_Manager::_nil ();
  return
      ACE_reinterpret_cast
        (
          TAO_RTScheduler_Manager_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast (ptr_arith_t,
                                      &TAO_RTScheduler_Manager::_narrow)
              )
        );
}

TAO_RTScheduler_Manager_ptr
TAO_RTScheduler_Manager::_duplicate (TAO_RTScheduler_Manager_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

void *
TAO_RTScheduler_Manager::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &TAO_RTScheduler_Manager::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char*
TAO_RTScheduler_Manager::_interface_repository_id (void) const
{
  return "IDL:RTScheduler_Manager:1.0";
}

