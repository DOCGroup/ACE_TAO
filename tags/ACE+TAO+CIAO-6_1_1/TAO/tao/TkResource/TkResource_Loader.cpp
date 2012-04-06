// $Id$

#include "tao/TkResource/TkResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/TkResource/TkResource_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  TkResource_Loader::TkResource_Loader (void)
  {
    TkResource_Factory *tmp = 0;

    ACE_NEW (tmp,
             TkResource_Factory ());

    TAO_ORB_Core::set_gui_resource_factory( tmp );
  }

  TkResource_Loader::~TkResource_Loader (void)
  {
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
