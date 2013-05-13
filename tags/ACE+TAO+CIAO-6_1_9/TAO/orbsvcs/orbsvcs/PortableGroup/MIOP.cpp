// $Id$

#include "orbsvcs/PortableGroup/MIOP.h"
#include "orbsvcs/PortableGroup/UIPMC_Factory.h"
#include "ace/Service_Config.h"
#include "orbsvcs/PortableGroup/PortableGroup_Loader.h"
#include "orbsvcs/PortableGroup/PG_Object_Adapter_Factory.h"
#include "orbsvcs/PortableGroup/miop_resource.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /*static*/ int
  MIOP_Loader::Initializer (void)
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
