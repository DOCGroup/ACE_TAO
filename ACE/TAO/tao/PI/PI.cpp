#include "tao/PI/PI.h"
#include "tao/PI/ORBInitializer_Registry_Impl.h"
#include "tao/PI/PolicyFactory_Loader.h"
#include "tao/PI/ClientRequestInterceptor_Factory_Impl.h"
#include "tao/PI/PICurrent_Loader.h"
#include "tao/PI/PI_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/ORB_Constants.h"

ACE_RCSID (PI,
           PI,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


// This initializer is very similar to the ORBInitializer_Registry::init.
// This one is used during static initialization while the other is used
// when PI is a shared library.
int
TAO_PI_Init::Initializer (void)
{
#if TAO_HAS_INTERCEPTORS == 1
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_ClientRequestInterceptor_Adapter_Factory_Impl);

  ACE_Service_Config::process_directive (ace_svc_desc_TAO_PICurrent_Loader);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

  int const status = ACE_Service_Config::process_directive (
                            ace_svc_desc_ORBInitializer_Registry);

#if  TAO_HAS_INTERCEPTORS == 1
  PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
    PortableInterceptor::ORBInitializer::_nil ();

  PortableInterceptor::ORBInitializer_var orb_initializer;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      /// Register the PI ORBInitializer.

      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_PI_ORBInitializer,
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
                               "(%P | %t) Caught exception:");
        }
      return -1;
    }
  ACE_ENDTRY;
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
  return status;
}

TAO_END_VERSIONED_NAMESPACE_DECL
