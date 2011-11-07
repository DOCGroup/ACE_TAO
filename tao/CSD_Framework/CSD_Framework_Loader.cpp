// -*- C++ -*-
// $Id$

#include "tao/CSD_Framework/CSD_Framework_Loader.h"
#include "tao/CSD_Framework/CSD_Object_Adapter_Factory.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "tao/CSD_Framework/CSD_ORBInitializer.h"
#include "tao/PI/DLL_Resident_ORB_Initializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CSD_Framework_Loader::TAO_CSD_Framework_Loader (void)
  : initialized_ (false)
{
}

TAO_CSD_Framework_Loader::~TAO_CSD_Framework_Loader (void)
{
}


int
TAO_CSD_Framework_Loader::static_init (void)
{
  ACE_Service_Config::process_directive
     (ace_svc_desc_TAO_CSD_Object_Adapter_Factory);

  ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_CSD_Strategy_Repository);

  ACE_Service_Config::process_directive (ace_svc_desc_TAO_CSD_Framework_Loader);

  return 0;
}

int
TAO_CSD_Framework_Loader::init (int, ACE_TCHAR* [])
{
  ACE_TRACE ("TAO_CSD_Framework_Loader::init");

  // Only allow initialization once.
  if (this->initialized_)
    return 0;

  this->initialized_ = true;

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

      PortableInterceptor::register_orb_initializer (dll_initializer.in ());
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


/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_CSD_FW, TAO_CSD_Framework_Loader)
ACE_STATIC_SVC_DEFINE (TAO_CSD_Framework_Loader,
                       ACE_TEXT ("CSD_Framework_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_CSD_Framework_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL
