// $Id$

#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if ! defined (__ACE_INLINE__)
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (RTCORBA,
           Network_Priority_Mapping_Manager,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Network_Priority_Mapping_Manager::~TAO_Network_Priority_Mapping_Manager (
    void
  )
{
  delete this->mapping_;
}

TAO_Network_Priority_Mapping_Manager_ptr
TAO_Network_Priority_Mapping_Manager::_narrow (CORBA::Object_ptr obj)
{
  return
    TAO_Network_Priority_Mapping_Manager::_duplicate (
        dynamic_cast<TAO_Network_Priority_Mapping_Manager_ptr> (obj)
      );
}

TAO_Network_Priority_Mapping_Manager_ptr
TAO_Network_Priority_Mapping_Manager::_duplicate (
    TAO_Network_Priority_Mapping_Manager_ptr obj
  )
{
  if (!CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }

  return obj;
}

const char*
TAO_Network_Priority_Mapping_Manager::_interface_repository_id (void) const
{
  return "IDL:Network_Priority_Mapping_Manager:1.0";
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

