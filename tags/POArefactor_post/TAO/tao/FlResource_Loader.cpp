//$Id$
#include "FlResource_Loader.h"
#include "ORB_Core.h"
#include "FlResource_Factory.h"

ACE_RCSID( TAO_FlResource,
           FlResource_Loader,
           "$Id$");

namespace TAO
{

  FlResource_Loader::FlResource_Loader ()
  {
    FlResource_Factory *tmp = 0;

    ACE_NEW (tmp,
             FlResource_Factory ());

    TAO_ORB_Core::set_gui_resource_factory( tmp );
  }

  FlResource_Loader::~FlResource_Loader ()
  {
  }
}
