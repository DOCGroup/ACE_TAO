// $Id$

#include "tao/CSD_Framework/CSD_Framework_Loader.h"
#include "tao/CSD_Framework/CSD_Object_Adapter_Factory.h"
#include "tao/CSD_Framework/CSD_Strategy_Repository.h"
#include "ace/Dynamic_Service.h"
#include "ace/Service_Gestalt.h"

ACE_RCSID (CSD_Framework,
           CSD_Framework_Loader,
           "$Id$")

#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_CSD_Framework_Loader::init (void)
{

  // The ORB_Core's POA factory information is held in a static entity
  // Therefore we need to force this initialization to occur in the
  // global configuration context. Whenever the ORB Core's static resource
  // problem is resolved, this should be removed.
  ACE_Service_Gestalt * theone = ACE_Service_Config::global ();
  ACE_Service_Config_Guard config_guard (theone);

  ACE_Service_Config::process_directive (ace_svc_desc_TAO_CSD_Object_Adapter_Factory);

  ACE_Service_Config::process_directive (ace_svc_desc_TAO_CSD_Strategy_Repository);

  TAO_ORB_Core::set_poa_factory ("TAO_CSD_Object_Adapter_Factory",
                                 "dynamic TAO_CSD_Object_Adapter_Factory Service_Object * TAO_CSD_Framework:_make_TAO_CSD_Object_Adapter_Factory()");

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
