// $Id$

#include "RT_Object_Adapter_Factory.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/PortableServer/Object_Adapter.h"
#include "tao/ORB_Core.h"
#include "RT_Servant_Dispatcher.h"
#include "RT_Policy_Validator.h"
#include "RT_Collocation_Resolver.h"

ACE_RCSID (TAO_RTPortableServer,
           RT_Object_Adapter_Factory,
           "$Id$")


TAO_RT_Object_Adapter_Factory::TAO_RT_Object_Adapter_Factory (void)
{
}

TAO_Adapter*
TAO_RT_Object_Adapter_Factory::create (TAO_ORB_Core *orb_core)
{
  // Setup the POA_Current object in the ORB
  CORBA::Object_var current = new TAO_POA_Current;
  orb_core->poa_current (current.in ());

  // Set the name of the collocation resolver to be RT_Collocation_Resolver.
  TAO_ORB_Core::set_collocation_resolver ("RT_Collocation_Resolver");
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Collocation_Resolver);

  TAO_Object_Adapter *object_adapter = 0;
  ACE_NEW_RETURN (object_adapter,
                  TAO_Object_Adapter (orb_core->server_factory ()->
                                        active_object_map_creation_parameters (),
                                      *orb_core),
                  0);

  // Create and register the RT servant dispatcher.
  TAO_RT_Servant_Dispatcher *rt_servant_dispatcher;
  ACE_NEW_RETURN (rt_servant_dispatcher,
                  TAO_RT_Servant_Dispatcher,
                  0);
  object_adapter->servant_dispatcher (rt_servant_dispatcher);

  // Create and add the RT policy validator.
  TAO_POA_RT_Policy_Validator *rt_validator;
  ACE_NEW_RETURN (rt_validator,
                  TAO_POA_RT_Policy_Validator (*orb_core),
                  0);
  object_adapter->validator ().add_validator (rt_validator);

  return object_adapter;
}

ACE_FACTORY_DEFINE (TAO_RTPortableServer, TAO_RT_Object_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_RT_Object_Adapter_Factory,
                       ACE_TEXT ("TAO_RT_POA"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Object_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
