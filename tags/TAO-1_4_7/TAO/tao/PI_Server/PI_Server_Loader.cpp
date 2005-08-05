
// $Id$

#include "PI_Server_Loader.h"
#include "PortableServer_ORBInitializer.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"

ACE_RCSID (PI_Server,
           PI_Server_Loader,
           "$Id$")

TAO_PI_Server_Loader::TAO_PI_Server_Loader (void)
{
}

TAO_PI_Server_Loader::~TAO_PI_Server_Loader (void)
{

}

int
TAO_PI_Server_Loader::init (int,
			      ACE_TCHAR* [])
{
  ACE_TRACE ("TAO_PI_Server_Loader::init");

  static int initialized = 0;

  // Only allow initialization once.
  if (initialized)
    return 0;

  initialized = 1;

  // Register the ORB initializer.
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
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
      ACE_TRY_CHECK;

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_orb_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P | %t) Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

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
