// -*- C++ -*-
// $Id$

#include "tao/PI_Server/PI_Server_Loader.h"
#include "tao/PI_Server/PortableServer_ORBInitializer.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PI_Server_Loader::TAO_PI_Server_Loader (void)
  : initialized_ (false)
{
}

TAO_PI_Server_Loader::~TAO_PI_Server_Loader (void)
{
}

int
TAO_PI_Server_Loader::init (int, ACE_TCHAR* [])
{
  ACE_TRACE ("TAO_PI_Server_Loader::init");

  // Only allow initialization once.
  if (this->initialized_)
    return 0;

  this->initialized_ = true;

  // Register the ORB initializer.
  try
    {
      /// Register the Messaging ORBInitializer.
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_PortableServer_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}


/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_PI_Server, TAO_PI_Server_Loader)
ACE_STATIC_SVC_DEFINE (TAO_PI_Server_Loader,
                       ACE_TEXT ("PI_Server_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_PI_Server_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL
