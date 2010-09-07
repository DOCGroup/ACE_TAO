// $Id$

#include "tao/RTCORBA/Priority_Mapping_Manager.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if ! defined (__ACE_INLINE__)
#include "tao/RTCORBA/Priority_Mapping_Manager.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Priority_Mapping_Manager::~TAO_Priority_Mapping_Manager (void)
{
  delete this->mapping_;
}

TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager::_narrow (CORBA::Object_ptr obj)
{
  return
    TAO_Priority_Mapping_Manager::_duplicate (
        dynamic_cast<TAO_Priority_Mapping_Manager_ptr> (obj)
      );
}

TAO_Priority_Mapping_Manager_ptr
TAO_Priority_Mapping_Manager::_duplicate (TAO_Priority_Mapping_Manager_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_add_ref ();
  return obj;
}

const char*
TAO_Priority_Mapping_Manager::_interface_repository_id (void) const
{
  return "IDL:Priority_Mapping_Manager:1.0";
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

