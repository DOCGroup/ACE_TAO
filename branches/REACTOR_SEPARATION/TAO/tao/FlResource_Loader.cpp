//$Id$
#include "FlResource_Loader.h"
#include "ORB_Core.h"
#include "FlResource_Factory.h"

ACE_RCSID(tao, FlResource_Loader, "$Id$");

namespace TAO
{

  FlResource_Loader::FlResource_Loader ()
  {
    TAO_ORB_Core::set_gui_resource_factory( new FlResource_Factory () );
  }

  FlResource_Loader::~FlResource_Loader ()
  {
  }
}
