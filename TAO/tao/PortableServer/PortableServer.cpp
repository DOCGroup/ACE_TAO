/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PortableServer.cpp
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#include "PortableServer.h"
#include "Object_Adapter.h"
#include "PortableServer_ORBInitializer.h"

ACE_RCSID (PortableServer, PortableServer, "$Id$")

int
TAO_POA_Initializer::init (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      /// Register the Messaging ORBInitializer.
      PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_PortableServer_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
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

  return
    ACE_Service_Config::process_directive (ace_svc_desc_TAO_Object_Adapter_Factory);
}
