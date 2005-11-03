// $Id$

#include "CSD_Framework_Loader.h"
#include "CSD_Object_Adapter_Factory.h"
#include "CSD_Strategy_Repository.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (CSD_Framework,
           CSD_Framework_Loader,
           "$Id$")

#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_CSD_Framework_Loader::init (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_CSD_Object_Adapter_Factory);

  TAO_ORB_Core::set_poa_factory ("TAO_CSD_Object_Adapter_Factory",
                                 "dynamic TAO_CSD_Object_Adapter_Factory Service_Object * _make_TAO_CSD_Object_Adapter_Factory()");


  ACE_Service_Config::process_directive (ace_svc_desc_TAO_CSD_Strategy_Repository);

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
