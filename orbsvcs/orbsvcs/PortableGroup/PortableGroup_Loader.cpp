// $Id$

#include "orbsvcs/PortableGroup/PortableGroup_Loader.h"
#include "orbsvcs/PortableGroup/PG_Object_Adapter_Factory.h"
#include "orbsvcs/PortableGroup/PortableGroup_ORBInitializer.h"
#include "ace/Dynamic_Service.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PortableGroup_Loader::TAO_PortableGroup_Loader (void)
{
}

int
TAO_PortableGroup_Loader::init (int /*argc*/,
                                ACE_TCHAR* /*argv*/ [])
{
  ACE_TRACE ("TAO_PortableGroup_Loader::init");
  // Register the ORB initializer.
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var orb_initializer;

      /// Register the Portablegroup ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_PortableGroup_ORBInitializer (),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught while initializing the PortableGroup:");
      return 1;
    }

  return 0;
}

int
TAO_PortableGroup_Loader::Initializer (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_PortableGroup_Loader);

  return -1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_PortableGroup_Loader,
                       ACE_TEXT ("PortableGroup_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_PortableGroup_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_PortableGroup, TAO_PortableGroup_Loader)
