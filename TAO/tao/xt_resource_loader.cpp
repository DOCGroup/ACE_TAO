//$Id$
#include "ORB_Core.h"
#include "tao/xt_resource_loader.h"
#include "tao/xt_resource.h"


// Must be called with lock held
TAO_XtResource_Loader::TAO_XtResource_Loader (XtAppContext context)
{
    TAO_XtResource_Factory::set_context (context);
    TAO_ORB_Core::set_reactor_factory ("XtResource_Factory");
    ACE_Service_Config::process_directive (ace_svc_desc_TAO_XtResource_Factory);
}

TAO_XtResource_Loader::~TAO_XtResource_Loader ()
{
}
