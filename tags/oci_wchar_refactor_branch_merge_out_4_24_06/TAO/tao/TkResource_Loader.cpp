//$Id$
#include "TkResource_Loader.h"
#include "ORB_Core.h"
#include "TkResource_Factory.h"

ACE_RCSID( TAO_TkResource,
           TkResource_Loader,
           "$Id$");

namespace TAO
{

  TkResource_Loader::TkResource_Loader ()
  {
    TkResource_Factory *tmp = 0;

    ACE_NEW (tmp,
             TkResource_Factory ());

    TAO_ORB_Core::set_gui_resource_factory( tmp );
  }

  TkResource_Loader::~TkResource_Loader ()
  {
  }
}
