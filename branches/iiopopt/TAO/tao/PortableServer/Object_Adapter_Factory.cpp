// $Id$

#include "tao/PortableServer/Object_Adapter_Factory.h"
#include "tao/PortableServer/Object_Adapter.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Adapter*
TAO_Object_Adapter_Factory::create (TAO_ORB_Core *orb_core)
{
  TAO_Adapter* adapter = 0;
  ACE_NEW_RETURN (adapter,
                  TAO_Object_Adapter (orb_core->server_factory ()->
                                      active_object_map_creation_parameters (),
                                     *orb_core),
                  0);
  return adapter;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_PortableServer, TAO_Object_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Object_Adapter_Factory,
                       ACE_TEXT ("TAO_Object_Adapter_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Object_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

