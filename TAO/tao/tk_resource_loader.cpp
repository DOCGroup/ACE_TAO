//$Id$
#include "ORB_Core.h"
#include "tao/tk_resource_loader.h"
#include "tao/tk_resource.h"

TAO_TkResource_Loader::TAO_TkResource_Loader ()
{
    TAO_ORB_Core::set_reactor_factory ("TkResource_Factory");
    ACE_Service_Config::process_directive (ace_svc_desc_TAO_TkResource_Factory);
}

TAO_TkResource_Loader::~TAO_TkResource_Loader ()
{
}
