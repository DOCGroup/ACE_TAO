#include "FT_ServerService_Activate.h"
#include "FT_ServerORBInitializer.h"
#include "tao/ORB_Core.h"
#include "tao/Service_Callbacks.h"
#include "ace/Dynamic_Service.h"
#include <iostream>

ACE_RCSID(FaultTolerance, FT_ServerService_Activate, "$Id$")

static bool initialized = false;

TAO_FT_ServerService_Activate::TAO_FT_ServerService_Activate (void)
{
}

TAO_FT_ServerService_Activate::~TAO_FT_ServerService_Activate (void)
{
}



int
TAO_FT_ServerService_Activate::Initializer (void)
{
  if (initialized == false)
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var orb_initializer;

      // Register the FTCORBA ServerORBInitializer.
      ACE_NEW_RETURN (temp_orb_initializer,
                      TAO_FT_ServerORBInitializer,
                      -1);

      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      initialized = true;
    }

  return 0;
}

ACE_FACTORY_DEFINE (TAO_FT,TAO_FT_ServerService_Activate)

ACE_STATIC_SVC_DEFINE (TAO_FT_ServerService_Activate,
                       ACE_TEXT ("FT_ServerService_Activate"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_FT_ServerService_Activate),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dynamic_Service<TAO_FT_ServerService_Activate>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dynamic_Service<TAO_FT_ServerService_Activate>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
