//$Id$

#include "tao/FoxResource/FoxResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/FoxResource/FoxResource_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  FoxResource_Loader::FoxResource_Loader (FXApp *app)
  {
    FoxResource_Factory *tmp = 0;

    ACE_NEW (tmp,
             FoxResource_Factory (app));

    TAO_ORB_Core::set_gui_resource_factory( tmp );
  }

  FoxResource_Loader::~FoxResource_Loader (void)
  {
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
