
// $Id$

#include "RTScheduler_Loader.h"
#include "RTScheduler_Initializer.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"

ACE_RCSID (TAO, RTScheduler_Loader, "$Id$")


TAO_RTScheduler_Loader::TAO_RTScheduler_Loader (void)
{
}

TAO_RTScheduler_Loader::~TAO_RTScheduler_Loader (void)
{

}

int
TAO_RTScheduler_Loader::init (int /*argc*/,
                         ACE_TCHAR* /*argv*/[])
{
  ACE_TRACE ("TAO_RTScheduler_Loader::init");

  static int initialized = 0;

  // Only allow initialization once.
  if (initialized)
    return 0;

  initialized = 1;

  // Register the ORB initializer.
  ACE_TRY_NEW_ENV
    {
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var orb_initializer;

      /// Register the RTCORBA ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_RTScheduler_ORB_Initializer,
                        CORBA::NO_MEMORY (
					  CORBA::SystemException::_tao_minor_code (
										  TAO_DEFAULT_MINOR_CODE,
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
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught while initializing the RTScheduler:");
      return 1;
    }
  ACE_ENDTRY;

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
