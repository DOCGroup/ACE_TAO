#include "PI_Server.h"
#include "PI_Server_Loader.h"

ACE_RCSID (PI_Server,
           PI_Server,
           "$Id$")

int
TAO_PI_Server_Init::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_PI_Server_Loader);
}

