// $Id$

#include "orbsvcs/PortableGroup/PG_Object_Adapter_Factory.h"
#include "orbsvcs/PortableGroup/PG_Servant_Dispatcher.h"

#include "tao/PortableServer/Object_Adapter.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PG_Object_Adapter_Factory::TAO_PG_Object_Adapter_Factory (void)
{
}

TAO_Adapter*
TAO_PG_Object_Adapter_Factory::create (TAO_ORB_Core *orb_core)
{
  TAO_Object_Adapter *object_adapter = 0;
  ACE_NEW_RETURN (object_adapter,
                  TAO_Object_Adapter (orb_core->server_factory ()->
                                        active_object_map_creation_parameters (),
                                      *orb_core),
                  0);

  // Create and register the RT servant dispatcher.
  TAO_PG_Servant_Dispatcher *rt_servant_dispatcher = 0;
  ACE_NEW_RETURN (rt_servant_dispatcher,
                  TAO_PG_Servant_Dispatcher,
                  0);
  object_adapter->servant_dispatcher (rt_servant_dispatcher);


  return object_adapter;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_PortableGroup, TAO_PG_Object_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_PG_Object_Adapter_Factory,
                       ACE_TEXT ("TAO_GOA"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_PG_Object_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
