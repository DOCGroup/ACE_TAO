//$Id$
#include "XtResource_Loader.h"
#include "ORB_Core.h"
#include "XtResource_Factory.h"

ACE_RCSID(tao, XtResource_Loader, "$Id$");

namespace TAO
{

  XtResource_Loader::XtResource_Loader (XtAppContext context)
  {
    TAO_ORB_Core::set_gui_resource_factory( new XtResource_Factory (context) );
  }

  XtResource_Loader::~XtResource_Loader ()
  {
  }
}
