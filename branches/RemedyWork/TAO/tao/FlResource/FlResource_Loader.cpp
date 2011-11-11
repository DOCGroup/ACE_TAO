// -*- C++ -*-
// $Id$

#include "tao/FlResource/FlResource_Loader.h"
#include "tao/ORB_Core.h"
#include "tao/FlResource/FlResource_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  FlResource_Loader::FlResource_Loader (void)
  {
    FlResource_Factory *tmp = 0;

    ACE_NEW (tmp,
             FlResource_Factory ());

    TAO_ORB_Core::set_gui_resource_factory (tmp);
  }

  FlResource_Loader::~FlResource_Loader (void)
  {
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
