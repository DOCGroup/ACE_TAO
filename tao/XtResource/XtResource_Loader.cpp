//$Id$
#include "tao/XtResource/XtResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/XtResource/XtResource_Factory.h"

namespace TAO
{

  XtResource_Loader::XtResource_Loader (XtAppContext context)
  {
    XtResource_Factory *tmp = 0;

    ACE_NEW (tmp,
             XtResource_Factory (context));

    TAO_ORB_Core::set_gui_resource_factory( tmp );
  }

  XtResource_Loader::~XtResource_Loader ()
  {
  }
}
