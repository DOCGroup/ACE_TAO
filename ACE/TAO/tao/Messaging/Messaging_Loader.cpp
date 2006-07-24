
// $Id$

#include "tao/Messaging/Messaging_Loader.h"
#include "tao/Messaging/Messaging_ORBInitializer.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"

ACE_RCSID (Messaging, Messaging_Loader, "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Messaging_Loader::TAO_Messaging_Loader (void)
{
}

TAO_Messaging_Loader::~TAO_Messaging_Loader (void)
{
}

int
TAO_Messaging_Loader::init (int,
                            ACE_TCHAR* [])
{
  ACE_TRACE ("TAO_Messaging_Loader::init");

  static int called_once = 0;

  if (called_once != 0)
    return 0;
  called_once = 1;

  PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
    PortableInterceptor::ORBInitializer::_nil ();
  PortableInterceptor::ORBInitializer_var orb_initializer;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      /// Register the Messaging ORBInitializer.

      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_Messaging_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      orb_initializer = temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Caught exception:");
        }
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

/////////////////////////////////////////////////////////////////////

ACE_FACTORY_DEFINE (TAO_Messaging, TAO_Messaging_Loader)
ACE_STATIC_SVC_DEFINE (TAO_Messaging_Loader,
                       ACE_TEXT ("Messaging_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Messaging_Loader),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
