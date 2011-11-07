// $Id$

#include "tao/RTScheduling/RTScheduler_Loader.h"
#include "tao/RTScheduling/RTScheduler_Initializer.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RTScheduler_Loader::TAO_RTScheduler_Loader (void)
  : initialized_ (false)
{
}

TAO_RTScheduler_Loader::~TAO_RTScheduler_Loader (void)
{
}

int
TAO_RTScheduler_Loader::init (int, ACE_TCHAR* [])
{
  ACE_TRACE ("TAO_RTScheduler_Loader::init");

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "In RTScheduler_Loader::init\n"));

  // Only allow initialization once.
  if (this->initialized_)
    return 0;

  this->initialized_ = true;

  ACE_Service_Gestalt *gestalt = ACE_Service_Config::current ();

  ACE_Service_Object * const rts_loader =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (
      gestalt,
      "RTScheduler_Loader",
      true);

  if (rts_loader != 0 && rts_loader != this)
    {
      return rts_loader->init (0, 0);
    }

  // Register the ORB initializer.
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      /// Register the RTCORBA ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_RTScheduler_ORB_Initializer,
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
      ex._tao_print_exception (
        "Unexpected exception caught while initializing the RTScheduler:");
      return 1;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_RTScheduler, TAO_RTScheduler_Loader)
ACE_STATIC_SVC_DEFINE (TAO_RTScheduler_Loader,
                       ACE_TEXT ("RTScheduler_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_RTScheduler_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL
