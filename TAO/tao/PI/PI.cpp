#include "PI.h"
#include "ORBInitializer_Registry_Impl.h"
#include "PolicyFactory_Loader.h"

ACE_RCSID (PI,
           PI,
           "$Id$")

int
TAO_PI_Init::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_ORBInitializer_Registry);
}

