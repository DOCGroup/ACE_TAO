// $Id$

#include "RT_Object_Adapter_Factory.h"
#include "RT_POA_Initializer.h"
#include "tao/PortableServer/Object_Adapter.h"
#include "tao/ORB_Core.h"

ACE_RCSID (TAO_RTPortableServer,
           RT_Object_Adapter_Factory,
           "$Id$")


TAO_RT_Object_Adapter_Factory::TAO_RT_Object_Adapter_Factory (void)
{
}

TAO_Adapter*
TAO_RT_Object_Adapter_Factory::create (TAO_ORB_Core *orb_core)
{
  // Register the RT_POA initializer so that it can register all of its
  // hooks when the time comes.
  TAO_RT_POA_Initializer *rt_initializer;
  ACE_NEW_RETURN (rt_initializer,
                  TAO_RT_POA_Initializer,
                  0);
  orb_core->add_poa_extension_initializer (rt_initializer);

  // Setup the POA_Current object in the ORB
  CORBA::Object_var current = new TAO_POA_Current;
  orb_core->poa_current (current.in ());

  return new TAO_Object_Adapter (orb_core->server_factory ()->
                                        active_object_map_creation_parameters (),
                                 *orb_core);
}

ACE_FACTORY_DEFINE (TAO_RTPortableServer, TAO_RT_Object_Adapter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_RT_Object_Adapter_Factory,
                       ACE_TEXT ("TAO_RT_POA"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RT_Object_Adapter_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)


