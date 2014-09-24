// $Id$

#include "tao/CSD_ThreadPool/CSD_ThreadPool.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy_Factory.h"
#include "tao/CSD_Framework/CSD_Framework_Loader.h"
#include "tao/debug.h"
#include "ace/Dynamic_Service.h"

#include "tao/CSD_Framework/CSD_ORBInitializer.h"
#include "tao/PI/DLL_Resident_ORB_Initializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/ORB_Core.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_CSD_ThreadPool::init (void)
{
  static int initialized = 0;
  if (initialized == 1)
    return 0;
  initialized = 1;

  TAO_CSD_Framework_Loader::static_init();

  // Register the ORB initializer.
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      /// Register the CSD ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_CSD_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      PortableInterceptor::ORBInitializer_var orb_initializer;
      orb_initializer = temp_orb_initializer;

      PortableInterceptor::ORBInitializer_ptr temp_dll_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_THROW_EX (temp_dll_initializer,
        PortableInterceptor::DLL_Resident_ORB_Initializer(
          orb_initializer.in (),
          ACE_TEXT ("TAO_CSD_ThreadPool")),
          CORBA::NO_MEMORY (
            CORBA::SystemException::_tao_minor_code (
              TAO::VMCID,
              ENOMEM),
            CORBA::COMPLETED_NO));

      PortableInterceptor::ORBInitializer_var dll_initializer;
      dll_initializer = temp_dll_initializer;

      PortableInterceptor::register_orb_initializer (
          dll_initializer.in ());
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught while "
        "initializing the CSD Framework");
      return 1;
    }


  return ACE_Service_Config::process_directive (
            ace_svc_desc_TAO_CSD_TP_Strategy_Factory);
}

TAO_END_VERSIONED_NAMESPACE_DECL
