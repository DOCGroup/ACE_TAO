#include "PI_Server.h"
#include "PI_Server_Loader.h"
#include "ServerRequestInterceptor_Factory_Impl.h"

ACE_RCSID (PI_Server,
           PI_Server,
           "$Id$")

int
TAO_PI_Server_Init::Initializer (void)
{
#if TAO_HAS_INTERCEPTORS == 1
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_ServerRequestInterceptor_Adapter_Factory_Impl);
#endif  /* TAO_HAS_INTERCEPTORS */

  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_PI_Server_Loader);
}

