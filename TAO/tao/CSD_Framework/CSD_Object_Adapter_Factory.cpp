// -*- C++ -*-
#include "tao/CSD_Framework/CSD_Object_Adapter_Factory.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "tao/CSD_Framework/CSD_Object_Adapter.h"
#include "tao/CSD_Framework/CSD_Default_Servant_Dispatcher.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/Dynamic_Service.h"

#include <memory>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Adapter*
TAO_CSD_Object_Adapter_Factory::create (TAO_ORB_Core *oc)
{
  // Create the CSD object adapter.
  TAO_CSD_Object_Adapter *object_adapter {};
  ACE_NEW_RETURN (object_adapter,
                  TAO_CSD_Object_Adapter (oc->server_factory ()->
                                          active_object_map_creation_parameters (),
                                          *oc),
                  nullptr);
  std::unique_ptr<TAO_CSD_Object_Adapter> object_adapter_owner {object_adapter};

  // Create and register the CSD servant dispatcher.
  TAO_CSD_Default_Servant_Dispatcher *csd_servant_dispatcher {};
  ACE_NEW_RETURN (csd_servant_dispatcher,
                  TAO_CSD_Default_Servant_Dispatcher,
                  nullptr);
  std::unique_ptr<TAO_CSD_Default_Servant_Dispatcher> dispatcher_owner {
    csd_servant_dispatcher};
  object_adapter->servant_dispatcher (dispatcher_owner.release ());

  return object_adapter_owner.release ();
}

int
TAO_CSD_Object_Adapter_Factory::init (int /* argc */, ACE_TCHAR * /* argv */ [])
{
  TAO_CSD_Strategy_Repository * const repo =
    ACE_Dynamic_Service<TAO_CSD_Strategy_Repository>::instance ("TAO_CSD_Strategy_Repository");

  if (repo != nullptr)
    repo->init (0, nullptr);

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
