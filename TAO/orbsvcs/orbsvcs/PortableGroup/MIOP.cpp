#include "MIOP.h"
#include "UIPMC_Factory.h"
#include "ace/Service_Config.h"
#include "PortableGroup_Loader.h"
#include "PG_Object_Adapter_Factory.h"

ACE_RCSID (PortableGroup,
           UIPMC_Factory,
           "$Id$")

namespace TAO
{
  /*static*/ int
  MIOP_Loader::Initializer (void)
  {
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_PortableGroup_Loader);

    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_PG_Object_Adapter_Factory);

    return ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_UIPMC_Protocol_Factory);
  }
}
