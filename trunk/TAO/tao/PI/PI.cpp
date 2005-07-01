#include "PI.h"
#include "ORBInitializer_Registry_Impl.h"
#include "PolicyFactory_Loader.h"
#include "ClientRequestInterceptor_Factory_Impl.h"

ACE_RCSID (PI,
           PI,
           "$Id$")

int
TAO_PI_Init::Initializer (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_ClientRequestInterceptor_Adapter_Factory_Impl);

  return ACE_Service_Config::process_directive (ace_svc_desc_ORBInitializer_Registry);
}

