// $Id$

#include "tao/CSD_Framework/CSD_Framework_Loader.h"
#include "tao/CSD_Framework/CSD_Object_Adapter_Factory.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "tao/CSD_Framework/CSD_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Gestalt.h"

ACE_RCSID (CSD_Framework,
           CSD_Framework_Loader,
           "$Id$")

#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_CSD_Framework_Loader::init (void)
{
  ACE_Service_Config::process_directive
     (ace_svc_desc_TAO_CSD_Object_Adapter_Factory);

  ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_CSD_Strategy_Repository);

  // Register the ORB initializer.
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      /// Register the RTCORBA ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_CSD_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      PortableInterceptor::ORBInitializer_var orb_initializer;
      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught while "
        "initializing the CSD Framework");
      return 1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
