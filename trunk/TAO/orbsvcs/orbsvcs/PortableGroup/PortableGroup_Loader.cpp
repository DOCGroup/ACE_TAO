// This may look like C, but it's really -*- C++ -*-
//
// $Id$

#include "PortableGroup_Loader.h"
#include "PortableGroup_ORBInitializer.h"
#include "tao/ORB_Core.h"

ACE_RCSID(PortableGroup, PortableGroup_Loader, "$Id$")

TAO_PortableGroup_Loader::TAO_PortableGroup_Loader (void)
{
}

int
TAO_PortableGroup_Loader::init (int /*argc*/,
                                char* /*argv*/ [])
{
  ACE_TRACE ("TAO_PortableGroup_Loader::init");

  // Register the ORB initializer.
  ACE_TRY_NEW_ENV
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var orb_initializer;

      /// Register the RTCORBA ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_PortableGroup_ORBInitializer (),
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;
      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught while initializing the PortableGroup:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

int
TAO_PortableGroup_Loader::Initializer (void)
{
  ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_PortableGroup_Loader);

  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_PortableGroup_Loader,
                       ACE_TEXT ("PortableGroup_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_PortableGroup_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_PortableGroup, TAO_PortableGroup_Loader)
