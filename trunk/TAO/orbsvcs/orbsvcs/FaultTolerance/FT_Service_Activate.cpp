#include "FT_Service_Activate.h"
#include "FT_Service_Callbacks.h"
#include "FT_ORBInitializer.h"
#include "tao/Service_Callbacks.h"
#include "ace/Dynamic_Service.h"


ACE_RCSID(FaultTolerance, FT_Service_Activate, "$Id$")

TAO_FT_Service_Activate::TAO_FT_Service_Activate (void)
{
}

TAO_FT_Service_Activate::~TAO_FT_Service_Activate (void)
{
}

int
TAO_FT_Service_Activate::init (int /*argc*/,
                               ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argv);

  cout << "Did we get here " <<endl;
  PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
    PortableInterceptor::ORBInitializer::_nil ();
  PortableInterceptor::ORBInitializer_var orb_initializer;

  // Register the RTCORBA ORBInitializer.
  ACE_NEW_RETURN (temp_orb_initializer,
                  TAO_FT_ORBInitializer,
                  -1);

  orb_initializer = temp_orb_initializer;

  PortableInterceptor::register_orb_initializer (orb_initializer.in ());

  return 0;
}


TAO_Service_Callbacks *
TAO_FT_Service_Activate::activate_services (TAO_ORB_Core *orb_core)
  ACE_THROW_SPEC (())
{
  TAO_FT_Service_Callbacks *ft_service_callback = 0;

  cout << "Did we get here 1" <<endl;
  // Construct service callback object
  ACE_NEW_RETURN (ft_service_callback,
                  TAO_FT_Service_Callbacks (orb_core),
                  0);

  return ft_service_callback;
}


int
TAO_FT_Service_Activate::Initializer (void)
{
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_FT_Service_Activate);

  return 0;
}

ACE_FACTORY_DEFINE (TAO_FT,TAO_FT_Service_Activate)

ACE_STATIC_SVC_DEFINE (TAO_FT_Service_Activate,
                       ACE_TEXT ("FT_Service_Activate"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_FT_Service_Activate),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dynamic_Service<TAO_FT_Service_Activate>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dynamic_Service<TAO_FT_Service_Activate>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
