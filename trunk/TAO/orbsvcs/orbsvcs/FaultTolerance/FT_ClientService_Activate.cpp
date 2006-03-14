#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"
#include "orbsvcs/FaultTolerance/FT_Service_Callbacks.h"
#include "orbsvcs/FaultTolerance/FT_ClientORBInitializer.h"
#include "orbsvcs/FaultTolerance/FT_Endpoint_Selector_Factory.h"
#include "tao/ORB_Core.h"
#include "tao/Service_Callbacks.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(FaultTolerance, FT_ClientService_Activate, "$Id$")

static bool initialized = false;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FT_ClientService_Activate::TAO_FT_ClientService_Activate (void)
{
}

TAO_FT_ClientService_Activate::~TAO_FT_ClientService_Activate (void)
{
}


TAO_Service_Callbacks *
TAO_FT_ClientService_Activate::activate_services (TAO_ORB_Core *orb_core)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_FT_Service_Callbacks *ft_service_callback = 0;

  // Construct service callback object
  ACE_NEW_RETURN (ft_service_callback,
                  TAO_FT_Service_Callbacks (orb_core),
                  0);


  return ft_service_callback;
}


int
TAO_FT_ClientService_Activate::Initializer (void)
{
  if (initialized == false)
    {
      ACE_Service_Config::static_svcs ()->
        insert (&ace_svc_desc_TAO_FT_ClientService_Activate);

      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var orb_initializer;

      // Register the RTCORBA ORBInitializer.
      ACE_NEW_RETURN (temp_orb_initializer,
                      TAO_FT_ClientORBInitializer,
                      -1);

      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      // Set the name of the endpoint selector factory
      TAO_ORB_Core::set_endpoint_selector_factory ("FT_Endpoint_Selector_Factory");
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_FT_Endpoint_Selector_Factory);

      initialized = true;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_FT_ClientORB,TAO_FT_ClientService_Activate)

ACE_STATIC_SVC_DEFINE (TAO_FT_ClientService_Activate,
                       ACE_TEXT ("FT_ClientService_Activate"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_FT_ClientService_Activate),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
