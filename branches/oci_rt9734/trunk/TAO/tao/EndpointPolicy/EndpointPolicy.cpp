// @(#) $Id$

#include "tao/EndpointPolicy/EndpointPolicy.h"
#include "tao/EndpointPolicy/EndpointPolicy_ORBInitializer.h"
#include "tao/EndpointPolicy/Endpoint_Acceptor_Filter_Factory.h"

#include "tao/ORB_Core.h"
#include "tao/ORBInitializer_Registry.h"

#include "tao/PI/PI.h"

ACE_RCSID (EndpointPolicy, EndpointPolicy, "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_EndpointPolicy_Initializer::init (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_Endpoint_Acceptor_Filter_Factory);

  PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
  PortableInterceptor::ORBInitializer::_nil ();
  PortableInterceptor::ORBInitializer_var orb_initializer;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      /// Register the EndpointPolicy ORBInitializer.
      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_EndpointPolicy_ORBInitializer,
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

  ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_Endpoint_Acceptor_Filter_Factory,1); // force replacement


  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
