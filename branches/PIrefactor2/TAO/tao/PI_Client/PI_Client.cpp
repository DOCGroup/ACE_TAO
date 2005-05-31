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

ACE_STATIC_SVC_DEFINE (TAO_PolicyFactory_Registry,
                       ACE_TEXT ("PolicyFactory_Registry"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_PolicyFactory_Registry),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_PI_CLIENT, TAO_PolicyFactory_Registry)
