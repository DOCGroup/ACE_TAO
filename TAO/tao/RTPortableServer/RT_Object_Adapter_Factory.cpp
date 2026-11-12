#include "tao/RTPortableServer/RT_Object_Adapter_Factory.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/PortableServer/Object_Adapter.h"
#include "tao/ORB_Core.h"
#include "tao/RTPortableServer/RT_Servant_Dispatcher.h"
#include "tao/RTPortableServer/RT_Policy_Validator.h"
#include "tao/RTPortableServer/RT_Collocation_Resolver.h"
#include <memory>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Adapter*
TAO_RT_Object_Adapter_Factory::create (TAO_ORB_Core *orb_core)
{
  if (!orb_core->orb_params ()->disable_rt_collocation_resolver ())
    {
      // Set the name of the collocation resolver to be RT_Collocation_Resolver.
      orb_core->orb_params ()->collocation_resolver_name ("RT_Collocation_Resolver");
      ACE_Service_Config::process_directive (
          ace_svc_desc_TAO_RT_Collocation_Resolver);
    }

  TAO_Object_Adapter *object_adapter_raw {};
  ACE_NEW_RETURN (object_adapter_raw,
                  TAO_Object_Adapter (orb_core->server_factory ()->
                                        active_object_map_creation_parameters (),
                                      *orb_core),
                  nullptr);
  std::unique_ptr<TAO_Object_Adapter> object_adapter (object_adapter_raw);

  // Create and register the RT servant dispatcher.
  TAO_RT_Servant_Dispatcher *rt_servant_dispatcher_raw {};
  ACE_NEW_RETURN (rt_servant_dispatcher_raw,
                  TAO_RT_Servant_Dispatcher,
                  nullptr);
  std::unique_ptr<TAO_RT_Servant_Dispatcher> rt_servant_dispatcher (rt_servant_dispatcher_raw);
  object_adapter->servant_dispatcher (rt_servant_dispatcher.get ());
  rt_servant_dispatcher.release ();

  // Create and add the RT policy validator.
  TAO_POA_RT_Policy_Validator *rt_validator_raw {};
  ACE_NEW_RETURN (rt_validator_raw,
                  TAO_POA_RT_Policy_Validator (*orb_core),
                  nullptr);
  std::unique_ptr<TAO_POA_RT_Policy_Validator> rt_validator (rt_validator_raw);
  object_adapter->validator ().add_validator (rt_validator.get ());
  rt_validator.release ();

  return object_adapter.release ();
}

ACE_FACTORY_DEFINE (TAO_RTPortableServer, TAO_RT_Object_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_RT_Object_Adapter_Factory,
                       ACE_TEXT ("TAO_RT_Object_Adapter_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Object_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
