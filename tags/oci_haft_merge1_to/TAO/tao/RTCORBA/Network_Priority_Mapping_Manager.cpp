// $Id$

#include "Network_Priority_Mapping_Manager.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if ! defined (__ACE_INLINE__)
#include "Network_Priority_Mapping_Manager.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO, RT_ORB, "$Id$")

TAO_Network_Priority_Mapping_Manager::~TAO_Network_Priority_Mapping_Manager (void)
{
  delete mapping_;
}

TAO_Network_Priority_Mapping_Manager_ptr
TAO_Network_Priority_Mapping_Manager::_narrow (CORBA::Object_ptr obj
                                       ACE_ENV_ARG_DECL)
{
  return TAO_Network_Priority_Mapping_Manager::_unchecked_narrow (obj ACE_ENV_ARG_PARAMETER);
}

TAO_Network_Priority_Mapping_Manager_ptr
TAO_Network_Priority_Mapping_Manager::_unchecked_narrow (CORBA::Object_ptr obj
							 ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (obj))
    return TAO_Network_Priority_Mapping_Manager::_nil ();
  return
      ACE_reinterpret_cast
        (
          TAO_Network_Priority_Mapping_Manager_ptr,
            obj->_tao_QueryInterface
              (
                ACE_reinterpret_cast (ptrdiff_t,
                                      &TAO_Network_Priority_Mapping_Manager::_narrow)
              )
        );
}

TAO_Network_Priority_Mapping_Manager_ptr
TAO_Network_Priority_Mapping_Manager::_duplicate (TAO_Network_Priority_Mapping_Manager_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

void *
TAO_Network_Priority_Mapping_Manager::_tao_QueryInterface (ptrdiff_t type)
{
  void *retv = 0;
  if (type == ACE_reinterpret_cast
    (ptrdiff_t,
      &TAO_Network_Priority_Mapping_Manager::_narrow))
    retv = ACE_reinterpret_cast (void*, this);
  else if (type == ACE_reinterpret_cast (ptrdiff_t, &CORBA::Object::_narrow))
    retv = ACE_reinterpret_cast (void *,
      ACE_static_cast (CORBA::Object_ptr, this));

  if (retv)
    this->_add_ref ();
  return retv;
}

const char*
TAO_Network_Priority_Mapping_Manager::_interface_repository_id (void) const
{
  return "IDL:Network_Priority_Mapping_Manager:1.0";
}
#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

