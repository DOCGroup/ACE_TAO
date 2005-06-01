#include "PI_Client.h"
#include "PolicyFactory_Registry.h"
#include "tao/ORB_Core.h"

ACE_RCSID (tao,
           PolicyFactory_Registry,
           "$Id$")

int
TAO_PI_Client_Init::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_PolicyFactory_Registry);
}

