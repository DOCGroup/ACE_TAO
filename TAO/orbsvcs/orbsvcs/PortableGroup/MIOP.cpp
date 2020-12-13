#include "MIOP.h"
#include "UIPMC_Factory.h"
#include "ace/Service_Config.h"
#include "PortableGroup_Loader.h"
#include "PG_Object_Adapter_Factory.h"
#include "miop_resource.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /*static*/ int
  MIOP_Loader::Initializer ()
  {
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_PortableGroup_Loader);

    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_PG_Object_Adapter_Factory);

    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_MIOP_Resource_Factory);

    return ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_UIPMC_Protocol_Factory);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
