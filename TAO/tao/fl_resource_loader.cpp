//$Id$
#include "ORB_Core.h"
#include "tao/fl_resource_loader.h"
#include "tao/fl_resource.h"

TAO_FlResource_Loader::TAO_FlResource_Loader ()
{
    TAO_ORB_Core::set_reactor_factory ("FlResource_Factory");
    ACE_Service_Config::process_directive (ace_svc_desc_TAO_FlResource_Factory);
}

TAO_FlResource_Loader::~TAO_FlResource_Loader ()
{
}
