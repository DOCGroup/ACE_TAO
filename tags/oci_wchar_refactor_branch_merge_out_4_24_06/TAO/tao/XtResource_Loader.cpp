//$Id$
#include "XtResource_Loader.h"
#include "ORB_Core.h"
#include "XtResource_Factory.h"

ACE_RCSID( TAO_XtResource,
           XtResource_Loader,
           "$Id$");

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
