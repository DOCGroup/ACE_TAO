// $Id$

#include "Priority_Mapping_Manager.h"

#if ! defined (__ACE_INLINE__)
#include "Priority_Mapping_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO, RT_ORB, "$Id$")

TAO_Priority_Mapping_Manager::~TAO_Priority_Mapping_Manager (void)
{
  delete mapping_;
}

TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager::_narrow (CORBA::Object_ptr obj,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  return TAO_Priority_Mapping_Manager::_unchecked_narrow (obj, ACE_TRY_ENV);
}

TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager::_unchecked_narrow (CORBA::Object_ptr obj,
                                                 CORBA::Environment &)
{
  if (CORBA::is_nil (obj))
    return TAO_Priority_Mapping_Manager::_nil ();
  return
      ACE_reinterpret_cast
        (
          TAO_Priority_Mapping_Manager_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast (ptr_arith_t,
                                      &TAO_Priority_Mapping_Manager::_narrow)
              )
        );
}

TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager::_duplicate (TAO_Priority_Mapping_Manager_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

void *
TAO_Priority_Mapping_Manager::_tao_QueryInterface (ptr_arith_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptr_arith_t,
      &TAO_Priority_Mapping_Manager::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptr_arith_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char*
TAO_Priority_Mapping_Manager::_interface_repository_id (void) const
{
  return "IDL:Priority_Mapping_Manager:1.0";
}

