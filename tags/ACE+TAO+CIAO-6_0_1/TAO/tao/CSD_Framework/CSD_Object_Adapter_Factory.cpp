// -*- C++ -*-
// $Id$

#include "tao/CSD_Framework/CSD_Object_Adapter_Factory.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "tao/CSD_Framework/CSD_Object_Adapter.h"
#include "tao/CSD_Framework/CSD_Default_Servant_Dispatcher.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CSD_Object_Adapter_Factory::TAO_CSD_Object_Adapter_Factory (void)
{
}

TAO_Adapter*
TAO_CSD_Object_Adapter_Factory::create (TAO_ORB_Core *oc)
{
  // Create the CSD object adapter.
  TAO_CSD_Object_Adapter *object_adapter = 0;
  ACE_NEW_RETURN (object_adapter,
                  TAO_CSD_Object_Adapter (oc->server_factory ()->
                                          active_object_map_creation_parameters (),
                                          *oc),
                  0);

  // Create and register the CSD servant dispatcher.
  TAO_CSD_Default_Servant_Dispatcher * csd_servant_dispatcher = 0;
  ACE_NEW_RETURN (csd_servant_dispatcher,
                  TAO_CSD_Default_Servant_Dispatcher,
                  0);
  object_adapter->servant_dispatcher (csd_servant_dispatcher);

  return object_adapter;
}

int
TAO_CSD_Object_Adapter_Factory::init (int /* argc */, ACE_TCHAR* /* argv */ [])
{
  TAO_CSD_Strategy_Repository *repo =
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance ("TAO_CSD_Strategy_Repository");

  if (repo != 0)
    repo->init(0,0);

  return 0;
}


ACE_FACTORY_DEFINE (TAO_CSD_FW, TAO_CSD_Object_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_CSD_Object_Adapter_Factory,
                       ACE_TEXT ("TAO_CSD_Object_Adapter_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CSD_Object_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL
