//$Id$
#include "tao/XtResource/XtResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/XtResource/XtResource_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

TAO_END_VERSIONED_NAMESPACE_DECL
