#include "PI.h"
#include "ORBInitializer_Registry_Impl.h"
#include "PolicyFactory_Loader.h"
#include "ClientRequestInterceptor_Factory_Impl.h"
#include "PICurrent_Loader.h"

ACE_RCSID (PI,
           PI,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_PI_Init::Initializer (void)
{
#if TAO_HAS_INTERCEPTORS == 1
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_ClientRequestInterceptor_Adapter_Factory_Impl);

  ACE_Service_Config::process_directive (ace_svc_desc_TAO_PICurrent_Loader);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

  return ACE_Service_Config::process_directive (ace_svc_desc_ORBInitializer_Registry);
}

TAO_END_VERSIONED_NAMESPACE_DECL
