#include "MIOP.h"
#include "UIPMC_Factory.h"
#include "ace/Service_Config.h"
#include "PortableGroup_Loader.h"

ACE_RCSID (PortableGroup,
           UIPMC_Factory,
           "$Id$")

namespace TAO
{
  /*static*/ int
  MIOP_Loader::Initializer (void)
  {
    /*ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_PortableGroup_Loader);
    */
    return ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_UIPMC_Protocol_Factory);
  }
}
