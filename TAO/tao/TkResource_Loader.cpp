//$Id$
#include "TkResource_Loader.h"
#include "ORB_Core.h"
#include "TkResource_Factory.h"

ACE_RCSID(tao, TkResource_Loader, "$Id$");

namespace TAO
{

  TkResource_Loader::TkResource_Loader ()
  {
    TAO_ORB_Core::set_gui_resource_factory( new TkResource_Factory () );
  }

  TkResource_Loader::~TkResource_Loader ()
  {
  }
}
